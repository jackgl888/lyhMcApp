/*-----------------------------------------------------------------------------
 * @file        : bsp_rs485.c
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2021/08/27
 * @date changed: 2021/08/27
 * @brief       : 485通信配置文件
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/

#ifdef USER_CONFIG
	#include "user_config.h"
#endif

// 不精确的延时
void _485_delay(__IO uint32_t nCount)
{
	for (; nCount != 0; nCount--);
}

/*-----------------------------------------------------------------------------
 * @name  : RS485_TX_SET
 * @brief : 设置RS485接收发送模式
 * @param : huart：串口号
 * 			status：设置485发送或接收
 * @retval:
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
void RS485_TX_SET( GPIO_TypeDef *RS485_RE_Port, uint16_t RS485_RE_Pin, FunctionalState status)
{
	status ? HAL_GPIO_WritePin(RS485_RE_Port, RS485_RE_Pin, GPIO_PIN_SET):
			 HAL_GPIO_WritePin(RS485_RE_Port, RS485_RE_Pin, GPIO_PIN_RESET);
}

/*-----------------------------------------------------------------------------
 * @name  : RS485_Init
 * @brief : 设置RS485接收模式
 * @param :
 * @retval:
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
void RS485_Init(void)
{
#if( RS485_ENABLE1 == 1 ||  RS485_ENABLE2 == 1  || RS485_ENABLE2 == 1 || RS485_ENABLE3 == 1 || \
	 RS485_ENABLE5 == 1 ||  RS485_ENABLE5 == 1  || RS485_ENABLE6 == 1 || RS485_ENABLE8 == 1 )
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef  GPIO_InitStruct = {0};

	/* 485使能引脚配置 */
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
#endif

#if( RS485_ENABLE1 == 1 )
	/* 串口1 */
	UART1_RE_CLK_ENABLE();
	GPIO_InitStruct.Pin = UART1_RE_Pin;
	HAL_GPIO_Init(UART1_RE_GPIO_Port, &GPIO_InitStruct);
	UART1_485_RX();
#endif

#if( RS485_ENABLE2 == 1 )
	/* 串口2 */
	UART2_RE_CLK_ENABLE();
	GPIO_InitStruct.Pin = UART2_RE_Pin;
	HAL_GPIO_Init(UART2_RE_GPIO_Port, &GPIO_InitStruct);
	UART2_485_RX();
#endif

#if( RS485_ENABLE3 == 1 )
	/* 串口3 */
	UART3_RE_CLK_ENABLE();
	GPIO_InitStruct.Pin = UART3_RE_Pin;
	HAL_GPIO_Init(UART3_RE_GPIO_Port, &GPIO_InitStruct);
	UART3_485_RX();
#endif

#if( RS485_ENABLE4 == 1 )
	/* 串口4 */
	UART4_RE_CLK_ENABLE();
	GPIO_InitStruct.Pin = UART4_RE_Pin;
	HAL_GPIO_Init(UART4_RE_GPIO_Port, &GPIO_InitStruct);
	UART4_485_RX();
#endif

#if( RS485_ENABLE5 == 1 )
	/* 串口5 */
	UART5_RE_CLK_ENABLE();
	GPIO_InitStruct.Pin = UART5_RE_Pin;
	HAL_GPIO_Init(UART5_RE_GPIO_Port, &GPIO_InitStruct);
	UART5_485_RX();
#endif

#if( RS485_ENABLE6 == 1 )
	/* 串口6 */
	UART6_RE_CLK_ENABLE();
	GPIO_InitStruct.Pin = UART6_RE_Pin;
	HAL_GPIO_Init(UART6_RE_GPIO_Port, &GPIO_InitStruct);
	UART6_485_RX();
#endif

#if( RS485_ENABLE7 == 1 )
	/* 串口7 */
	UART7_RE_CLK_ENABLE();
	GPIO_InitStruct.Pin = UART7_RE_Pin;
	HAL_GPIO_Init(UART7_RE_GPIO_Port, &GPIO_InitStruct);
	UART7_485_RX();
#endif

#if( RS485_ENABLE8 == 1 )
	/* 串口8 */
	UART8_RE_CLK_ENABLE();
	GPIO_InitStruct.Pin = UART8_RE_Pin;
	HAL_GPIO_Init(UART8_RE_GPIO_Port, &GPIO_InitStruct);
	UART8_485_RX();
#endif
}


/*-----------------------------------------------------------------------------
 * @name  : UART1_Send
 * @brief : 串口1 485发送
 * @param : txbuf 发送buf指针
 *          dataLen 数据长度
 * @retval: 0 OK  1 Error
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t UART1_Send(uint8_t *txbuf, uint16_t dataLen)
{
	uint8_t err = 0;
	/* 等待其他线程处理完成 */
	xSemaphoreTake(uart1DMATxMutex, portMAX_DELAY);
