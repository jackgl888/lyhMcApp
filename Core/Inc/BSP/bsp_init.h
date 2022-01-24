/*-----------------------------------------------------------------------------
 * @file        : bsp_init.h
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2020/12/26
 * @date changed: 2020/12/26
 * @brief       : 板级硬件初始化文件
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/

#ifndef INC_BSP_BSP_INIT_H_
#define INC_BSP_BSP_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

//十进制转ASCII码
#define DECToASC(a)  (uint8_t)(0x30+a)

//ASCII码转十进制
#define ASCToDEC(a)  (uint8_t)(a%(0x30))

/*--------------------------------USER_OS_IPC----------------------------------*/
//LWIP
extern TaskHandle_t PHY_Link_Check_Handle;
extern TaskHandle_t TCP_TASK_Handle; //TCP任务句柄


//串口发送DMA处理
extern SemaphoreHandle_t uart1DMATxCpltSem;	//串口1 DMA 发送完成信号
extern SemaphoreHandle_t uart2DMATxCpltSem;	//串口2 DMA 发送完成信号
extern SemaphoreHandle_t uart3DMATxCpltSem;	//串口3 DMA 发送完成信号
extern SemaphoreHandle_t uart4DMATxCpltSem;	//串口4 DMA 发送完成信号
extern SemaphoreHandle_t uart5DMATxCpltSem;	//串口5 DMA 发送完成信号
extern SemaphoreHandle_t uart6DMATxCpltSem;	//串口6 DMA 发送完成信号
extern SemaphoreHandle_t uart7DMATxCpltSem;	//串口7 DMA 发送完成信号
extern SemaphoreHandle_t uart8DMATxCpltSem;	//串口8 DMA 发送完成信号
extern SemaphoreHandle_t uart1DMATxMutex;   //串口1 通信保护（多个线程调用锁）
extern SemaphoreHandle_t uart2DMATxMutex;   //串口2 通信保护（多个线程调用锁）
extern SemaphoreHandle_t uart3DMATxMutex;   //串口3 通信保护（多个线程调用锁）
extern SemaphoreHandle_t uart4DMATxMutex;   //串口4 通信保护（多个线程调用锁）
extern SemaphoreHandle_t uart5DMATxMutex;   //串口5 通信保护（多个线程调用锁）
extern SemaphoreHandle_t uart6DMATxMutex;   //串口6 通信保护（多个线程调用锁）
extern SemaphoreHandle_t uart7DMATxMutex;   //串口7 通信保护（多个线程调用锁）
extern SemaphoreHandle_t uart8DMATxMutex;   //串口8 通信保护（多个线程调用锁）
//任务线程保护互斥量
extern SemaphoreHandle_t  ReadFlashMutex;    //读flash保护互斥量
extern SemaphoreHandle_t  WriteFlashMutex;   //写flash保护互斥量
//AC-DC双向电源模块
extern SemaphoreHandle_t  PowerSupplyMutex;   //双向电源通信保护互斥量
extern SemaphoreHandle_t  PowerSupplySem;     //双向电源错误重发管理
//下位机通信
extern SemaphoreHandle_t LowerPcSyncSem;      //创建二值信号量，用于通信同步
extern SemaphoreHandle_t LowerPcRxSem;        //创建二值信号量，用于接收数据
extern SemaphoreHandle_t LowerPcSem;          //创建二值信号量，用于错误重发
extern SemaphoreHandle_t LowerPcMutex;        //保护互斥量，用于通信数据处理保护
extern SemaphoreHandle_t LowerPcCaliMutex;    //保护互斥量，用于校准数据处理保护
//上位机位机通信
extern SemaphoreHandle_t UpperPcSem;          //创建二值信号量，用于错误重发
extern SemaphoreHandle_t UpperPcMutex;        //保护互斥量，用于通信数据处理保护
//事件
extern EventGroupHandle_t WatchDogEvent;   //用于独立开门狗的喂狗事件
extern EventGroupHandle_t TimingCommEvent; //用于下位机定时通信处理事件

/* 任务测试数组 */
extern uint32_t threadTimetest[20];


/* 初始化需要用到的函数声明 */
void BSP_Init(void);
void Test_Task(void *parameter);
void NAND_Test_Task(void *parameter);
void QUEUE_Test_Task(void *parameter);
void AppTaskCreate(void *parameter);
void ThreadTime(uint32_t num);
void SystemClock_Config(void);
void Cache_Enable(void);
void MPU_Config(void);
uint8_t DataU16H_ToU8(uint16_t parm);
uint8_t DataU16L_ToU8(uint16_t parm);
uint16_t Data2U8ToU16(uint8_t data_L, uint8_t data_H);
uint32_t Data4U8ToU32(uint8_t data16L_L, uint8_t data16L_H, uint8_t data16H_L, uint8_t data16H_H);

#ifdef __cplusplus
}
#endif

#endif /* INC_BSP_BSP_INIT_H_ */
