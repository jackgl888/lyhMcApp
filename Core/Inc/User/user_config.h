/*-----------------------------------------------------------------------------
 * @file        : user_config.h
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2020/09/15
 * @date changed: 2020/12/26
 * @brief       : 用户配置选项
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/

#ifndef INC_USER_USER_CONFIG_H_
#define INC_USER_USER_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------系统级选项----------------------------------*/
#define __OS_ENABLE				1	//操作系统
#define __LWIP_ENABLE			1	//LWIP
#define __DSP_CALCULTATE		1	//DSP运算库
#define __BSP_FLASH             0   //内部Flash
#define __SYS_IWDG				0	//事件组独立看门狗

#define __USER_LOSE_POWER       0   //是否开启断电续接
#define __USER_DEBUG_PRINTF     0   //ITM是否输出printf调试信息
#define __USER_PRINTF		    0	//是否输出printf调试信息
#define __USER_ASSERT			0	//是否输出调试信息的同时使用自定义的断言

#define __BSP_ADC               1   //ADC
#define __BSP_DAC               0   //DAC

#define __TEST_NAND_FLASH       1   //是否开启NAND FLASH测试



/*-------System_file-----------------*/
#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include "stm32h7xx_it.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "math.h"
#include "assert.h"


/*-------FreeRTOS_header_file--------*/
#if __OS_ENABLE
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "list.h"
#endif


/*-------LWIP_header_file------------*/
#if __LWIP_ENABLE
#include "lwip.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/sockets.h"
#include "lwip/opt.h"
#include "lwip/timeouts.h"
#include "ethernetif.h"
#include "lwip/tcpip.h"
#endif


/*-------FATFS_header_file-----------*/
#include "fatfs.h"
#include "exfuns.h"


/*-------DSP_Lib_header_file---------*/
#if __DSP_CALCULTATE
#include "arm_math.h"
#endif


/*-------Peripheral_header_file------*/
#include "crc.h"
#include "dma.h"
#include "fatfs.h"
#include "iwdg.h"
#include "rtc.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "fdcan.h"


/*-------BSP_header_file------------*/
#include <bsp/bsp_init.h>
#include <bsp_key.h>
#include <bsp_multi_button.h>
#include <bsp/bsp_rs485.h>
#include <bsp/bsp_sdram.h>
#include <bsp/bsp_nand.h>
#include <bsp/bsp_adc.h>


/*-------User_header_file------------*/
#include <user/user_flash.h>
#include <user/user_tcpecho.h>
#include <user/user_malloc.h>
#include <user/user_test_nand_flash.h>
#include <user/user_ftl.h>
#include <user/user_bi_power_supply.h>
#include <user/user_kalman.h>
#include <user/user_circle_queue.h>
#include <user/user_protocol.h>
#include <user/user_lower_computer.h>
#include <user/user_upper_computer.h>
#include <user/user_lose_power.h>
#include "user_disk_manage.h"
#include "user_dram_manage.h"



/*--------------------------------OS_IPC_MACRO--------------------------------*/
//CANopen读字典消息队列的长度消息单元大小
#define QUEUE_LEN   4  /* 队列的长度，最大可包含多少个消息 */
#define QUEUE_SIZE  2  /* 队列中每个消息大小（字节） */

//时间组通用
#define ALL_EVENT   0xFFFFFFU

//喂狗事件组掩码
#define FEED_DOG_EVENT1     (0x01 << 0)    //设置事件掩码的位 1
#define FEED_DOG_EVENT2     (0x01 << 1)    //设置事件掩码的位 2
#define FEED_DOG_EVENT3     (0x01 << 2)    //设置事件掩码的位 3
#define FEED_DOG_EVENT4     (0x01 << 3)    //设置事件掩码的位 4
#define FEED_DOG_EVENT5     (0x01 << 4)    //设置事件掩码的位 5
#define FEED_DOG_EVENT6     (0x01 << 5)    //设置事件掩码的位 6
#define FEED_DOG_EVENT7     (0x01 << 6)    //设置事件掩码的位 7
#define FEED_DOG_EVENT8     (0x01 << 7)    //设置事件掩码的位 8
#define FEED_DOG_EVENT9     (0x01 << 8)    //设置事件掩码的位 9
#define FEED_DOG_EVENT10    (0x01 << 9)    //设置事件掩码的位 10
#define FEED_DOG_EVENT11    (0x01 << 10)   //设置事件掩码的位 11
#define FEED_DOG_EVENT12    (0x01 << 11)   //设置事件掩码的位 12
#define FEED_DOG_EVENT13    (0x01 << 12)   //设置事件掩码的位 13
#define FEED_DOG_EVENT14    (0x01 << 13)   //设置事件掩码的位 14
#define FEED_DOG_EVENT15    (0x01 << 14)   //设置事件掩码的位 15
#define FEED_DOG_EVENT16    (0x01 << 15)   //设置事件掩码的位 16

