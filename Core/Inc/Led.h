#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"

enum LED_STATE{
	LED_ON = 1,
	LED_OFF = 2,
	LED_BLINK = 3
};


class Led {
private:

	LED_STATE _state;
	int _peroid;
	GPIO_TypeDef* _GPIOx;
	uint16_t _GPIO_Pin;

public:
    Led(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) :
    	_state (LED_OFF),
		_peroid(0),
		_GPIOx(GPIOx),
		_GPIO_Pin(GPIO_Pin)
    {
		HAL_GPIO_WritePin(_GPIOx, _GPIO_Pin, GPIO_PIN_RESET);
    }

    ~Led() {

    }

    void ledOn();

    void ledOff();

    void ledBlink(int period);

    void ledRunBlink();

    LED_STATE getState();

    void setState(LED_STATE newState);
};

#endif


