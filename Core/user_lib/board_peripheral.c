/*
 * board_peripheral.c
 *
 *  Created on: Jul 8, 2023
 *      Author: Mai Ba
 */
#include "board_peripheral.h"
#include "myEepromUtility.h"

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "rtc.h"

#include <time.h>


#define tm time_t

//extern int LOG(const char *fmt,...);

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

#define UART_MAIN  huart1
// chinh lại huong dong ma nap o day //



//#define huart3   huart3

uint32_t dataBom=0;
uint32_t dataTron=0;
volatile pump bomMau[16];
volatile tron bomTron;
volatile tron dayNap;
volatile dongNap_t dongNap;
uint32_t buff595pump=0;
uint32_t pulse_1ml=322; //số xung để bơm được 1ml
uint32_t pulse_01ml=33;
uint32_t pulse_001ml=10;
uint8_t pump_index=0;
uint16_t gocDongNap=90;
uint16_t gocMoNap=0;

uint16_t startAddressPumpBomMau = 100;


float num1=0;
uint16_t num2=0, num3=0, num4=0, num5=0, num6=0,num7=0;

mytime_t timeNow;
mytime_t timeAlarm[10];

bool enTron=false;
uint8_t slowTron;
bool khuayMauTrucTiep=false;

// xuan ......//
static volatile uint32_t t_on = 0;
static volatile uint32_t counter = 0;
static volatile uint32_t step = 0;



static volatile uint8_t ruaDauPhun_Alarm = ON;
static volatile uint8_t Khuay_Alarm = ON;  // 1-> ACTIVE , 0 -> DISABLE

khuayMau_t khuayMau=
{
  .khoiDongKhuayMau.time.addr.gio =10,
  .khoiDongKhuayMau.time.addr.phut=12,
  .khoiDongKhuayMau.time.addr.giay=14,

  .khuayMau.time.addr.gio =16,
  .khuayMau.time.addr.phut=18,
  .khuayMau.time.addr.giay=20,

  .luongMauPhunRa.address =22,
  .tocDoKhuayMau.address  =24,
  .tocDoBomVeSinh.address =26,

  .levelThap.address     =28,
  .levelTrungBinh.address=30,
  .levelCao.address      =32,

  .veSinhDauPhun        =false,
  .countTimeKhuayMau    =0,
  .countTimeNghiKhuayMau=0
  //.enTimeKhuayMau=false
};

uint16_t gocDongNap_addr=46;
uint16_t gocMoNap_addr=48;
uint16_t doRongXungBomMau_addr=50;
uint32_t pulse_1ml;
uint32_t pulse_01ml;
uint32_t pulse_001ml;

void delay_us(uint32_t microseconds)
{
    TIM2->PSC = (SystemCoreClock / 1000000) - 1; // Thiết lập bộ chia tỷ số cho Timer2
    TIM2->ARR = microseconds - 1; // Thiết lập giá trị tự động nạp cho Timer2
    TIM2->CNT = 0; // Đặt giá trị đếm về 0

    TIM2->CR1 |= TIM_CR1_CEN; // Kích hoạt Timer2

    while (!(TIM2->SR & TIM_SR_UIF)); // Đợi cho đến khi cờ tràn bật lên

    TIM2->SR &= ~TIM_SR_UIF; // Xóa cờ tràn

    TIM2->CR1 &= ~TIM_CR1_CEN; // Tắt Timer2
}

void DoorInit(uint16_t interval)
{
	SET(EN17_GPIO_Port, EN17_Pin);
	SET(DIR17_GPIO_Port, DIR17_Pin);

	CLEAR(EN17_GPIO_Port, EN17_Pin);
	for(uint i = 0; i < 32000; i++ )
	{
		delay_us(interval);
		HAL_GPIO_TogglePin(PUL17_GPIO_Port, PUL17_Pin);
	}
	SET(EN17_GPIO_Port, EN17_Pin);
}

