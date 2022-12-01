/*
 * Buzzer.cpp
 *
 *  Created on: Nov 27, 2022
 *      Author: student
 */

#include "Buzzer.h"
#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


Buzzer::Buzzer(TIM_HandleTypeDef* _pwmTimer):pwmTimer(_pwmTimer) {
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	state = BUZ_STATE_OFF;
}

Buzzer::~Buzzer() {
	// TODO Auto-generated destructor stub
}


void Buzzer::buzzerStop()
{
	state = BUZ_STATE_OFF;

	HAL_TIM_Base_Stop(pwmTimer);
	HAL_TIM_PWM_Stop(pwmTimer, TIM_CHANNEL_1);
}

void Buzzer::buzzerPlayTone(int frequency, int timeMs)
{
	state = BUZ_STATE_TONE;

	int per = (100000 + frequency / 2) / frequency;
	__HAL_TIM_SET_COUNTER(pwmTimer, 0);
	__HAL_TIM_SET_AUTORELOAD(pwmTimer, per);
	__HAL_TIM_SET_COMPARE(pwmTimer, TIM_CHANNEL_1, (per /2));

	HAL_TIM_PWM_Start(pwmTimer, TIM_CHANNEL_1);
	HAL_TIM_Base_Start(pwmTimer);
}
