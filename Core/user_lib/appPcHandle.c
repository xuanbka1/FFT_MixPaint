/*
 * appPcHandle.c
 *
 *  Created on: Xuan 23_01_24
 *      Author:  Xuantn93
 *      edit xuan:
 *      cam bien 3 -> cam bien thung son
 *      cam bien 2: cam  bien mo nap hoan toan
 *      cam bien 1: cam bien dong nap hoan toan
 *
 *      24_01_> chinh timer 3 ve ty le 71-9 -> 10us
 */
#include "stdio.h"
#include "appPcHandle.h"
#include "my_debug.h"
#include "board_peripheral.h"
#include "eeprom_24c32.h"
#include "driver_rtc.h"
#include "notifyHandler.h"

#include "test.h"


#define UART_RS232  huart3

// DRV 8825 //
//#define DOOR_CLOSE_DIR    1
//#define DOOR_OPEN_DIR     0

// redefine DIRECT for TMC2225 replace DRV8825 //
#define DOOR_CLOSE_DIR    0
#define DOOR_OPEN_DIR     1

#define DOOR_SLEEP        1
#define DOOR_RUN          0

bool connectIsOpen=false;

bool sttSensor1=false;
bool sttSensorLast1=false;
bool sttSensor2=false;
bool sttSensorLast2=false;
bool sttSensor3 =false;
bool sttSensorLast3=false;

bool dangPhaMau=false;

#define  DOOR_OPENING    1
#define  DOOR_CLOSING    2
#define  DOOR_OPEN_OK    3
#define  DOOR_CLOSE_OK   4
#define  DOOR_START      100
#define  DOOR_IDLE       0

#define DOOR_DELAY   3000

#define  PUMP_ACTION_NONE   0
#define  PUMP_ACTION_PUMP   1
#define  PUMP_ACTION_CLEAN  2

#define  PUMP_MACHINE_IDLE        0
#define  PUMP_MACHINE_OPEN_DOOR   1
#define  PUMP_MACHINE_SET_EN      2
#define  PUMP_MACHINE_BEGIN       3
#define  PUMP_MACHINE_FINISH      4
#define  PUMP_MACHINE_CLOSE_DOOR  5

char notify_message_action[30];
char notify_message_finish[30];

static uint8_t s_action_pump =  PUMP_ACTION_NONE;
static uint8_t s_pump_machine = PUMP_MACHINE_IDLE;

static uint8_t DOOR_STATE = DOOR_IDLE;


/*****************************************************************/
/*  static volatile variable  sensor value   */
#define  STATE_SENSOR_ON    0
#define  STATE_SENSOR_OFF   1

static volatile int Sensor_Door_Open_State  = -1;
static volatile int Sensor_Door_Close_State = -1;
static volatile int Sensor_Laze_State       = -1;
static volatile uint8_t update_Sensor[4] = {0,0,0,0};



// multi-task declare //
static void ledTask(void);
static void sensorTask(void);
static void alarmTask(void);
static void uartTask(void);
static void controlMotorTask(void);
static void doorTask(uint8_t doorState);

static volatile uint8_t s_doorAction = DOOR_IDLE;
static volatile uint16_t s_doorDelay = 0;

/** fucntion for setting **/
static void motorSetting(char* str);

// sensor function //
static void update_sensor1(void);
static void update_sensor2(void);
static void update_sensor3(void);
static void update_sensor4(void);
static void stopPump(void);
static void stopKhuay(void);
static void AlarmSetting(void);

static void test_all(void);

/*
 * process Interrupt GPIO sensor input
 * */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_0)  // INPUT1  -> do not connect
	{
		update_sensor1();
	}
	else if (GPIO_Pin == GPIO_PIN_1)  // INPUT 2 // -> OPTIC
	{
		update_sensor2();
	}
	else if (GPIO_Pin == GPIO_PIN_2)  // INPUT 3 // OPEN DOOR
	{
		update_sensor3();
	}
	else if (GPIO_Pin == GPIO_PIN_3)  // INPUT4 -- CLOSE DOOR
	{
		update_sensor4();
	}
	else
		;

}
/*****************************************************************/


static volatile uint32_t DoorTick = 0;
static void checkSensor(void);
static void testDongNap(void);
static void OpenDoor(void);
static void CloseDoor(void);
static void CloseDoor_Now(void);
static void UpdateStatus(void);
static void DoorTask(void);
void ruaDauPhun(void); // auto //

void ruaDauPhun_Mililit(void); // rua theo mililit //

static void test_motor(void);

App myApp=
{
	.charRec=0,
	.dataDoneFrame=false,
	.enReadFrame=false,
	.arrData="",
	.countCharRec=0,

	.countTimeUpdateDate=0,
	.appRequestReadData=false,
	.timeUpdateData=500, //500ms
	.timeUpdateStr="500ms",
	//.hanldeAppRequets=handleAppRequets,
	.send=SendToApp
};



// @+data+#\n\r
void SendToApp(uint8_t selectCom,const char *fmt, ...)
{
	static char buffer1[256];
	unsigned int len=0;
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer1, sizeof(buffer1), fmt, args);
	va_end(args);
	len=strlen(buffer1);

	char header[1];
	//header + data: @data
	header[0]=APP_FRAME_START;
	HAL_UART_Transmit(&UART_RS232,header,1,100);
	HAL_UART_Transmit(&UART_RS232,buffer1,len,100);

    // footer: #\n\r
	header[0]=APP_FRAME_STOP;
	HAL_UART_Transmit(&UART_RS232,header,1,100);
	HAL_UART_Transmit(&UART_RS232,"\r\n",2,100);
}