void DoorTest(uint16_t interval)
{
	SET(EN17_GPIO_Port, EN17_Pin);
	SET(DIR17_GPIO_Port, DIR17_Pin);

	CLEAR(EN17_GPIO_Port, EN17_Pin);
	for(uint i = 0; i < 32000; i++ )
	{
		delay_us(interval);
		HAL_GPIO_TogglePin(PUL17_GPIO_Port, PUL17_Pin);
	}
	SET(EN17_GPIO_Port, EN17_Pin);

}

void pump_init()
{
	bomMau[0].GPIOx     = PUL1_GPIO_Port;
	bomMau[0].GPIO_Pin  = PUL1_Pin;

	bomMau[0].DIR_Port  = GPIOC;
	bomMau[0].DIR_Pin   = GPIO_PIN_2;

	bomMau[0].EN_Port   = EN1_GPIO_Port;
	bomMau[0].EN_Pin    = EN1_Pin;
    /***************************************************************/

	bomMau[1].GPIOx     = PWM1_GPIO_Port;
	bomMau[1].GPIO_Pin  = PWM1_Pin;

	bomMau[1].DIR_Port  = DIR2_GPIO_Port;
	bomMau[1].DIR_Pin   = DIR2_Pin;

	bomMau[1].EN_Port   = EN2_GPIO_Port;
	bomMau[1].EN_Pin    = EN2_Pin;

	/**************************************************************/

	bomMau[2].GPIOx 	= PWM2_GPIO_Port;
	bomMau[2].GPIO_Pin 	= PWM2_Pin;

	bomMau[2].DIR_Port 	= DIR3_GPIO_Port;
	bomMau[2].DIR_Pin 	= DIR3_Pin;

	bomMau[2].EN_Port 	= EN3_GPIO_Port;
	bomMau[2].EN_Pin 	= EN3_Pin;

	/**************************************************************/
	bomMau[3].GPIOx     = PWM3_GPIO_Port;
	bomMau[3].GPIO_Pin  = PWM3_Pin;

	bomMau[3].DIR_Port 	= DIR4_GPIO_Port;
	bomMau[3].DIR_Pin  	= DIR4_Pin;

	bomMau[3].EN_Port  	= EN4_GPIO_Port;
	bomMau[3].EN_Pin   	= EN4_Pin;

    /**************************************************************/
	bomMau[4].GPIOx     = PWM4_GPIO_Port;
	bomMau[4].GPIO_Pin  = PWM4_Pin;

	bomMau[4].DIR_Port 	= DIR5_GPIO_Port;
	bomMau[4].DIR_Pin  	= DIR5_Pin;

	bomMau[4].EN_Port  	= EN5_GPIO_Port;
	bomMau[4].EN_Pin   	= EN5_Pin;

	/**************************************************************/
	bomMau[5].GPIOx     = PWM5_GPIO_Port;
	bomMau[5].GPIO_Pin  = PWM5_Pin;

	bomMau[5].DIR_Port  = DIR6_GPIO_Port;
	bomMau[5].DIR_Pin   = DIR6_Pin;

    // edit pin5 to pin 6 -> fix error motor6 is hold GND -> motor so hot

	bomMau[5].EN_Port   = EN6_GPIO_Port;
	bomMau[5].EN_Pin    = EN6_Pin;

	/**************************************************************/
	bomMau[6].GPIOx     = PWM6_GPIO_Port;
	bomMau[6].GPIO_Pin  = PWM6_Pin;

	bomMau[6].DIR_Port  = DIR7_GPIO_Port;
	bomMau[6].DIR_Pin   = DIR7_Pin;

	bomMau[6].EN_Port   = EN7_GPIO_Port;
	bomMau[6].EN_Pin    = EN7_Pin;
	/**************************************************************/

	bomMau[7].GPIOx     = PWM7_GPIO_Port;
	bomMau[7].GPIO_Pin  = PWM7_Pin;

	bomMau[7].DIR_Port  = DIR8_GPIO_Port;
	bomMau[7].DIR_Pin   = DIR8_Pin;

	bomMau[7].EN_Port   = EN8_GPIO_Port;
	bomMau[7].EN_Pin    = EN8_Pin;
	/**************************************************************/
	bomMau[8].GPIOx     = PWM8_GPIO_Port;
	bomMau[8].GPIO_Pin  = PWM8_Pin;

	bomMau[8].DIR_Port  = DIR9_GPIO_Port;
	bomMau[8].DIR_Pin   = DIR9_Pin;

	bomMau[8].EN_Port   = EN9_GPIO_Port;
	bomMau[8].EN_Pin    = EN9_Pin;

	/**************************************************************/

	bomMau[9].GPIOx 	= PWM9_GPIO_Port;
	bomMau[9].GPIO_Pin 	= PWM9_Pin;

	bomMau[9].DIR_Port  = DIR10_GPIO_Port;
	bomMau[9].DIR_Pin   = DIR10_Pin;

	bomMau[9].EN_Port   = EN10_GPIO_Port;
	bomMau[9].EN_Pin    = EN10_Pin;

	/**************************************************************/
	bomMau[10].GPIOx 	 = PWM10_GPIO_Port;
	bomMau[10].GPIO_Pin  = PWM10_Pin;

	bomMau[10].DIR_Port  = DIR11_GPIO_Port;
	bomMau[10].DIR_Pin   = DIR11_Pin;

	bomMau[10].EN_Port   = EN11_GPIO_Port;
	bomMau[10].EN_Pin    = EN11_Pin;

	/*************************************************************/

	bomMau[11].GPIOx 	 = PWM11_GPIO_Port;
	bomMau[11].GPIO_Pin  = PWM11_Pin;

	bomMau[11].DIR_Port  = DIR12_GPIO_Port;
	bomMau[11].DIR_Pin   = DIR12_Pin;

	bomMau[11].EN_Port   = EN12_GPIO_Port;
	bomMau[11].EN_Pin    = EN12_Pin;
	/**********************************************************/

	bomMau[12].GPIOx 	 = PWM12_GPIO_Port;
	bomMau[12].GPIO_Pin  = PWM12_Pin;

	bomMau[12].DIR_Port  = DIR13_GPIO_Port;
	bomMau[12].DIR_Pin   = DIR13_Pin;

	bomMau[12].EN_Port   = EN13_GPIO_Port;
	bomMau[12].EN_Pin    = EN13_Pin;

	/**********************************************************/
	bomMau[13].GPIOx 	 = PWM13_GPIO_Port;
	bomMau[13].GPIO_Pin  = PWM13_Pin;

	bomMau[13].DIR_Port  = DIR14_GPIO_Port;
	bomMau[13].DIR_Pin   = DIR14_Pin;

	bomMau[13].EN_Port   = EN14_GPIO_Port;
	bomMau[13].EN_Pin    = EN14_Pin;

	/**********************************************************/
	bomMau[14].GPIOx 	 = PWM14_GPIO_Port;
	bomMau[14].GPIO_Pin  = PWM14_Pin;

	bomMau[14].DIR_Port  = DIR15_GPIO_Port;
	bomMau[14].DIR_Pin   = DIR15_Pin;

	bomMau[14].EN_Port   = EN15_GPIO_Port;
	bomMau[14].EN_Pin    = EN15_Pin;

	/**********************************************************/
	bomMau[15].GPIOx 	 = PWM15_GPIO_Port;
	bomMau[15].GPIO_Pin  = PWM15_Pin;

	bomMau[15].DIR_Port  = DIR16_GPIO_Port;
	bomMau[15].DIR_Pin   = DIR16_Pin;

	bomMau[15].EN_Port   = EN16_GPIO_Port;
	bomMau[15].EN_Pin    = EN16_Pin;


	////////////////////////////////////////////////////////////
	for(uint8_t i = 0; i < 16; i++)
	{
		// clear pin EN tp hold motor //
		SET(bomMau[i].EN_Port, bomMau[i].EN_Pin);
		SET(bomMau[i].DIR_Port, bomMau[i].DIR_Pin);

	}
	CLEAR(bomMau[0].DIR_Port, bomMau[0].DIR_Pin);
	HAL_GPIO_WritePin(EN9_GPIO_Port, EN9_Pin, 1);
	setAlarm_RuaDauPhun(0);

	// clear pin khuay //
	SET(EN_ALL_16_GPIO_Port, EN_ALL_16_Pin);

}

