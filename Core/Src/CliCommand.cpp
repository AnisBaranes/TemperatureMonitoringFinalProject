#include <iostream>
#include <cstdlib>
#include "CliCommand.h"
#include "Led.h"
#include "project_init.h"
#include "Rtc.h"
#include "Log.h"

extern Led led;
extern TemperatureSettings temperatureSettings;
extern Rtc rtc;
extern Log log;

class LedOn: public CliCommand {
private:
	Led* _led;
public:
	LedOn(Led* led){
		_led = led;
	}

	void doCommand(const char* param){
		_led-> ledOn();
	}

};

class WarningTempThreshold: public CliCommand {
private:
	TemperatureSettings* _temperatureSettings;
public:
	WarningTempThreshold(TemperatureSettings* temperatureSettings){
		_temperatureSettings = temperatureSettings;
		_temperatureSettings->magicNumber = 2591;
	}

	void doCommand(const char* param){
		_temperatureSettings->warningThreshold = atof(param);
	}
};

class CriticalTempThreshold: public CliCommand {
private:
	TemperatureSettings* _temperatureSettings;
public:
	CriticalTempThreshold(TemperatureSettings* temperatureSettings){
		_temperatureSettings = temperatureSettings;
		_temperatureSettings->magicNumber = 2591;
	}

	void doCommand(const char* param){
		_temperatureSettings->criticalThreshold = atof(param);
	}
};

class SetDateTimeRTC: public CliCommand {
private:
	Rtc* _rtc;
public:
	SetDateTimeRTC(Rtc* rtc){
		_rtc = rtc;
	}

	void doCommand(const char* param){
		DateTime _dateTime = DateTime(0,0,0,0,0,0,0);
		sscanf(param, "%d/%d/%d-%d:%d", &_dateTime._day, &_dateTime._month, &_dateTime._year, &_dateTime._hours, &_dateTime._min);
		_rtc->rtcSetTime(&_dateTime);
	}
};

class PrintLog: public CliCommand {
private:
	Log* _log;
public:
	PrintLog(Log* log){
		_log = log;
	}

	void doCommand(const char* param){
		_log ->printLog();
	}
};

class ClearLog: public CliCommand {
private:
	Log* _log;
public:
	ClearLog(Log* log){
		_log = log;
	}

	void doCommand(const char* param){
		_log ->clearLog();
	}
};

void Container::cliInit()
{
	registerCommand("led-on", new LedOn(&led));
	registerCommand("warning", new WarningTempThreshold(&temperatureSettings));
	registerCommand("critical", new CriticalTempThreshold(&temperatureSettings));
	registerCommand("DateTime", new SetDateTimeRTC(&rtc));
	registerCommand("PrintLog", new PrintLog(&log));
	registerCommand("ClearLog", new PrintLog(&log));
}


