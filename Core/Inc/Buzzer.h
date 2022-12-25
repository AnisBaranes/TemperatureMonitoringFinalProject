/*
 * Buzzer.h
 *
 *  Created on: Nov 27, 2022
 *      Author: student
 */

#ifndef SRC_BUZZER_H_
#define SRC_BUZZER_H_

#include "main.h"

enum BuzzerState
{
	BUZ_STATE_OFF,
	BUZ_STATE_TONE,
	BUZ_STATE_MUSIC,
	BUZ_RESET //TODO: complete later
};

class Buzzer {
private:
	BuzzerState _state;
	TIM_HandleTypeDef* _pwmTimer;

public:
	Buzzer(TIM_HandleTypeDef* _pwmTimer);
	virtual ~Buzzer();

	void buzzerStop();
	void buzzerPlayTone(int frequency, int timeMs);
};

#endif /* SRC_BUZZER_H_ */



/*
 * Buzzer.h
 */


