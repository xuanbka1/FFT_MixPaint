/*
 * my_debug.h
 *
 *  Created on: Feb 13, 2023
 *      Author: Mai Ba
 *
 *  edit by Xuan@22112023
 */

#ifndef MY_DEBUG_H_
#define MY_DEBUG_H_

#include "stm32f1xx_hal.h"
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <stdbool.h>

#define delay_ms HAL_Delay

#define DEBUG_EN

typedef struct
{
	char char_data_rec;
	char arr_data_rec[300];
	bool data_done_frame;
	uint16_t arr_data_index;
	bool en_get_char_data;
}Uart_debug;

extern Uart_debug my_debug;
extern Uart_debug my_modbus;

bool trim(char *source,char *des,char key_start[30],char key_stop[30]);
bool trim_with_length(char *source,char *des,char key_start[30],unsigned int length);


#endif /* USER_LIB_MY_UTILITIES_MY_DEBUG_H_ */
