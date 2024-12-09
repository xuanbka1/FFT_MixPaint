#include "myEepromUtility.h"

eep_t eepData[NUMBER_VAR_EEP];

void eepUtil_init(char *listName)
{
	const char delimiter[] = ",";
	char *token;
	int dataCount = 0;
	token = strtok(listName, delimiter);
	while (token != NULL && dataCount < NUMBER_VAR_EEP) 
	{
		strncpy(eepData[dataCount].name, token, sizeof(eepData[dataCount].name) - 1);
		eepData[dataCount].name[sizeof(eepData[dataCount].name) - 1] = '\0';
		token = strtok(NULL, delimiter);
		dataCount++;
	}
}

int findIndexWithName(char name[NAME_LENGTH])
{
    for(int i=0;i<NUMBER_VAR_EEP;i++) if(strcmp(name,eepData[i].name)==0) return i;
    return -1;
}

bool eepUtil_readWithName(char name[NAME_LENGTH],uint16_t result)
{
	int index=-1;
	index=findIndexWithName(name);
	if(index>=0) 
	{
		uint16_t address=index*2;
		result=eep_read_word(address);
		return true;
	}
	return false;
}	

bool eepUtil_writeWithName(char name[NAME_LENGTH],uint16_t value)
{
	int index=-1;
	index=findIndexWithName(name);
	if(index>=0) 
	{
		uint16_t address=index*2;
		value=eep_write_word(address,value);
		return true;
	}
	return false;
}	












