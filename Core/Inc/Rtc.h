/*
 * Rtc.h
 *
 *  Created on: Nov 15, 2022
 *      Author: student
 */

#ifndef SRC_RTC_H_
#define SRC_RTC_H_

#include "main.h"
#include "DateTime.h"

//	typedef struct DateTime_
//	{
//		int sec;
//		int min;
//		int hours;
//		int weekDay;
//		int day;
//		int month;
//		int year;
//	} DateTime;

class Rtc {

private:
	I2C_HandleTypeDef * hi2c;
	uint8_t devAddr;
	DateTime* dateTime;
public:
	Rtc(I2C_HandleTypeDef * hi2c, uint32_t devAddr);
	virtual ~Rtc();
	void rtcWrite(uint16_t memAddr, uint8_t * buffer, uint16_t size);
	void rtcRead(uint16_t memAddr, uint8_t * buffer, uint16_t size);
	void rtcStart();
	void rtcStop();
	int rtcIsRunning();
	//uint32_t rtcGetSeconds();

	void rtcGetTime();
	//uint32_t rtcGetSecondsAndPull(DateTime* dateTime);
	uint32_t dateTimeGetSeconds();
	void rtcSetTime(DateTime * dateTime);
};

#endif /* SRC_RTC_H_ */
