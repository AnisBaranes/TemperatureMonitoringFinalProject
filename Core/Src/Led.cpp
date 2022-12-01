#include "Led.h"

 void Led::ledOn()
 {
	 state = LED_ON;
	 HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
 }

void Led::ledOff()
{
	state = LED_OFF;
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

void Led::ledBlink(int period)
{
	state = LED_BLINK;
	peroid = period;
}

void Led::ledRunBlink()
{
	if (state == LED_BLINK) {
		HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
	}
}

LED_STATE Led::getState(){
	return state;
}
void Led::setState(LED_STATE newState){
	state = newState;
}
