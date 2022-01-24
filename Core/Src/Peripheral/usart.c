/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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

/* 串口句柄 */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart6;
UART_HandleTypeDef huart7;
UART_HandleTypeDef huart8;

DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_uart5_rx;
DMA_HandleTypeDef hdma_uart5_tx;
DMA_HandleTypeDef hdma_usart6_rx;
DMA_HandleTypeDef hdma_usart6_tx;
DMA_HandleTypeDef hdma_uart7_rx;
DMA_HandleTypeDef hdma_uart7_tx;
DMA_HandleTypeDef hdma_uart8_rx;
DMA_HandleTypeDef hdma_uart8_tx;

//串口接收缓冲区
uint8_t uart1RxBuf[UART1_RX_SIZE] = {0};
uint8_t uart2RxBuf[UART2_RX_SIZE] = {0};
uint8_t uart3RxBuf[UART3_RX_SIZE] = {0};
uint8_t uart4RxBuf[UART4_RX_SIZE] = {0};
uint8_t uart5RxBuf[UART5_RX_SIZE] = {0};
uint8_t uart6RxBuf[UART6_RX_SIZE] = {0};
uint8_t uart7RxBuf[UART7_RX_SIZE] = {0};
uint8_t uart8RxBuf[UART8_RX_SIZE] = {0};


/*-----------------------------------------------------------------------------
 * @name  : MX_USART1_UART_Init
 * 			MX_USART2_UART_Init
 * 			MX_USART3_UART_Init
 * 			MX_UART4_UART_Init
 * 			MX_UART5_UART_Init
 * 			MX_USART6_UART_Init
 * 			MX_UART7_UART_Init
 * 			MX_UART8_UART_Init
 * @brief : 串口功能配置函数
 * @param : None
 * @retval: None
 * @date  : 2021/08/26
 * @note  : 默认串口为：8位数据、无奇偶校验、1个停止位
 * ---------------------------------------------------------------------------*/
/* USART1 init function */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = MY_USART1_BAUDRATE;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

  /* 开启DMA接收 */
#if( MY_USART1_DMA_RX == 1 )
  HAL_UART_Receive_DMA(&huart1, uart1RxBuf, UART1_RX_SIZE);
#endif

}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = MY_USART2_BAUDRATE;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

  /* 开启DMA接收 */
#if( MY_USART2_DMA_RX == 1 )
  HAL_UART_Receive_DMA(&huart2, uart2RxBuf, UART2_RX_SIZE);
#endif

}

/* USART3 init function */
void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART2;
  huart3.Init.BaudRate = MY_USART3_BAUDRATE;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

  /* 开启DMA接收 */
#if( MY_USART3_DMA_RX == 1 )
  HAL_UART_Receive_DMA(&huart3, uart3RxBuf, UART3_RX_SIZE);
#endif

}

/* UART4 init function */
void MX_UART4_UART_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = MY_UART4_BAUDRATE;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
  {
    Error_Handler();
  }

  /* 开启DMA接收 */
#if( MY_UART4_DMA_RX == 1 )
  HAL_UART_Receive_DMA(&huart4, uart4RxBuf, UART4_RX_SIZE);
#endif

}

/* UART5 init function */
void MX_UART5_UART_Init(void)
{

  huart5.Instance = UART5;
  huart5.Init.BaudRate = MY_UART5_BAUDRATE;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart5, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart5, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart5) != HAL_OK)
  {
    Error_Handler();
  }

  /* 开启DMA接收 */
#if( MY_UART5_DMA_RX == 1 )
  HAL_UART_Receive_DMA(&huart5, uart5RxBuf, UART5_RX_SIZE);
#endif

}

/* USART6 init function */
void MX_USART6_UART_Init(void)
{

  huart6.Instance = USART6;
  huart6.Init.BaudRate = MY_USART6_BAUDRATE;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart6.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart6, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart6, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart6) != HAL_OK)
  {
    Error_Handler();
  }

  /* 开启DMA接收 */
#if( MY_USART6_DMA_RX == 1 )
  HAL_UART_Receive_DMA(&huart6, uart6RxBuf, UART6_RX_SIZE);
#endif

}

/* UART7 init function */
void MX_UART7_UART_Init(void)
{

  huart7.Instance = UART7;
  huart7.Init.BaudRate = MY_UART7_BAUDRATE;
  huart7.Init.WordLength = UART_WORDLENGTH_8B;
  huart7.Init.StopBits = UART_STOPBITS_1;
  huart7.Init.Parity = UART_PARITY_NONE;
  huart7.Init.Mode = UART_MODE_TX_RX;
  huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart7.Init.OverSampling = UART_OVERSAMPLING_16;
  huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart7.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart7) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart7, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart7, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart7) != HAL_OK)
  {
    Error_Handler();
  }

  /* 开启DMA接收 */