void uartAppHandle(App *myUart)
{
	//LOG("rx=%c",myUart->charRec );

	if(myUart->charRec==APP_FRAME_STOP)
	{
		myUart->enReadFrame=false;
		myUart->dataDoneFrame=true;
	}
	if(myUart->enReadFrame)
	{
		myUart->arrData[myUart->countCharRec]=myUart->charRec;
		myUart->countCharRec++;
	}

	if(myUart->charRec==APP_FRAME_START)
		myUart->enReadFrame=true;
}

void uartAppClear(App *myUart)
{
	myUart->charRec=0;
	myUart->dataDoneFrame=false;
	myUart->enReadFrame=false;
	memset(myUart->arrData,0,sizeof(myUart->arrData));
	myUart->countCharRec=0;
}

bool bomMauStart	 = false;
uint16_t countFinish = 0;
bool huyLenh 		 = false;
//-----------------------------------------------------------
void handleAppRequets()
{
	// test xung //
	//test_motor();

	// test_microstep();
	// first time check LAZE and BOX sensor //
	UpdateStatus();
	if (read_sensor3 == 0)
		HAL_GPIO_WritePin(LAZE_GPIO_Port, LAZE_Pin, 1);
	else
		HAL_GPIO_WritePin(LAZE_GPIO_Port, LAZE_Pin, 0);

	//test_all();
	while(1)
	{
		sensorTask();
		ledTask();
		uartTask();
		doorTask(NULL);
		controlMotorTask();
		AlarmTask();
		checkSensor();

		if(getAlarm_RuaDauPhun())   // sua loi dang bom thi rua dau phun //
		{
			setAlarm_RuaDauPhun(0);
			if(s_pump_machine == PUMP_MACHINE_IDLE)  // neu pump busy, do not action//
				ruaDauPhun();
			else
			{
				printf(" Pump BUSY --> ko rua dau phun \n\r");
			}
		}
	}//while
}


void allPumpIsRun()
{
	for(int i=0;i<16;i++)
	{
		bomMau[i].sleep=RUN;
	}
}

void allPumpIsSleep()
{
	for(int i=0;i<16;i++)
	{
		bomMau[i].sleep=SLEEP;
		bomMau[i].Counter_Ton = 0;
	}
	HAL_GPIO_WritePin(EN1_GPIO_Port, 	EN1_Pin, 1);
	HAL_GPIO_WritePin(EN2_GPIO_Port, 	EN2_Pin, 1);
	HAL_GPIO_WritePin(EN3_GPIO_Port, 	EN3_Pin, 1);
	HAL_GPIO_WritePin(EN4_GPIO_Port, 	EN4_Pin, 1);
	HAL_GPIO_WritePin(EN5_GPIO_Port, 	EN5_Pin, 1);
	HAL_GPIO_WritePin(EN6_GPIO_Port, 	EN6_Pin, 1);
	HAL_GPIO_WritePin(EN7_GPIO_Port, 	EN7_Pin, 1);
	HAL_GPIO_WritePin(EN8_GPIO_Port, 	EN8_Pin, 1);
	HAL_GPIO_WritePin(EN9_GPIO_Port, 	EN9_Pin, 1);
	HAL_GPIO_WritePin(EN10_GPIO_Port, 	EN10_Pin, 1);
	HAL_GPIO_WritePin(EN11_GPIO_Port, 	EN11_Pin, 1);
	HAL_GPIO_WritePin(EN12_GPIO_Port, 	EN12_Pin, 1);
	HAL_GPIO_WritePin(EN13_GPIO_Port, 	EN13_Pin, 1);
	HAL_GPIO_WritePin(EN14_GPIO_Port, 	EN14_Pin, 1);
	HAL_GPIO_WritePin(EN15_GPIO_Port, 	EN15_Pin, 1);
	HAL_GPIO_WritePin(EN16_GPIO_Port, 	EN16_Pin, 1);
}

void setValue(pump *temp,float ml,uint16_t pulse1ml,uint16_t pulse01ml,uint16_t pulse001ml)
{
	temp->mililit=ml;
	temp->pulse1ml=pulse1ml;
	temp->pulse01ml=pulse01ml;
	temp->pulse001ml=pulse001ml;
}


/* Open Door Setting  */
static void OpenDoor(void)
{
	printf(" OpenDoor command \r\n");
	if (Sensor_Door_Open_State == STATE_SENSOR_ON)
		printf(" cua dang mo -> ko mo duoc \r\n");
	else
	{
		s_doorAction = DOOR_OPENING;
		HAL_GPIO_WritePin(EN17_GPIO_Port, EN17_Pin, 0);
		HAL_GPIO_WritePin(DIR17_GPIO_Port, DIR17_Pin, DOOR_OPEN_DIR);
		HAL_Delay(1);
		HAL_TIM_Base_Start_IT(&htim5);
		ENABLE_TIMER_DOOR;

		printf(" Bat timer Open Door \r\n");
	}
}

/* Close door BarMental  Dong cua test ok
 * Close After delay time */
static void CloseDoor(void)
{
	HAL_Delay(s_doorDelay);   // delay close //
	CloseDoor_Now();

}

