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
	int _FlashInterruptIndex;
	uint32_t * _address;
	char _data[64];
	uint16_t _size;
	uint32_t _pageSize;
	uint32_t _pagesInBank;

public:
	Flash();
	virtual ~Flash();
	void flash_erase(int page); //(uint32_t * address);
	uint8_t flash_write(uint32_t* address, uint64_t *data, uint16_t size);
	void flash_print();
	void flash_eraseIT();
	void MyFlashInterruptHandler();
};

#endif /* SRC_FLASH_H_ */


