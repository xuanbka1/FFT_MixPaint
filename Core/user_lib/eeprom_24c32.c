/*
 * eeprom_24c32.c
 *
 *  Created on: Oct 10, 2022
 *      Author: Mai Ba
 */

#include "eeprom_24c32.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "cmsis_os.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

uint8_t eep_write_buffer (uint16_t start_address, char *data,uint16_t lenght)
{
	char result=0;
	result = HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, start_address, I2C_MEMADD_SIZE_16BIT, data, lenght, 100);  // write the data to the EEPROM
	HAL_Delay (T_DELAY);  // Write cycle delay (5ms)
	return result;
}

uint8_t eep_read_buffer (uint16_t start_address, char *data,uint16_t lenght)
{
	char result=0;
	result = HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, start_address, I2C_MEMADD_SIZE_16BIT, data, lenght, 100);
	HAL_Delay (T_DELAY);  // Write cycle delay (5ms)
	return result;
}


uint8_t eep_write_byte (uint16_t address, uint8_t data)
{
	char result=0;
	char buf[1]={data};
	result = HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, address, I2C_MEMADD_SIZE_16BIT, buf, 1, 100);  // write the data to the EEPROM
	HAL_Delay (T_DELAY);  // Write cycle delay (5ms)
	return result;
}

uint8_t eep_read_byte (uint16_t address)
{
	char result=0;
	char buf[1]={0};
	result = HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, address, I2C_MEMADD_SIZE_16BIT, buf, 1, 100);
	HAL_Delay (T_DELAY);  // Write cycle delay (5ms)
	return buf[0];
}


uint8_t eep_write_word (uint16_t address, uint16_t data)
{
	char result=0;
	uint8_t buf[2];
	buf[0]=(data&0xff00)>>8;
	buf[1]=data&0xff;
	result = HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, address*2, I2C_MEMADD_SIZE_16BIT, buf, 2, 100);  // write the data to the EEPROM
	HAL_Delay(T_DELAY);  // Write cycle delay (5ms)
	return result;
}

uint16_t eep_read_word (uint16_t address)
{
	unsigned int result=0;
	uint8_t buf[2]={0};
	result = HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, address*2, I2C_MEMADD_SIZE_16BIT, buf, 2, 100);
	result=(buf[0]<<8)|buf[1];
	HAL_Delay (T_DELAY);
	return result;
}

uint8_t eepWriteFloat(uint16_t address,float value)
{
	//<dấu><phần nguyên><thập phân>
	char result=0;
	float valueDuong=0;
	if(value<0) valueDuong=value*-1; else valueDuong=value;
	uint8_t buf[4]="";
	if(value<0) buf[0]=0; //dấu âm
	else buf[0]=1; //dấu dương
	uint16_t nguyen=(int)valueDuong;
	buf[1]=nguyen/256;
	buf[2]=nguyen%256;
	uint16_t thapPhan=((int)(valueDuong*100))%100;
	buf[3]=thapPhan;
	result=eep_write_word_buffer(address, buf, 4);
	return result;
}

float eepReadFloat(uint16_t address)
{
	char result=0;
	char buf[4]="";
	eep_read_word_buffer(address, buf, 4);
	float value=0;
	value=buf[1]*256+buf[2]+buf[3]*0.01;
	if(buf[0]==0) value*=-1;
	return value;
}

uint8_t eep_write_word_buffer_max10 (uint16_t start_address, uint16_t *data,uint16_t number_write_reg)
{
	uint8_t data8[100];
	uint8_t result=0;
	for(int i=0,j=0;i<number_write_reg*2;i=i+2)
	{
		data8[i]=data[j]/256;
		data8[i+1]=data[j]%256;
		j++;
	}
	result = HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, start_address*2, I2C_MEMADD_SIZE_16BIT, data8, number_write_reg*2, 100);  // write the data to the EEPROM
	HAL_Delay (T_DELAY);  // Write cycle delay (5ms)
	return result;
}

uint8_t eep_read_word_buffer_max10 (uint16_t start_address, uint16_t *data,uint16_t number_read_reg)
{
	char result=0;
	uint8_t data8[100];
	result = HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, start_address*2, I2C_MEMADD_SIZE_16BIT, data8, number_read_reg*2, 100);
	for(int i=0,j=0;i<number_read_reg*2;i=i+2)
	{
		data[j]=data8[i]*256+data8[i+1];
		j++;
	}
	HAL_Delay (T_DELAY);  // Write cycle delay (5ms)
	return result;
}

uint8_t eep_write_word_buffer (uint16_t start_address, uint16_t *data,uint16_t number_write_reg)
{
	/*
	uint8_t res=0;
	int natural=number_write_reg/10;
	int remainder=number_write_reg%10;
	int i;

	//ghi liên tục 10 thanh ghi
	for(i=0;i<natural;i++)
	{
		uint16_t buff[10];
		for(int j=0;j<10;j++) buff[j]=data[i*10+j];
		res=eep_write_word_buffer_max10(start_address+i*10, buff, 10);
	}

	//ghi phần thanh ghi dư
	if(remainder>0)
	{
		uint16_t buff[10];
		for(int j=0;j<remainder;j++) buff[j]=data[i*10+j];
		res=eep_write_word_buffer_max10(start_address+i*10, buff, remainder);
	}
	return res;
	*/
	for(int i=0;i<number_write_reg;i++) eep_write_word(start_address+i, data[i]);
	return 1;
}

uint8_t eep_read_word_buffer (uint16_t start_address, uint16_t *data,uint16_t number_read_reg)
{
	/*
	char result=0;
	int natural=number_read_reg/10;
	int remainder=number_read_reg%10;
	int i;

	//phần nguyên của 10 thanh ghi
	uint8_t data8[number_read_reg*2];
	for(i=0;i<natural;i++)
	{
		result=eep_read_word_buffer_max10(start_address+i*10, data8+i*10, 10);
	}

	//đọc phần dư của vùng nhớ <10 thanh ghi
	if(remainder>0)
		result=eep_read_word_buffer_max10(start_address+i*10, data8+i*10, remainder);

	//chuyển đổi dữ liệu thanh ghi 8bit sang thanh ghi 16bit
	i=0;
	for(int j=0;i<number_read_reg*2;i=i+2)
	{
		data[j]=data8[i]+data8[i+1]*256;
		j++;
	}
	return result;
	*/
	for(int i=0;i<number_read_reg;i++) data[i]=eep_read_word(start_address+i);
	return 1;
}

/*
 *
 */
//uint16_t strlen16


















