/**
  ******************************************************************************
  * @file    iwdg.c
  * @brief   This file provides code for the configuration
  *          of the IWDG instances.
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
#ifdef USER_CONFIG
	#include "user_config.h"
#endif

IWDG_HandleTypeDef hiwdg1;

/*-----------------------------------------------------------------------------
 * @name  : MX_IWDG1_Init
 * @brief : IWDG1 init function
 * @param :
 * @retval:
 * @date  : 2021/08/27
 * @note  : 看门狗使用内部32K的时钟（有偏差）
 * 			溢出时间T=(2500*128)/32 = 10000ms = 10s
 * ---------------------------------------------------------------------------*/
static void MX_IWDG1_Init(void)
{
	hiwdg1.Instance = IWDG1;
	hiwdg1.Init.Prescaler = IWDG_PRESCALER_128;	//设置 IWDG 分频系数
	hiwdg1.Init.Window = IWDG_WINDOW_DISABLE;	//4095 //窗口值配置为缺省的最大值 0xfff 就是默认关闭窗口功能
	hiwdg1.Init.Reload = 2500;                  //重装载值
	if (HAL_IWDG_Init(&hiwdg1) != HAL_OK)
	{
		Error_Handler();
	}
}

/*-----------------------------------------------------------------------------
 * @name  : IWDG_Feed
 * @brief : 喂独立看门狗
 * @param :
 * @retval:
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
static void IWDG_Feed(void)
{
	HAL_IWDG_Refresh(&hiwdg1); //重装载
}

/*-----------------------------------------------------------------------------
 * @name  : Watchdog_Task
 * @brief : 系统看门狗事件任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
void Watchdog_Task(void *parameter)
{
	EventBits_t uxBitsToWaitFor = 0;

	MX_IWDG1_Init();

	while(1)
	{
		//其他线程正常才事件触发，进入喂狗
		//逻辑与运算，退出时清除事件
		//永久阻塞等待事件
		uxBitsToWaitFor = xEventGroupWaitBits(WatchDogEvent,
				                              FEED_DOG_EVENT,
											  pdTRUE, pdTRUE,
											  portMAX_DELAY);

		/* 如果其他线程正常 */
		if ((FEED_DOG_EVENT & uxBitsToWaitFor) == FEED_DOG_EVENT)
		{
			ThreadTime(0);

			IWDG_Feed();
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
