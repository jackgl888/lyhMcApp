/*-----------------------------------------------------------------------------
 * @file        : user_config.c
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2020/09/15
 * @date changed: 2020/12/26
 * @brief       : 用户配置选项接口
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#ifdef USER_CONFIG
	#include "user_config.h"
#endif

/*-----------------------------------------------------------------------------
 * @name  : None
 * @brief : printf重映射
 * @param : None
 * @retval: None
 * @date  : 2020/07/20
 * @note  : 需要根据实际项目更改串口号
 * ---------------------------------------------------------------------------*/
#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(HUART_NUMBER_ADDR, (uint8_t*)&ch, 1, 1000);
//	ITM_SendChar(ch); // ITM 0端口调试
	return ch;
}

/*-----------------------------------------------------------------------------
 * @name  : USER_DEBUG
 * @brief : 输出调试信息
 * @param : fmt 第一个输入的参数的指针（固定参数后为可变参）
 * 			... 可变参数
 * @retval: 输出调试字符的长度
 * @date  : 2020/07/20
 * @note  : 目前版本的调试接口一次性输出 < 200 bytes
 * ---------------------------------------------------------------------------*/
#define TXBUF_SIZE_MAX	200
uint8_t txbuf[2][TXBUF_SIZE_MAX] = {0};
uint32_t User_Printf(UART_HandleTypeDef *huart, char *fmt, ...)
{
    va_list args;
    uint32_t length;
    va_start(args, fmt);
    length = vsnprintf((char *)txbuf[0], sizeof(txbuf[0]), (char *)fmt, args);
    va_end(args);
    HAL_UART_Transmit(huart, (uint8_t *)txbuf[0], length, 1000);
    return length;
}

/*-----------------------------------------------------------------------------
 * @name  : ITM_SendChar_Multichannel
 * @brief : 重写 ITM_SendChar(),使其兼具多port选择
 * @param :
 * @retval:
 * @date  : 2021/03/09
 * @note  :
 * ---------------------------------------------------------------------------*/
__STATIC_INLINE uint32_t ITM_SendChar_Multichannel (uint32_t ch, ITM_PORT_E itm_port_num)
{
  if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) &&
      ((ITM->TER & (itm_port_num + 1)) != 0UL))
  {
    while (ITM->PORT[itm_port_num].u32 == 0UL)
    {
      __NOP();
    }
    ITM->PORT[itm_port_num].u8 = (uint8_t)ch;
  }
  return (ch);
}

/*-----------------------------------------------------------------------------
 * @name  : my_write
 * @brief : 重写 _write 函数，使其兼具多port选择
 * @param :
 * @retval:
 * @date  : 2021/03/09
 * @note  :
 * ---------------------------------------------------------------------------*/
static int my_write(ITM_PORT_E itm_port_num, char *ptr, int len)
{
	int DataIdx;
	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		ITM_SendChar_Multichannel(*ptr++, itm_port_num);
	}
	return len;
}

/*-----------------------------------------------------------------------------
 * @name  : My_Printf
 * @brief : 输出调试信息，使用 STM32CubeIDE ITM 进行输出调试信息。
 * @param : fmt 第一个输入的参数的指针（固定参数后为可变参）
 * 			... 可变参数
 * @retval: 输出调试字符的长度
 * @date  : 2021/03/09
 * @note  :
 * ---------------------------------------------------------------------------*/
uint32_t My_Printf(ITM_PORT_E itm_port_num, char *fmt, ...)
{
    va_list args;
    uint32_t length;
    uint8_t txbuf[TXBUF_SIZE_MAX] = {0};

    va_start(args, fmt);
    length = vsnprintf((char *)txbuf, sizeof(txbuf), (char *)fmt, args);
    va_end(args);
    my_write(itm_port_num, (char *)txbuf, length);
    memset(txbuf, 0, TXBUF_SIZE_MAX);
    return length;
}

/*----------------- (C) COPYRIGHT Lyric Robot-----END OF FILE-----------------*/