#define FEED_DOG_EVENT      (FEED_DOG_EVENT1 | \
		                     FEED_DOG_EVENT2 | \
							 FEED_DOG_EVENT3 | \
							 FEED_DOG_EVENT4 | \
							 FEED_DOG_EVENT5 | \
							 FEED_DOG_EVENT6 | \
							 FEED_DOG_EVENT7 | \
							 FEED_DOG_EVENT8)



//通信事件组掩码
#define TIMING_COMM_LOSE_POWER_SYNC_EVENT      (0x01 << 0)    //设置事件掩码的位 0 通信同步
#define TIMING_COMM_SYNC_EVENT                 (0x01 << 1)    //设置事件掩码的位 1 通信同步
#define TIMING_COMM_GET_BOARD_DATA_EVENT       (0x01 << 2)    //设置事件掩码的位 2 获取实时数据指令
#define TIMING_COMM_QUERY_DATA_EVENT           (0x01 << 3)    //设置事件掩码的位 3 查询指令



#define TIMING_COMM_EVENT                      (TIMING_COMM_GET_BOARD_DATA_EVENT | \
										        TIMING_COMM_QUERY_DATA_EVENT)





/*--------------------------------USER_APP----------------------------------*/

typedef enum
{
	CONTROL_FREE = 0x00,  //无
	CONTROL_MIDDLE_PC,    //中位机
	CONTROL_UPPER_PC      //上位机
}Priority_E;

/* 控制优先级 */
Priority_E ePriority;

/* 枚举ITM调试的所有Port */
typedef enum
{
	ITM_PORT_0 = 0,
	ITM_PORT_1,
	ITM_PORT_2,
	ITM_PORT_3,
	ITM_PORT_4,
	ITM_PORT_5,
	ITM_PORT_6,
	ITM_PORT_7,
	ITM_PORT_8,
	ITM_PORT_9,
	ITM_PORT_10,
	ITM_PORT_11,
	ITM_PORT_12,
	ITM_PORT_13,
	ITM_PORT_14,
	ITM_PORT_15,
	ITM_PORT_16,
	ITM_PORT_17,
	ITM_PORT_18,
	ITM_PORT_19,
	ITM_PORT_20,
	ITM_PORT_21,
	ITM_PORT_22,
	ITM_PORT_23,
	ITM_PORT_24,
	ITM_PORT_25,
	ITM_PORT_26,
	ITM_PORT_27,
	ITM_PORT_28,
	ITM_PORT_29,
	ITM_PORT_30,
	ITM_PORT_31,
}ITM_PORT_E;

/*--------------------------------USER_DEBUG----------------------------------*/
//相关宏
#define HUART_NUMBER_ADDR 	(&huart1)	//选择用于输出调试信息的串口号

// ## 的意思是可变参数被忽略或为空
//__FILE__本身会被编译器置换为字符常量
#if __USER_ASSERT
#define DEBUG_PRINTF(fmt, ...)     printf("File: "__FILE__", Line: %d:\r\n"fmt"\r\n", __LINE__, ##__VA_ARGS__)
#elif __USER_PRINTF
#define DEBUG_PRINTF(fmt, ...)     User_Printf(HUART_NUMBER_ADDR, fmt, ##__VA_ARGS__)
#elif __USER_DEBUG_PRINTF
#define DEBUG_PRINTF(fmt, ...)
#define DEBUG_PRINTF1(fmt, ...)    My_Printf(ITM_PORT_1, fmt, ##__VA_ARGS__) // ITM 1端口调试
#else
#define DEBUG_PRINTF(fmt, ...)
#define DEBUG_PRINTF1(fmt, ...)
#endif


//API声明
uint32_t User_Printf(UART_HandleTypeDef *huart, char *fmt, ...); //可用任意多个串口打印（重封装printf）
uint32_t My_Printf(ITM_PORT_E itm_port_num, char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_USER_CONFIG_H_ */