/* Close Door right away -> not include delaytime  */
static void CloseDoor_Now(void)
{
	printf(" Close Door command \r\n");
	if (Sensor_Door_Close_State == STATE_SENSOR_ON)
		printf(" cua dang dong -> ko mo duoc \r\n");
	else
	{
		s_doorAction = DOOR_CLOSING;
		HAL_GPIO_WritePin(EN17_GPIO_Port, EN17_Pin, 0);
		HAL_GPIO_WritePin(DIR17_GPIO_Port, DIR17_Pin, DOOR_CLOSE_DIR);
		HAL_Delay(5);
		HAL_TIM_Base_Start_IT(&htim5);
		ENABLE_TIMER_DOOR;

		printf(" Bat timer cua \r\n");
	}
}

static void controlMotorTask()
{
	switch (s_pump_machine)
	{
	case PUMP_MACHINE_OPEN_DOOR:
		if (Sensor_Door_Close_State == STATE_SENSOR_ON && Sensor_Door_Open_State == STATE_SENSOR_OFF)
		{
			OpenDoor();
			s_pump_machine = PUMP_MACHINE_SET_EN;
		}
		break;

	case PUMP_MACHINE_SET_EN:
		if (Sensor_Door_Open_State == STATE_SENSOR_ON && Sensor_Door_Close_State == STATE_SENSOR_OFF)
		{
			for (uint8_t index = 0; index < 16; index++)
			{
				if (bomMau[index].sleep == RUN)
				{
					printf(" bat cac chaan EN cua: %d \n\r",index );
					HAL_GPIO_WritePin(bomMau[index].EN_Port, bomMau[index].EN_Pin, 0);
				}
			}
			HAL_Delay(5); /* delay for opto driver ready */
			s_pump_machine = PUMP_MACHINE_BEGIN;

		}
		break;

	case PUMP_MACHINE_BEGIN:

		On_Timer_Pha();
		printf("gui dang pha mau ??? \n\r");
		sendFrame(notify_message_action);
		s_pump_machine = PUMP_MACHINE_FINISH;
		break;

	case PUMP_MACHINE_FINISH:
		uint8_t  sleep_cnt = 0;
		for (uint8_t index = 0; index < 16; index++)
		{
			if (bomMau[index].sleep == SLEEP)
			{
				sleep_cnt++;
			}
		}
		if(sleep_cnt == 16)
		{
			printf("sleep all is OK-> disable timer \n\r");
			HAL_TIM_Base_Stop_IT(&htim3);
			DISABLE_TIMER_PHA_MAU;
			sendFrame(notify_message_finish);
			s_pump_machine = PUMP_MACHINE_CLOSE_DOOR;
		}
			break;

	case PUMP_MACHINE_CLOSE_DOOR:
		if (Sensor_Door_Open_State == STATE_SENSOR_ON)
		{
			HAL_TIM_Base_Stop_IT(&htim3);
			DISABLE_TIMER_PHA_MAU;
			HAL_GPIO_WritePin(EN9_GPIO_Port, EN9_Pin, 1);  // fix pump dont sleep after pump .... //
			HAL_Delay(3000);
			CloseDoor();
			s_pump_machine = PUMP_MACHINE_IDLE;
			speaker_efect();

			setAlarm_Khuay(ON);
		}
			break;
	default:
		break;
	}
}

/*rau dau phun auto 1000 xung */
 void ruaDauPhun(void)
{
	 uint8_t id;
	 uint16_t _default_fulse;

	 _default_fulse =  khuayMau.luongMauPhunRa.value * 100;

	if (Sensor_Laze_State == STATE_SENSOR_OFF) // thung sơn chua co //
	{
		sendFrame("choThungSonVao");
		printf("choThungSonVao\n\r");
		DISABLE_TIMER_PHA_MAU;
		printf("Disable timer 3 \n\r");

		while (Sensor_Laze_State == STATE_SENSOR_OFF)
		{
			speaker_efect();
		}
	}
    /* cho thung son vao OK*/
	HAL_Delay(2000); // delay 3s //

	for ( id = 0; id < 16; id++) {
		bomMau[id].mililit = khuayMau.luongMauPhunRa.value;
		bomMau[id].pulse1ml = _default_fulse;
		bomMau[id].countHighSpeed = bomMau[id].pulse1ml * bomMau[id].mililit;
		bomMau[id].countLowSpeed = 0;
		calc_colorPulseOutput(&bomMau[id]);
		bomMau[id].sleep=RUN;
	}
	s_pump_machine = PUMP_MACHINE_OPEN_DOOR;
}

/*
 * rau dau phun theo miilit
 * @Test M1[12408]-M2[4612]-M3[4820]-M4[9800]-M5[10620]-M6[5208]-M7[4808]-M8[5320]-M9[10112]-M10[4904]-M11[5052]-M12[5092]-M13[4956]-M14[4672]-M15[5160]-M16[9800]#
 * */
