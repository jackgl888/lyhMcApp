/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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

/* Includes ------------------------------------------------------------------*/
#ifdef USER_CONFIG
	#include "user_config.h"
#endif
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim3;


/*-----------------------------------------------------------------------------
 * @name  : MX_TIM3_Init
 * @brief : 通用定时器3中断初始化,定时器2在APB1上，APB1的定时器时钟为240MHz
 * @param : arr：自动重装值
 * 			psc：时钟预分频数
 * @retval:
 * @date  : 2020/07/20
 * @note  : 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us
 * 			Ft=定时器工作频率,单位:Mhz
 * 			定时器3挂在APB1上，时钟为HCLK/2
 * ---------------------------------------------------------------------------*/
void MX_TIM3_Init(uint16_t arr, uint16_t psc)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = psc;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = arr;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_Base_Start_IT(&htim3); //开启定时器中断
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
	/* TIM2 clock enable */
	__HAL_RCC_TIM3_CLK_ENABLE();

	/* TIM3 interrupt Init */
	HAL_NVIC_SetPriority(TIM3_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
  }

}


void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance==TIM3)
  {
	/* Peripheral clock disable */
	__HAL_RCC_TIM3_CLK_DISABLE();
	/* TIM3 interrupt Deinit */
	HAL_NVIC_DisableIRQ(TIM3_IRQn);
  }
}


/**
  * @brief This function handles TIM3 global interrupt.
  * 100ms一次中断
  */
void TIM3_IRQHandler(void)
{
	/* USER CODE BEGIN TIM3_IRQn 0 */
//-----------------------------------------------------------------------------
	static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	static uint32_t timeMS = 0, timeS = 0; //时间计数：定时器时基-毫秒、计时-秒

	/* 通信同步 */
	if(tCommPro.Comm_Sync_Flag == COMM_OK)
    {
		if(timeMS != 0)
		{
			if((timeMS % 1000) == 0) //1秒触发一次
			{
				timeS++;

				xEventGroupSetBitsFromISR(TimingCommEvent, TIMING_COMM_GET_BOARD_DATA_EVENT, &xHigherPriorityTaskWoken); //事件置位
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			}
		}

		timeMS += 100; //100ms一次中断
		if(timeMS > 4000000000)
		{
			timeMS = 0; //清零
			timeS = 0; //清零
		}
    }

    /* TCP 超时时间计算 */
    for(uint8_t i=0; i<3; i++)
    {
        if(tCommPro.Tcp_Comm_Flag[i] == 1)
        {
            tCommPro.Tcp_Timeout_MS[i] += 100; //100ms一次中断
        	if((tCommPro.Tcp_Timeout_MS[i] != 0) && ((tCommPro.Tcp_Timeout_MS[i] % 1000) == 0)) //1秒触发一次
        	{
        		tCommPro.Tcp_Timeout_S[i]++;
        	}
        	if(tCommPro.Tcp_Timeout_MS[i] > 4000000000)
        	{
        		tCommPro.Tcp_Timeout_MS[i] = 0; //清零
        		tCommPro.Tcp_Timeout_S[i] = 0; //清零
        	}
        }
        else if(tCommPro.Tcp_Comm_Flag[i] == 0)
        {
    		tCommPro.Tcp_Timeout_MS[i] = 0; //清零
    		tCommPro.Tcp_Timeout_S[i] = 0; //清零
        }
    }

//-----------------------------------------------------------------------------

	/* USER CODE END TIM3_IRQn 0 */
	HAL_TIM_IRQHandler(&htim3);

	/* USER CODE BEGIN TIM3_IRQn 1 */

	/* USER CODE END TIM3_IRQn 1 */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
