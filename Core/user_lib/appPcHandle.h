/*
 * appPcHandle.h
 *
 *  Created on: Jun 14, 2023
 *      Author: Mai Ba
 */

#ifndef _APPPCHANDLE_H_
#define _APPPCHANDLE_H_

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"

#include "my_debug.h"
#include "board_peripheral.h"
/*
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern int LOG(const char *fmt,...);


#define FIND_AND_NUL(s, p, c) ( \
   (p) = strchr(s, c), \
   *(p) = '\0', \
   ++(p), \
   (p))



#define APP_FRAME_START '@'
#define APP_FRAME_STOP '#'
extern uint8_t comOpen;

#define DISABLE_TIMER_PHA_MAU       TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN))
#define ENABLE_TIMER_PHA_MAU        TIM3->CR1 |= TIM_CR1_CEN

#define DISABLE_TIMER_DOOR    		TIM5->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN))
#define ENABLE_TIMER_DOOR     		TIM5->CR1 |= TIM_CR1_CEN

#define DISABLE_TIMER_ALARM    		TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN))
#define ENABLE_TIMER_ALARM     		TIM2->CR1 |= TIM_CR1_CEN




#define read_sensor1 	HAL_GPIO_ReadPin(SENSOR1_GPIO_Port, SENSOR1_Pin)   // sensor 1
#define read_sensor2 	HAL_GPIO_ReadPin(SENSOR2_GPIO_Port, SENSOR2_Pin)   // sensor 2
#define read_sensor3 	HAL_GPIO_ReadPin(SENSOR3_GPIO_Port, SENSOR3_Pin)   // sensor 3

typedef struct appPc
{
	char charRec;
	bool dataDoneFrame;
	bool enReadFrame;
	uint16_t countCharRec;
	char arrData[500];
	char timeUpdateStr[20];

	uint32_t countTimeUpdateDate;
	bool appRequestReadData;
	uint32_t timeUpdateData;
	void (*hanldeAppRequets)(void);
	void (*handleSendToApp)(void);
	void (*send)(uint8_t selectCom,const char *fmt, ...);
}App;

extern App myApp;

extern bool sttSensor1;
extern bool sttSensorLast1;
extern bool sttSensor2;
extern bool sttSensorLast2;
extern bool sttSensor3;
extern bool sttSensorLast3;

void uartAppHandle(App *myUart);
void SendToApp(uint8_t selectCom,const char *fmt, ...);
void uartAppClear(App *myUart);
void handleAppRequets();
void SendToBLE(const char *fmt, ...);
void allPumpIsRun();
void allPumpIsSleep();
void setValue(pump *temp,float ml,uint16_t pulse1ml,uint16_t pulse01ml,uint16_t pulse001ml);


void On_Timer_Pha(void);


#endif