void ruaDauPhun_Mililit(void) // rua theo mililit //
{
	uint8_t MAX_MOTOR = 16;
	uint32_t fulse;

	/* check cam bien quang thug son */
	if (Sensor_Laze_State == STATE_SENSOR_OFF) // thung sơn chua co //
		{
			sendFrame("choThungSonVao");
			printf("choThungSonVao\n\r");
			DISABLE_TIMER_PHA_MAU;
			printf("Disable timer 3 \n\r");

			while (Sensor_Laze_State == STATE_SENSOR_OFF)
			{
				speaker_efect();
			}
		}

	/* cho thung son vao OK*/
	HAL_Delay(2000); // delay 2s //
	OpenDoor();
	printf("Khac phuc loi-->> RuaDauPhun-Mililit\n\r");
	/*    phan tich du lieu   */
	/*
	 @Test M1[12408]-M2[4612]-M3[4820]-M4[9800]-M5[10620]-M6[5208]-M7[4808]-M8[5320]-M9[10112]-M10[4904]-M11[5052]-M12[5092]-M13[4956]-M14[4672]-M15[5160]-M16[9800]#
	 */
	char txt[17][20];
	for (int i = 0; i < 16; i++)
		memset(txt[i], 0, 20);

	printf("data: %s \n\r", myApp.arrData);

	trim(myApp.arrData, txt[0],  "M1",  "-");
	trim(myApp.arrData, txt[1],  "M2",  "-");
	trim(myApp.arrData, txt[2],  "M3",  "-");
	trim(myApp.arrData, txt[3],  "M4",  "-");
	trim(myApp.arrData, txt[4],  "M5",  "-");
	trim(myApp.arrData, txt[5],  "M6",  "-");
	trim(myApp.arrData, txt[6],  "M7",  "-");
	trim(myApp.arrData, txt[7],  "M8",  "-");
	trim(myApp.arrData, txt[8],  "M9",  "-");
	trim(myApp.arrData, txt[9],  "M10", "-");
	trim(myApp.arrData, txt[10], "M11", "-");
	trim(myApp.arrData, txt[11], "M12", "-");
	trim(myApp.arrData, txt[12], "M13", "-");
	trim(myApp.arrData, txt[13], "M14", "-");
	trim(myApp.arrData, txt[14], "M15", "-");
	trim(myApp.arrData, txt[15], "M16", "-");

	for (int i = 0; i < 16; i++) {
		fulse = 0;
		printf(" MOTOR[%d] = %s \r\n", i, (const char*) txt[i]);
		sscanf(txt[i], "[%d]", &fulse);
		printf("num value pulse = %d \n\r", fulse);
		bomMau[i].mililit = 1;
		bomMau[i].countHighSpeed = fulse;
		bomMau[i].countLowSpeed = 0;
		bomMau[i].pulseSetHighSpeed = fulse*2;
		bomMau[i].pulseSetLowSpeed = 0;
		bomMau[i].sleep = RUN;
	}
	s_pump_machine = PUMP_MACHINE_OPEN_DOOR;
	sprintf(notify_message_action, "%s", "dangRuaDauPhun" );
	sprintf(notify_message_finish, "%s", "RuaDauPhunXong" );

	/*
	 @Test M1[12408]-M2[4612]-M3[4820]-M4[9800]-M5[10620]-M6[5208]-M7[4808]-M8[5320]-M9[10112]-M10[4904]-M11[5052]-M12[5092]-M13[4956]-M14[4672]-M15[5160]-M16[9800]#
	 */
}


/*
 * sensor task
 * */
static void sensorTask(void)
{
	if (update_Sensor[0] == 1)
	{
		update_Sensor[0] = 0;
		//todo:
	}

	if (update_Sensor[1] == 1)
	{
		update_Sensor[1] = 0;

		if (Sensor_Laze_State == STATE_SENSOR_ON)  // // sensor 2
		{
			printf("Sensor_Laze_State == STATE_SENSOR_ON \n\r");
			sendFrame("sensor3 on\n\r");
			HAL_GPIO_WritePin(LAZE_GPIO_Port, LAZE_Pin, 1);
		}
		else
		{
			printf("Sensor_Laze_State == STATE_SENSOR_OFF \n\r");
			sendFrame("sensor3 off\n\r");
			HAL_GPIO_WritePin(LAZE_GPIO_Port, LAZE_Pin, 0);
		}
	}

	if (update_Sensor[2] == 1)  // sensor 3
	{
		update_Sensor[2] = 0;

		if (Sensor_Door_Open_State == STATE_SENSOR_ON)
		{
			printf("Sensor_Door_Open_State == STATE_SENSOR_ON \n\r");
			sendFrame("sensor2 on\n\r");

		}
		else
		{
			printf("Sensor_Door_Open_State == STATE_SENSOR_OFF \n\r");
			sendFrame("sensor2 off\n\r");

		}
	}

	if (update_Sensor[3] == 1)  // sensor 4
	{
		update_Sensor[3] = 0;
		if (Sensor_Door_Close_State == STATE_SENSOR_ON)
		{
			printf("Sensor_Door_Close_State == STATE_SENSOR_ON \n\r");
			sendFrame("sensor1 on\n\r");
		}
		else
		{
			printf("Sensor_Door_Close_State == STATE_SENSOR_OFF \n\r");
			sendFrame("sensor1 off\n\r");
		}
	}
}

static void ledTask(void)
{
	static uint32_t led_tick = 0;
	if (HAL_GetTick() - led_tick > 1000)
	{
		led_tick = HAL_GetTick();
		HAL_GPIO_TogglePin(ST1_GPIO_Port, ST1_Pin);
		HAL_GPIO_TogglePin(ST2_GPIO_Port, ST2_Pin);
	}
}

