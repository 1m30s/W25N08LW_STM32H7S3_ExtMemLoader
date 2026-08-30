/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    extmemloader_init.h
  * @author  MCD Application Team
  * @brief   Header file of Loader_Src.c
  *
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
#ifndef EXTMEMLOADER_INIT_H
#define EXTMEMLOADER_INIT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7rsxx_hal.h"

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

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

uint32_t extmemloader_Init(void);
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#define CN8_16_Pin GPIO_PIN_3
#define CN8_16_GPIO_Port GPIOG
#define CN8_14_Pin GPIO_PIN_2
#define CN8_14_GPIO_Port GPIOG
#define CN7_1_Pin GPIO_PIN_6
#define CN7_1_GPIO_Port GPIOC
#define CN7_11_Pin GPIO_PIN_7
#define CN7_11_GPIO_Port GPIOC
#define CN7_9_Pin GPIO_PIN_15
#define CN7_9_GPIO_Port GPIOA
#define CN7_7_Pin GPIO_PIN_12
#define CN7_7_GPIO_Port GPIOB
#define CN7_3_Pin GPIO_PIN_15
#define CN7_3_GPIO_Port GPIOB
#define CN7_5_Pin GPIO_PIN_13
#define CN7_5_GPIO_Port GPIOB
#endif /* EXTMEMLOADER_INIT_H */