#if( MY_UART7_DMA_RX == 1 )
  HAL_UART_Receive_DMA(&huart7, uart7RxBuf, UART7_RX_SIZE);
#endif

}

/* UART8 init function */
void MX_UART8_UART_Init(void)
{

  huart8.Instance = UART8;
  huart8.Init.BaudRate = MY_UART8_BAUDRATE;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  huart8.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart8.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK)
  {
    Error_Handler();
  }

  /* 开启DMA接收 */
#if( MY_UART8_DMA_RX == 1 )
  HAL_UART_Receive_DMA(&huart8, uart8RxBuf, UART8_RX_SIZE);
#endif

}

/*-----------------------------------------------------------------------------
 * @name  : MX_USART_UART_Init
 * @brief : 板卡级串口初始化函数
 * @param :
 * @retval:
 * @date  : 2021/08/26
 * @note  :
 * ---------------------------------------------------------------------------*/
void MX_USART_UART_Init(void)
{
#if MY_USART1_ENABLE1
	MX_USART1_UART_Init();
#endif

#if MY_USART2_ENABLE1
	MX_USART2_UART_Init();
#endif

#if MY_USART3_ENABLE1
	MX_USART3_UART_Init();
#endif

#if MY_UART4_ENABLE1
	MX_UART4_UART_Init();
#endif

#if MY_UART5_ENABLE1
	MX_UART5_UART_Init();
#endif

#if MY_USART6_ENABLE1
	MX_USART6_UART_Init();
#endif

#if MY_UART7_ENABLE1
	MX_UART7_UART_Init();
#endif

#if MY_UART8_ENABLE1
	MX_UART8_UART_Init();
#endif
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  if(uartHandle->Instance==USART1)
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

	//打开时钟
	MY_USART1_TX_CLK_ENABLE();
	MY_USART1_RX_CLK_ENABLE();

	//引脚复用配置
    GPIO_InitStruct.Pin = MY_USART1_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = MY_USART1_ALTERNATE;
    HAL_GPIO_Init(MY_USART1_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_USART1_RX_PIN;
    GPIO_InitStruct.Alternate = MY_USART1_ALTERNATE;
    HAL_GPIO_Init(MY_USART1_RX_PORT, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
#if(MY_USART1_DMA_RX == 1)
    hdma_usart1_rx.Instance = DMA1_Stream0;
    hdma_usart1_rx.Init.Request = DMA_REQUEST_USART1_RX;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);
#endif

    /* USART1_TX Init */
#if(MY_USART1_DMA_TX == 1)
    hdma_usart1_tx.Instance = DMA2_Stream0;
    hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);
#endif

    /* USART1 interrupt Init */
#if MY_USART1_IT_ENABLE
	#if(MY_USART1_IDLE_ENABLE == 1)
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //打开空闲中断
	#endif
    HAL_NVIC_SetPriority(USART1_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
#endif

  }
  else if(uartHandle->Instance==USART2)
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

	//打开时钟
	MY_USART2_TX_CLK_ENABLE();
	MY_USART2_RX_CLK_ENABLE();

	//引脚复用配置
    GPIO_InitStruct.Pin = MY_USART2_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = MY_USART2_ALTERNATE;
    HAL_GPIO_Init(MY_USART2_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_USART2_RX_PIN;
    GPIO_InitStruct.Alternate = MY_USART2_ALTERNATE;
    HAL_GPIO_Init(MY_USART2_RX_PORT, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_RX Init */
#if(MY_USART2_DMA_RX == 1)
    hdma_usart2_rx.Instance = DMA1_Stream1;
    hdma_usart2_rx.Init.Request = DMA_REQUEST_USART2_RX;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);
#endif

    /* USART2_TX Init */
#if(MY_USART2_DMA_TX == 1)
    hdma_usart2_tx.Instance = DMA2_Stream1;
    hdma_usart2_tx.Init.Request = DMA_REQUEST_USART2_TX;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);
#endif

    /* USART2 interrupt Init */
#if MY_USART2_IT_ENABLE
	#if(MY_USART2_IDLE_ENABLE == 1)
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);  //打开空闲中断
	#endif
    HAL_NVIC_SetPriority(USART2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
#endif

  }
  else if(uartHandle->Instance==USART3)
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

	//打开时钟
	MY_USART3_TX_CLK_ENABLE();
	MY_USART3_RX_CLK_ENABLE();

	//引脚复用配置
    GPIO_InitStruct.Pin = MY_USART3_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = MY_USART3_ALTERNATE;
    HAL_GPIO_Init(MY_USART2_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MY_USART3_RX_PIN;
    GPIO_InitStruct.Alternate = MY_USART3_ALTERNATE;
    HAL_GPIO_Init(MY_USART2_RX_PORT, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
#if(MY_USART3_DMA_RX == 1)
    hdma_usart3_tx.Instance = DMA1_Stream2;
    hdma_usart3_tx.Init.Request = DMA_REQUEST_USART3_RX;
    hdma_usart3_tx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_tx);
#endif

    /* USART3_TX Init */
#if(MY_USART3_DMA_TX == 1)
    hdma_usart3_tx.Instance = DMA2_Stream2;
    hdma_usart3_tx.Init.Request = DMA_REQUEST_USART3_TX;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);