/**
 * test ok
 */
void speaker_efect()
{
	for (int i = 0; i < 2; i++)
	{
		HAL_GPIO_WritePin(SPEAKER_GPIO_Port, SPEAKER_Pin, 1);
		HAL_Delay(100);
		HAL_GPIO_WritePin(SPEAKER_GPIO_Port, SPEAKER_Pin, 0);
		HAL_Delay(100);
	}
}


void rs485_print(const char *fmt, ...)
{
  static char buffer1[256];
  unsigned int len=0;
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer1, sizeof(buffer1), fmt, args);
  va_end(args);
  len=strlen(buffer1);

}


void rs232_print(const char *fmt, ...)
{
	printf(fmt);
}

/**
 * send frame with format to App for notify information
 */
void sendFrame(const char *fmt, ...)
{
  static char buffer1[256];
  unsigned int len=0;
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer1, sizeof(buffer1), fmt, args);
  va_end(args);
  len=strlen(buffer1);

  char start[1]={'@'};
  HAL_UART_Transmit(&UART_MAIN,  start,1,100);
  HAL_UART_Transmit(&UART_MAIN,  buffer1,len,100);
  char stop[1]={'#'};
  HAL_UART_Transmit(&UART_MAIN,  stop,1,100);

  ///
  printf("send_frame -> %s \r\n", fmt);
}


