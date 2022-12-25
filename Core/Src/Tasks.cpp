#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "Tasks.h"
#include "Rtc.h"
#include "Flash.h"
#include "DateTime.h"
#include "DHT.h"
#include "Led.h"
#include "Buzzer.h"
#include "project_init.h"
#include "CliCommand.h"
#include "CommTask.h"
#include "fatfs.h"
#include "Log.h"

using namespace std;

extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim1;
extern Led led;
extern Buzzer buzzer;
extern Log logFile;
extern Log tempLogFile;
extern TemperatureSettings temperatureSettings;
extern double warningThreshold;
extern double criticalThreshold;

DHT dht = DHT(GPIOB, GPIO_PIN_5, &htim1);
Flash flash = Flash();


double curTemperature = 24;
int isBtnPressed = 0;


extern "C" int _write(int fd, char* ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t *) ptr, len, HAL_MAX_DELAY);
    return len;
}


extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	dht.Dht_onGpioInterrupt(pin);

	if(pin == D2_BTN_Pin)
	{
		if(HAL_GPIO_ReadPin(D2_BTN_GPIO_Port, D2_BTN_Pin)){
			isBtnPressed = 1;
			buzzer.buzzerStop();
		}
		else{
			isBtnPressed = 0;
		}
	}
}

extern "C" void StartDefaultTask(void *argument)
{
 for(;;)
 {
	 osDelay(10);
 }
}

extern "C" void CheckTemeratureTask(void *argument)
{
  for(;;)
  {
	 dht.Dht_readAsync();
	 osDelay(50);
	 if (dht.Dht_hasData())
	 {
		 curTemperature = dht.Dht_getTempperature();
	 }
	 osDelay(950);
  }
}

extern "C" void LedRunBlinkTask(void *argument)
{
  for(;;)
  {
	  led.ledRunBlink();
	  osDelay(700);
  }
}

extern "C" void CommTask(void *argument)
{
  for(;;)
  {
	  if(commTask()) {
		  printf("---------- MENU ----------\r\n");
		  printf("1. Set Date & Time (DateTime [dd] / [mm] / [yy] - [hh] : [mm])\r\n");
		  printf("2. Set Warning Threshold (warning x)\r\n");
		  printf("3. Set Critical Threshold (critical x)\r\n");
		  printf("4. Print Log (PrintLog)\r\n");
		  printf("5. Clear Log (ClearLog)\r\n");

		  handleCommand();

		  uint32_t * address = (uint32_t *)(0x08080000);
		  flash.flash_erase(256); //(address);
		  uint64_t * data = (uint64_t *)&temperatureSettings;
		  flash.flash_write(address, data, sizeof(temperatureSettings));
	  }
  }
}

extern "C" void MonitorTemperatureTask(void *argument)
{
 for(;;)
 {
	 if(!isBtnPressed && (curTemperature > temperatureSettings.warningThreshold) && (curTemperature < temperatureSettings.criticalThreshold))
	 {
		 led.ledOn();
		 logFile.writeToLog(1, curTemperature);
	 }
	 if(!isBtnPressed && (curTemperature > temperatureSettings.criticalThreshold))
	 {
		 led.ledBlink(700);
		 buzzer.buzzerPlayTone(360, 180);
		 logFile.writeToLog(2, curTemperature);
	 }
	 if(curTemperature < temperatureSettings.warningThreshold)
	 {
		 led.ledOff();
		 buzzer.buzzerStop();
		 isBtnPressed = 0;
		 logFile.writeToLog(3, curTemperature);
	 }
	 osDelay(10);
 }
}

extern "C" void TemperatureLogTask(void *argument)
{
	for(;;)
	{
		tempLogFile.writeToLog(0, curTemperature);
		osDelay(60000);
	}
}

