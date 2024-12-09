/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PUL11_Pin GPIO_PIN_2
#define PUL11_GPIO_Port GPIOE
#define EN11_Pin GPIO_PIN_3
#define EN11_GPIO_Port GPIOE
#define DIR11_Pin GPIO_PIN_4
#define DIR11_GPIO_Port GPIOE
#define PUL10_Pin GPIO_PIN_5
#define PUL10_GPIO_Port GPIOE
#define EN10_Pin GPIO_PIN_6
#define EN10_GPIO_Port GPIOE
#define DIR10_Pin GPIO_PIN_0
#define DIR10_GPIO_Port GPIOC
#define PUL9_Pin GPIO_PIN_1
#define PUL9_GPIO_Port GPIOC
#define EN9_Pin GPIO_PIN_2
#define EN9_GPIO_Port GPIOC
#define DIR9_Pin GPIO_PIN_3
#define DIR9_GPIO_Port GPIOC
#define INPUT1_Pin GPIO_PIN_0
#define INPUT1_GPIO_Port GPIOA
#define INPUT1_EXTI_IRQn EXTI0_IRQn
#define INPUT2_Pin GPIO_PIN_1
#define INPUT2_GPIO_Port GPIOA
#define INPUT2_EXTI_IRQn EXTI1_IRQn
#define INPUT3_Pin GPIO_PIN_2
#define INPUT3_GPIO_Port GPIOA
#define INPUT3_EXTI_IRQn EXTI2_IRQn
#define INPUT4_Pin GPIO_PIN_3
#define INPUT4_GPIO_Port GPIOA
#define INPUT4_EXTI_IRQn EXTI3_IRQn
#define LAZE_Pin GPIO_PIN_4
#define LAZE_GPIO_Port GPIOA
#define PUL8_Pin GPIO_PIN_5
#define PUL8_GPIO_Port GPIOA
#define EN8_Pin GPIO_PIN_6
#define EN8_GPIO_Port GPIOA
#define DIR8_Pin GPIO_PIN_7
#define DIR8_GPIO_Port GPIOA
#define PUL7_Pin GPIO_PIN_4
#define PUL7_GPIO_Port GPIOC
#define EN7_Pin GPIO_PIN_5
#define EN7_GPIO_Port GPIOC
#define DIR7_Pin GPIO_PIN_0
#define DIR7_GPIO_Port GPIOB
#define PUL6_Pin GPIO_PIN_1
#define PUL6_GPIO_Port GPIOB
#define EN6_Pin GPIO_PIN_2
#define EN6_GPIO_Port GPIOB
#define DIR6_Pin GPIO_PIN_7
#define DIR6_GPIO_Port GPIOE
#define PUL5_Pin GPIO_PIN_8
#define PUL5_GPIO_Port GPIOE
#define EN5_Pin GPIO_PIN_10
#define EN5_GPIO_Port GPIOE
#define DIR5_Pin GPIO_PIN_11
#define DIR5_GPIO_Port GPIOE
#define PUL4_Pin GPIO_PIN_12
#define PUL4_GPIO_Port GPIOE
#define EN4_Pin GPIO_PIN_13
#define EN4_GPIO_Port GPIOE
#define DIR4_Pin GPIO_PIN_14
#define DIR4_GPIO_Port GPIOE
#define PUL3_Pin GPIO_PIN_15
#define PUL3_GPIO_Port GPIOE
#define BTN_Pin GPIO_PIN_12
#define BTN_GPIO_Port GPIOB
#define ST2_Pin GPIO_PIN_13
#define ST2_GPIO_Port GPIOB
#define ST1_Pin GPIO_PIN_14
#define ST1_GPIO_Port GPIOB
#define BUZZ_Pin GPIO_PIN_15
#define BUZZ_GPIO_Port GPIOB
#define EN3_Pin GPIO_PIN_13
#define EN3_GPIO_Port GPIOD
#define DIR3_Pin GPIO_PIN_14
#define DIR3_GPIO_Port GPIOD
#define PUL2_Pin GPIO_PIN_15
#define PUL2_GPIO_Port GPIOD
#define EN2_Pin GPIO_PIN_6
#define EN2_GPIO_Port GPIOC
#define DIR2_Pin GPIO_PIN_7
#define DIR2_GPIO_Port GPIOC
#define PUL1_Pin GPIO_PIN_8
#define PUL1_GPIO_Port GPIOC
#define EN1_Pin GPIO_PIN_9
#define EN1_GPIO_Port GPIOC
#define DIR1_Pin GPIO_PIN_8
#define DIR1_GPIO_Port GPIOA
#define EN_ALL_16_Pin GPIO_PIN_15
#define EN_ALL_16_GPIO_Port GPIOA
#define PUL_ALL_16_Pin GPIO_PIN_10
#define PUL_ALL_16_GPIO_Port GPIOC
#define DIR_ALL_16_Pin GPIO_PIN_11
#define DIR_ALL_16_GPIO_Port GPIOC
#define PUL17_Pin GPIO_PIN_12
#define PUL17_GPIO_Port GPIOC
#define EN17_Pin GPIO_PIN_0
#define EN17_GPIO_Port GPIOD
#define DIR17_Pin GPIO_PIN_1
#define DIR17_GPIO_Port GPIOD
#define PUL16_Pin GPIO_PIN_2
#define PUL16_GPIO_Port GPIOD
#define EN16_Pin GPIO_PIN_3
#define EN16_GPIO_Port GPIOD
#define DIR16_Pin GPIO_PIN_4
#define DIR16_GPIO_Port GPIOD
#define PUL15_Pin GPIO_PIN_5
#define PUL15_GPIO_Port GPIOD
#define EN15_Pin GPIO_PIN_6
#define EN15_GPIO_Port GPIOD
#define DIR15_Pin GPIO_PIN_7
#define DIR15_GPIO_Port GPIOD
#define PUL14_Pin GPIO_PIN_3
#define PUL14_GPIO_Port GPIOB
#define EN14_Pin GPIO_PIN_4
#define EN14_GPIO_Port GPIOB
#define DIR14_Pin GPIO_PIN_5
#define DIR14_GPIO_Port GPIOB
#define PUL13_Pin GPIO_PIN_6
#define PUL13_GPIO_Port GPIOB
#define EN13_Pin GPIO_PIN_7
#define EN13_GPIO_Port GPIOB
#define DIR13_Pin GPIO_PIN_8
#define DIR13_GPIO_Port GPIOB
#define PUL12_Pin GPIO_PIN_9
#define PUL12_GPIO_Port GPIOB
#define EN12_Pin GPIO_PIN_0
#define EN12_GPIO_Port GPIOE
#define DIR12_Pin GPIO_PIN_1
#define DIR12_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