void pwmPump(pump *Pump,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	/*
	if((Pump->speed-Pump->countHighSpeed)>0) Pump->countHighSpeed++;
	else
	{
		Pump->countHighSpeed=0;
		HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
	}
	*/
}

void check_dir_sleep_Colorpump()
{
    uint8_t i = 0;

    for(i = 0; i <16; i++)
    {
    	// check DIR //
		if (bomMau[i].dir == THUAN)
			SET(bomMau[i].DIR_Port, bomMau[i].DIR_Pin);
		else
			CLEAR(bomMau[i].DIR_Port, bomMau[i].DIR_Pin);
    }
}
/*
 *  dieu khien xung bom mau */

void outputPwmColorPump(pump *pumpTemp, uint8_t index)
{
	uint16_t slow = 5;

	if (pumpTemp->sleep != SLEEP)
	{
		pumpTemp->Counter_Ton++;
		if (pumpTemp->Counter_Ton > pumpTemp->Ton)
		{
			pumpTemp->Counter_Ton = 0;

			if (pumpTemp->countHighSpeed < pumpTemp->pulseSetHighSpeed)
			{
				pumpTemp->countHighSpeed++;
				HAL_GPIO_TogglePin(pumpTemp->GPIOx, pumpTemp->GPIO_Pin);
				printf(".");
			}

			else  // bom cham .....//
			{
				//bơm phần thập phân

				if (pumpTemp->countLowSpeed < pumpTemp->pulseSetLowSpeed)
				{
					if (pumpTemp->countDelay < slow)
						pumpTemp->countDelay++;
					else
					{
						pumpTemp->countDelay = 0;
						pumpTemp->countLowSpeed++;
						HAL_GPIO_TogglePin(pumpTemp->GPIOx, pumpTemp->GPIO_Pin);
						//printf("bom phan thap phan\r\n");
					}
				}
				//dừng bơm
				else
				{
					printf(" index: %d-------->STOP \r\n", index);
					pumpTemp->sleep = SLEEP;
					out(pumpTemp->GPIOx, pumpTemp->GPIO_Pin, 0);
					out(pumpTemp->EN_Port, pumpTemp->EN_Pin, 1); // disable motor//
					//khuayMau.veSinhDauPhun = false;
				}
			}
		}
	}
	else

	{
		//printf(" PAUSE \r\n");
	}
}

