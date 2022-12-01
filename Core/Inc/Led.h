#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"

typedef enum {
	LED_ON = 1,
	LED_OFF = 2,
	LED_BLINK = 3
}LED_STATE;


class Led {
private:

	LED_STATE state;
	int peroid;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;

public:
    Led(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) :
    	state (LED_OFF),
		peroid(0),
		GPIOx(GPIOx),
		GPIO_Pin(GPIO_Pin)
    {
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
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


