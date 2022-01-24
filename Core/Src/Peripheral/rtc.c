/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */
//判断条件数值
#define RECORDNUM     0x1215

RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;
/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{
	/** Initialize RTC Only
	*/
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 0x007F;
	hrtc.Init.SynchPrediv = 0x00FF;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initialize RTC and set the Time and Date
	* 通过判断备份寄存器R0,若不等于判断条件，即执行时间初始化
	* 此举为了防止STM32每次上电都初始化时间设置，当有需要时，只需要改写判断条件数值即可*/
	uint16_t record = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0);
	if(record != RECORDNUM)
	{
		RTC_Set_TimeDate(2021, 9, 1, RTC_WEEKDAY_MONDAY, 10 , 00, 00);
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, RECORDNUM);
	}
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	if(rtcHandle->Instance==RTC)
	{
		/* USER CODE BEGIN RTC_MspInit 0 */

		/* USER CODE END RTC_MspInit 0 */
		/** Initializes the peripherals clock
		*/
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
		PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		/* RTC clock enable */
		__HAL_RCC_RTC_ENABLE();
		/* USER CODE BEGIN RTC_MspInit 1 */

		/* USER CODE END RTC_MspInit 1 */
	}
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{
	if(rtcHandle->Instance==RTC)
	{
		/* USER CODE BEGIN RTC_MspDeInit 0 */

		/* USER CODE END RTC_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_RTC_DISABLE();
		/* USER CODE BEGIN RTC_MspDeInit 1 */

		/* USER CODE END RTC_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */
/*-----------------------------------------------------------------------------
 * @name  : RTC_Set_TimeDate
 * @brief : RTC设置时间函数
 * @param : year:年份（2021-2099） month（1-12 ） date（0-31） weekday(周几)   hours（0-24） minutes(0-60) seconds(0-60)
 * @retval: None
 * @date  : 2021/06/26
 * @note  :
 * ---------------------------------------------------------------------------*/
void RTC_Set_TimeDate(uint16_t year, uint8_t month, uint8_t date, uint8_t weekday, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	sDate.WeekDay = weekday;
	sDate.Month = month;
	sDate.Date = date;
	sDate.Year = year-2000;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	sTime.Hours = hours;
	sTime.Minutes = minutes;
	sTime.Seconds = seconds;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

/*-----------------------------------------------------------------------------
 * @name  : RTC_Read_TimeDate
 * @brief : RTC读取时间函数
 * @param : TimeStruct: 储存时间的结构体地址， DateStruct：储存日期的结构体地址
 * @retval: None
 * @date  : 2021/06/26
 * @note  :
 * ---------------------------------------------------------------------------*/
void RTC_Read_TimeDate(RTC_TimeTypeDef* TimeStruct, RTC_DateTypeDef* DateStruct)
{
	HAL_RTC_GetTime(&hrtc, TimeStruct, FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, DateStruct, FORMAT_BIN);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
