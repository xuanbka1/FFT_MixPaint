/*
 * test.c
 *
 *  Created on: Mar 8, 2024
 *      Author: ADMIN
 */


#include "stdio.h"
#include "appPcHandle.h"
#include "my_debug.h"
#include "board_peripheral.h"
#include "eeprom_24c32.h"
#include "driver_rtc.h"
#include "notifyHandler.h"

#include "test.h"

void test_microstep(void)
{
	uint8_t index = 0;
	uint32_t limit[16] = {0};
	uint32_t counter[16] = {0};
	//SET(bomMau[0].DIR_Port, bomMau[0].DIR_Pin);
	for (index = 0; index < 16; index++)
		CLEAR(bomMau[index].EN_Port, bomMau[index].EN_Pin);


	limit[0] = 1000000;
	limit[1] = 2000000;
	while (1)
	{
		for (uint32_t i = 0; i < 100; i++)
		{
			__NOP();
		}
		for (index = 0; index < 16; index++)
		{
			counter[index]++;

			if(counter[index] < limit[index])
			{
			//HAL_GPIO_TogglePin(bomMau[index].DIR_Port, bomMau[index].DIR_Pin);
			HAL_GPIO_WritePin(bomMau[index].DIR_Port, bomMau[index].DIR_Pin, 0);
			HAL_GPIO_TogglePin(bomMau[index].GPIOx, bomMau[index].GPIO_Pin);
			}
			else
			{
				SET(bomMau[index].EN_Port, bomMau[index].EN_Pin);
			}
		}
	}
}
