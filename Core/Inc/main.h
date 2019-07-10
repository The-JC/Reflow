/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_dma.h"

#include "stm32f1xx_ll_exti.h"

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
void InitSystem(void);
void TimerCaptureCompare_Callback(void);
uint32_t getTimeDelay(void);
void setTime(uint32_t t);
void setTemp(uint16_t t);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HEATER_Pin GPIO_PIN_6
#define HEATER_GPIO_Port GPIOA
#define ZEROX_Pin GPIO_PIN_7
#define ZEROX_GPIO_Port GPIOA
#define ZEROX_EXTI_IRQn EXTI9_5_IRQn
#define LD_Power_Pin GPIO_PIN_4
#define LD_Power_GPIO_Port GPIOC
#define LEFT_Pin GPIO_PIN_0
#define LEFT_GPIO_Port GPIOB
#define RIGHT_Pin GPIO_PIN_1
#define RIGHT_GPIO_Port GPIOB
#define DOWN_Pin GPIO_PIN_2
#define DOWN_GPIO_Port GPIOB
#define DOWN_EXTI_IRQn EXTI2_IRQn
#define SELECT_Pin GPIO_PIN_10
#define SELECT_GPIO_Port GPIOB
#define SELECT_EXTI_IRQn EXTI15_10_IRQn
#define UP_Pin GPIO_PIN_11
#define UP_GPIO_Port GPIOB
#define UP_EXTI_IRQn EXTI15_10_IRQn
#define CS2_Pin GPIO_PIN_12
#define CS2_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_15
#define CS_GPIO_Port GPIOB
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