#endif

    /* USART3 interrupt Init */
#if MY_USART3_IT_ENABLE
	#if(MY_USART3_IDLE_ENABLE == 1)
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);  //打开空闲中断
	#endif
    HAL_NVIC_SetPriority(USART3_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
#endif

  }
  else if(uartHandle->Instance==UART4)
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART4;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();

	//打开时钟
	MY_UART4_TX_CLK_ENABLE();
	MY_UART4_RX_CLK_ENABLE();

	//引脚复用配置
	GPIO_InitStruct.Pin = MY_UART4_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = MY_UART4_ALTERNATE;
	HAL_GPIO_Init(MY_UART4_TX_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = MY_UART4_RX_PIN;
	GPIO_InitStruct.Alternate = MY_UART4_ALTERNATE;
	HAL_GPIO_Init(MY_UART4_RX_PORT, &GPIO_InitStruct);

    /* UART4 DMA Init */
    /* UART4_RX Init */
#if(MY_UART4_DMA_RX == 1)
    hdma_uart4_rx.Instance = DMA1_Stream3;
    hdma_uart4_rx.Init.Request = DMA_REQUEST_UART4_RX;
    hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_rx.Init.Mode = DMA_NORMAL;
    hdma_uart4_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_uart4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart4_rx);
#endif

    /* UART4_TX Init */
#if(MY_UART4_DMA_TX == 1)
    hdma_uart4_tx.Instance = DMA2_Stream3;
    hdma_uart4_tx.Init.Request = DMA_REQUEST_UART4_TX;
    hdma_uart4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_tx.Init.Mode = DMA_NORMAL;
    hdma_uart4_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_uart4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart4_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_uart4_tx);
#endif

    /* UART4 interrupt Init */
#if (MY_UART4_IT_ENABLE == 1)
	#if(MY_UART4_IDLE_ENABLE == 1)
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);  //打开空闲中断
	#endif
    HAL_NVIC_SetPriority(UART4_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
#endif
  }
  else if(uartHandle->Instance==UART5)
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART5;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* UART5 clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();

	//打开时钟
	MY_UART5_TX_CLK_ENABLE();
	MY_UART5_RX_CLK_ENABLE();

	//引脚复用配置
	GPIO_InitStruct.Pin = MY_UART5_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = MY_UART5_ALTERNATE;
	HAL_GPIO_Init(MY_UART5_TX_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = MY_UART5_RX_PIN;
	GPIO_InitStruct.Alternate = MY_UART5_ALTERNATE;
	HAL_GPIO_Init(MY_UART5_RX_PORT, &GPIO_InitStruct);

    /* UART5 DMA Init */
    /* UART5_RX Init */
#if(MY_UART5_DMA_RX == 1)
    hdma_uart5_rx.Instance = DMA1_Stream4;
    hdma_uart5_rx.Init.Request = DMA_REQUEST_UART5_RX;
    hdma_uart5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart5_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart5_rx.Init.Mode = DMA_NORMAL;
    hdma_uart5_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_uart5_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart5_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart5_rx);
#endif

    /* UART5_TX Init */
#if(MY_UART5_DMA_TX == 1)
    hdma_uart5_tx.Instance = DMA2_Stream4;
    hdma_uart5_tx.Init.Request = DMA_REQUEST_UART5_TX;
    hdma_uart5_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart5_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart5_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart5_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart5_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart5_tx.Init.Mode = DMA_NORMAL;
    hdma_uart5_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_uart5_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart5_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_uart5_tx);
#endif

    /* UART5 interrupt Init */
