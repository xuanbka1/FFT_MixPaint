/*
 * driver_rtc.c
 *
 *  Created on: Jan 2, 2024
 *      Author: ADMIN
 */

#include "driver_rtc.h"

/*
 * convert uint to hex BCD:
 * example: 58 -> 0x58
 *          12 -> 0x12
 * */

static uint8_t convertInt_toHex(uint8_t value)
{
	uint8_t table_H[10] = {0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90};
	uint8_t table_L[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

	uint8_t HighByte = 0, LowByte = 0;
	uint8_t index = 0;

	HighByte = value / 10;
	LowByte = value %10;

	return (table_H[HighByte] | table_L[LowByte]);
}


void RTC_init(void){
	RTC_TimeTypeDef sTime ={ 0 };
	RTC_DateTypeDef DateToUpdate = { 0 };
	hrtc.Instance = RTC;
	hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}

	// check backup and set it //
	 if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) !=0x32F2)
	  {
		  printf(" init set time for first boot \n\r");
		  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
		  RTC_setTime(0,0,0);
	  }
}


void RTC_setTime (uint8_t hour, uint8_t min, uint8_t sec)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;


	sTime.Hours   = convertInt_toHex(hour); // set hours
	sTime.Minutes = convertInt_toHex(min); // set minutes
	sTime.Seconds = convertInt_toHex(sec);; // set seconds

	//  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	//  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	{
		printf("set time error \n\r");
	}
	sDate.WeekDay = RTC_WEEKDAY_THURSDAY; // day
	sDate.Month = RTC_MONTH_AUGUST; // month
	sDate.Date = 0x9; // date
	sDate.Year = 0x18; // year
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	{
		printf("set date error \n\r");
	}
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2); // backup register
}


void RTC_getTime(void)
{
	RTC_DateTypeDef gDate;
	RTC_TimeTypeDef gTime;
	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
	/* Display time Format: hh:mm:ss */
	printf("%02d:%02d:%02d \n\r", gTime.Hours, gTime.Minutes, gTime.Seconds);
	/* Display date Format: dd-mm-yy */
	//printf("%02d-%02d-%2d \n\r",gDate.Date, gDate.Month, 2000 + gDate.Year);
}


void RTC_alarmService(void)
{

}
