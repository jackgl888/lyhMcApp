/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sd_diskio.h
  * @brief   Header for sd_diskio.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Note: code generation based on sd_diskio_dma_rtos_template.h */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NAND_DISKIO_H
#define __NAND_DISKIO_H

/* USER CODE BEGIN firstSection */
/* can be used to modify / undefine following code or add new definitions */
/* USER CODE END firstSection */

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#define NAND_DISK_USE_OS

#ifdef NAND_DISK_USE_OS
#include "FreeRTOS.h"
#include "semphr.h"
#define	NAND_DISK_API_CREATELOCK(lock)		do{lock = xSemaphoreCreateMutex();}while(0);
#define NAND_DISK_API_DELETELOCK(lock)		do{(lock!=NULL) ? vSemaphoreDelete(lock) : pdFAIL;}while(0);
#define	NAND_DISK_API_LOCK(lock)			do{(lock!=NULL) ? xSemaphoreTake(lock, portMAX_DELAY) : pdFAIL;}while(0);
#define NAND_DISK_API_UNLOCK(lock)			do{(lock!=NULL) ? xSemaphoreGive(lock) : pdFAIL;}while(0);
#else
#define	NAND_DISK_API_CREATELOCK(lock)
#define NAND_DISK_API_DELETELOCK(lock)
#define	NAND_DISK_API_LOCK(lock)
#define NAND_DISK_API_UNLOCK(lock)
#endif

extern const Diskio_drvTypeDef  NAND_Driver;

/* USER CODE BEGIN lastSection */
/* can be used to modify / undefine previous code or add new definitions */
/* USER CODE END lastSection */

#endif /* __SD_DISKIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

