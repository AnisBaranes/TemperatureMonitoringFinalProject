
#include "project_init.h"
#include "CliCommand.h"
#include "Led.h"
#include "CommTask.h"
#include "Buzzer.h"
#include "Rtc.h"
#include "fatfs.h"
#include "Log.h"

extern TIM_HandleTypeDef htim3;

Rtc rtc = Rtc(&hi2c1, 0xD0);
Led led = Led(RED_LED_D12_GPIO_Port, RED_LED_D12_Pin);
Container container = {};
Buzzer buzzer = Buzzer(&htim3);
TemperatureSettings temperatureSettings;
Log log = Log("text.txt");
FATFS FatFs;

void myMain()
{
	//HAL_TIM_Base_Start(&htim1);

	 printf("---------- MENU ----------\r\n");
			  printf("1. Set Date & Time (DateTime [dd] / [mm] / [yy] / [hh] : [mm])\r\n");
			  printf("2. Set Warning Threshold (warning x)\r\n");
			  printf("3. Set Critical Threshold (critical x)\r\n");
			  printf("4. Print Log (PrintLog)\r\n");
			  printf("5. Clear Log (ClearLog)\r\n");

	container.cliInit();

	//Read from flash
	TemperatureSettings* data_ch = (TemperatureSettings*)(0x08080000);
	if (data_ch->magicNumber == 2591) {
		memcpy(&temperatureSettings, data_ch, sizeof(TemperatureSettings));
	}
	else {
		temperatureSettings.warningThreshold = 35;
		temperatureSettings.criticalThreshold = 40;
		temperatureSettings.magicNumber = 2591;
	}

	FRESULT fres = f_mount(&FatFs, "", 1); //1=mount now

}


