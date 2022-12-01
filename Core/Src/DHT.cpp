/*
 * DHT.cpp
 *
 *  Created on: 20 Nov 2022
 *      Author: student
 */

#include "DHT.h"
#include "main.h"
#include "cmsis_os.h"

#define MAX_BITS 40

DHT::DHT(GPIO_TypeDef * _gpioPort, uint16_t _gpioPin, TIM_HandleTypeDef * _timer):
gpioPort(_gpioPort),
gpioPin (_gpioPin),
timer (_timer),
counter (0),
maxCounter (0),
state (DHT_STATE_NO_DATA),
temperature (0.0),
humidity (0.0)
{

}

DHT::~DHT() {
	// TODO Auto-generated destructor stub
}


void DHT::setGpioOutput()
{
	GPIO_InitTypeDef gpioStruct = {0};

	gpioStruct.Pin = gpioPin;
	gpioStruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioStruct.Pull = GPIO_NOPULL;
	gpioStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(gpioPort, &gpioStruct);
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
}

void DHT::setGpioInput()
{
	GPIO_InitTypeDef gpioStruct = {0};

	gpioStruct.Pin = gpioPin;
	gpioStruct.Mode = GPIO_MODE_INPUT;
	gpioStruct.Pull = GPIO_NOPULL;
	gpioStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(gpioPort, &gpioStruct);
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
}

void DHT::setGpioExti()
{
	GPIO_InitTypeDef gpioStruct = {0};

	gpioStruct.Pin = gpioPin;
	gpioStruct.Mode = GPIO_MODE_IT_FALLING;
	gpioStruct.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(gpioPort, &gpioStruct);

	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void DHT::Dht_readAsync()
{
	setGpioOutput();

	HAL_GPIO_WritePin(gpioPort, gpioPin, GPIO_PIN_RESET);
	osDelay(20); //18
	HAL_GPIO_WritePin(gpioPort, gpioPin, GPIO_PIN_SET);
	state = DHT_STATE_POWER_ON_ACK;
	HAL_TIM_Base_Start(timer);
	//state = DHT_STATE_POWER_ON;

	__HAL_TIM_SET_COUNTER(timer, 0);
	setGpioExti();

}

void DHT::Dht_onGpioInterrupt(uint16_t pin)
{
	if (gpioPin != pin) {
		return;
	}

	uint32_t timeMs = __HAL_TIM_GET_COUNTER(timer);

	switch (state)
	{
	case DHT_STATE_POWER_ON_ACK:
		if (timeMs > 50) {
			state = DHT_STATE_ERROR;
		}
		state = DHT_STATE_INIT_RESPONSE;
		break;

	case DHT_STATE_INIT_RESPONSE:
		if (timeMs > 200) {
			state = DHT_STATE_ERROR;
		}

		memset(data, 0, sizeof(data));
		bit = 0;
		state = DHT_STATE_RECEIVE_DATA;

		break;

	case DHT_STATE_RECEIVE_DATA:
		{
			if (timeMs > 140) {
				//state = DHT_STATE_ERROR;
				break;
			}

			// 50us in low + 50 us in high (> 30 and < 70)
			int byte = bit / 8;
			data[byte] <<= 1;

			if (timeMs > 100) {
				// '1' is detected
				data[byte] |= 1;
			}

			bit++;
			if (bit >= MAX_BITS) {

				uint8_t checksum = data[0] + data[1] +
						data[2] + data[3];

				if (checksum == data[4]) {
					state = DHT_STATE_READY;

					humidity = (double)data[0] + ((double)data[1]) / 10;
					temperature = (double)data[2] + ((double)data[3]) / 10;
				}
				else {
					state = DHT_STATE_ERROR;
				}

				// stop timer and disable GPIO interrupts
				HAL_TIM_Base_Stop(timer);
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);

			}

		}
		break;

	default:
		// in all other states ignore the interrupt
		break;
	}

	__HAL_TIM_SET_COUNTER(timer, 0);
}

int DHT::Dht_hasData()
{
	int hasData = state == DHT_STATE_READY;

	if (hasData) {
		// reset state to avoid multiple reads
		state = DHT_STATE_NO_DATA;
	}
	return hasData;
}

double DHT::Dht_getHumidty()
{
	return humidity;
}

double DHT::Dht_getTempperature()
{
	return temperature;
}
