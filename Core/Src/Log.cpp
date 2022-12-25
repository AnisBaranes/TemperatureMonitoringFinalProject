/*
 * Log.cpp
 *
 *  Created on: Nov 15, 2022
 *      Author: student
 */
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "Log.h"
#include "fatfs.h"

Log::Log(const char* fileName) {
	strcpy(_fileName, fileName);
}

Log::~Log() {
	// TODO Auto-generated destructor stub
}

void Log::writeToLog(int state, double temperature){

	HAL_Delay(1000);

	FIL fil;
	FRESULT fres;

	BYTE readBuf[30];
	const char* msg;

	switch(state){
			case 1:
				//msg = strcat("Warning threshold raised: Severity Medium", (char*)(&temperature));
				msg = "Warning threshold raised: Severity Medium";
				break;
			case 2:
				msg = "Critical threshold raised: Severity High";
				break;
			case 3:
				msg = "Normal Temperature: Severity Low";
				break;
			default:
				msg = (char*)(&temperature);
				break;
		}


	fres = f_open(&fil, _fileName,  FA_OPEN_APPEND | FA_WRITE); //FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	if(fres == FR_OK) {
		//printf("I was able to open 'text.txt' for writing\r\n");
	} else {
		printf("f_open error (%i)\r\n", fres);
	}

	strncpy((char*)readBuf,msg, 19);
	UINT bytesWrote;
	fres = f_write(&fil, readBuf, 19, &bytesWrote);
	if(fres == FR_OK) {
		//printf("Wrote %i bytes to 'text.txt'!\r\n", bytesWrote);
	} else {
		printf("f_write error (%i)\r\n", fres);
	}

	f_close(&fil);
}

void Log::printLog() {

	char str[100];
	FIL fil;
	FRESULT fres;

	fres = f_open(&fil, _fileName, FA_READ);
	if(fres == FR_OK) {
		while (!f_eof(&fil))
		{
			f_gets(str, 100, &fil);
			printf("%s\r\n", str);
		}
	}
	else
	{
		printf("f_open error (%i)\r\n", fres);
	}
	f_close(&fil);
}

void Log::clearLog(){

	f_unlink(_fileName);
}
