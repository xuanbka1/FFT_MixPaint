#ifndef MYEEPROMUTILITY_H
#define MYEEPROMUTILITY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "eeprom_24c32.h"

#define NUMBER_VAR_EEP 20
#define NAME_LENGTH 30
typedef struct 
{
    char name[NAME_LENGTH];
    uint16_t value;
}eep_t;

extern eep_t eepData[NUMBER_VAR_EEP];

void eepUtil_init(char *listName);
int  findIndexWithName(char name[NAME_LENGTH]);
bool eepUtil_readWithName(char name[NAME_LENGTH],uint16_t result);
bool eepUtil_writeWithName(char name[NAME_LENGTH],uint16_t value);

#endif

