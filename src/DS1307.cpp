/*
  DS1307.cpp - The DS1307 library is provided to demonstrate the Arduino Time library.
  Created by Tondar, February 9, 2020.
  Released into the public domain.
*/

#include "Arduino.h"
#include <Wire.h>
#include "DS1307.h"

#define DS1307_ADDRESS 0x68
#define ADDRESS_OFFSET 0x08

byte dec_to_bcd(byte dec)
{
    return ((dec / 10 * 16) + (dec % 10));
}

byte bcd_to_dec(byte BCD)
{
    return ((BCD / 16 * 10) + (BCD % 16));
}

void sfHour(byte &hr, format f)
{
    if (f == F24)
        bitClear(hr, 6);
    else
    {
        bitSet(hr, 6);
        if (f == FPM)
            bitSet(hr, 5);
        else
            bitClear(hr, 5);
    }
}

void gfHour(uint8_t &hr, format &f)
{
    bool b12h = bitRead(hr, 6);
    if (b12h)
    {
        f = (format)(bitRead(hr, 5) + 1);
        hr &= 0x1f;
    }
    else
        f = F24;
}

void DS1307::begin(void)
{
    Wire.begin();
}

void DS1307::readRam(uint8_t *buf, uint8_t adr, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        if ((i % 32) == 0)
        {
            Wire.beginTransmission(DS1307_ADDRESS);
            Wire.write(i + adr + ADDRESS_OFFSET);
            Wire.endTransmission();
            Wire.requestFrom(DS1307_ADDRESS, 32);
            while (Wire.available() == 0)
                ;
        }
        buf[i] = Wire.read();
    }
    Wire.endTransmission();
}

void DS1307::writeRam(uint8_t *buf, uint8_t adr, uint8_t size)
{

    for (uint8_t i = 0; i < size; i++)
    {
        if ((i % 31) == 0)
        {
            Wire.endTransmission();
            Wire.beginTransmission(DS1307_ADDRESS);
            Wire.write(i + adr + ADDRESS_OFFSET);
        }
        Wire.write(buf[i]);
    }
    Wire.endTransmission();
}

void DS1307::startSqw1hz(void)
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(7);
    Wire.write(0x10);
    Wire.endTransmission();
}

void DS1307::stopSqw1hz(void)
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(7);
    Wire.write(0x0);
    Wire.endTransmission();
}

void DS1307::setPartTime(addresses adr, uint8_t val, format f = F24)
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(adr);
    val = dec_to_bcd(val);
    if (adr == HOUR)
        sfHour(val, f);
    Wire.write(val);
    Wire.endTransmission();
}

void DS1307::getPartTime(addresses adr, uint8_t &val, format &f)
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(adr);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_ADDRESS, 1);
    while (Wire.available() == 0)
        ;
    val = Wire.read();
    if (adr == HOUR)
        gfHour(val, f);
    val = bcd_to_dec(val);
}

void DS1307::getPartTime(addresses adr, uint8_t &val)
{
    format f = F24;
    getPartTime(adr, val, f);
}

void DS1307::setTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, format f)
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0);
    Wire.write(dec_to_bcd(second));
    Wire.write(dec_to_bcd(minute));
    hour = dec_to_bcd(hour);
    sfHour(hour, f);
    Wire.write(hour);
    Wire.write(1); // day of week
    Wire.write(dec_to_bcd(day));
    Wire.write(dec_to_bcd(month));
    Wire.write(dec_to_bcd(year));
    Wire.endTransmission();
}

void DS1307::getTime(uint8_t &year, uint8_t &month, uint8_t &day, uint8_t &hour, uint8_t &minute, uint8_t &second, format &f)
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_ADDRESS, 7);
    while (Wire.available() == 0)
        ;
    second = bcd_to_dec(Wire.read());
    minute = bcd_to_dec(Wire.read());
    hour = Wire.read();
    gfHour(hour, f);
    hour = bcd_to_dec(hour);
    Wire.read(); // day of week
    day = bcd_to_dec(Wire.read());
    month = bcd_to_dec(Wire.read());
    year = bcd_to_dec(Wire.read());
}
