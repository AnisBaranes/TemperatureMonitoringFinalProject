/*
 * DateTime.h
 *
 *  Created on: Nov 15, 2022
 *      Author: student
 */

#ifndef SRC_DATETIME_H_
#define SRC_DATETIME_H_

class DateTime {
public:
	int _sec;
	int _min;
	int _hours;
	int _weekDay;
	int _day;
	int _month;
	int _year;

	DateTime(int sec, int min, int hours, int weekDay, int day, int month, int year):
		_sec(sec), _min(min), _hours(hours), _weekDay(weekDay), _day(day), _month(month), _year(year){};
	virtual ~DateTime(){};
};

#endif /* SRC_DATETIME_H_ */


