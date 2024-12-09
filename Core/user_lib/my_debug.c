/*
 * my_debug.c
 *
 *  Created on: Feb 13, 2023
 *      Author: Mai Ba
 */
#include "my_debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>


Uart_debug my_debug=
{
	.char_data_rec=0,
	.arr_data_rec={0},
	.data_done_frame=false,
	.arr_data_index=0,
	.en_get_char_data=false
};

Uart_debug my_modbus=
{
	.char_data_rec=0,
	.arr_data_rec={0},
	.data_done_frame=false,
	.arr_data_index=0,
	.en_get_char_data=false
};


//----------------------------------------------------

bool trim(char *source,char *des,char key_start[30],char key_stop[30])
{
    unsigned int start_id=0,stop_id=0;
    if(strstr(source,key_start)==0) return false;
    if(strstr(source,key_stop)==0) return false;
    memset(des,0,strlen(des));
    start_id=strstr(source,key_start)+strlen(key_start)-source;
    stop_id=strstr(source+start_id,key_stop)-source;
    //printf("start_id: %d\n",start_id);
    //printf("stop_id: %d\n",stop_id);
    if(start_id>stop_id) return false;
    for(int i=start_id;i<stop_id;i++)
    	des[i-start_id]=source[i];
    return true;
}

bool trim_with_length(char *source,char *des,char key_start[30],unsigned int length)
{
    unsigned int start_id=0,stop_id=0;
    if(strstr(source,key_start)==0) return false;
    //if(strstr(source,key_stop)==0) return false;
    memset(des,0,strlen(des));
    start_id=strstr(source,key_start)+strlen(key_start)-source;
    stop_id=start_id+length;
    //printf("start_id: %d\n",start_id);
    //printf("stop_id: %d\n",stop_id);
    if(start_id>stop_id) return false;
    for(int i=start_id;i<stop_id;i++) des[i-start_id]=source[i];
    return true;
}






