#include <SPI.h>
#include <stdint.h>
#include <kinetis_flexcan.h>
#include <FlexCAN.h>
#include <SdFat.h>
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#include <TimeLib.h>
=======
>>>>>>> Added Added CAN BUS and SD card initialization
=======
#include <TimeLib.h>
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added Added CAN BUS and SD card initialization
=======
#include <TimeLib.h>
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added Added CAN BUS and SD card initialization
=======
#include <TimeLib.h>
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added Added CAN BUS and SD card initialization
#include "constants.h"

// SD card variables
SdFat sd;
File outFile;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
char filename[20];
=======
>>>>>>> Added Added CAN BUS and SD card initialization
=======
char filename[20];
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added Added CAN BUS and SD card initialization
=======
char filename[20];
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added Added CAN BUS and SD card initialization
=======
char filename[20];
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added Added CAN BUS and SD card initialization

// CAN BUS driver
class CanListener : public CANListener {
public:
	//overrides the parent version
	bool frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller);
};

CanListener canListener;
unsigned int txTimer, rxTimer;

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

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added timestamp code and filename format
time_t getTeensy3Time() {
	return Teensy3Clock.get();
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> Added Added CAN BUS and SD card initialization
=======
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added Added CAN BUS and SD card initialization
=======
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added Added CAN BUS and SD card initialization
=======
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added Added CAN BUS and SD card initialization
void setup() {
    //init SD Card
    if (!sd.begin())
    {
        Serial.println("Error: SD connection failed");
        while (1);
    }

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added timestamp code and filename format
=======
>>>>>>> Added timestamp code and filename format
	// set the Time library to use Teensy 3.0's RTC to keep time
	setSyncProvider(getTeensy3Time);
	if (timeStatus() != timeSet) {
		Serial.println("Unable to sync with the RTC");
	}
	else {
		Serial.println("RTC has set the system time");
	}

	// Generate filename
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
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
=======
    //Create the File
    outFile = sd.open(str, FILE_WRITE);
>>>>>>> Added Added CAN BUS and SD card initialization
=======
	sprintf(filename, "%d_%d_%d_%d_%d_%d.json", year(), month(), day(), hour(), minute(), second());

    //Create the File
    outFile = sd.open(filename, FILE_WRITE);
>>>>>>> Added timestamp code and filename format
=======
    //Create the File
    outFile = sd.open(str, FILE_WRITE);
>>>>>>> Added Added CAN BUS and SD card initialization
=======
	sprintf(filename, "%d_%d_%d_%d_%d_%d.json", year(), month(), day(), hour(), minute(), second());

    //Create the File
    outFile = sd.open(filename, FILE_WRITE);
>>>>>>> Added timestamp code and filename format
=======
    //Create the File
    outFile = sd.open(str, FILE_WRITE);
>>>>>>> Added Added CAN BUS and SD card initialization
=======
	sprintf(filename, "%d_%d_%d_%d_%d_%d.json", year(), month(), day(), hour(), minute(), second());

    //Create the File
    outFile = sd.open(filename, FILE_WRITE);
>>>>>>> Added timestamp code and filename format
=======
    //Create the File
    outFile = sd.open(str, FILE_WRITE);
>>>>>>> Added Added CAN BUS and SD card initialization
    if (!outFile) {
        Serial.println("Error: failed to open file");
        return;
    };

    // Initialize the CAN bus
	Can0.begin(500000);
	Can0.attachObj(&canListener);
	canListener.attachGeneralHandler();
}

void loop() {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	char message[255];
	sprintf(
		message,
		"{time:%d-%d-%d %d:%d:%d, rpm:%d, oilTemp:%f, waterTemp:%f, brakeTemp:%d, gear:%d, speed:%d, voltage:%f, fanOn:%d}",
		day(),
		month(),
		year(),
		hour(),
		minute(),
		second(),
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
=======
    runShiftRegister();
>>>>>>> Added Added CAN BUS and SD card initialization
=======
    
>>>>>>> Added timestamp code and filename format
=======
    runShiftRegister();
>>>>>>> Added Added CAN BUS and SD card initialization
=======
    
>>>>>>> Added timestamp code and filename format
=======
    runShiftRegister();
>>>>>>> Added Added CAN BUS and SD card initialization
=======
    
>>>>>>> Added timestamp code and filename format
=======
    runShiftRegister();
>>>>>>> Added Added CAN BUS and SD card initialization
}