/*
 * Tính toán số xung cần xuất ra để điều khiển bơm
 */
void calc_colorPulseOutput(pump *pumpTemp)
{
	uint32_t Nguyen		=	(uint32_t)    (pumpTemp->mililit);
	uint16_t boiSo01ml 	= 	(uint16_t) 	  (pumpTemp->mililit * 10)  % 10;
	uint16_t boiSo001ml = 	(uint16_t) 	  (pumpTemp->mililit * 100) % 10;

	pumpTemp->pulseSetHighSpeed = (Nguyen * pumpTemp->pulse1ml)*2;  // double for toggle pulse //
	pumpTemp->pulseSetLowSpeed  = (boiSo01ml * pumpTemp->pulse01ml + boiSo001ml * pumpTemp->pulse001ml)*2;  // double for toggle pulse //

	///// 08-05-2024 //
	// > 1.0 -> total is high fulse //
	if(pumpTemp->pulseSetHighSpeed  > 0)
	{
		pumpTemp->pulseSetHighSpeed = pumpTemp->pulseSetHighSpeed + pumpTemp->pulseSetLowSpeed;
		pumpTemp->pulseSetLowSpeed= 0;
	}
	else;

	pumpTemp->countHighSpeed = 0;
	pumpTemp->countLowSpeed  = 0;

	printf("CALC: 1ml=%u, 01ml=%u, 001ml=%u --> Fulse->1ml: %u - 01ml: %u - 001ml: %u \r\n", Nguyen, boiSo01ml, boiSo001ml, pumpTemp->pulse1ml, pumpTemp->pulse01ml , pumpTemp->pulse001ml);
	printf("CALC: pulseHighSpeed=%u, pulseLowSpeed=%u \r\n", pumpTemp->pulseSetHighSpeed, pumpTemp->pulseSetLowSpeed);
}

// 1 cycles = 50us //
void pump_pwm_handle()
{
	for (uint8_t i = 0; i < 16; i++)
	{
		outputPwmColorPump(&bomMau[i], i);
	}
	check_dir_sleep_Colorpump();
}

void pump_tron_handle()
{
	HAL_GPIO_TogglePin(PUL_ALL_16_GPIO_Port, PUL_ALL_16_Pin);
}

void dayNapDauPhun()
{

}


void AlarmTask()
{
	static uint32_t tick_1s = 0;
	static int last_min = 0;
	RTC_TimeTypeDef gTime;
	if (HAL_GetTick() - tick_1s > 1000)
	{
		tick_1s = HAL_GetTick();

		timeNow.time_in_sec++;
		checkAlarm();
		checkRuaDauPhun();

		int _hour = (timeNow.time_in_sec / 3600);
		int _min = (timeNow.time_in_sec / 60) % 60;
		int _sec = (timeNow.time_in_sec % 60);

		//printf("time now_in sec: %d-> %02d:%02d:%02d \n\r",timeNow.time_in_sec, _hour, _min, _sec);
		if (_hour == 24 && _min == 0 && _sec == 0)  // reset for newday //
			timeNow.time_in_sec = 0;

		if (last_min != _min)
		{
			last_min = _min;
			printf("time now_in sec: %d-> %02d:%02d:%02d \n\r", timeNow.time_in_sec, _hour, _min, _sec);
		}
	}
	else;

}

void CheckTimeNow(void)
{
   return;
}

void AlarmOff(void)
{
	for (uint8_t i = 0; i < 10; i++)
	{
		timeAlarm[i].en = 0;
	}
	DISABLE_TIMER_TRON;
	HAL_GPIO_WritePin(EN_ALL_16_GPIO_Port, EN_ALL_16_Pin, 1);
	HAL_GPIO_WritePin(DIR_ALL_16_GPIO_Port, DIR_ALL_16_Pin, 0);
}

extern void ruaDauPhun();

