/*
 * Rtc.cpp
 *
 *  Created on: Nov 15, 2022
 *      Author: student
 */

#include "Rtc.h"
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>

#define RTC_START_STOP      (1 << 7)
#define RTC_DATE_TIME_SIZE  7
#define DEVICE_ADDR         0xD0

const uint32_t DaysInYear    = 365;
const uint32_t SecondsInMin  = 60;
const uint32_t SecondsInHour = 3600;
const uint32_t SecondsInDay  = 86400;

static const int _daysUntilMonth[] = {
		0,
		31,
		59,
		90,
		120,
		151,
		181,
		212,
		243,
		273,
		304,
		334,
		365
};


Rtc::Rtc(I2C_HandleTypeDef * hi2c, uint32_t _devAddr)
{
	_hi2c = hi2c;
	_devAddr = _devAddr;
}

Rtc::~Rtc() {

}

void Rtc::rtcWrite(uint16_t memAddr, uint8_t * buffer, uint16_t size)
{
	HAL_StatusTypeDef returnVal = HAL_I2C_Mem_Write(_hi2c, _devAddr, memAddr, 1, buffer, size, 0xFF);
	if(returnVal){
		printf("Rtc::rtcWrite: ERR\r'n");
	}
}

void Rtc::rtcRead(uint16_t memAddr, uint8_t * buffer, uint16_t size)
{
	HAL_StatusTypeDef returnVal = HAL_I2C_Mem_Read(_hi2c, _devAddr, memAddr, 1, buffer, size, 0xFF);
	if(returnVal) {
		printf("Rtc::rtcRead: ERR\r'n");
	}
}

void Rtc::rtcStart()
{
	uint8_t sec = 0;
	HAL_StatusTypeDef returnVal;

	returnVal = HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	if(returnVal) {
		printf("Rtc::rtcStart: Read ERR\r'n");
	}

	sec &= ~RTC_START_STOP;
	returnVal = HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	if(returnVal) {
		printf("Rtc::rtcStart: Write ERR\r'n");
	}
}

void Rtc::rtcStop()
{
	uint8_t sec = 0;
	HAL_StatusTypeDef returnVal;

	returnVal = HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	if(returnVal) {
		printf("Rtc::rtcStop: Read ERR\r'n");
	}

	sec |= RTC_START_STOP;
	returnVal = HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	if(returnVal) {
		printf("Rtc::rtcStop: Write ERR\r'n");
	}
}

int Rtc::rtcIsRunning()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	return (sec & RTC_START_STOP) == 0;
}

static int bcdToInt(uint8_t bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static uint8_t intToBcd(int value, int minVal, int maxVal)
{
	if (value < minVal || value > maxVal) {
		return 0;
	}
	return ((value / 10) << 4) | (value % 10);
}

void Rtc::rtcGetTime()
{
	uint8_t buffer[RTC_DATE_TIME_SIZE];
	HAL_StatusTypeDef returnVal = HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF);
	if(returnVal) {
		printf("Rtc::rtcGetTime: Read ERR\r'n");
	}

	// remove stop bit if set
	buffer[0] &= ~RTC_START_STOP;
	_dateTime->_sec = bcdToInt(buffer[0]);
	_dateTime->_min = bcdToInt(buffer[1]);
	_dateTime->_hours = bcdToInt(buffer[2]);
	_dateTime->_weekDay = buffer[3] & 0x07;
	_dateTime->_day = bcdToInt(buffer[4]);
	_dateTime->_month = bcdToInt(buffer[5]);
	_dateTime->_year = bcdToInt(buffer[6]);
}


uint32_t Rtc::dateTimeGetSeconds()
{
	uint32_t seconds = _dateTime->_sec +
				_dateTime->_min * SecondsInMin +
				_dateTime->_hours * SecondsInHour +
				_dateTime->_day * SecondsInDay +
				_daysUntilMonth[_dateTime->_month - 1] * SecondsInDay +
				_dateTime->_year * DaysInYear * SecondsInDay;
	if (_dateTime->_year % 4 == 0 && _dateTime->_month > 2) {
		// if current year is a leap year and month is after February
		// add seconds for February 29
		seconds += SecondsInDay;
	}
	// add seconds for all previous leap years
	seconds += (_dateTime->_year / 4) * SecondsInDay;
	return seconds;
}

void Rtc::rtcSetTime(DateTime * _dateTime)
{
	uint8_t buffer[RTC_DATE_TIME_SIZE];

	buffer[0] = intToBcd(_dateTime->_sec, 0, 59);
	buffer[1] = intToBcd(_dateTime->_min, 0, 59);
	buffer[2] = intToBcd(_dateTime->_hours, 0, 59);
	buffer[3] = _dateTime->_weekDay < 1 || _dateTime->_weekDay > 7 ? 0 : _dateTime->_weekDay;
	buffer[4] = intToBcd(_dateTime->_day, 1, 31);
	buffer[5] = intToBcd(_dateTime->_month, 1, 12);
	buffer[6] = intToBcd(_dateTime->_year, 1, 99);

	HAL_StatusTypeDef returnVal = HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF);
	if(returnVal) {
		printf("Rtc::rtcSetTime: Write ERR\r'n");
	}
}





