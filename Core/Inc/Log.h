/*
 * Log.h
 *
 *  Created on: Nov 15, 2022
 *      Author: student
 */

#ifndef SRC_LOG_H_
#define SRC_LOG_H_

#include "main.h"
#include "DateTime.h"

class Log {
private:
	DateTime* _dateTime;
	char _fileName[20];
public:
	Log(const char* fileName);
	virtual ~Log();
	void writeToLog(int state, double temperature);
	void printLog();
	void clearLog();

};

#endif /* SRC_LOG_H_ */
