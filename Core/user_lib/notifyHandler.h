/*
 * notifyHandler.h
 *
 *  Created on: Feb 21, 2024
 *      Author: ADMIN
 */

#include "board_peripheral.h"
#include "ringbuffer.h"
#include "stdint.h"


#define 	EVENT_NONE              0
#define 	EVENT_BOARD_CONFIRM     1
#define 	EVENT_dangPhaMau        2
#define 	EVENT_phaMauXong        3
#define 	EVENT_dungPhaMau        4
#define 	EVENT_dangRuaDauPhun    5
#define 	EVENT_RuaDauPhunXong    6
#define 	EVENT_sensor1_off       7
#define 	EVENT_sensor2_off       8
#define 	EVENT_sensor3_off       9

#define 	EVENT_sensor1_on        10
#define 	EVENT_sensor2_on        11
#define 	EVENT_sensor3_on        12


void notify_init(void);
void notify_push_event(uint8_t event);
uint8_t notify_get_event();
void notify_service(void);

