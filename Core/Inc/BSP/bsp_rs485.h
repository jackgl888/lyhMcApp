/*-----------------------------------------------------------------------------
 * @file        : bsp_rs485.h
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2021/08/27
 * @date changed: 2021/08/27
 * @brief       : 485通信依赖文件
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/

#ifndef INC_BSP_BSP_RS485_H_
#define INC_BSP_BSP_RS485_H_

#include "stm32h7xx.h"
#include "usart.h"

/* 接口声明 */
void RS485_TX_SET( GPIO_TypeDef *RS485_RE_Port, uint16_t RS485_RE_Pin, FunctionalState status);

/*----------- 485_ENABLE_OPTION ----------*/

#define RS485_ENABLE1       1
#define RS485_ENABLE2       0
#define RS485_ENABLE3       0
#define RS485_ENABLE4       0
#define RS485_ENABLE5       0
#define RS485_ENABLE6       0
#define RS485_ENABLE7       0
#define RS485_ENABLE8       0

/*----------- 485_ENABLE_PORT ------------*/
#define UART1_RE_GPIO_Port		GPIOB
#define UART2_RE_GPIO_Port		GPIOC
#define UART3_RE_GPIO_Port		GPIOD
#define UART4_RE_GPIO_Port		GPIOC
#define UART5_RE_GPIO_Port		GPIOB
#define UART6_RE_GPIO_Port		GPIOC
#define UART7_RE_GPIO_Port		GPIOF
#define UART8_RE_GPIO_Port		GPIOB

/*----------- 485_ENABLE_PIN ---------*/
#define UART1_RE_Pin		    GPIO_PIN_8
#define UART2_RE_Pin		    GPIO_PIN_9
#define UART3_RE_Pin		    GPIO_PIN_10
#define UART4_RE_Pin		    GPIO_PIN_12
#define UART5_RE_Pin		    GPIO_PIN_7
#define UART6_RE_Pin		    GPIO_PIN_8
#define UART7_RE_Pin		    GPIO_PIN_8
#define UART8_RE_Pin		    GPIO_PIN_9

/*----------- 485_CLK_ENABLE ----------------------------------*/
#define UART1_RE_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define UART2_RE_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()
#define UART3_RE_CLK_ENABLE()	__HAL_RCC_GPIOD_CLK_ENABLE()
#define UART4_RE_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()
#define UART5_RE_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define UART6_RE_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()
#define UART7_RE_CLK_ENABLE()	__HAL_RCC_GPIOF_CLK_ENABLE()
#define UART8_RE_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()

/*------------ 485_TX ------------------------------------------------------*/
#define UART1_485_TX()	HAL_GPIO_WritePin(UART1_RE_GPIO_Port, UART1_RE_Pin, GPIO_PIN_SET)
#define UART2_485_TX()	HAL_GPIO_WritePin(UART2_RE_GPIO_Port, UART2_RE_Pin, GPIO_PIN_SET)
#define UART3_485_TX()	HAL_GPIO_WritePin(UART3_RE_GPIO_Port, UART3_RE_Pin, GPIO_PIN_SET)
#define UART4_485_TX()	HAL_GPIO_WritePin(UART4_RE_GPIO_Port, UART4_RE_Pin, GPIO_PIN_SET)
#define UART5_485_TX()	HAL_GPIO_WritePin(UART5_RE_GPIO_Port, UART5_RE_Pin, GPIO_PIN_SET)
#define UART6_485_TX()	HAL_GPIO_WritePin(UART6_RE_GPIO_Port, UART6_RE_Pin, GPIO_PIN_SET)
#define UART7_485_TX()	HAL_GPIO_WritePin(UART7_RE_GPIO_Port, UART7_RE_Pin, GPIO_PIN_SET)
#define UART8_485_TX()	HAL_GPIO_WritePin(UART8_RE_GPIO_Port, UART8_RE_Pin, GPIO_PIN_SET)

/*------------ 485_RX ------------------------------------------------------*/
#define UART1_485_RX()	HAL_GPIO_WritePin(UART1_RE_GPIO_Port, UART1_RE_Pin, GPIO_PIN_RESET)
#define UART2_485_RX()	HAL_GPIO_WritePin(UART2_RE_GPIO_Port, UART2_RE_Pin, GPIO_PIN_RESET)
#define UART3_485_RX()	HAL_GPIO_WritePin(UART3_RE_GPIO_Port, UART3_RE_Pin, GPIO_PIN_RESET)
#define UART4_485_RX()	HAL_GPIO_WritePin(UART4_RE_GPIO_Port, UART4_RE_Pin, GPIO_PIN_RESET)
#define UART5_485_RX()	HAL_GPIO_WritePin(UART5_RE_GPIO_Port, UART5_RE_Pin, GPIO_PIN_RESET)
#define UART6_485_RX()	HAL_GPIO_WritePin(UART6_RE_GPIO_Port, UART6_RE_Pin, GPIO_PIN_RESET)
#define UART7_485_RX()	HAL_GPIO_WritePin(UART7_RE_GPIO_Port, UART7_RE_Pin, GPIO_PIN_RESET)
#define UART8_485_RX()	HAL_GPIO_WritePin(UART8_RE_GPIO_Port, UART8_RE_Pin, GPIO_PIN_RESET)

/*----------------------------------------------------------------------------
 				以下是用户自定义区
  ---------------------------------------------------------------------------*/

void RS485_Init(void);

uint8_t UART1_Send(uint8_t *txbuf, uint16_t dataLen);
uint8_t UART2_Send(uint8_t *txbuf, uint16_t dataLen);
uint8_t UART3_Send(uint8_t *txbuf, uint16_t dataLen);
uint8_t UART4_Send(uint8_t *txbuf, uint16_t dataLen);
uint8_t UART5_Send(uint8_t *txbuf, uint16_t dataLen);
uint8_t UART6_Send(uint8_t *txbuf, uint16_t dataLen);
uint8_t UART7_Send(uint8_t *txbuf, uint16_t dataLen);
uint8_t UART8_Send(uint8_t *txbuf, uint16_t dataLen);

uint32_t Uart8_String(char *fmt, ...);

#endif /* INC_BSP_BSP_RS485_H_ */
