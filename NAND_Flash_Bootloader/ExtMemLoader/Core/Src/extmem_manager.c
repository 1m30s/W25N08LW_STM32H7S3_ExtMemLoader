/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : extmem_manager.c
  * @version        : 1.0.0
  * @brief          : This file implements the extmem configuration
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

/* Includes ------------------------------------------------------------------*/
#include "extmem_manager.h"
#include <string.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

extern XSPI_HandleTypeDef hxspi1;
extern XSPI_HandleTypeDef hxspi2;
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Init External memory manager
  * @retval None
  */
void MX_EXTMEM_MANAGER_Init(void)
{

  /* USER CODE BEGIN MX_EXTMEM_Init_PreTreatment */
#if(0)

  /* USER CODE END MX_EXTMEM_Init_PreTreatment */

  /* Initialization of the memory parameters */
  memset(extmem_list_config, 0x0, sizeof(extmem_list_config));

  /* EXTMEMORY_1 */
  extmem_list_config[0].MemType = EXTMEM_USER;

  EXTMEM_Init(EXTMEMORY_1, 0);

  /* USER CODE BEGIN MX_EXTMEM_Init_PostTreatment */

#endif
  /* Initialization of the memory parameters */
  memset(extmem_list_config, 0x0, sizeof(extmem_list_config));

  extmem_list_config[0].MemType = EXTMEM_CUSTOM;
  extmem_list_config[0].Handle = &hxspi1;
//  extmem_list_config[0].CustomObject = extmem_w958d6nb; // 16bit hyperram
  extmem_list_config[0].CustomObject = extmem_W959D8NF;
  EXTMEM_Init(EXTMEMORY_1, 400000000); // XSPI Clock = 400MHz (Note: clock must be specified for custom driver!!)

  /* USER CODE END MX_EXTMEM_Init_PostTreatment */
}