void checkRuaDauPhun()
{
	static uint8_t en_timer[10] = {0};
		uint8_t total_state = 0;
		//uint8_t last_en_timer[10] = {0};
		static uint8_t startTimer = 0;

		for(int i=0; i<10; i++)
		{
			if(timeAlarm[i].en == 2)  // == 2  is rua dau phun //
			{
				if(timeAlarm[i].time_in_sec < timeNow.time_in_sec && timeAlarm[i].time_in_sec + 2 > timeNow.time_in_sec)
				{
					en_timer[i] = 1;
					printf("[ALARM_RUA_DAU_PHUN------>] \r\n");
					ruaDauPhun_Alarm = 1;
				}
				else
				{
					en_timer[i] = 0;
				}
				total_state +=en_timer[i];
			}
			else
			{
				en_timer[i] = 0;
			}
			total_state += en_timer[i];
		}


	    if(total_state > 0 && startTimer == 0)
	    {
	    	startTimer = 1;
	    }
	    else if(total_state == 0)
	    {
	    	startTimer = 0;
	    }
	    else;
		total_state= 0;
}

/* check alarm Khuay mau
 *  timeAlarm[i].time =  thơi gian khuay mau
 *
 * */

void checkAlarm()  // check alarm khuay mau //
{
	static uint8_t en_timer[10] = {0};
	uint8_t  total_state = 0;
	static uint8_t startTimer = 0;

	// if khuay  == OFF, return //
	if(Khuay_Alarm == OFF)
	{
		printf(" alarm khuay off \n\r");
		return;  // break if khuay is OFF
	}

	for (uint8_t i = 0; i < 10; i++)
	{
		if(timeAlarm[i].en == 1)  // == 1  khuay //
		{
			if(timeAlarm[i].time_in_sec < timeNow.time_in_sec && timeAlarm[i].time_in_sec + timeAlarm[i].time*60 > timeNow.time_in_sec)
			{
				en_timer[i] = 1;
				//printf(" alarm ---\n\r");
				HAL_GPIO_WritePin(EN_ALL_16_GPIO_Port, EN_ALL_16_Pin, 0);
				HAL_GPIO_WritePin(DIR_ALL_16_GPIO_Port, DIR_ALL_16_Pin, 0);
			}
			else
			{
				en_timer[i] = 0;
			}
			total_state +=en_timer[i];
		}
		else
		{
			en_timer[i] = 0;
		}
		total_state += en_timer[i];
	}


    if(total_state > 0 && startTimer == 0)
    {
    	startTimer = 1;
    	printf("[ALARM_KHUAY_MAU------>] \r\n");
    	ENABLE_TIMER_TRON;
    }
    else if(total_state == 0)
    {
    	startTimer = 0;
    	DISABLE_TIMER_TRON;
    	HAL_GPIO_WritePin(EN_ALL_16_GPIO_Port, EN_ALL_16_Pin, 1);
    	HAL_GPIO_WritePin(DIR_ALL_16_GPIO_Port, DIR_ALL_16_Pin, 0);
    	HAL_GPIO_WritePin(PUL_ALL_16_GPIO_Port, PUL_ALL_16_Pin, 0);
    	//printf("[ALARM_ KHAY MAU OFF------>] \r\n");
    }
    else;

	total_state= 0;
}