#if (MY_UART5_IT_ENABLE == 1)
	#if(MY_UART5_IDLE_ENABLE == 1)
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);  //打开空闲中断
	#endif
    HAL_NVIC_SetPriority(UART5_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
#endif
  }
  else if(uartHandle->Instance==USART6)
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART6;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART6 clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();

	//打开时钟
	MY_USART6_TX_CLK_ENABLE();
	MY_USART6_RX_CLK_ENABLE();

	//引脚复用配置
	GPIO_InitStruct.Pin = MY_USART6_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = MY_USART6_ALTERNATE;
	HAL_GPIO_Init(MY_USART6_TX_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = MY_USART6_RX_PIN;
	GPIO_InitStruct.Alternate = MY_USART6_ALTERNATE;
	HAL_GPIO_Init(MY_USART6_RX_PORT, &GPIO_InitStruct);


    /* USART6 DMA Init */
    /* USART6_RX Init */
#if(MY_USART6_DMA_RX == 1)
    hdma_usart6_rx.Instance = DMA1_Stream5;
    hdma_usart6_rx.Init.Request = DMA_REQUEST_USART6_RX;
    hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_rx.Init.Mode = DMA_NORMAL;
    hdma_usart6_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart6_rx);
#endif

    /* USART6_TX Init */
#if(MY_USART6_DMA_TX == 1)
    hdma_usart6_tx.Instance = DMA2_Stream5;
    hdma_usart6_tx.Init.Request = DMA_REQUEST_USART6_TX;
    hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_tx.Init.Mode = DMA_NORMAL;
    hdma_usart6_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart6_tx);
#endif

    /* USART6 interrupt Init */
#if MY_USART6_IT_ENABLE
	#if(MY_USART6_IDLE_ENABLE == 1)
	__HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);  //打开空闲中断
	#endif
    HAL_NVIC_SetPriority(USART6_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
#endif
  }
  else if(uartHandle->Instance==UART7)
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART7;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* UART7 clock enable */
    __HAL_RCC_UART7_CLK_ENABLE();

	//打开时钟
	MY_UART7_TX_CLK_ENABLE();
	MY_UART7_RX_CLK_ENABLE();

	//引脚复用配置
	GPIO_InitStruct.Pin = MY_UART7_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = MY_UART7_ALTERNATE;
	HAL_GPIO_Init(MY_UART7_TX_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = MY_UART7_RX_PIN;
	GPIO_InitStruct.Alternate = MY_UART7_ALTERNATE;
	HAL_GPIO_Init(MY_UART7_RX_PORT, &GPIO_InitStruct);

    /* UART7 DMA Init */
    /* UART7_RX Init */
#if(MY_UART7_DMA_RX == 1)
    hdma_uart7_rx.Instance = DMA1_Stream6;
    hdma_uart7_rx.Init.Request = DMA_REQUEST_UART7_RX;
    hdma_uart7_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart7_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart7_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart7_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart7_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart7_rx.Init.Mode = DMA_NORMAL;
    hdma_uart7_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_uart7_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart7_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart7_rx);
#endif

    /* UART7_TX Init */
#if(MY_UART7_DMA_TX == 1)
    hdma_uart7_tx.Instance = DMA2_Stream6;
    hdma_uart7_tx.Init.Request = DMA_REQUEST_UART7_TX;
    hdma_uart7_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart7_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart7_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart7_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart7_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart7_tx.Init.Mode = DMA_NORMAL;
    hdma_uart7_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_uart7_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart7_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_uart7_tx);
#endif

    /* UART7 interrupt Init */
#if MY_UART7_IT_ENABLE
	#if(MY_UART7_IDLE_ENABLE == 1)
	__HAL_UART_ENABLE_IT(&huart7, UART_IT_IDLE);  //打开空闲中断
	#endif
    HAL_NVIC_SetPriority(UART7_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(UART7_IRQn);
#endif
  }
  else if(uartHandle->Instance==UART8)
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART8;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* UART8 clock enable */
    __HAL_RCC_UART8_CLK_ENABLE();

	//打开时钟
	MY_UART8_TX_CLK_ENABLE();
	MY_UART8_RX_CLK_ENABLE();

	//引脚复用配置
	GPIO_InitStruct.Pin = MY_UART8_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = MY_UART8_ALTERNATE;
	HAL_GPIO_Init(MY_UART8_TX_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = MY_UART8_RX_PIN;
	GPIO_InitStruct.Alternate = MY_UART8_ALTERNATE;
	HAL_GPIO_Init(MY_UART8_RX_PORT, &GPIO_InitStruct);

    /* UART8 DMA Init */
    /* UART8_RX Init */
#if(MY_UART8_DMA_RX == 1)
    hdma_uart8_rx.Instance = DMA1_Stream7;
    hdma_uart8_rx.Init.Request = DMA_REQUEST_UART8_RX;
    hdma_uart8_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart8_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart8_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart8_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart8_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart8_rx.Init.Mode = DMA_NORMAL;
    hdma_uart8_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_uart8_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart8_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart8_rx);
#endif

    /* UART8_TX Init */