static void uartTask(void)
{
	if(myApp.dataDoneFrame)
	{
		HAL_TIM_Base_Stop_IT(&htim2);
		HAL_TIM_Base_Stop_IT(&htim3);
		HAL_TIM_Base_Stop_IT(&htim4);
		speaker_efect();

		printf("rev>> %s \n\r",myApp.arrData );
		// process command //

		if(strstr(myApp.arrData, "timeNow")) // connect and update time
		{
			char txtTime[20] = "";
			trim(myApp.arrData, txtTime, "[", "]");
			sscanf(txtTime, "%d:%d:%d", &timeNow.gio, &timeNow.phut, &timeNow.giay);

			timeNow.time_in_sec = timeNow.gio * 3600 + timeNow.phut * 60 + timeNow.giay;
			printf("\r\n Sync-> time now: %d:%d:%d --> time_in_sec = %d \n\r",timeNow.gio,timeNow.phut,timeNow.giay, timeNow.time_in_sec);

			UpdateStatus();
			sendFrame("boardConfirm");
		}
		else if (strstr(myApp.arrData, ">OpenDoor"))   // open door  //
		{
			OpenDoor();
		}
		else if (strstr(myApp.arrData, ">CloseDoor"))  // close door //
		{
			CloseDoor_Now();
		}
		else if(strstr(myApp.arrData,"dataColorPump")) // pump color //
		{
			sendFrame("boardConfirm");

			motorSetting(myApp.arrData);

			DISABLE_TIMER_TRON;
			HAL_GPIO_WritePin(EN_ALL_16_GPIO_Port, EN_ALL_16_Pin, 1);
			HAL_GPIO_WritePin(DIR_ALL_16_GPIO_Port, DIR_ALL_16_Pin, 0);
			//disable alarm //
			setAlarm_Khuay(OFF);
			//AlarmOff();
		}

		/* dung pha mau */
		else if(strstr(myApp.arrData,"<stopPhaMau>"))
		{
			printf("stopPhaMau--> Huy_Lenh\n\r");
			s_action_pump = PUMP_MACHINE_CLOSE_DOOR;
			stopPump();
		}

		else if(strstr(myApp.arrData,"RuaDauPhun"))
		{
			printf(" command rua dau phun \n\r");
			//ruaDauPhun();
			ruaDauPhun_Mililit();
		}

		else if (strstr(myApp.arrData, "luongMauPhunRa"))  /// luong mau phu ra khi ve sinh //
		{
			printf("luongMauPhunRa\n\r");
			int value;
			sscanf(myApp.arrData, "luongMauPhunRa[%d]", &value);
			khuayMau.luongMauPhunRa.value = value;
			eep_write_word(khuayMau.luongMauPhunRa.address, khuayMau.luongMauPhunRa.value);
			printf("\nkhuayMau.luongMauPhunRa.value=%d \r\n", khuayMau.luongMauPhunRa.value);
		}

		/*  cai dat thoi chu ki xung bom son */
		else if (strstr(myApp.arrData, "Ton_Bommau"))  ////// OK ////
		{
			int Ton;
			sscanf(myApp.arrData, "Ton_Bommau[%d]", &Ton);
			printf("Setting Ton_Bommau: %d \r\n", Ton);
			eep_write_word(doRongXungBomMau_addr, Ton);
			TimerBomMau_SetFulseUs((uint16_t)Ton);
		}
		/*  cai dat thoi chu kin xung tron son */
		else if (strstr(myApp.arrData, "Ton_TronSon"))
		{
			HAL_TIM_Base_Stop_IT(&htim4);
			int Ton;
			sscanf(myApp.arrData, "Ton_TronSon[%d]", &Ton);
			printf("Setting Ton_TronSon: %d \n\r", Ton);
			khuayMau.doRongXung.value = Ton;
			eep_write_word(khuayMau.doRongXung.address, khuayMau.doRongXung.value);
			printf("Khuay mau Ton=%d\n\r", Ton);
			TimerKhuayMau_SetFulseUs((uint16_t) Ton);
		}

		else if(strstr(myApp.arrData,"Alarm"))
		{
			AlarmSetting();
		}

		else if (strstr(myApp.arrData, "KhuayMau"))
		{
			printf("Khac phuc loi-->>KhuayMau\n\r");
			//điều khiển khuấy màu trực tiếp
			setAlarm_Khuay(OFF);
			HAL_GPIO_WritePin(EN_ALL_16_GPIO_Port, EN_ALL_16_Pin, 0);
			HAL_GPIO_WritePin(DIR_ALL_16_GPIO_Port, DIR_ALL_16_Pin, 0);
			ENABLE_TIMER_TRON;
		}
		else if (strstr(myApp.arrData, "DungKhuay"))
		{
			printf("Khac phuc loi-->> DungKhuay\n\r");
			setAlarm_Khuay(ON);
			DISABLE_TIMER_TRON;
			HAL_GPIO_WritePin(EN_ALL_16_GPIO_Port, EN_ALL_16_Pin, 1);
			HAL_GPIO_WritePin(DIR_ALL_16_GPIO_Port, DIR_ALL_16_Pin, 0);
			//disable alarm //
			AlarmOff();
		}
		else;

		uartAppClear(&myApp);
//		HAL_TIM_Base_Start_IT(&htim2);

//		HAL_TIM_Base_Start_IT(&htim4);
	}
}

static void doorTask(uint8_t doorState)
{
	if(s_doorAction == DOOR_OPENING)
	{
		if(Sensor_Door_Open_State == STATE_SENSOR_ON)
		{
			HAL_TIM_Base_Stop_IT(&htim5);
			doorState = DOOR_OPEN_OK;
		}
	}

	if(s_doorAction == DOOR_CLOSING)
	{
		if(Sensor_Door_Close_State == STATE_SENSOR_ON)
		{
			HAL_Delay(s_doorDelay);
			HAL_TIM_Base_Stop_IT(&htim5);
			doorState = DOOR_CLOSE_OK;
		}
	}
}

