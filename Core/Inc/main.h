/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f2xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
ADC_HandleTypeDef* Get_HADC1_Ptr(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LeftReceiver_Pin GPIO_PIN_1
#define LeftReceiver_GPIO_Port GPIOC
#define RightEncoderCh1_Pin GPIO_PIN_0
#define RightEncoderCh1_GPIO_Port GPIOA
#define RightEncoderCh2_Pin GPIO_PIN_1
#define RightEncoderCh2_GPIO_Port GPIOA
#define FrontLeftReceiver_Pin GPIO_PIN_6
#define FrontLeftReceiver_GPIO_Port GPIOA
#define RightReceiver_Pin GPIO_PIN_5
#define RightReceiver_GPIO_Port GPIOC
#define FrontRightReceiver_Pin GPIO_PIN_1
#define FrontRightReceiver_GPIO_Port GPIOB
#define RightEmitter_Pin GPIO_PIN_12
#define RightEmitter_GPIO_Port GPIOB
#define FrontRightEmitter_Pin GPIO_PIN_6
#define FrontRightEmitter_GPIO_Port GPIOC
#define FrontLeftEmitter_Pin GPIO_PIN_9
#define FrontLeftEmitter_GPIO_Port GPIOC
#define LeftEncoderCh1_Pin GPIO_PIN_8
#define LeftEncoderCh1_GPIO_Port GPIOA
#define LeftEncoderCh2_Pin GPIO_PIN_9
#define LeftEncoderCh2_GPIO_Port GPIOA
#define LeftEmitter_Pin GPIO_PIN_12
#define LeftEmitter_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
