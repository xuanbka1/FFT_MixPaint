/*
 * test.h
 *
 *  Created on: Mar 8, 2024
 *      Author: ADMIN
 */

#ifndef USER_LIB_TEST_H_
#define USER_LIB_TEST_H_

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"

#include "my_debug.h"
#include "board_peripheral.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void test_microstep(void);

#endif /* USER_LIB_TEST_H_ */