//redefine for new code HARD_//
//redefine for new code HARD_//
#define SPEAKER_GPIO_Port   GPIOB
#define SPEAKER_Pin         GPIO_PIN_15

#define SENSOR1_GPIO_Port   INPUT4_GPIO_Port   // Close Door //
#define SENSOR1_Pin         INPUT4_Pin

#define SENSOR2_GPIO_Port   INPUT3_GPIO_Port   // Open Door
#define SENSOR2_Pin         INPUT3_Pin

#define SENSOR3_GPIO_Port   INPUT2_GPIO_Port   // Thung SON //
#define SENSOR3_Pin         INPUT2_Pin

//refefine PORT-PIn for PWM Pump - edit bye xuan //

#define PWM0_Pin 			PUL1_Pin
#define PWM0_GPIO_Port 		PUL1_GPIO_Port

#define PWM1_Pin 			PUL2_Pin
#define PWM1_GPIO_Port 		PUL2_GPIO_Port

#define PWM2_Pin 			PUL3_Pin
#define PWM2_GPIO_Port 		PUL3_GPIO_Port

#define PWM3_Pin 			PUL4_Pin
#define PWM3_GPIO_Port 		PUL4_GPIO_Port

#define PWM4_Pin 			PUL5_Pin
#define PWM4_GPIO_Port 		PUL5_GPIO_Port

#define PWM5_Pin 			PUL6_Pin
#define PWM5_GPIO_Port 		PUL6_GPIO_Port

#define PWM6_Pin 			PUL7_Pin
#define PWM6_GPIO_Port 		PUL7_GPIO_Port

#define PWM7_Pin 			PUL8_Pin
#define PWM7_GPIO_Port 		PUL8_GPIO_Port

#define PWM8_Pin 			PUL9_Pin
#define PWM8_GPIO_Port 		PUL9_GPIO_Port

#define PWM9_Pin 			PUL10_Pin
#define PWM9_GPIO_Port 		PUL10_GPIO_Port

#define PWM10_Pin 			PUL11_Pin
#define PWM10_GPIO_Port 	PUL11_GPIO_Port

#define PWM11_Pin 			PUL12_Pin
#define PWM11_GPIO_Port 	PUL12_GPIO_Port

#define PWM12_Pin 			PUL13_Pin
#define PWM12_GPIO_Port 	PUL13_GPIO_Port

#define PWM13_Pin 			PUL14_Pin
#define PWM13_GPIO_Port 	PUL14_GPIO_Port

#define PWM14_Pin 			PUL15_Pin
#define PWM14_GPIO_Port 	PUL15_GPIO_Port

#define PWM15_Pin 			PUL16_Pin
#define PWM15_GPIO_Port 	PUL16_GPIO_Port

#define PWM_TRON_Pin 		PUL_ALL_16_Pin
#define PWM_TRON_GPIO_Port 	PUL_ALL_16_GPIO_Port


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
