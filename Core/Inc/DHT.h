/*
 * DHT.h
 *
 *  Created on: 20 Nov 2022
 *      Author: student
 */

#ifndef SRC_DHT_H_
#define SRC_DHT_H_

#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

enum DhtState
	{
		DHT_STATE_NO_DATA,
		DHT_STATE_READY,
		DHT_STATE_ERROR,
		DHT_STATE_POWER_ON,
		DHT_STATE_POWER_ON_ACK,
		DHT_STATE_INIT_RESPONSE,
		DHT_STATE_RECEIVE_DATA
	};


class DHT {
private:
	DhtState _dhtState{};
	GPIO_TypeDef * _gpioPort;
	uint16_t _gpioPin;
	TIM_HandleTypeDef * _timer;
	int _counter;
	int _maxCounter;
	int _state;
	uint8_t _data[5];
	int _bit;
	double _temperature;
	double _humidity;
public:
	DHT(GPIO_TypeDef * gpioPort, uint16_t gpioPin, TIM_HandleTypeDef * timer);
	virtual ~DHT();
	void setGpioOutput();
	void setGpioInput();
	void setGpioExti();
	void Dht_onGpioInterrupt(uint16_t pin);
	void Dht_readAsync();
	int Dht_hasData();
	double Dht_getHumidty();
	double Dht_getTempperature();

};

#endif /* SRC_DHT_H_ */
