/*
DS1307.h - The DS1307 library is provided to demonstrate the Arduino Time library.
Created by Tondar, February 9, 2020.
Released into the public domain.
*/

#ifndef DS1307_h
#define DS1307_h

#include "Arduino.h"

enum addresses
{
	SECOND,
	MINUTE,
	HOUR,
	DAY = 4,
	MONTH,
	YEAR,
};

enum format
{
	F24,
	FAM,
	FPM,
};

class DS1307
{
public:
	// Startup
	void begin(void);
	// Start frequency 1hz for the SQW pin
	void startSqw1hz(void);
	// Stop frequency 1hz for the SQW pin
	void stopSqw1hz(void);
	// Set part of Time
	// adr: addresses
	// val: uint8
	// f: format (Optional for HOUR)
	void setPartTime(addresses adr, uint8_t val, format f = F24);
	// Get part of Time
	// adr: addresses
	// val: uint8
	void getPartTime(addresses adr, uint8_t &val);
	// Get part of Time
	// adr: addresses
	// val: uint8
	// f: format (for HOUR)
	void getPartTime(addresses adr, uint8_t &val, format &f);
	// Set the Time
	void setTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, format f);
	// Get the Time
	void getTime(uint8_t &year, uint8_t &month, uint8_t &day, uint8_t &hour, uint8_t &minute, uint8_t &second, format &f);
	// Read data from the RAM
	// buffer array: uint8
	// adr: uint8; 0..55
	// size: uint8
	void readRam(uint8_t *buf, uint8_t adr, uint8_t size);
	// Write data to the RAM
	// adr: addresses
	// adr: uint8; 0..55
	// size: uint8
	void writeRam(uint8_t *buf, uint8_t adr, uint8_t size);
};

#endif