void loadConfigInEeprom()
{
	//Dừng khuấy màu

	bomTron.en=false;
	//dừng bơm
	//write4Byte595_bom(0xffffffff, 1);

	//Thời gian nghỉ khuấy màu
	khuayMau.khoiDongKhuayMau.time.gio=eep_read_word(khuayMau.khoiDongKhuayMau.time.addr.gio);
	khuayMau.khoiDongKhuayMau.time.phut=eep_read_word(khuayMau.khoiDongKhuayMau.time.addr.phut);
	khuayMau.khoiDongKhuayMau.time.giay=eep_read_word(khuayMau.khoiDongKhuayMau.time.addr.giay);
	//thời gian khuấy màu
	khuayMau.khuayMau.time.phut=eep_read_word(khuayMau.khuayMau.time.addr.phut);
	khuayMau.khuayMau.time.giay=eep_read_word(khuayMau.khuayMau.time.addr.giay);
	//tốc độ bơm khi vệ sinh đầu phun
	khuayMau.tocDoBomVeSinh.value=eep_read_word(khuayMau.tocDoBomVeSinh.address);
	//lượng màu phun ra
	khuayMau.luongMauPhunRa.value=eep_read_word(khuayMau.luongMauPhunRa.address);
	//góc đóng mở nắp


	gocDongNap=eep_read_word(gocDongNap_addr);
	gocMoNap=eep_read_word(gocMoNap_addr);
	//Độ rộng xung điều khiển bơm màu


	uint16_t TonBomMau=0;

   TonBomMau=eep_read_word(doRongXungBomMau_addr);
	if(TonBomMau > 9999)  // = 65535 //
	{
		TonBomMau=200;
		eep_write_word(doRongXungBomMau_addr,TonBomMau);
		printf(" set ton bom mau default = 200 \n\r");
	}
//	else if(TonBomMau<50)
//	{
//		TonBomMau=50;
//		eep_write_word(doRongXungBomMau_addr,TonBomMau);
//	}

	TimerBomMau_SetFulseUs(TonBomMau);

	//Độ rộng xung khuấy màu
	khuayMau.doRongXung.value = eep_read_word(khuayMau.doRongXung.address);
	if(khuayMau.doRongXung.value > 9999)
	{
		khuayMau.doRongXung.value = 400;
		eep_write_word(khuayMau.doRongXung.address,350);
		printf(" set T tron default = 400 \n\r");
	}
	bomTron.Tset=khuayMau.doRongXung.value/10;
	bomTron.countT=0;

	TimerKhuayMau_SetFulseUs(khuayMau.doRongXung.value);

	khuayMau.levelThap.address=28;
	khuayMau.levelTrungBinh.address=30;
	khuayMau.levelCao.address=32;
	//tốc độ khuấy màu
	//level thấp
	khuayMau.levelThap.value=eep_read_word(khuayMau.levelThap.address);
	if(khuayMau.levelThap.value>100)
	{
		khuayMau.levelThap.value=10;
		eep_write_word(khuayMau.levelThap.address,khuayMau.levelThap.value);
	}
	//level trung bình
	khuayMau.levelTrungBinh.value=eep_read_word(khuayMau.levelTrungBinh.address);
	if(khuayMau.levelTrungBinh.value>100)
	{
		khuayMau.levelTrungBinh.value=5;
		eep_write_word(khuayMau.levelTrungBinh.address,khuayMau.levelTrungBinh.value);
	}
	//level cao
	khuayMau.levelCao.value=eep_read_word(khuayMau.levelCao.address);
	if(khuayMau.levelCao.value>100)
	{
		khuayMau.levelCao.value=1;
		eep_write_word(khuayMau.levelCao.address,khuayMau.levelCao.value);
	}
	if(khuayMau.tocDoKhuayMau.value==THAP) bomTron.slowLevel=khuayMau.levelThap.value;
	else if(khuayMau.tocDoKhuayMau.value==TRUNGBINH) bomTron.slowLevel=khuayMau.levelTrungBinh.value;
	else if(khuayMau.tocDoKhuayMau.value==CAO) bomTron.slowLevel=khuayMau.levelCao.value;

    /*******************************************************************************************/
	HAL_Delay(50);
	printf("load config ton \n\r");
	for(int i=0;i<16;i++)
	{
			uint16_t data= {0};
			data = eep_read_word(200+i*2);
			if(data = 0xFFFF)
				data = 1;
			bomMau[i].Ton = data;
			printf("bomMau[%d]=%d", i, bomMau[i].Ton);

			bomMau[i].delayLowSpeed =  TonBomMau;
	}

	printf("\n\r");
	printf("Delay low counter = %d \n\r",bomMau[0].delayLowSpeed );


	HAL_Delay(50);
	//hẹn giờ khuấy màu -- 100- 186
	for (int i = 0; i < 10; i++) {
		timeAlarm[i].addr.gio = 100 + 8 * i;
		timeAlarm[i].addr.phut = 100 + 8 * i + 2;
		timeAlarm[i].addr.time = 100 + 8 * i + 4;
		timeAlarm[i].addr.en = 100 + 8 * i + 6;
		//rs232_print("addr.gio=%d addr.phut=%d addr.time=%d addr.en=%d\n",timeAlarm[i].addr.gio,timeAlarm[i].addr.phut,timeAlarm[i].addr.time,timeAlarm[i].addr.en);
	}

	/* load time setting alarmmmm */
	printf("...............readback alarm when 121023 ...............\r\n");
	for(int i=0;i<10;i++)
	{
		uint16_t data[4]= {0};

		data[0] = eep_read_word(timeAlarm[i].addr.gio);
		data[1] = eep_read_word(timeAlarm[i].addr.phut);
		data[2] = eep_read_word(timeAlarm[i].addr.time);
		data[3] = eep_read_word(timeAlarm[i].addr.en);

		timeAlarm[i].gio  = data[0];
		timeAlarm[i].phut = data[1];
		timeAlarm[i].time = data[2];
		timeAlarm[i].en   = data[3];

		timeAlarm[i].time_in_sec = timeAlarm[i].gio* 3600 + timeAlarm[i].phut*60;
		printf("gio:%d phut:%d interval: %d state:%d \r\n", timeAlarm[i].gio, timeAlarm[i].phut, timeAlarm[i].time , timeAlarm[i].en);
	}
	HAL_Delay(50);

	/** load config Ton for 16P PWM **/


}

