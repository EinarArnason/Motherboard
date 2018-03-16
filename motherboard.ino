#include <stdint.h>
#include <FlexCAN.h>
#include <SdFat.h>
#include <TimeLib.h>
#include "constants.h"
#include <XBee.h>

// XBee wirless module
XBee xbee = XBee();
// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x13A200, 0x416C0C05);
uint8_t payload[] = { 1, 2, 3, 4, 5 };
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

// SD card variables
SdFatSdioEX sd;
File outFile;
char filename[255];

// CAN BUS driver
class CanListener : public CANListener {
public:
	//overrides the parent version
	bool frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller);
};

CAN_filter_t defaultMask;

CanListener canListener;
unsigned int txTimer, rxTimer;
static CAN_message_t msg;

// Vehicle values
uint16_t rpm;
float oilTemp;
float waterTemp;
uint16_t brakeTemp;
uint8_t gear;
uint16_t speed;
float voltage;
bool fanOn;

bool CanListener::frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller) {

	char canInput[255];
	sprintf(canInput, "ID: %d - 0x%x%x 0x%x%x 0x%x%x 0x%x%x", frame.buf[0], frame.buf[1], frame.buf[2], frame.buf[3], frame.buf[4], frame.buf[5], frame.buf[6], frame.buf[7]);
	Serial.println(canInput);

	switch (frame.id) {
	case 1:
		rpm = frame.buf[0] | (frame.buf[1] << 8);
		voltage = CANIntToFloat(frame.buf[2] | (frame.buf[3] << 8));
		waterTemp = CANKelvinToFloat(frame.buf[4] | (frame.buf[5] << 8));
		speed = frame.buf[6] | (frame.buf[7] << 8);
		break;
	case 2:
		oilTemp = CANKelvinToFloat(frame.buf[0] | (frame.buf[1] << 8));
		gear = frame.buf[2] | (frame.buf[3] << 8);
		break;
	}

	uint8_t payload[] = {
		frame.id,
		frame.buf[0],
		frame.buf[1],
		frame.buf[2],
		frame.buf[3],
		frame.buf[4],
		frame.buf[5],
		frame.buf[6],
		frame.buf[7],
		frame.len
	};
	ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
	ZBTxStatusResponse txStatus = ZBTxStatusResponse();
	xbee.send(zbTx);

	return true;
}

time_t getTeensy3Time() {
	return Teensy3Clock.get();
}

void setup() {

	Serial.begin(9600);
	Serial.println("Start!");
	Serial2.begin(9600);
	xbee.setSerial(Serial2);

	//init SD Card
	if (!sd.begin()) {
		Serial.println("Error: SD connection failed");
	}

	// set the Time library to use Teensy 3.0's RTC to keep time
	setSyncProvider(getTeensy3Time);
	if (timeStatus() != timeSet) {
		Serial.println("Unable to sync with the RTC");
	}
	else {
		Serial.println("RTC has set the system time");
	}

	// Generate filename
	if (
		sprintf(
			filename,
			"%d_%d_%d_%d_%d_%d.json",
			year(),
			month(),
			day(),
			hour(),
			minute(),
			second()
		) < 0) {
		Serial.println("Error: failed to generate filename");
	}

	//Create the File
	outFile = sd.open(filename, FILE_WRITE);
	if (!outFile) {
		Serial.println("Error: failed to open file");
		return;
	};

	// Initialize the CAN bus
	Can0.begin(500000);
	Can0.attachObj(&canListener);
	Can1.begin(500000);
	canListener.attachGeneralHandler();
	msg.ext = 0;
	msg.id = 0x100;
	msg.len = 8;
	msg.buf[0] = 10;
	msg.buf[1] = 20;
	msg.buf[2] = 0;
	msg.buf[3] = 100;
	msg.buf[4] = 128;
	msg.buf[5] = 64;
	msg.buf[6] = 32;
	msg.buf[7] = 16;
}

void loop() {
	char message[255];
	sprintf(
		message,
		"{\"time\":%d, \"rpm\":%d, \"oilTemp\":%f, \"waterTemp\":%f, \"brakeTemp\":%d, \"gear\":%d, \"speed\":%d, \"voltage\":%f, \"fanOn\":%d}",
		now(),
		rpm,
		oilTemp,
		waterTemp,
		brakeTemp,
		gear,
		speed,
		voltage,
		fanOn
	);
	outFile.println(message);
	outFile.flush();

	xbee.send(zbTx);
	Serial.println(txStatus.getDeliveryStatus());
	delay(1000);
}