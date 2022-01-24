/*-----------------------------------------------------------------------------
 * @file        : user_upper_computer.h
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2019/10/18
 * @date changed: 2021/10/22
 * @brief       : 中上位机通信处理程序
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#ifndef INC_USER_USER_UPPER_COMPUTER_H_
#define INC_USER_USER_UPPER_COMPUTER_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifdef USER_CONFIG
	#include "user_config.h"
#endif


#define UPPER_PC_IMMEDIATE_ACK         0 //是否直接应答 0：否  1：是


/* 缓存区配置 */
#if (LOWER_CH_TOTAL_NUM == 8U)
#define TEMP_BUF_MAX_LEN               (2*1024) //临时缓存区数据大小
#elif (LOWER_CH_TOTAL_NUM == 16U)
#define TEMP_BUF_MAX_LEN               (4*1024) //临时缓存区数据大小
#endif

#define UPPER_PC_BUF_MAX_LEN           (1460)   //限制单条数据大小
#define UPPER_PC_BUF_MAX_CNT           (64)     //限制缓冲区总数据容量(条数)


/* 锁保护 */
#define	UPPER_PC_LOCK()                do{(UpperPcMutex==NULL) ? xSemaphoreTake(UpperPcMutex, portMAX_DELAY) : pdFAIL;}while(0);
#define UPPER_PC_UNLOCK()              do{(UpperPcMutex==NULL) ? xSemaphoreGive(UpperPcMutex) : pdFAIL;}while(0);


#pragma  pack(push)  	//保存对齐状态
#pragma  pack(1)		//改为1字节对齐，防止内存对齐导致的错误

//--------------------  用户数据结构-------------------------


/* 工步加载参数结构体 */
typedef struct
{
	GlobalPramSet_T tGlobalPram;       //全局参数
	uint8_t buf[UPPER_PC_BUF_MAX_LEN]; //工步加载数据
	uint16_t len;                      //工步加载数据长度
}StepLoadBuf_T;


StepLoadBuf_T tStepLoadBuf;



#pragma pack(pop)


//----------------------函数声明-----------------------------
void UpperPc_DeviceInfoInit(void);

void UpperPc_TxTask(void *parameter);

void UpperPc_RxTask(void *parameter);



#ifdef __cplusplus
}
#endif

#endif /* INC_USER_USER_UPPER_COMPUTER_H_ */
