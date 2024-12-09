/*
 * driver_rtc.h
 *
 *  Created on: Jan 2, 2024
 *      Author: ADMIN
 */

#ifndef USER_LIB_DRIVER_RTC_H_
#define USER_LIB_DRIVER_RTC_H_

#include "main.h"
#include "rtc.h"
#include "stdint.h"

void RTC_init(void);
void RTC_setTime (uint8_t hour, uint8_t min, uint8_t sec);
void RTC_getTime(void);
void RTC_alarmService(void);


#endif /* USER_LIB_DRIVER_RTC_H_ */
