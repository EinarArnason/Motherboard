/******************************************************************
Team Sleipnir communications motherboard

Hardware:
- Teensy 3.6
- XBee-PRO S2C
- MCP2551 CAN transceiver

Written by Einar Arnason
******************************************************************/

#include <XBee.h>
#include <SoftwareSerial.h>
#include <stdint.h>
#include <FlexCAN.h>
#include <SdFat.h>
#include <TimeLib.h>
#include "constants.h"

// SD card variables
SdFat sd;
File outFile;
char filename[20];

// XBee driver
XBee xbee = XBee();

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0xFF, 0xFE);
char payload[PAYLOAD_SIZE];

// CAN BUS driver
class CanListener : public CANListener {
public:
	//overrides the parent version
	bool frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller);
};

CanListener canListener;

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

	return true;
}

time_t getTeensy3Time() {
	return Teensy3Clock.get();
}

uint8_t payloadLength() {
	for (int i = 0; i < PAYLOAD_SIZE; i++) {
		if (payload[i] == '!') {
			return i;
		}
	}

	return PAYLOAD_SIZE;
}

void setup() {
	/*
    //init SD Card
    while (!sd.begin())
    {
        Serial.println("Error: SD connection failed");
		delay(1000);
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
	sprintf(filename, "%d_%d_%d_%d_%d_%d.json", year(), month(), day(), hour(), minute(), second());

    //Create the File
    outFile = sd.open(filename, FILE_WRITE);
    if (!outFile) {
        Serial.println("Error: failed to open file");
        return;
    };

    // Initialize the CAN bus
	Can0.begin(500000);
	Can0.attachObj(&canListener);
	canListener.attachGeneralHandler();
	*/

	Serial.begin(9600);

	// Initialize XBee serial
	Serial1.begin(9600);
	xbee.setSerial(Serial1);
}

void loop() {

	sprintf(payload, 
		"{\"rpm\": %d, \"speed\": %d, \"oilTemp\": %0.2f, \"waterTemp\": %0.2f, \"volt\": %0.2f}!", 
		random(2000, 14000), random(0, 255), 
		random(3000, 8000) / 100.0, 
		random(3000, 8000) / 100.0, 
		random(1160, 1290) / 100.0);
	ZBTxRequest zbTx = ZBTxRequest(addr64, (uint8_t*)payload, payloadLength());
	ZBTxStatusResponse txStatus = ZBTxStatusResponse();
	xbee.send(zbTx);
	// after sending a tx request, we expect a status response
	// wait up to half second for the status response
	if (xbee.readPacket(500)) {
		// got a response!

		// should be a znet tx status            	
		if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
			xbee.getResponse().getZBTxStatusResponse(txStatus);

			// get the delivery status, the fifth byte
			if (txStatus.getDeliveryStatus() == SUCCESS) {
				// success.  time to celebrate
				Serial.println("Success!");
			}
			else {
				// the remote XBee did not receive our packet. is it powered on?
				Serial.println("Fail!");
			}
		}
	}
	else if (xbee.getResponse().isError()) {
		Serial.print("Error reading packet.  Error code: ");  
		Serial.println(xbee.getResponse().getErrorCode());
	}
	else {
		// local XBee did not provide a timely TX Status Response -- should not happen
		Serial.println("local XBee did not provide a timely TX Status Response");
	}
	delay(1000);
}