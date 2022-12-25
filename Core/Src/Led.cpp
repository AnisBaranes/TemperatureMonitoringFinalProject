#include "Led.h"

 void Led::ledOn()
 {
	 _state = LED_ON;
	 HAL_GPIO_WritePin(_GPIOx, _GPIO_Pin, GPIO_PIN_SET);
 }

void Led::ledOff()
{
	_state = LED_OFF;
	HAL_GPIO_WritePin(_GPIOx, _GPIO_Pin, GPIO_PIN_RESET);
}

void Led::ledBlink(int period)
{
	_state = LED_BLINK;
	_peroid = period;
}

void Led::ledRunBlink()
{
	if (_state == LED_BLINK) {
		HAL_GPIO_TogglePin(_GPIOx, _GPIO_Pin);
	}
}

LED_STATE Led::getState(){
	return _state;
}
void Led::setState(LED_STATE newState){
	_state = newState;
}