#if(MY_UART8_DMA_TX == 1)
    hdma_uart8_tx.Instance = DMA2_Stream7;
    hdma_uart8_tx.Init.Request = DMA_REQUEST_UART8_TX;
    hdma_uart8_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart8_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart8_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart8_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart8_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart8_tx.Init.Mode = DMA_NORMAL;
    hdma_uart8_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_uart8_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart8_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_uart8_tx);
#endif

    /* UART8 interrupt Init */
#if (MY_UART8_IT_ENABLE == 1)
	#if(MY_UART8_IDLE_ENABLE == 1)
	__HAL_UART_ENABLE_IT(&huart8, UART_IT_IDLE);  //打开空闲中断
	#endif
    HAL_NVIC_SetPriority(UART8_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(UART8_IRQn);
#endif
  }

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /* USART1 GPIO Configuration */
	HAL_GPIO_DeInit(MY_USART1_TX_PORT, MY_USART1_TX_PIN);
	HAL_GPIO_DeInit(MY_USART1_RX_PORT, MY_USART1_RX_PIN);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
#if MY_USART1_IT_ENABLE
    HAL_NVIC_DisableIRQ(USART1_IRQn);
#endif
  }
  else if(uartHandle->Instance==USART2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /* USART2 GPIO Configuration */
	HAL_GPIO_DeInit(MY_USART2_TX_PORT, MY_USART2_TX_PIN);
	HAL_GPIO_DeInit(MY_USART2_RX_PORT, MY_USART2_RX_PIN);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART2 interrupt Deinit */
#if MY_USART2_IT_ENABLE
    HAL_NVIC_DisableIRQ(USART2_IRQn);
#endif
  }
  else if(uartHandle->Instance==USART3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /* USART3 GPIO Configuration */
	HAL_GPIO_DeInit(MY_USART3_TX_PORT, MY_USART3_TX_PIN);
	HAL_GPIO_DeInit(MY_USART3_RX_PORT, MY_USART3_RX_PIN);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART3 interrupt Deinit */
#if MY_USART3_IT_ENABLE
    HAL_NVIC_DisableIRQ(USART3_IRQn);
#endif
  }
  else if(uartHandle->Instance==UART4)
  {
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();

    /* UART4 GPIO Configuration */
	HAL_GPIO_DeInit(MY_UART4_TX_PORT, MY_UART4_TX_PIN);
	HAL_GPIO_DeInit(MY_UART4_RX_PORT, MY_UART4_RX_PIN);

    /* UART4 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* UART4 interrupt Deinit */
#if MY_UART4_IT_ENABLE
    HAL_NVIC_DisableIRQ(UART4_IRQn);
#endif
  }
  else if(uartHandle->Instance==UART5)
  {
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();

    /* UART5 GPIO Configuration */
	HAL_GPIO_DeInit(MY_UART5_TX_PORT, MY_UART5_TX_PIN);
	HAL_GPIO_DeInit(MY_UART5_RX_PORT, MY_UART5_RX_PIN);

    /* UART5 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* UART5 interrupt Deinit */
#if MY_UART5_IT_ENABLE
    HAL_NVIC_DisableIRQ(UART5_IRQn);
#endif
  }
  else if(uartHandle->Instance==USART6)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();

    /* USART6 GPIO Configuration */
	HAL_GPIO_DeInit(MY_USART6_TX_PORT, MY_USART6_TX_PIN);
	HAL_GPIO_DeInit(MY_USART6_RX_PORT, MY_USART6_RX_PIN);

    /* USART6 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART6 interrupt Deinit */
#if MY_USART6_IT_ENABLE
    HAL_NVIC_DisableIRQ(USART6_IRQn);
#endif
  }
  else if(uartHandle->Instance==UART7)
  {
    /* Peripheral clock disable */
    __HAL_RCC_UART7_CLK_DISABLE();

    /* UART7 GPIO Configuration */
	HAL_GPIO_DeInit(MY_UART7_TX_PORT, MY_UART7_TX_PIN);
	HAL_GPIO_DeInit(MY_UART7_RX_PORT, MY_UART7_RX_PIN);

    /* UART7 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* UART7 interrupt Deinit */
#if MY_UART7_IT_ENABLE
    HAL_NVIC_DisableIRQ(UART7_IRQn);
#endif
  }
  else if(uartHandle->Instance==UART8)
  {
    /* Peripheral clock disable */
    __HAL_RCC_UART8_CLK_DISABLE();

    /* UART8 GPIO Configuration */
	HAL_GPIO_DeInit(MY_UART8_TX_PORT, MY_UART8_TX_PIN);
	HAL_GPIO_DeInit(MY_UART8_RX_PORT, MY_UART8_RX_PIN);

    /* UART8 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* UART8 interrupt Deinit */
#if MY_UART8_IT_ENABLE
    HAL_NVIC_DisableIRQ(UART8_IRQn);
#endif
  }

}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
    /* 串口1中断处理函数 */
	HAL_UART_IRQHandler(&huart1);

