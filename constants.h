#pragma once
#include <stdint.h>

/*
	Pin assignment
*/
// LCD
const uint8_t RA8875_INT = 2;
const uint8_t RA8875_CS = 10;
const uint8_t RA8875_RESET = 9;
// Shift register
const uint8_t SR_CLOCK_OUT = 16;
const uint8_t SR_DATA_OUT = 15;
const uint8_t SR_LATCH = 14;

const uint16_t MAX_RPM = 14000;

// Translate float values from CAN BUS
inline float CANIntToFloat(uint16_t floatValue) {
	return floatValue / 1000.0;
}
// Translate kelvin temperature values from CAN BUS
inline float CANKelvinToFloat(uint16_t kelvinValue) {
	float result = kelvinValue / 10.0;
	result = result - 273.15;

	return result;
}