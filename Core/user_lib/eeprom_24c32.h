/*
 * eeprom_24c32.h
 *
 *  Created on: Oct 10, 2022
 *      Author: Mai Ba
 */

#ifndef __EEPROM_24C32_H_
#define __EEPROM_24C32_H_
#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

extern I2C_HandleTypeDef    hi2c2;
#define EEPROM_I2C          &hi2c2
#define T_DELAY 10
// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0

uint8_t eep_write_buffer (uint16_t start_address, char *data,uint16_t lenght);
uint8_t eep_read_buffer (uint16_t start_address, char *data,uint16_t lenght);
uint8_t eep_write_byte (uint16_t address, uint8_t data);
uint8_t eep_read_byte (uint16_t address);

uint8_t eep_write_word (uint16_t address, uint16_t data);
uint16_t eep_read_word (uint16_t address);
uint8_t eep_write_word_buffer_max10 (uint16_t start_address, uint16_t *data,uint16_t number_write_reg);
uint8_t eep_read_word_buffer_max10 (uint16_t start_address, uint16_t *data,uint16_t number_read_reg);
uint8_t eep_write_word_buffer (uint16_t start_address, uint16_t *data,uint16_t number_write_reg);
uint8_t eep_read_word_buffer (uint16_t start_address, uint16_t *data,uint16_t number_read_reg);
uint8_t eepWriteFloat(uint16_t address,float value);
float eepReadFloat(uint16_t address);


#endif


