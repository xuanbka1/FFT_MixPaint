/*
 * buzzer.c
 *
 *  Created on: Dec 1, 2023
 *      Author: ADMIN
 */

#include "buzzer.h"
#include "main.h"

static uint16_t beep = 0;
static uint16_t loop = 0;

uint8_t checkBuzzerOff = 0;
uint32_t timeBuzzer;

/*******************************************************************************************
 * @brief  buzzer_configuration: config gpio of buzzer
 * @param  None
 * @return None
 * @note   None
 ********************************************************************************************/
void buzzer_configuration(void)
{
	HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, 0);
	beep = 0;
	loop = 0;
}

/*******************************************************************************************
 * @brief  buzzer_on: turn on buzzer
 * @param  None
 * @return None
 * @note   None
 ********************************************************************************************/
void buzzer_on(void)
{
	HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, 1);
}

/*******************************************************************************************
 * @brief  buzzer_off: turn off buzzer
 * @param  None
 * @return None
 * @note   None
 ********************************************************************************************/
void buzzer_off(void)
{
	HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, 0);
}

/*******************************************************************************************
 * @brief  buzzer_toggle: when buzzer is enabled, buzzer beep once times with duration is BEEP_TIME
 * @param  None
 * @return None
 * @note   None
 ********************************************************************************************/

void buzzer_toggle(void)
{
	if (beep == 1)
	{
		timeBuzzer = HAL_GetTick();
		buzzer_on();
		beep = 0;
		checkBuzzerOff = 1;
	}

	if (checkBuzzerOff == 1)
	{
		if (HAL_GetTick() - timeBuzzer > BEEP_TIME)
		{
			buzzer_off();
			checkBuzzerOff = 0;
		}
	}

	if (loop > 0)
	{
		if (HAL_GetTick() - timeBuzzer > 3 * BEEP_TIME)
		{
			loop--;
			beep = 1;
		}
	}

}

void buzzer_set(uint16_t _beep_time)
{
	beep = _beep_time;
}

void buzzer_loop(uint16_t loop_num)
{
	loop = loop_num;
}


