/**
	******************************************************************************
	* @file    ringbuffer.h
	* @author  MCD Application Team
	* @version V1.0.1
	* @date    16-Jan-2019
	* @brief   This file contains definitions for STM32F407_TGgen2 board's hardware resources.
	******************************************************************************
	* @attention
	*
	* 
	*
	******************************************************************************
	*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "string.h"

#ifdef __cplusplus
}
#endif

typedef struct
{
	uint8_t *name;
	uint8_t *buffer;
	uint32_t Length;
	uint32_t Wp; // write
	uint32_t Rp; // read
	uint32_t Space;
	uint32_t DataSize;
} ringbuffer_def;

void RingBuffer_Init(ringbuffer_def *RingBuff, uint8_t *Name, uint8_t *buff, uint32_t leng);
int8_t RingBuffer_Space(ringbuffer_def *RingBuff);
uint32_t RingBuffer_PushByte(ringbuffer_def *RingBuff, uint8_t dat);
uint32_t RingBuffer_PopByte(ringbuffer_def *RingBuff, uint8_t *dat);
uint32_t RingBuffer_Push(ringbuffer_def *RingBuff, uint8_t *dat, uint32_t leng);
uint32_t RingBuffer_Pop(ringbuffer_def *RingBuff, uint8_t *dat, uint32_t leng);
uint8_t *RingBuffer_get(ringbuffer_def *RingBuff, uint32_t *leng);
void Ringbuffer_view(ringbuffer_def *RingBuff);
/**
	* @}
	*/
#endif /* __RINGBUFFER_H */
/************************ (C) COPYRIGHT VNET *****END OF FILE****/