/*
 * M1[1,2,3,4,5]
 -1  số mililit
 -2  số ống màu
 -3  xung 1ml
 -4  xung 0.1ml
 -5  xung 0.01ml
 * */
static void motorSetting(char *str)
{

	printf(" goto motor setting: %s \n\r", str);

	uint8_t MAX_PUMP = 7;
	uint8_t motorIndex;
	char txt[7][20];

	uint8_t i;

    /* check sensor thung son and notify */
    if(Sensor_Laze_State != STATE_SENSOR_ON)
    {
    	speaker_efect();
    	speaker_efect();
    	sendFrame("choThungSonVao");
    	printf("choThungSonVao\n\r");
    	return;
    }

	/* reset setting of motor */
	for (motorIndex = 0; motorIndex < 16; motorIndex++)
	{
		bomMau[i].mililit = 0;
		bomMau[i].countHighSpeed = 0;
		bomMau[i].countLowSpeed = 0;
		bomMau[i].Counter_Ton = 0;
		bomMau[i].sleep = SLEEP;
		HAL_GPIO_WritePin(bomMau[i].EN_Port, bomMau[i].EN_Pin, 1);
	}

	/* memset memory control */
	for (i = 0; i < MAX_PUMP; i++)
		memset(txt[i], 0, 20);

	/* parse data control message */
	char* time0 = str + 17;

	char* time1 = FIND_AND_NUL(time0+3, time1, ']');   /*  printf("time 1 = %s \n\r", time0);    */    sprintf(txt[0], "%s",(const char*) time0);
	char* time2 = FIND_AND_NUL(time1+3, time2, ']');   /*  printf("time 2 = %s \n\r", time1+4);  */    sprintf(txt[1], "%s",(const char*) time1+4);
	char* time3 = FIND_AND_NUL(time2+3, time3, ']');   /*  printf("time 3 = %s \n\r", time2+4);  */    sprintf(txt[2], "%s",(const char*) time2+4);
	char* time4 = FIND_AND_NUL(time3+3, time4, ']');   /*  printf("time 3 = %s \n\r", time3+4);  */    sprintf(txt[3], "%s",(const char*) time3+4);
	char* time5 = FIND_AND_NUL(time4+3, time5, ']');   /*  printf("time 3 = %s \n\r", time4+4);  */    sprintf(txt[4], "%s",(const char*) time4+4);
	char* time6 = FIND_AND_NUL(time5+3, time6, ']');   /*  printf("time 3 = %s \n\r", time5+4);  */    sprintf(txt[5], "%s",(const char*) time5+4);
	char* time7 = FIND_AND_NUL(time6+3, time7, ']');   /*  printf("time 3 = %s \n\r", time6+4);  */    sprintf(txt[6], "%s",(const char*) time6+4);

	for (i = 0; i < MAX_PUMP; i++)
	{
		printf("txt[%d] = %s \n\r", i, (const char*) txt[i]);
	}
	printf("  \n\r");
	/* scanf value char to int */
    for (i = 0; i < MAX_PUMP; i++)
	{
	    sscanf(txt[i], "%f,%d,%d,%d,%d", &num1, &num2, &num3, &num4, &num5);
	    if (num2 > 0 && num2 < 20)
	    {
	    	printf("\r\n Ong mau: %d ", num2);
	    	uint8_t id = num2 - 1;
	    	bomMau[id].mililit		=	0;
	    	bomMau[id].pulse1ml		=	0;
	    	bomMau[id].pulse01ml	=	0;
	    	bomMau[id].pulse001ml	=	0;

	    	bomMau[id].mililit		=	num1;
	    	bomMau[id].pulse1ml		=	num3;
	    	bomMau[id].pulse01ml	=	num4;
	    	bomMau[id].pulse001ml	=	num5;

			bomMau[id].sleep=RUN;
            // enable EN pin //

			s_action_pump = PUMP_ACTION_PUMP;
			s_pump_machine = PUMP_MACHINE_OPEN_DOOR;

			sprintf(notify_message_action, "%s", "dangPhaMau" );
			sprintf(notify_message_finish, "%s", "phaMauXong" );

			calc_colorPulseOutput(&bomMau[id]);
			//bomMauStart=true;
	    	printf("app_Handler: ml=%f,pulse1ml=%d,pulse01ml=%d,pulse001ml=%d \r\n", bomMau[id].mililit, bomMau[id].pulse1ml, bomMau[id].pulse01ml, bomMau[id].pulse001ml);

			num1=0; num2=0; num3=0; num4=0; num5=0;
	    }
	}
}
/**************************************************************************
 * Sensor donot connect
 **************************************************************************/
static void update_sensor1(void)
{
	update_Sensor[0] = 1;
	if (HAL_GPIO_ReadPin(INPUT1_GPIO_Port, INPUT1_Pin) == GPIO_PIN_SET)
	{
		printf(">>>>>>>>>>>>>IN 1 ONN \n\r");
	}
	else
	{
		printf(">>>>>>>>>>>>>IN 1 OFF \n\r");
	}
}
/**************************************************************************
 * Sensor Optic laser
 **************************************************************************/
