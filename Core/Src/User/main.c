/*-----------------------------------------------------------------------------
 * @file        : main.c
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2020/12/26
 * @date changed: 2020/12/26
 * @brief       : 主文件，执行main函数和初始化任务
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#ifdef USER_CONFIG
	#include "user_config.h"
#endif

TaskHandle_t AppTaskCreate_Handle   = NULL; //创建任务句柄

/*-----------------------------------------------------------------------------
 * @name  : main
 * @brief : 主函数
 * @param :
 * @retval:
 * @date  : 2021/08/25
 * @note  :
 * ---------------------------------------------------------------------------*/
int main(void)
{
	/* 定义一个创建信息返回值，默认为pdPASS */
    BaseType_t xReturn = pdPASS;

    /* 板卡级硬件初始化 */
    BSP_Init();

    /* 创建AppTaskCreate任务 */
    xReturn = xTaskCreate((TaskFunction_t) AppTaskCreate,
    		              (const char*   ) "AppTaskCreate",
			              (uint16_t      ) 512,
			              (void*         ) NULL,
			              (UBaseType_t   ) 1,
			              (TaskHandle_t* ) &AppTaskCreate_Handle);
    /* 启动任务调度 */
    if (pdPASS == xReturn)
    {
    	DEBUG_PRINTF("Create_Task sucess...\r\n");
    	/* 启动任务，开启调度 */
        vTaskStartScheduler();
    }
    else
        return -1;

    while(1); /* 正常不会执行到这里 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
