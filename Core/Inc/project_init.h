/*
 * project_init.h
 *
 *  Created on: Nov 24, 2022
 *      Author: student
 */

#ifndef INC_PROJECT_INIT_H_
#define INC_PROJECT_INIT_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

extern TIM_HandleTypeDef htim1;
extern I2C_HandleTypeDef hi2c1;

struct TemperatureSettings {
	double warningThreshold;
	double criticalThreshold;
	int magicNumber;
};


void myMain();
void cliInit();

#ifdef __cplusplus
}
#endif



#endif /* INC_PROJECT_INIT_H_ */
