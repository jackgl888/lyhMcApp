#ifndef DRAM_MANAGE_H
#define DRAM_MANAGE_H

#include "stm32h7xx_hal.h"


#define RECORD_SAVE_DRAM_LEN   (3*128*1024)    //用于存储记录数据的SDRAM区队列单元个数
#define RECORD_SAVE_DRAM_SIZE  (1)             //每个队列单元的大小 bytes


typedef struct myDramQueue *myDramQueueHandle_t;

myDramQueueHandle_t myDramQueueCreate(size_t queue_len,size_t item_size);
void myDramQueueDelete(myDramQueueHandle_t queue);
uint8_t myDramQueueToNand(BTE_File_Name_E fileName, myDramQueueHandle_t queue, size_t num);
uint8_t uRecordDataSaveLose(void);
uint8_t uRecordDataSave(const void *buf, size_t num);
uint8_t uRecordDataPack(UpperControlCmd_E eCmd, QueueHandle_T * pQueue, uint32_t *wl);
uint8_t uRecordDataMemoryInit(void);

#endif