static volatile dong_counter = 0;
void dongNapQuay(uint8_t goc,bool dir)
{
//	HAL_GPIO_WritePin(DOOR_DIR_Port, DOOR_DIR_Pin, 0);
//	HAL_GPIO_WritePin(DOOR_EN_Port, DOOR_EN_Pin, 0);
//	HAL_GPIO_WritePin(DOOR_PUL_Port, DOOR_PUL_Pin, 0);
//
//	dongNap.pulseSet = goc * 1.0 / 1.8;
//	dongNap.pulseSet *= 2;
//	dongNap.countPulse = 0;
//	dongNap.en = true;
//
//	printf("fulse = %d , count = %d , en = %d  \n\r", dongNap.pulseSet, dongNap.countPulse, dongNap.en );
	dong_counter = 0;
	HAL_TIM_Base_Start_IT(&htim5);

}

void dongNapHandle()
{
     HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
}


/**
 *  set interval timer bommau us
 *  deinit timer -> prescaler = 71 , auto period = value
 *  base clock: 72.000.000hz
 *  prescaler: 71 + 1 -> timerClk = 1.000.000hz = 1Mhz = 1us
 *  period: value = value * 1us.
 *  note:  50 < Value < 400 -> motor stable
 */
void TimerBomMau_SetFulseUs(uint16_t value)
{

	printf("update timer bom mau -> %d us \r\n", value);
	HAL_TIM_Base_Stop_IT(&htim3);
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 71;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = value-1;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	//HAL_TIM_Base_Start_IT(&htim3);
}


void TimerKhuayMau_SetFulseUs(uint16_t value)
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	  /* USER CODE BEGIN TIM6_Init 1 */

	  /* USER CODE END TIM6_Init 1 */
	  htim6.Instance = TIM6;
	  htim6.Init.Prescaler = 71;
	  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim6.Init.Period = value-1;
	  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

/// rua dau phun //
uint8_t getAlarm_RuaDauPhun(void)
{
	return ruaDauPhun_Alarm;
}
void setAlarm_RuaDauPhun(uint8_t value)
{
	ruaDauPhun_Alarm = value;
}

/**
 * Value = ON or OFF
 * */

void setAlarm_Khuay(uint8_t value)
{
	Khuay_Alarm = value;
}


