/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
/*----------------------串口宏选项-----------------------------
    默认串口为：8位数据、无奇偶校验、1个停止位、默认中断抢占优先级为7
 *-----------------------------------------------------------*/
//串口1
#define MY_USART1_ENABLE1           1
#define MY_USART1_BAUDRATE			921600
#define MY_USART1_TX_PORT			GPIOA
#define MY_USART1_RX_PORT			GPIOA
#define MY_USART1_TX_PIN            GPIO_PIN_10
#define MY_USART1_RX_PIN            GPIO_PIN_9
#define MY_USART1_ALTERNATE			GPIO_AF7_USART1
#define MY_USART1_TX_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define MY_USART1_RX_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define MY_USART1_IT_ENABLE			1
#define MY_USART1_IDLE_ENABLE		1
#define MY_USART1_DMA_TX			1
#define MY_USART1_DMA_RX			1
//串口2
#define MY_USART2_ENABLE1           0
#define MY_USART2_BAUDRATE			921600
#define MY_USART2_TX_PORT			GPIOD
#define MY_USART2_RX_PORT			GPIOD
#define MY_USART2_TX_PIN            GPIO_PIN_5
#define MY_USART2_RX_PIN            GPIO_PIN_6
#define MY_USART2_ALTERNATE			GPIO_AF7_USART2
#define MY_USART2_TX_CLK_ENABLE()   __GPIOD_CLK_ENABLE()
#define MY_USART2_RX_CLK_ENABLE()   __GPIOD_CLK_ENABLE()
#define MY_USART2_IT_ENABLE			1
#define MY_USART2_IDLE_ENABLE		0
#define MY_USART2_DMA_TX			1
#define MY_USART2_DMA_RX			0
//串口3
#define MY_USART3_ENABLE1           0
#define MY_USART3_BAUDRATE			115200
#define MY_USART3_TX_PORT			GPIOD
#define MY_USART3_RX_PORT			GPIOD
#define MY_USART3_TX_PIN            GPIO_PIN_8
#define MY_USART3_RX_PIN            GPIO_PIN_9
#define MY_USART3_ALTERNATE			GPIO_AF7_USART3
#define MY_USART3_TX_CLK_ENABLE()   __GPIOD_CLK_ENABLE()
#define MY_USART3_RX_CLK_ENABLE()   __GPIOD_CLK_ENABLE()
#define MY_USART3_IT_ENABLE			1
#define MY_USART3_IDLE_ENABLE		0
#define MY_USART3_DMA_TX			1
#define MY_USART3_DMA_RX			0
//串口4
#define MY_UART4_ENABLE1            0
#define MY_UART4_BAUDRATE			115200
#define MY_UART4_TX_PORT			GPIOC
#define MY_UART4_RX_PORT			GPIOC
#define MY_UART4_TX_PIN            	GPIO_PIN_10
#define MY_UART4_RX_PIN            	GPIO_PIN_11
#define MY_UART4_ALTERNATE			GPIO_AF8_UART4
#define MY_UART4_TX_CLK_ENABLE()  	__GPIOC_CLK_ENABLE()
#define MY_UART4_RX_CLK_ENABLE()   	__GPIOC_CLK_ENABLE()
#define MY_UART4_IT_ENABLE			1
#define MY_UART4_IDLE_ENABLE		0
#define MY_UART4_DMA_TX		     	1
#define MY_UART4_DMA_RX			    0
//串口5
#define MY_UART5_ENABLE1            0
#define MY_UART5_BAUDRATE			115200
#define MY_UART5_TX_PORT			GPIOB
#define MY_UART5_RX_PORT			GPIOB
#define MY_UART5_TX_PIN            	GPIO_PIN_6
#define MY_UART5_RX_PIN            	GPIO_PIN_5
#define MY_UART5_ALTERNATE			GPIO_AF14_UART5
#define MY_UART5_TX_CLK_ENABLE()   	__GPIOB_CLK_ENABLE()
#define MY_UART5_RX_CLK_ENABLE()   	__GPIOB_CLK_ENABLE()
#define MY_UART5_IT_ENABLE		    1
#define MY_UART5_IDLE_ENABLE		0
#define MY_UART5_DMA_TX			    1
#define MY_UART5_DMA_RX			    0
//串口6
#define MY_USART6_ENABLE1           0
#define MY_USART6_BAUDRATE			115200
#define MY_USART6_TX_PORT			GPIOC
#define MY_USART6_RX_PORT			GPIOC
#define MY_USART6_TX_PIN            GPIO_PIN_6
#define MY_USART6_RX_PIN            GPIO_PIN_7
#define MY_USART6_ALTERNATE			GPIO_AF7_USART6
#define MY_USART6_TX_CLK_ENABLE()   __GPIOC_CLK_ENABLE()
#define MY_USART6_RX_CLK_ENABLE()   __GPIOC_CLK_ENABLE()
#define MY_USART6_IT_ENABLE			1
#define MY_USART6_IDLE_ENABLE		0
#define MY_USART6_DMA_TX		    1
#define MY_USART6_DMA_RX			0
//串口7
#define MY_UART7_ENABLE1            0
#define MY_UART7_BAUDRATE			115200
#define MY_UART7_TX_PORT			GPIOF
#define MY_UART7_RX_PORT			GPIOF
#define MY_UART7_TX_PIN            	GPIO_PIN_7
#define MY_UART7_RX_PIN            	GPIO_PIN_6
#define MY_UART7_ALTERNATE			GPIO_AF7_UART7
#define MY_UART7_TX_CLK_ENABLE()   	__GPIOF_CLK_ENABLE()
#define MY_UART7_RX_CLK_ENABLE()   	__GPIOF_CLK_ENABLE()
#define MY_UART7_IT_ENABLE			1
#define MY_UART7_IDLE_ENABLE		0
#define MY_UART7_DMA_TX		        1
#define MY_UART7_DMA_RX		    	0
//串口8
#define MY_UART8_ENABLE1            1
#define MY_UART8_BAUDRATE			9600
#define MY_UART8_TX_PORT			GPIOE
#define MY_UART8_RX_PORT			GPIOE
#define MY_UART8_TX_PIN            	GPIO_PIN_1
#define MY_UART8_RX_PIN            	GPIO_PIN_0
#define MY_UART8_ALTERNATE			GPIO_AF8_UART8
#define MY_UART8_TX_CLK_ENABLE()   	__GPIOE_CLK_ENABLE()
#define MY_UART8_RX_CLK_ENABLE()   	__GPIOE_CLK_ENABLE()
#define MY_UART8_IT_ENABLE			1
#define MY_UART8_IDLE_ENABLE		1
#define MY_UART8_DMA_TX			    1
#define MY_UART8_DMA_RX			    1

