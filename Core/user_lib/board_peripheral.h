/*
 * board_peripheral.h
 *
 *  Created on: Jul 8, 2023
 *      Author: Mai Ba
 */

#ifndef _BOARD_PERIPHERAL_H_
#define _BOARD_PERIPHERAL_H_

#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

//#include "_timeval.h"
#define DISABLE_TIMER_TRON    		TIM6->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN)); \
									HAL_TIM_Base_Stop_IT(&htim6);

#define ENABLE_TIMER_TRON     		TIM6->CR1 |= TIM_CR1_CEN ; \
									HAL_TIM_Base_Start_IT(&htim6);


#define out(port,pin,stt)     HAL_GPIO_WritePin(port,pin,stt)

#define SET(port,pin)         HAL_GPIO_WritePin(port, pin, 1)
#define CLEAR(port,pin)       HAL_GPIO_WritePin(port,pin, 0)

#define GPIO_WRITE(port,pin,stt)     HAL_GPIO_WritePin(port,pin,stt)

extern uint32_t dataBom;
extern uint32_t dataTron;

#define THUAN 1
#define NGUOC 0
#define SLEEP 0
#define RUN 1
#define on 1
#define off 0

#define ON     1
#define OFF    0


#define EN_DEBUG

typedef struct
{
	 uint8_t gio;
	 uint8_t phut;
	 uint8_t giay;
	 uint8_t en;      // trang thai hen gio
	 uint8_t time;    //thời gian khuấy màu - interval //
	 uint32_t countTime;
	 uint32_t time_in_sec;

	struct address_t
	{
		uint16_t gio;
		uint16_t phut;
		uint16_t giay;
		uint16_t time;
		uint16_t en;    // dia chi luu trang thai hen gio //
	}addr;
}mytime_t;

extern mytime_t timeNow;
extern mytime_t timeAlarm[10];

#define THAP 1
#define TRUNGBINH 2
#define CAO 3

#define KHUAYMAU_THAP 13
#define KHUAYMAU_TB 7
#define KHUAYMAU_CAO 3

typedef struct
{
	struct khoiDongKhuayMau_t
	{
		mytime_t time;
		volatile bool en;
	}khoiDongKhuayMau;

	struct khuayMau_t
	{
		mytime_t time;
		volatile bool en;
	}khuayMau;

	struct luongMauPhunRa_t
	{
		uint16_t value;
		uint16_t address;
	}luongMauPhunRa;

	struct tocDoKhuayMau_t
	{
		uint16_t value;
	 	uint16_t address;
	}tocDoKhuayMau;


	struct tocDoBomVeSinh_t
	{
		uint16_t value;
		uint16_t address;
	}tocDoBomVeSinh;

	bool veSinhDauPhun;
	uint32_t countTimeKhuayMau;
	uint32_t countTimeNghiKhuayMau;
	bool enTimeKhuayMau;

	struct tocDoKhuayMau_t doRongXung;
	struct tocDoKhuayMau_t levelThap;
	struct tocDoKhuayMau_t levelTrungBinh;
	struct tocDoKhuayMau_t levelCao;
}khuayMau_t;


extern khuayMau_t khuayMau;
#define EN_TRON 0xffffffff
#define DIS_TRON 0

//extern uint16_t pulse_1ml_addr;
//extern uint16_t pulse_01ml_addr;
//extern uint16_t pulse_001ml_addr;
extern uint8_t pump_index;
extern uint16_t gocDongNap;
extern uint16_t gocMoNap;
extern uint16_t gocDongNap_addr;
extern uint16_t gocMoNap_addr;
extern uint16_t doRongXungBomMau_addr;
extern uint32_t pulse_1ml;
extern uint32_t pulse_01ml;
extern uint32_t pulse_001ml;


typedef struct
{
	 bool dir;
	 bool sleep;
	  uint32_t pulseSetHighSpeed;
	  uint32_t pulseSetLowSpeed;
	  uint32_t countHighSpeed;
	  uint32_t countLowSpeed;
	  float mililit;

	  uint32_t countDelay;
	  uint32_t pulse1ml;
	  uint32_t pulse01ml;
	  uint32_t pulse001ml;

	 uint16_t pulse1mlAddress;
	 uint16_t pulse01mlAddress;
	 uint16_t pulse001mlAddress;

	 uint16_t Ton;
	 uint32_t Counter_Ton;

	 uint16_t delayLowSpeed;


	 GPIO_TypeDef  *GPIOx;
	 uint16_t       GPIO_Pin;

	 /* xuan add */
	 GPIO_TypeDef *DIR_Port;
	 uint16_t      DIR_Pin;

	 GPIO_TypeDef *EN_Port;
	 uint16_t      EN_Pin;

}pump;

extern volatile pump bomMau[16];
extern uint16_t startAddressPumpBomMau;
extern uint32_t buff595pump;

typedef struct
{
   bool en;
   uint8_t slowLevel;
   uint8_t countSlow;
   uint8_t countT;
   uint8_t Tset;
}tron;
extern volatile tron bomTron;
extern volatile tron dayNap;

typedef struct
{
	bool en;
	uint16_t countPulse;
	uint16_t pulseSet;
	uint16_t pulseOpen;
	uint16_t pulseClose;
}dongNap_t;

extern volatile dongNap_t dongNap;

extern float num1;
extern uint16_t num2, num3, num4, num5;
extern bool khuayMauTrucTiep;

void DoorInit(uint16_t value);
void DoorTest(uint16_t interval);
void CheckTimeNow(void);
void AlarmTask(void);
void AlarmOff(void);


void pump_init();
void rs485_print(const char *fmt, ...);
void rs232_print(const char *fmt, ...);
void sendFrame(const char *fmt, ...);
void writeByte595_bom(uint32_t data,bool enOut);
void writeByte595_tron(uint32_t data,bool enOut);
void write4Byte595_bom(uint32_t data,bool enOut);
void write4Byte595_tron(uint32_t data,bool enOut);
void setPump(pump *Pump,uint8_t speed,bool dir,bool sleep);
void pwmPump(pump *Pump,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void out_pulse(uint16_t soXung);
void delay_us(uint32_t microseconds);
void pump_pwm_handle();
void check_dir_sleep_Colorpump();
void outputPwmColorPump(pump *pumpTemp, uint8_t index);
void outputPwmColorPump_timerSlow(pump *pumpTemp);
void calc_colorPulseOutput(pump *pumpTemp);
void speaker_efect();
void outputPwmTron();
void timer2Handle();
void loadConfigInEeprom();
void pump_tron_handle();
void checkAlarm();
void dayNapDauPhun();
void dongNapQuay(uint8_t goc,bool dir);
void dongNapHandle();


void checkRuaDauPhun();

// config timer 3 - timer bom mau //
void TimerBomMau_SetFulseUs(uint16_t value);


uint8_t getAlarm_RuaDauPhun(void);
void setAlarm_RuaDauPhun(uint8_t value);





#endif /* SRC_USER_LIB_BOARD_PERIPHERAL_H_ */




