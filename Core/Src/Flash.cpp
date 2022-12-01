/*
 * Flash.cpp
 *
 *  Created on: Nov 15, 2022
 *      Author: student
 */

#include "Flash.h"

Flash::Flash() {
	FlashInterruptIndex = 0;
	address = (uint32_t *)(0x08080000);
	//data[64] = "Test of anis";
	size = 64;
}

Flash::~Flash() {

}


void Flash::flash_erase(int page)
{
	uint32_t page_error = 0;

	FLASH_EraseInitTypeDef earseInit;
	earseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	earseInit.Banks = FLASH_BANK_2;
	earseInit.Page = page; //256;
	earseInit.NbPages = 1;

	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&earseInit, &page_error);
	HAL_FLASH_Lock();

}

uint8_t Flash::flash_write(uint32_t* address, uint64_t *data, uint16_t size)
{
	HAL_FLASH_Unlock();
	for (int i = 0; i < size/8; i++)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ((uint32_t)address) + (i*8), data[i]) != HAL_OK)
			return 1;
	}
	return 0;
	HAL_FLASH_Lock();
}


void Flash::flash_print()
{
	char* data_ch = (char*)(0x08080000);
	printf("data = %s \r\n", data_ch);
}

void Flash::flash_eraseIT()
{
	HAL_FLASH_Unlock();

	FLASH_EraseInitTypeDef earseInit;
	earseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	earseInit.Banks = FLASH_BANK_2;
	earseInit.Page = 256;
	earseInit.NbPages = 1;

	HAL_FLASHEx_Erase_IT(&earseInit);
	//HAL_FLASH_Lock();
}

void Flash::MyFlashInterruptHandler()
{
	if(FlashInterruptIndex <= 64)
	{
		HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_DOUBLEWORD, ((uint32_t)address) + FlashInterruptIndex , *(uint64_t*)(data + FlashInterruptIndex));
		FlashInterruptIndex += sizeof(uint64_t);
	}

}