#if(MY_USART1_IDLE_ENABLE == 1)
	/* 如果是空闲中断，则清除标志，更改缓冲区，等待下一次处理 */
	if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
	{
	    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		uint32_t len_dmarev = 0; //本次空闲中断接收的字节数
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		len_dmarev = UART1_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		//DMA访问的ram，CPU访问的是cache，使用下面函数使ram和cache一致
		SCB_InvalidateDCache_by_Addr((uint32_t *)uart1RxBuf, UART1_RX_SIZE);

		//如果本次接收到数据
		if(len_dmarev != 0)
		{
			memcpy(&tLowerPC_Rx.RxDataBuf, (uint8_t *)uart1RxBuf, len_dmarev); //拷贝数据
			tLowerPC_Rx.RxDataLen = len_dmarev;

			xSemaphoreGiveFromISR(LowerPcRxSem, &xHigherPriorityTaskWoken); //释放信号量
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
		HAL_UART_AbortReceive(&huart1);                           //DMA接收复位
		memset(uart1RxBuf, 0, UART1_RX_SIZE);                     //清除垃圾缓冲
		HAL_UART_Receive_DMA(&huart1, uart1RxBuf, UART1_RX_SIZE); //开启下一次接收
	}
#endif
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
    /* 串口2中断处理函数 */
	HAL_UART_IRQHandler(&huart2);

#if(MY_USART2_IDLE_ENABLE == 1)
	/* 如果是空闲中断，则清除标志，更改缓冲区，等待下一次处理 */
	if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE))
	{
		uint32_t len_dmarev = 0; //本次空闲中断接收的字节数
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		len_dmarev = UART2_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
		//DMA访问的ram，CPU访问的是cache，使用下面函数使ram和cache一致
		SCB_InvalidateDCache_by_Addr((uint32_t *)uart2RxBuf, UART2_RX_SIZE);

		//如果本次接收到数据
		if(len_dmarev != 0)
		{

		}
		HAL_UART_AbortReceive(&huart2);                           //DMA接收复位
		memset(uart2RxBuf, 0, UART2_RX_SIZE);                     //清除垃圾缓冲
		HAL_UART_Receive_DMA(&huart2, uart2RxBuf, UART2_RX_SIZE); //开启下一次接收
	}
#endif
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
    /* 串口3中断处理函数 */
	HAL_UART_IRQHandler(&huart3);

#if(MY_USART3_IDLE_ENABLE == 1)
	/* 如果是空闲中断，则清除标志，更改缓冲区，等待下一次处理 */
	if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE))
	{
		uint32_t len_dmarev = 0; //本次空闲中断接收的字节数
		__HAL_UART_CLEAR_IDLEFLAG(&huart3);
		len_dmarev = UART3_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
		//DMA访问的ram，CPU访问的是cache，使用下面函数使ram和cache一致
		SCB_InvalidateDCache_by_Addr((uint32_t *)uart3RxBuf, UART3_RX_SIZE);

		//如果本次接收到数据
		if(len_dmarev != 0)
		{

		}
		HAL_UART_AbortReceive(&huart3);                           //DMA接收复位
		memset(uart3RxBuf, 0, UART3_RX_SIZE);                     //清除垃圾缓冲
		HAL_UART_Receive_DMA(&huart3, uart3RxBuf, UART3_RX_SIZE); //开启下一次接收
	}
#endif
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
    /* 串口4中断处理函数 */
	HAL_UART_IRQHandler(&huart4);

#if(MY_UART4_IDLE_ENABLE == 1)
	/* 如果是空闲中断，则清除标志，更改缓冲区，等待下一次处理 */
	if(__HAL_UART_GET_FLAG(&huart4, UART_FLAG_IDLE))
	{
		uint32_t len_dmarev = 0; //本次空闲中断接收的字节数
		__HAL_UART_CLEAR_IDLEFLAG(&huart4);
		len_dmarev = UART4_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
		//DMA访问的ram，CPU访问的是cache，使用下面函数使ram和cache一致
		SCB_InvalidateDCache_by_Addr((uint32_t *)uart4RxBuf, UART4_RX_SIZE);

		//如果本次接收到数据
		if(len_dmarev != 0)
		{

		}
		HAL_UART_AbortReceive(&huart4);                           //DMA接收复位
		memset(uart4RxBuf, 0, UART4_RX_SIZE);                     //清除垃圾缓冲
		HAL_UART_Receive_DMA(&huart4, uart4RxBuf, UART4_RX_SIZE); //开启下一次接收
	}
