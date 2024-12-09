/*
 * notifyHandler.c
 *
 *  Created on: Feb 21, 2024
 *      Author: ADMIN
 */

#include "notifyHandler.h"


uint8_t event_in;
uint8_t event_out;

ringbuffer_def notify_event;
uint8_t buffer_event[256];

/* list command notify */
static const char * notify_cmd[] = {
		"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"
		};


/* Declare Event Buffer */

void notify_init(void)
{
	 RingBuffer_Init(&notify_event, "event_buffer", &buffer_event, 256);
}

void notify_push_event(uint8_t data)
{
	unsigned char event[1];
	event[0] = data;
	RingBuffer_Push(&notify_event, event, 1);
}


uint8_t notify_get_event(void)
{
    if (notify_event.DataSize > 0) // check event in lcd buffer event //
    {
        RingBuffer_Pop(&notify_event, buffer_event, 1);
        return buffer_event[0];
    }
    else
       return 0;
}



void notify_service(void)
{
	static uint32_t service_tick = 0;
	uint8_t event;

	if (HAL_GetTick() - service_tick > 50)
	{
		service_tick = HAL_GetTick();
		event =  0;
		event = notify_get_event();

		switch (event)
		{
		case 0:

			break;
		case 1:
			sendFrame("event 1 >>>>>>>>>>>>>>>> \r\n");
			break;
		case 2:
			sendFrame("event 2 >>>>>>>>>>>>>>>>\r\n");
			break;
		case 3:
			sendFrame("event 3 >>>>>>>>>>>>>>>>\r\n");
			break;
		case 4:
			sendFrame("event 4 >>>>>>>>>>>>>>>>\r\n");
			break;


		default:
			break;
		}
	}
}

