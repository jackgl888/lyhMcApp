/*-----------------------------------------------------------------------------
 * @file        : user_circle_queue.h
 * @author      : Lyric-L
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2021/10/06
 * @date changed: 2021/10/06
 * @brief       : 环形队列
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#ifndef INC_USER_USER_CIRCLE_QUEUE_H_
#define INC_USER_USER_CIRCLE_QUEUE_H_

#include "main.h"
#include "string.h"
#include "user_malloc.h"
#include "assert.h"
#include "stdlib.h"

/* 只有一个生产者和消费者的情况下不需要锁保护 */
#define QUEUE_USE_LOCK

/* 环形队列是否使用断言 */
#define QUEUE_USE_ASSERT     1

/* 环形队列是否使用用户内存管理 */
/* 未使用 <stdlib.h> 中的 calloc 和 free 函数 */
#define QUEUE_USE_MALLOC     1

//TODO:队列参数配置
#define QUEUE_STATUS         0x0111 //FIFO初始化状态
#define QUEUE_OneSize        2048   //限制单条数据大小
#define QUEUE_MaxCnt         48     //限制缓冲区总数据容量(条数)


#ifdef QUEUE_USE_LOCK
#include "FreeRTOS.h"
#include "semphr.h"

#define	QUEUE_API_CREATELOCK(v)		do{(v)->lock = xSemaphoreCreateMutex();assert((v)->lock);}while(0);
#define QUEUE_API_DELETELOCK(v)		do{((v)->lock!=NULL) ? vSemaphoreDelete((v)->lock) : pdFAIL;(v)->lock = NULL;}while(0);
#define	QUEUE_API_LOCK(v)			do{((v)->lock!=NULL) ? xSemaphoreTake((v)->lock, portMAX_DELAY) : pdFAIL;}while(0);
#define QUEUE_API_UNLOCK(v)			do{((v)->lock!=NULL) ? xSemaphoreGive((v)->lock) : pdFAIL;}while(0);
#else
#define	QUEUE_API_CREATELOCK(v)
#define QUEUE_API_DELETELOCK(v)
#define	QUEUE_API_LOCK(v)
#define QUEUE_API_UNLOCK(v)
#endif

//队列状态枚举
typedef enum
{
	QUEUE_OK       = 0x00,
	QUEUE_ERROR    = 0x01,
	QUEUE_ISFULL   = 0x02,
	QUEUE_TRUE     = 0x03,
	QUEUE_FALSE    = 0x04
}QueueStatusTypeDef_E;


#pragma  pack(push)  	//保存对齐状态
#pragma  pack(1)		//改为1字节对齐，防止内存对齐导致的错误


//环形队列缓存区
typedef struct
{
	uint8_t *pQueueBuff;       //记录数据的缓冲区指针
	uint16_t *pByteCntBuff;    //记录数据大小的缓冲区
	uint16_t InitStatus;       //初始化状态
	__IO uint16_t OneSize;     //单条数据大小
	__IO uint16_t Cnt;         //缓冲区总数据容量(条数)
	__IO uint16_t WriteCnt;    //写入位置
	__IO uint16_t ReadCnt;     //读取位置
	__IO uint16_t NotReadCnt;  //未读取数量
	__IO uint16_t DataLen;     //读取到数据的长度
#ifdef QUEUE_USE_LOCK
	SemaphoreHandle_t   lock;  //保护锁
#endif
}QueueHandle_T;

QueueHandle_T tCtrlQueue; //控制环形队列

QueueHandle_T tLowerPcRxQueue; //下位机接收环形队列
QueueHandle_T tLowerPcTxQueue; //下位机发送环形队列
QueueHandle_T tUpperPcRxQueue; //上位机接收环形队列
QueueHandle_T tUpperPcTxQueue; //上位机发送环形队列

QueueHandle_T tRecordDataQueue;   //通道记录数据环形队列



#pragma pack(pop)


//函数声明
uint8_t uQueueInit(QueueHandle_T * pHandle, uint16_t OneSize, uint16_t MaxCnt);   //FIFO初始化
uint8_t uQueueDelete(QueueHandle_T *pQueue);                                      //删除队列：释放队列内存、释放队列句柄内存
uint8_t uQueueWrite(QueueHandle_T *pQueue, uint8_t *pBuf, uint16_t len);          //写入环形队列
uint8_t uQueueWriteNoCover(QueueHandle_T *pQueue, uint8_t *pBuf, uint16_t len);   //写入环形队列，写满不覆盖
uint8_t uQueueRead(QueueHandle_T *pHandle, uint8_t *pBuf, uint16_t *len);         //读取一条数据，并清除该条数据
uint8_t uQueueReadNoDel(QueueHandle_T *pQueue, uint8_t *pBuf, uint16_t *len);     //读取一条数据，不清除该条数据
uint8_t uQueueClear(QueueHandle_T *pQueue, uint16_t delCnt);                      //清除指定个数的数据
uint8_t uQueueClearALL(QueueHandle_T *pHandle);                                   //清除FIFO
uint16_t uQueueGetDataNum(QueueHandle_T *pHandle);                                //获取FIFO中已存数据数量
uint16_t uQueueLeftNum(QueueHandle_T *pQueue);                                    //获取FIFO剩余可存放数据个数
uint16_t uQueueCapacity(QueueHandle_T *pQueue);                                   //获取FIFO总容量
uint8_t uQueueIsFull(QueueHandle_T *pQueue);                                      //FIFO是否已满
uint8_t uQueueIsEmpty(QueueHandle_T *pQueue);                                     //FIFO是否已空


#endif /* INC_USER_USER_CIRCLE_QUEUE_H_ */