static void update_sensor2(void)
{
	update_Sensor[1] = 1;
	if (HAL_GPIO_ReadPin(INPUT2_GPIO_Port, INPUT2_Pin) == GPIO_PIN_SET)
	{
		printf(">>>>>>>>>>>>>IN 2 ONN \n\r");
		Sensor_Laze_State = STATE_SENSOR_OFF;
	}
	else
	{
		printf(">>>>>>>>>>>>>IN 2 OFF \n\r");
		Sensor_Laze_State = STATE_SENSOR_ON;
	}
}
/**************************************************************************
 * Sensor Open Door
 **************************************************************************/
static void update_sensor3(void)
{
	update_Sensor[2] = 1;
	if (HAL_GPIO_ReadPin(INPUT3_GPIO_Port, INPUT3_Pin) == GPIO_PIN_SET)
	{
		printf(">>>>>>>>>>>>>IN 3 ONN \n\r");
		Sensor_Door_Open_State = STATE_SENSOR_OFF;
	}
	else
	{
		printf(">>>>>>>>>>>>>IN 3 OFF \n\r");
		Sensor_Door_Open_State = STATE_SENSOR_ON;
	}
}
/**************************************************************************
 * Sensor Close Door
 **************************************************************************/
static void update_sensor4(void)
{
	update_Sensor[3] = 1;
	if (HAL_GPIO_ReadPin(INPUT4_GPIO_Port, INPUT4_Pin) == GPIO_PIN_SET)
	{
		printf(">>>>>>>>>>>>>IN 4 ONN \n\r");
		Sensor_Door_Close_State = STATE_SENSOR_OFF;
	}
	else
	{
		printf(">>>>>>>>>>>>>IN 4 OFF \n\r");
		Sensor_Door_Close_State = STATE_SENSOR_ON;
		DISABLE_TIMER_DOOR;
	}
}


void On_Timer_Pha(void)
{
	HAL_TIM_Base_Start_IT(&htim3);
	TIM3->CR1 |= TIM_CR1_CEN;
}

static void stopPump(void)
{
	printf("stopPhaMau--> Huy_Lenh\n\r");
	for (int i = 0; i < 16; i++)
	{
		bomMau[i].mililit = 0;
		bomMau[i].countHighSpeed = 0;
		bomMau[i].countLowSpeed = 0;
		calc_colorPulseOutput(&bomMau[i]);
		out(bomMau[i].GPIOx, bomMau[i].GPIO_Pin, 0);
	}
	allPumpIsSleep();
	sendFrame("boardConfirm");
	printf("stop timer 3\n\r");

}


static void stopKhuay(void)
{
	HAL_TIM_Base_Stop(&htim4);
	HAL_TIM_Base_Stop_IT(&htim4);
	HAL_GPIO_WritePin(EN_ALL_16_GPIO_Port, EN_ALL_16_Pin, 1);
	HAL_GPIO_WritePin(DIR_ALL_16_GPIO_Port, DIR_ALL_16_Pin, 0);
	//disable alarm //
	AlarmOff();
}

static void AlarmSetting(void)
{
	char alarmTxt[10][20];
	for (int i = 0; i < 10; i++)
	{
		memset(alarmTxt[i], 0, 20);
		timeAlarm[i].gio = 0;
		timeAlarm[i].phut = 0;
		timeAlarm[i].time = 0;
		timeAlarm[i].en = 0;
		timeAlarm[i].countTime = 0;
	}
	trim(myApp.arrData, alarmTxt[0], "A1", "-");
	trim(myApp.arrData, alarmTxt[1], "A2", "-");
	trim(myApp.arrData, alarmTxt[2], "A3", "-");
	trim(myApp.arrData, alarmTxt[3], "A4", "-");
	trim(myApp.arrData, alarmTxt[4], "A5", "-");
	trim(myApp.arrData, alarmTxt[5], "A6", "-");
	trim(myApp.arrData, alarmTxt[6], "A7", "-");
	trim(myApp.arrData, alarmTxt[7], "A8", "-");
	trim(myApp.arrData, alarmTxt[8], "A9", "-");
	trim(myApp.arrData, alarmTxt[9], "A10", "-");

	for (int i = 0; i < 10; i++)
	{
		printf(alarmTxt[i]);
		printf("\r\n");
	}

	//hẹn giờ khuấy màu
	for (int i = 0; i < 10; i++)
	{
		timeAlarm[i].addr.gio = 100 + 8 * i;
		timeAlarm[i].addr.phut = 100 + 8 * i + 2;
		timeAlarm[i].addr.time = 100 + 8 * i + 4;
		timeAlarm[i].addr.en = 100 + 8 * i + 6;
	}
	for (int i = 0; i < 10; i++)
	{
		char temp[20] = "";
		unsigned int tempTime = 0;
		sscanf(alarmTxt[i], "[%d,%d,%d,%d]", &timeAlarm[i].gio, &timeAlarm[i].phut, &tempTime, &timeAlarm[i].en);
		timeAlarm[i].time = tempTime;

		timeAlarm[i].time_in_sec = timeAlarm[i].gio * 3600 + timeAlarm[i].phut * 60;

		eep_write_word(timeAlarm[i].addr.gio, timeAlarm[i].gio);
		eep_write_word(timeAlarm[i].addr.phut, timeAlarm[i].phut);
		eep_write_word(timeAlarm[i].addr.time, timeAlarm[i].time);
		eep_write_word(timeAlarm[i].addr.en, timeAlarm[i].en);

		printf("gio:%d phut:%d interval: %d state:%-> alarmStatus= %d \r\n", timeAlarm[i].gio, timeAlarm[i].phut, tempTime, timeAlarm[i].en,
				timeAlarm[i].time_in_sec);
	}
}



