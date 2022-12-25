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

DHT::DHT(GPIO_TypeDef * gpioPort, uint16_t gpioPin, TIM_HandleTypeDef * timer):
_gpioPort(gpioPort),
_gpioPin (gpioPin),
_timer (timer),
_counter (0),
_maxCounter (0),
_state (DHT_STATE_NO_DATA),
_temperature (0.0),
_humidity (0.0)
{

}

DHT::~DHT() {
	// TODO Auto-generated destructor stub
}


void DHT::setGpioOutput()
{
	GPIO_InitTypeDef gpioStruct = {0};

	gpioStruct.Pin = _gpioPin;
	gpioStruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioStruct.Pull = GPIO_NOPULL;
	gpioStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(_gpioPort, &gpioStruct);
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
}

void DHT::setGpioInput()
{
	GPIO_InitTypeDef gpioStruct = {0};

	gpioStruct.Pin = _gpioPin;
	gpioStruct.Mode = GPIO_MODE_INPUT;
	gpioStruct.Pull = GPIO_NOPULL;
	gpioStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(_gpioPort, &gpioStruct);
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
}

void DHT::setGpioExti()
{
	GPIO_InitTypeDef gpioStruct = {0};

	gpioStruct.Pin = _gpioPin;
	gpioStruct.Mode = GPIO_MODE_IT_FALLING;
	gpioStruct.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(_gpioPort, &gpioStruct);

	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void DHT::Dht_readAsync()
{
	setGpioOutput();

	HAL_GPIO_WritePin(_gpioPort, _gpioPin, GPIO_PIN_RESET);
	osDelay(20); //18
	HAL_GPIO_WritePin(_gpioPort, _gpioPin, GPIO_PIN_SET);
	_state = DHT_STATE_POWER_ON_ACK;
	HAL_TIM_Base_Start(_timer);
	//state = DHT_STATE_POWER_ON;

	__HAL_TIM_SET_COUNTER(_timer, 0);
	setGpioExti();

}

void DHT::Dht_onGpioInterrupt(uint16_t pin)
{
	if (_gpioPin != pin) {
		return;
	}

	uint32_t timeMs = __HAL_TIM_GET_COUNTER(_timer);

	switch (_state)
	{
	case DHT_STATE_POWER_ON_ACK:
		if (timeMs > 50) {
			_state = DHT_STATE_ERROR;
		}
		_state = DHT_STATE_INIT_RESPONSE;
		break;

	case DHT_STATE_INIT_RESPONSE:
		if (timeMs > 200) {
			_state = DHT_STATE_ERROR;
		}

		memset(_data, 0, sizeof(_data));
		_bit = 0;
		_state = DHT_STATE_RECEIVE_DATA;

		break;

	case DHT_STATE_RECEIVE_DATA:
		{
			if (timeMs > 140) {
				//state = DHT_STATE_ERROR;
				break;
			}

			// 50us in low + 50 us in high (> 30 and < 70)
			int byte = _bit / 8;
			_data[byte] <<= 1;

			if (timeMs > 100) {
				// '1' is detected
				_data[byte] |= 1;
			}

			_bit++;
			if (_bit >= MAX_BITS) {

				uint8_t checksum = _data[0] + _data[1] +
						_data[2] + _data[3];

				if (checksum == _data[4]) {
					_state = DHT_STATE_READY;

					_humidity = (double)_data[0] + ((double)_data[1]) / 10;
					_temperature = (double)_data[2] + ((double)_data[3]) / 10;
				}
				else {
					_state = DHT_STATE_ERROR;
				}

				// stop timer and disable GPIO interrupts
				HAL_TIM_Base_Stop(_timer);
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);

			}

		}
		break;

	default:
		// in all other states ignore the interrupt
		break;
	}

	__HAL_TIM_SET_COUNTER(_timer, 0);
}

int DHT::Dht_hasData()
{
	int hasData = _state == DHT_STATE_READY;

	if (hasData) {
		// reset state to avoid multiple reads
		_state = DHT_STATE_NO_DATA;
	}
	return hasData;
}

double DHT::Dht_getHumidty()
{
	return _humidity;
}

double DHT::Dht_getTempperature()
{
	return _temperature;
}