#endif
}

/**
  * @brief This function handles UART5 global interrupt.
  */
void UART5_IRQHandler(void)
{
    /* 串口5中断处理函数 */
	HAL_UART_IRQHandler(&huart5);

#if(MY_UART5_IDLE_ENABLE == 1)
	/* 如果是空闲中断，则清除标志，更改缓冲区，等待下一次处理 */
	if(__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE))
	{
		uint32_t len_dmarev = 0; //本次空闲中断接收的字节数
		__HAL_UART_CLEAR_IDLEFLAG(&huart5);
		len_dmarev = UART5_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart5_rx);
		//DMA访问的ram，CPU访问的是cache，使用下面函数使ram和cache一致
		SCB_InvalidateDCache_by_Addr((uint32_t *)uart5RxBuf, UART5_RX_SIZE);

		//如果本次接收到数据
		if(len_dmarev != 0)
		{

		}
		HAL_UART_AbortReceive(&huart5);                           //DMA接收复位
		memset(uart5RxBuf, 0, UART5_RX_SIZE);                     //清除垃圾缓冲
		HAL_UART_Receive_DMA(&huart5, uart5RxBuf, UART5_RX_SIZE); //开启下一次接收
	}
#endif
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
    /* 串口6中断处理函数 */
	HAL_UART_IRQHandler(&huart6);

#if(MY_USART6_IDLE_ENABLE == 1)
	/* 如果是空闲中断，则清除标志，更改缓冲区，等待下一次处理 */
	if(__HAL_UART_GET_FLAG(&huart6, UART_FLAG_IDLE))
	{
		uint32_t len_dmarev = 0; //本次空闲中断接收的字节数
		__HAL_UART_CLEAR_IDLEFLAG(&huart6);
		len_dmarev = UART6_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart6_rx);
		//DMA访问的ram，CPU访问的是cache，使用下面函数使ram和cache一致
		SCB_InvalidateDCache_by_Addr((uint32_t *)uart6RxBuf, UART6_RX_SIZE);

		//如果本次接收到数据
		if(len_dmarev != 0)
		{

		}
		HAL_UART_AbortReceive(&huart6);                           //DMA接收复位
		memset(uart6RxBuf, 0, UART6_RX_SIZE);                     //清除垃圾缓冲
		HAL_UART_Receive_DMA(&huart6, uart6RxBuf, UART6_RX_SIZE); //开启下一次接收
	}
#endif
}

/**
  * @brief This function handles UART7 global interrupt.
  */
void UART7_IRQHandler(void)
{
    /* 串口7中断处理函数 */
	HAL_UART_IRQHandler(&huart7);

#if(MY_UART7_IDLE_ENABLE == 1)
	/* 如果是空闲中断，则清除标志，更改缓冲区，等待下一次处理 */
	if(__HAL_UART_GET_FLAG(&huart7, UART_FLAG_IDLE))
	{
		uint32_t len_dmarev = 0; //本次空闲中断接收的字节数
		__HAL_UART_CLEAR_IDLEFLAG(&huart7);
		len_dmarev = UART7_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart7_rx);
		//DMA访问的ram，CPU访问的是cache，使用下面函数使ram和cache一致
		SCB_InvalidateDCache_by_Addr((uint32_t *)uart7RxBuf, UART7_RX_SIZE);

		//如果本次接收到数据
		if(len_dmarev != 0)
		{

		}
		HAL_UART_AbortReceive(&huart7);                           //DMA接收复位
		memset(uart7RxBuf, 0, UART7_RX_SIZE);                     //清除垃圾缓冲
		HAL_UART_Receive_DMA(&huart7, uart7RxBuf, UART7_RX_SIZE); //开启下一次接收
	}
#endif
}

/**
  * @brief This function handles UART8 global interrupt.
  */
void UART8_IRQHandler(void)
{
    /* 串口8中断处理函数 */
	HAL_UART_IRQHandler(&huart8);

#if(MY_UART8_IDLE_ENABLE == 1)
	/* 如果是空闲中断，则清除标志，更改缓冲区，等待下一次处理 */
	if(__HAL_UART_GET_FLAG(&huart8, UART_FLAG_IDLE))
	{
		uint32_t len_dmarev = 0; //本次空闲中断接收的字节数
		__HAL_UART_CLEAR_IDLEFLAG(&huart8);
		len_dmarev = UART8_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart8_rx);
		//DMA访问的ram，CPU访问的是cache，使用下面函数使ram和cache一致
		SCB_InvalidateDCache_by_Addr((uint32_t *)uart8RxBuf, UART8_RX_SIZE);

		//如果本次接收到数据
		if(len_dmarev != 0)
		{

		}
		HAL_UART_AbortReceive(&huart8);                           //DMA接收复位
		memset(uart8RxBuf, 0, UART8_RX_SIZE);                     //清除垃圾缓冲
		HAL_UART_Receive_DMA(&huart8, uart8RxBuf, UART8_RX_SIZE); //开启下一次接收
	}
