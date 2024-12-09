/*
 * buzzer.h
 *
 *  Created on: Dec 1, 2023
 *      Author: ADMIN
 */

#ifndef USER_LIB_BUZZER_H_
#define USER_LIB_BUZZER_H_


#ifdef __cplusplus
 extern "C" {
#endif
#include "stdint.h"
#include "stdio.h"
#include "main.h"

#define BEEP_TIME  100  //500ms

void buzzer_configuration(void);
void buzzer_on(void);
void buzzer_off(void);
void buzzer_toggle(void);
void buzzer_set(uint16_t beep_time);
void buzzer_loop(uint16_t loop_num);

#ifdef __cplusplus
}
#endif

#endif /* USER_LIB_BUZZER_H_ */
