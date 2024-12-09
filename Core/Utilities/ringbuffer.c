/*
 * ringbuffer.c
 *
 *  Created on: Dec 28, 2018
 *      Author: ntrbac
 */
#include "ringbuffer.h"

void RingBuffer_Init(ringbuffer_def *RingBuff, uint8_t *Name, uint8_t *buff, uint32_t leng)
{
	RingBuff->name = Name;
	RingBuff->buffer = buff;
	RingBuff->Length = leng;
	RingBuff->Space = leng;
	RingBuff->DataSize = 0;
	RingBuff->Wp = 0;
	RingBuff->Rp = 0;
}
int8_t RingBuffer_Pos(ringbuffer_def *RingBuff)
{
	int8_t ret = 0;
	if (RingBuff->Wp > RingBuff->Rp)
	{
		ret = 1;
		// RingBuff->Space = (RingBuff->Length - RingBuff->Wp) + RingBuff->Rp;
	}
	else if (RingBuff->Wp < RingBuff->Rp)
	{
		ret = -1;
		// RingBuff->Space = RingBuff->Rp - RingBuff->Wp;
	}
	//  else{
	//    if (RingBuff->DataSize){
	//      RingBuff->Space = 0;
	//    }
	//    else
	//      RingBuff->Space = RingBuff->Length;
	//  }
	return ret;
}
uint32_t RingBuffer_PushByte(ringbuffer_def *RingBuff, uint8_t dat)
{
	if (RingBuff->Space)
	{
		RingBuff->buffer[RingBuff->Wp] = dat;
		RingBuff->Wp = (RingBuff->Wp + 1) % RingBuff->Length;
		RingBuff->DataSize++;
		RingBuff->Space--;
		return RingBuff->DataSize;
	}
	else
		return 0;
}
uint32_t RingBuffer_PopByte(ringbuffer_def *RingBuff, uint8_t *dat)
{
	if (RingBuff->DataSize)
	{
		*dat = RingBuff->buffer[RingBuff->Rp];
		RingBuff->Rp = (RingBuff->Rp + 1) % RingBuff->Length;
		RingBuff->DataSize--;
		RingBuff->Space++;
		return RingBuff->Space;
	}
	else
		return 0;
}
uint32_t RingBuffer_Push(ringbuffer_def *RingBuff, uint8_t *dat, uint32_t leng)
{
	uint32_t lengthSupported = 0, llen = 0, olen = 0;
	int8_t Head = 0;
	Head = RingBuffer_Pos(RingBuff);
	if (RingBuff->Space)
	{
		if (RingBuff->Space > leng)
		{
			lengthSupported = leng;
		}
		else
		{
			lengthSupported = RingBuff->Space;
		}
		if (Head == -1)
		{
			llen = lengthSupported;
		}
		else
		{
			if ((RingBuff->Length - RingBuff->Wp) > lengthSupported)
			{
				llen = lengthSupported;
			}
			else
			{
				llen = RingBuff->Length - RingBuff->Wp;
				olen = lengthSupported - llen;
			}
		}
		if (dat)
			memcpy(&RingBuff->buffer[RingBuff->Wp], dat, llen);
		if (olen)
		{
			if (dat)
				memcpy(&RingBuff->buffer[0], &dat[llen], olen);
			RingBuff->Wp = olen;
		}
		else
		{
			RingBuff->Wp = (RingBuff->Wp + lengthSupported) % RingBuff->Length;
		}
		RingBuff->DataSize += lengthSupported;
		RingBuff->Space -= lengthSupported;
		return lengthSupported;
	}
	else
		return 0;
}

uint32_t RingBuffer_Pop(ringbuffer_def *RingBuff, uint8_t *dat, uint32_t leng)
{
	uint32_t lengthSupported = 0, llen = 0, olen = 0;
	int8_t Head = 0;
	Head = RingBuffer_Pos(RingBuff);
	if (RingBuff->DataSize)
	{
		if (RingBuff->DataSize > leng)
		{
			lengthSupported = leng;
		}
		else
		{
			lengthSupported = RingBuff->DataSize;
		}
		if (Head == 1)
		{
			llen = lengthSupported;
		}
		else
		{
			if ((RingBuff->Length - RingBuff->Rp) > lengthSupported)
			{
				llen = lengthSupported;
			}
			else
			{
				llen = RingBuff->Length - RingBuff->Rp;
				olen = lengthSupported - llen;
			}
		}
		if (dat)
			memcpy(dat, &RingBuff->buffer[RingBuff->Rp], llen);
		if (olen)
		{
			if (dat)
				memcpy(&dat[llen], &RingBuff->buffer[0], olen);
			RingBuff->Rp = olen;
		}
		else
		{
			RingBuff->Rp = (RingBuff->Rp + lengthSupported) % RingBuff->Length;
		}
		RingBuff->DataSize -= lengthSupported;
		RingBuff->Space += lengthSupported;
		return lengthSupported;
	}
	else
		return 0;
}
uint8_t *RingBuffer_get(ringbuffer_def *RingBuff, uint32_t *leng)
{
	uint32_t lengthSupported = 0, llen = 0;
	int8_t Head = 0;
	Head = RingBuffer_Pos(RingBuff);
	if (RingBuff->DataSize)
	{
		lengthSupported = RingBuff->DataSize;
		if (Head == 1)
		{
			llen = lengthSupported;
		}
		else
		{
			llen = RingBuff->Length - RingBuff->Rp;
		}
		*leng = llen;
		return &RingBuff->buffer[RingBuff->Rp];
	}
	else
		return 0;
}

void Ringbuffer_view(ringbuffer_def *RingBuff)
{
	 //APP_DBG("%s @0x%x consist of %d bytes: pointer %d/%d, space %d, data size %d\n", RingBuff->name, (uint32_t)RingBuff->buffer, RingBuff->Length, RingBuff->Rp, RingBuff->Wp, RingBuff->Space, RingBuff->DataSize);
}