#endif
}

/*-----------------------------------------------------------------------------
 * @name  : HAL_UART_TxCpltCallback
 * @brief : 发送完成回调函数
 * @param : huart：串口句柄指针
 * @retval:
 * @date  : 2020/12/26
 * @note  :
 * ---------------------------------------------------------------------------*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if ( huart->Instance == huart1.Instance )
	{
	#if(MY_USART1_DMA_TX == 1)
    	xSemaphoreGiveFromISR(uart1DMATxCpltSem, &xHigherPriorityTaskWoken);	//DMA发送完成
    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	#endif
	}
	else if ( huart->Instance == huart2.Instance )
	{
	#if(MY_USART2_DMA_TX == 1)
    	xSemaphoreGiveFromISR(uart2DMATxCpltSem, &xHigherPriorityTaskWoken);	//DMA发送完成
    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	#endif
	}
	else if ( huart->Instance == huart3.Instance )
	{
	#if(MY_USART3_DMA_TX == 1)
    	xSemaphoreGiveFromISR(uart3DMATxCpltSem, &xHigherPriorityTaskWoken);	//DMA发送完成
    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	#endif
	}
	else if ( huart->Instance == huart4.Instance )
	{
	#if(MY_UART4_DMA_TX == 1)
    	xSemaphoreGiveFromISR(uart4DMATxCpltSem, &xHigherPriorityTaskWoken);	//DMA发送完成
    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	#endif
	}
	else if ( huart->Instance == huart5.Instance )
	{
	#if(MY_UART5_DMA_TX == 1)
    	xSemaphoreGiveFromISR(uart5DMATxCpltSem, &xHigherPriorityTaskWoken);	//DMA发送完成
    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	#endif
	}
	else if ( huart->Instance == huart6.Instance )
	{
	#if(MY_USART6_DMA_TX == 1)
    	xSemaphoreGiveFromISR(uart6DMATxCpltSem, &xHigherPriorityTaskWoken);	//DMA发送完成
    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	#endif
	}
	else if ( huart->Instance == huart7.Instance )
	{
	#if(MY_UART7_DMA_TX == 1)
    	xSemaphoreGiveFromISR(uart7DMATxCpltSem, &xHigherPriorityTaskWoken);	//DMA发送完成
    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	#endif
	}
	else if ( huart->Instance == huart8.Instance )
	{
	#if(MY_UART8_DMA_TX == 1)
    	xSemaphoreGiveFromISR(uart8DMATxCpltSem, &xHigherPriorityTaskWoken);	//DMA发送完成
    	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	#endif
	}
}

/*-----------------------------------------------------------------------------
 * @name  : HAL_UART_RxCpltCallback
 * @brief : 串口接收中断回调函数
 * @param : huart：串口句柄指针
 * @retval:
 * @date  : 2020/12/26
 * @note  :
 * ---------------------------------------------------------------------------*/
void HAL_UART_RxCpltCallback( UART_HandleTypeDef *huart )
{
	if (huart == &huart1)
	{
		HAL_UART_Receive_DMA(&huart1, uart1RxBuf, UART1_RX_SIZE); //开启下一次接收
	}

	if (huart == &huart2)
	{
		HAL_UART_Receive_DMA(&huart2, uart2RxBuf, UART2_RX_SIZE); //开启下一次接收
	}

	if (huart == &huart3)
	{
		HAL_UART_Receive_DMA(&huart3, uart3RxBuf, UART3_RX_SIZE); //开启下一次接收
	}

	if (huart == &huart4)
	{
		HAL_UART_Receive_DMA(&huart4, uart4RxBuf, UART4_RX_SIZE); //开启下一次接收
	}

	if (huart == &huart5)
	{
		HAL_UART_Receive_DMA(&huart5, uart5RxBuf, UART5_RX_SIZE); //开启下一次接收
	}

	if (huart == &huart6)
	{
		HAL_UART_Receive_DMA(&huart6, uart6RxBuf, UART6_RX_SIZE); //开启下一次接收
	}

	if (huart == &huart7)
	{
		HAL_UART_Receive_DMA(&huart7, uart7RxBuf, UART7_RX_SIZE); //开启下一次接收
	}

	if (huart == &huart8)
	{
		HAL_UART_Receive_DMA(&huart8, uart8RxBuf, UART8_RX_SIZE); //开启下一次接收
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
