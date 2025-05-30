/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define led_heartbeat_Pin GPIO_PIN_13
#define led_heartbeat_GPIO_Port GPIOC
#define PADDLE_PLUS_Pin GPIO_PIN_1
#define PADDLE_PLUS_GPIO_Port GPIOA
#define PADDLE_MINUS_Pin GPIO_PIN_2
#define PADDLE_MINUS_GPIO_Port GPIOA
#define CANBUS_CS_Pin GPIO_PIN_4
#define CANBUS_CS_GPIO_Port GPIOA
#define SDCARD_CS_Pin GPIO_PIN_0
#define SDCARD_CS_GPIO_Port GPIOB
#define TOUCHSCREEN_CS_Pin GPIO_PIN_1
#define TOUCHSCREEN_CS_GPIO_Port GPIOB
#define button4INT_Pin GPIO_PIN_13
#define button4INT_GPIO_Port GPIOB
#define button4INT_EXTI_IRQn EXTI15_10_IRQn
#define button1INT_Pin GPIO_PIN_8
#define button1INT_GPIO_Port GPIOA
#define button1INT_EXTI_IRQn EXTI9_5_IRQn
#define button2INT_Pin GPIO_PIN_9
#define button2INT_GPIO_Port GPIOA
#define button2INT_EXTI_IRQn EXTI9_5_IRQn
#define button3INT_Pin GPIO_PIN_10
#define button3INT_GPIO_Port GPIOA
#define button3INT_EXTI_IRQn EXTI15_10_IRQn
#define LCDCS_Pin GPIO_PIN_3
#define LCDCS_GPIO_Port GPIOB
#define LCDRS_Pin GPIO_PIN_4
#define LCDRS_GPIO_Port GPIOB
#define LCDRESET_Pin GPIO_PIN_5
#define LCDRESET_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