static void checkSensor(void)
{
//	static uint32_t delay_sensor1 = 0;
//		static uint32_t delay_sensor2 = 0 ;
//		static uint32_t delay_sensor3 = 0;
//
//		if (read_sensor1)
//		{
//			if(HAL_GetTick() - delay_sensor1 > 20)
//			{
//				delay_sensor1 = HAL_GetTick();
//				if (read_sensor1)
//					sttSensor1 = true;
//			}
//		}
//		else
//			sttSensor1 = false;
//
//		if (read_sensor2)
//		{
//			if (HAL_GetTick() - delay_sensor2 > 20)
//			{
//				if (read_sensor2)
//					sttSensor2 = true;
//			}
//
//		}
//		else
//			sttSensor2 = false;
//
//		if (read_sensor3)
//		{
//			if (HAL_GetTick() - delay_sensor3 > 20)
//			{
//				delay_sensor3 = HAL_GetTick();
//				if (read_sensor3)
//					sttSensor3 = true;
//			}
//		}
//		else
//			sttSensor3 = false;
//
//		// update status to app //
//		if (sttSensor1 != sttSensorLast1)
//		{
//			sttSensorLast1 = sttSensor1;
//			if (sttSensor1)
//			{
//				sendFrame("sensor1 off\n\r");
//				printf("Sensor 1 off\n\r");
//			}
//			else  //// dong
//			{
//				printf("Sensor 1 on\n\r");
//				sendFrame("sensor1 on\n\r");
//
//			}
//		}
//		if (sttSensor2 != sttSensorLast2)
//		{
//			sttSensorLast2 = sttSensor2;
//			if (sttSensor2)
//			{
//				sendFrame("sensor2 off\n\r");
//				printf("Sensor 2 off\n\r");
//			}
//			else
//			{
//				sendFrame("sensor2 on\n\r");
//				printf("Sensor 2 on\n\r");
//
//			}
//		}
//		if (sttSensor3 != sttSensorLast3)
//		{
//			sttSensorLast3 = sttSensor3;
//
//			if (sttSensor3)
//			{
//				sendFrame("sensor3 off\n\r");
//				printf("Sensor 3 off\n\r");
//				HAL_GPIO_WritePin(LAZE_GPIO_Port, LAZE_Pin, 0);
//			}
//			else
//			{
//				HAL_GPIO_WritePin(LAZE_GPIO_Port, LAZE_Pin, 1);
//				printf("sensor 3 on\n\r");
//				sendFrame("sensor3 on\n\r");
//			}
//		}
}
/*
 * update status of sensor the first connect
 *  SENSOR2-  THUNG SON   -- sensor on 3  -> INPUT2
	SENSOR3-  MO CUA      -- sensor on 2  -> INPUT3
	SENSOR4 - DONG CUA    -- sensor on 1  -> INPUT4
 * */
static void UpdateStatus(void)
{

	// update sensor optic //
	if (HAL_GPIO_ReadPin(INPUT2_GPIO_Port, INPUT2_Pin))
	{
		printf("update status INPUT2_GPIO_Port = 1 \n\r");
		Sensor_Laze_State = STATE_SENSOR_OFF;
		sendFrame("sensor3 off");
	}
	else
	{
		printf("update status INPUT2_GPIO_Port = 0 \n\r");
		Sensor_Laze_State = STATE_SENSOR_ON;
		sendFrame("sensor3 on");
	}
	HAL_Delay(1);

	// update sensor close door //
	if (HAL_GPIO_ReadPin(INPUT3_GPIO_Port, INPUT3_Pin))
	{
		printf("update status INPUT3_GPIO_Port = 1 \n\r");
		Sensor_Door_Open_State = STATE_SENSOR_OFF;
		sendFrame("sensor2 off");
	}
	else
	{
		printf("update status INPUT3_GPIO_Port = 0 \n\r");
		Sensor_Door_Open_State = STATE_SENSOR_ON;
		sendFrame("sensor2 on");
	}

	HAL_Delay(1);
	// update sensor open door //
	if (HAL_GPIO_ReadPin(INPUT4_GPIO_Port, INPUT4_Pin))
	{
		printf("update status INPUT4_GPIO_Port = 1 \n\r");
		Sensor_Door_Close_State = STATE_SENSOR_OFF;
		sendFrame("sensor1 off");
	}
	else
	{
		printf("update status INPUT4_GPIO_Port = 0 \n\r");
		Sensor_Door_Close_State = STATE_SENSOR_ON;
		sendFrame("sensor1 on");
	}
}



static void test_all(void)
{
    uint8_t i = 0;
    uint32_t counter = 0;

    for(i =0; i < 16; i++)
    {
    	HAL_GPIO_WritePin(bomMau[i].EN_Port,bomMau[i].EN_Pin , 0);
    	HAL_GPIO_WritePin(PUL17_GPIO_Port , PUL17_Pin, 0);
    }


    while(1)
    {
    	for(counter = 0;  counter < 1000; counter++)
    	{
    		__NOP();
    	}
    	HAL_GPIO_TogglePin(PUL17_GPIO_Port, PUL17_Pin);
    	HAL_GPIO_TogglePin(bomMau[counter].GPIOx , bomMau[counter].GPIO_Pin);
    }
}








