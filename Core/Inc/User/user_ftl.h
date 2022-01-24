/*
 * bsp_ftl.h
 *
 *  Created on: 2021年9月3日
 *      Author: Administrator
 */

#ifndef INC_BSP_USER_FTL_H_
#define INC_BSP_USER_FTL_H_

#include "main.h"


//坏块搜索控制
//如果设置为1,将在FTL_Format的时候,搜寻坏块,耗时久(512M,3分钟以上),且会导致RGB屏乱闪
#define FTL_USE_BAD_BLOCK_SEARCH		0 //定义是否使用坏块搜索



#pragma  pack(push)  	//保存对齐状态
#pragma  pack(1)		//改为1字节对齐，防止内存对齐导致的错误


//存储区域信息
typedef struct
{
	uint32_t BlockStartAddr; //块地址起始地址
	uint32_t StartAddr;      //页地址起始地址
	uint32_t SectorsUsedNum; //已使用扇区数量
}AreaUsedInformation_T;

#pragma  pack(pop)		//恢复对齐状态



uint8_t FTL_Init(void);
uint8_t FTL_CreateLUT(uint8_t mode);
uint8_t FTL_BlockCompare(uint32_t blockx,uint32_t cmpval);
uint32_t FTL_SearchBadBlock(void);
uint8_t FTL_Format(void);
void FTL_UsedSectorsMark(uint8_t area, uint32_t usednum);
uint32_t FTL_ReadUsedSectors(uint8_t area);
void FTL_BadBlockMark(uint32_t blocknum);
uint8_t FTL_CheckBadBlock(uint32_t blocknum);
uint8_t FTL_UsedBlockMark(uint32_t blocknum);
uint32_t FTL_FindUnusedBlock(uint32_t sblock,uint8_t flag);
uint32_t FTL_FindSamePlaneUnusedBlock(uint32_t sblock);
uint8_t FTL_CopyAndWriteToBlock(uint32_t Source_PageNum,uint16_t ColNum,uint8_t *pBuffer,uint32_t NumByteToWrite);
uint16_t FTL_LBNToPBN(uint32_t LBNNum);
uint8_t FTL_WriteSectors(uint8_t *pBuffer, uint32_t SectorNo, uint16_t SectorSize, uint32_t SectorCount);
uint8_t FTL_ReadSectors(uint8_t *pBuffer,uint32_t SectorNo,uint16_t SectorSize,uint32_t SectorCount);



#endif /* INC_BSP_USER_FTL_H_ */
