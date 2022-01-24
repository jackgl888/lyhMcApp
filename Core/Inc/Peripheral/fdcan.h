/**
  ******************************************************************************
  * @file    fdcan.h
  * @brief   This file contains all the function prototypes for
  *          the fdcan.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FDCAN_H__
#define __FDCAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
/* CAN收发数据部分格式 */
typedef union
{
	uint64_t data64;
	int32_t data32[2];
	int16_t data16[4];
	uint8_t data8[8];
}canData;

typedef union
{
	uint16_t id16;
	uint8_t id8[2];
}canID;

//CAN句柄和数据缓冲
extern FDCAN_HandleTypeDef FDCAN1_Handler;
extern uint8_t can1RxRTR;			//CAN远程帧标志缓存
extern canID can1RxID;			    //CAN帧ID缓存
extern canData can1RxData;			//CAN数据帧缓存
extern canData can1TxData;		    //CAN数据帧缓存

extern FDCAN_HandleTypeDef FDCAN2_Handler;
extern uint8_t can2RxRTR;			//CAN远程帧标志缓存
extern canID can2RxID;			    //CAN帧ID缓存
extern canData can2RxData;			//CAN数据帧缓存
extern canData can2TxData;		    //CAN数据帧缓存
/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
/* FDCAN初始化、收发函数声明 */
uint8_t FDCAN1_Mode_Init(uint16_t presc, uint8_t ntsjw, uint16_t ntsg1,uint8_t ntsg2, uint32_t mode);
uint8_t CAN1_Filter_Init(FDCAN_HandleTypeDef *hfdcan, uint32_t FilterIndex, uint32_t FilterID1, uint32_t FilterID2);
uint8_t FDCAN1_Send_Msg(uint8_t *msg, uint32_t len, uint32_t id);
uint8_t FDCAN1_Receive_Msg(uint8_t *buf);
uint8_t EmergencyDirective1(uint32_t id);
uint8_t FDCAN2_Mode_Init(uint16_t presc, uint8_t ntsjw, uint16_t ntsg1, uint8_t ntsg2, uint32_t mode);
uint8_t CAN2_Filter_Init(FDCAN_HandleTypeDef *hfdcan, uint32_t FilterIndex, uint32_t FilterID1, uint32_t FilterID2);
uint8_t FDCAN2_Send_Msg(uint8_t *msg, uint32_t len, uint32_t id);
uint8_t FDCAN2_Receive_Msg(uint8_t *buf);
uint8_t EmergencyDirective2(uint32_t id);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __FDCAN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