#if( RS485_ENABLE1 == 1 )
	UART1_485_TX();   //进入485发送模式
	_485_delay(1000); //延时
#endif
	err = HAL_UART_Transmit_DMA(&huart1, txbuf, dataLen);
	/* 等待DMA发送完成 */
	xSemaphoreTake(uart1DMATxCpltSem, portMAX_DELAY);
#if( RS485_ENABLE1 == 1 )
	_485_delay(100); //延时
	UART1_485_RX();   //进入485接收模式
#endif
	xSemaphoreGive(uart1DMATxMutex);
	return err;
}

/*-----------------------------------------------------------------------------
 * @name  : UART2_Send
 * @brief : 串口2 485发送
 * @param : txbuf 发送buf指针
 *          dataLen 数据长度
 * @retval: 0 OK  1 Error
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t UART2_Send(uint8_t *txbuf, uint16_t dataLen)
{
	uint8_t err = 0;
	/* 等待其他线程处理完成 */
	xSemaphoreTake(uart2DMATxMutex, portMAX_DELAY);
#if( RS485_ENABLE2 == 1 )
	UART2_485_TX();   //进入485发送模式
	_485_delay(1000); //延时
#endif
	err = HAL_UART_Transmit_DMA(&huart2, txbuf, dataLen);
	/* 等待DMA发送完成 */
	xSemaphoreTake(uart2DMATxCpltSem, portMAX_DELAY);
#if( RS485_ENABLE2 == 1 )
	_485_delay(100); //延时
	UART2_485_RX();   //进入485接收模式
#endif
	xSemaphoreGive(uart2DMATxMutex);
	return err;
}

/*-----------------------------------------------------------------------------
 * @name  : UART3_Send
 * @brief : 串口3 485发送
 * @param : txbuf 发送buf指针
 *          dataLen 数据长度
 * @retval: 0 OK  1 Error
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t UART3_Send(uint8_t *txbuf, uint16_t dataLen)
{
	uint8_t err = 0;
	/* 等待其他线程处理完成 */
	xSemaphoreTake(uart3DMATxMutex, portMAX_DELAY);
#if( RS485_ENABLE3 == 1 )
	UART3_485_TX();  //进入485发送模式
	_485_delay(1000); //延时
#endif
	err = HAL_UART_Transmit_DMA(&huart3, txbuf, dataLen);
	/* 等待DMA发送完成 */
	xSemaphoreTake(uart3DMATxCpltSem, portMAX_DELAY);
#if( RS485_ENABLE3 == 1 )
	_485_delay(100); //延时
	UART3_485_RX();  //进入485接收模式
#endif
	xSemaphoreGive(uart3DMATxMutex);
	return err;
}

/*-----------------------------------------------------------------------------
 * @name  : UART4_Send
 * @brief : 串口4 485发送
 * @param : txbuf 发送buf指针
 *          dataLen 数据长度
 * @retval: 0 OK  1 Error
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t UART4_Send(uint8_t *txbuf, uint16_t dataLen)
{
	uint8_t err = 0;
	/* 等待其他线程处理完成 */
	xSemaphoreTake(uart4DMATxMutex, portMAX_DELAY);
#if( RS485_ENABLE4 == 1 )
	UART4_485_TX();  //进入485发送模式
	_485_delay(1000); //延时
#endif
	err = HAL_UART_Transmit_DMA(&huart4, txbuf, dataLen);
	/* 等待DMA发送完成 */
	xSemaphoreTake(uart4DMATxCpltSem, portMAX_DELAY);
#if( RS485_ENABLE4 == 1 )
	_485_delay(100); //延时
	UART4_485_RX();  //进入485接收模式
#endif
	xSemaphoreGive(uart4DMATxMutex);
	return err;
}

/*-----------------------------------------------------------------------------
 * @name  : UART5_Send
 * @brief : 串口5 485发送
 * @param : txbuf 发送buf指针
 *          dataLen 数据长度
 * @retval: 0 OK  1 Error
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t UART5_Send(uint8_t *txbuf, uint16_t dataLen)
{
	uint8_t err = 0;
	/* 等待其他线程处理完成 */
	xSemaphoreTake(uart5DMATxMutex, portMAX_DELAY);
#if( RS485_ENABLE5 == 1 )
	UART5_485_TX();  //进入485发送模式
	_485_delay(1000); //延时
