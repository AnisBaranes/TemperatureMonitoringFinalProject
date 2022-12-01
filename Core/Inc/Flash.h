/*
 * Flash.h
 *
 *  Created on: Nov 15, 2022
 *      Author: student
 */

#ifndef SRC_FLASH_H_
#define SRC_FLASH_H_

#include "main.h"
#include <stdio.h>


class Flash {

private:
	int FlashInterruptIndex;
	uint32_t * address;
	char data[64];
	uint16_t size;

public:
	Flash();
	virtual ~Flash();
	void flash_erase(int page);
	uint8_t flash_write(uint32_t* address, uint64_t *data, uint16_t size);
	void flash_print();
	void flash_eraseIT();
	void MyFlashInterruptHandler();
};

#endif /* SRC_FLASH_H_ */