/* USER CODE END Includes */
//全局变量声明
#define UART1_RX_SIZE  2048
#define UART2_RX_SIZE  2048
#define UART3_RX_SIZE  200
#define UART4_RX_SIZE  200
#define UART5_RX_SIZE  200
#define UART6_RX_SIZE  200
#define UART7_RX_SIZE  200
#define UART8_RX_SIZE  200

extern uint8_t uart1RxBuf[UART1_RX_SIZE];
extern uint8_t uart2RxBuf[UART2_RX_SIZE];
extern uint8_t uart3RxBuf[UART3_RX_SIZE];
extern uint8_t uart4RxBuf[UART4_RX_SIZE];
extern uint8_t uart5RxBuf[UART5_RX_SIZE];
extern uint8_t uart6RxBuf[UART6_RX_SIZE];
extern uint8_t uart7RxBuf[UART7_RX_SIZE];
extern uint8_t uart8RxBuf[UART8_RX_SIZE];

/*---------------------------------
		   全局串口句柄声明
 *--------------------------------*/
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_uart5_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern DMA_HandleTypeDef hdma_uart7_rx;
extern DMA_HandleTypeDef hdma_uart7_tx;
extern DMA_HandleTypeDef hdma_uart8_rx;
extern DMA_HandleTypeDef hdma_uart8_tx;
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_UART4_UART_Init(void);
void MX_UART5_UART_Init(void);
void MX_USART6_UART_Init(void);
void MX_UART7_UART_Init(void);
void MX_UART8_UART_Init(void);

void MX_USART_UART_Init(void);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