#endif
	err = HAL_UART_Transmit_DMA(&huart5, txbuf, dataLen);
	/* 等待DMA发送完成 */
	xSemaphoreTake(uart5DMATxCpltSem, portMAX_DELAY);
#if( RS485_ENABLE5 == 1 )
	_485_delay(100); //延时
	UART5_485_RX();  //进入485接收模式
#endif
	xSemaphoreGive(uart5DMATxMutex);
	return err;
}


/*-----------------------------------------------------------------------------
 * @name  : UART6_Send
 * @brief : 串口6 485发送
 * @param : txbuf 发送buf指针
 *          dataLen 数据长度
 * @retval: 0 OK  1 Error
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t UART6_Send(uint8_t *txbuf, uint16_t dataLen)
{
	uint8_t err = 0;
	/* 等待其他线程处理完成 */
	xSemaphoreTake(uart6DMATxMutex, portMAX_DELAY);
#if( RS485_ENABLE6 == 1 )
	UART6_485_TX();  //进入485发送模式
	_485_delay(1000); //延时
#endif
	err = HAL_UART_Transmit_DMA(&huart6, txbuf, dataLen);
	/* 等待DMA发送完成 */
	xSemaphoreTake(uart6DMATxCpltSem, portMAX_DELAY);
#if( RS485_ENABLE6 == 1 )
	_485_delay(100); //延时
	UART6_485_RX();  //进入485接收模式
#endif
	xSemaphoreGive(uart6DMATxMutex);
	return err;
}

/*-----------------------------------------------------------------------------
 * @name  : UART7_Send
 * @brief : 串口7 DMA 485 发送
 * @param : txbuf 发送buf指针
 *          dataLen 数据长度
 * @retval: 0 OK  1 Error
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t UART7_Send(uint8_t *txbuf, uint16_t dataLen)
{
	uint8_t err = 0;
	/* 等待其他线程处理完成 */
	xSemaphoreTake(uart7DMATxMutex, portMAX_DELAY);
#if( RS485_ENABLE7 == 1 )
	UART7_485_TX();  //进入485发送模式
	_485_delay(1000); //延时
#endif
	err = HAL_UART_Transmit_DMA(&huart7, txbuf, dataLen);
	/* 等待DMA发送完成 */
	xSemaphoreTake(uart7DMATxCpltSem, portMAX_DELAY);
#if( RS485_ENABLE7 == 1 )
	_485_delay(100); //延时
	UART7_485_RX();  //进入485接收模式
#endif
	xSemaphoreGive(uart7DMATxMutex);
	return err;
}

/*-----------------------------------------------------------------------------
 * @name  : UART8_Send
 * @brief : 串口8 DMA发送
 * @param : txbuf 发送buf指针
 *          dataLen 数据长度
 * @retval: 0 OK  1 Error
 * @date  : 2021/08/27
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t UART8_Send(uint8_t *txbuf, uint16_t dataLen)
{
	uint8_t err = 0;
	/* 等待其他线程处理完成 */
	xSemaphoreTake(uart8DMATxMutex, portMAX_DELAY);
#if( RS485_ENABLE8 == 1 )
	UART8_485_TX();  //进入485发送模式
	_485_delay(1000); //延时
#endif
	err = HAL_UART_Transmit_DMA(&huart8, txbuf, dataLen);
	/* 等待DMA发送完成 */
	xSemaphoreTake(uart8DMATxCpltSem, portMAX_DELAY);
#if( RS485_ENABLE78 == 1 )
	_485_delay(100); //延时
	UART8_485_RX();  //进入485接收模式
#endif
	xSemaphoreGive(uart8DMATxMutex);
	return err;
}
/*-----------------------------------------------------------------------------
* @name  : Uart8_String(char *fmt, ...)
* @brief : 输出调试信息
* @param : fmt 第一个输入的参数的指针（固定参数后为可变参）
* 			... 可变参数
* @retval: 输出调试字符的长度
* @date  : 2021/08/27
* @note  : 目前版本的调试接口一次性输出 < 100 bytes
* ---------------------------------------------------------------------------*/
#define UART8_STRING_SIZE	100
uint8_t stringTxBuf8[UART8_STRING_SIZE] = {0};
uint32_t Uart8_String(char *fmt, ...)
{
   va_list args;
   uint32_t length;
   va_start(args, fmt);
   length = vsnprintf((char *)stringTxBuf8, UART8_STRING_SIZE, (char *)fmt, args);
   va_end(args);
   UART8_Send(stringTxBuf8, length);
   memset(stringTxBuf8, 0, UART8_STRING_SIZE);
   return length;
}



/*----------------- (C) COPYRIGHT Lyric Robot-----END OF FILE-----------------*/
