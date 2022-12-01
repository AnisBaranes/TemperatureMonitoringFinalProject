/*
 * CommTask.c
 */

#include "main.h"
#include "CommTask.h"
#include "CliCommand.h"
#include <string.h>
#include <stdio.h>

#define MAX_BUFFER_LENGTH 50
#define MAX_COMMANDS      20

extern UART_HandleTypeDef huart2;
extern Container container;

uint8_t cmdbuffer[MAX_BUFFER_LENGTH];
int cmdcount = 0;
int cmdprint = 0;


int commTask() {
	uint8_t ch;

	HAL_StatusTypeDef Status = HAL_UART_Receive(&huart2, &ch, 1, 10);
	if (Status != HAL_OK) {
		if ((huart2.Instance->ISR & USART_ISR_ORE) != 0) {
			__HAL_UART_CLEAR_OREFLAG(&huart2);
		}

		// here we have a time to print the command
		while (cmdprint < cmdcount) {
			HAL_UART_Transmit(&huart2, &cmdbuffer[cmdprint++], 1, 0xFFFF);
		}

		return 0;
	}

	if (ch != '\r' && ch != '\n') {
		//HAL_UART_Transmit(&huart2, &ch, 1, 0xFFFF);

		if (cmdcount >= MAX_BUFFER_LENGTH) {
			cmdcount = 0;
			cmdprint = 0;
		}

		cmdbuffer[cmdcount++] = ch;
		return 0;
	}

	// here we have a time to print the command
	while (cmdprint < cmdcount) {
		HAL_UART_Transmit(&huart2, &cmdbuffer[cmdprint++], 1, 0xFFFF);
	}

	HAL_UART_Transmit(&huart2, (uint8_t*) "\r\n", 2, 0xFFFF);

	cmdbuffer[cmdcount] = 0;
	cmdcount = 0;
	cmdprint = 0;
	return 1;
}

void handleCommand() {
	char cmd[20];
	char param[50];
	sscanf((const char*) cmdbuffer, "%s %s", cmd, param);

	container.doCommand(cmd,param);
}
