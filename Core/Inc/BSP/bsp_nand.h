/*
 * bsp_nand.h
 *
 *  Created on: 2021年9月2日
 *      Author: Administrator
 */

#ifndef INC_BSP_BSP_NAND_H_
#define INC_BSP_BSP_NAND_H_

#include "main.h"


#define NAND_MAX_PAGE_SIZE			2048		//定义NAND FLASH的最大的PAGE大小（不包括SPARE区），默认4096字节
#define NAND_ECC_SECTOR_SIZE		512			//执行ECC计算的单元大小，默认512字节


//NAND属性结构体
typedef struct
{
    uint16_t page_totalsize;     	//每页总大小，main区和spare区总和
    uint16_t page_mainsize;      	//每页的main区大小
    uint16_t page_sparesize;     	//每页的spare区大小
    uint16_t block_pagenum;      	//每个块包含的页数量
    uint16_t plane_blocknum;     	//每个plane包含的块数量
    uint16_t block_totalnum;     	//总的块数量
    uint16_t good_blocknum;      	//好块数量
    uint16_t valid_blocknum;     	//有效块数量(供文件系统使用的好块数量)
    uint16_t used_blocknum;     	//已被占用块数量
    uint32_t page_totalnum;     	//总的页数量
    uint32_t block_totalsectors;    //每个块的扇区数量
    uint32_t id;             		//NAND FLASH ID
    uint16_t *lut;      			//LUT表，用作逻辑块-物理块转换
	uint32_t ecc_hard;				//硬件计算出来的ECC值
	uint32_t ecc_hdbuf[NAND_MAX_PAGE_SIZE/NAND_ECC_SECTOR_SIZE];//ECC硬件计算值缓冲区
	uint32_t ecc_rdbuf[NAND_MAX_PAGE_SIZE/NAND_ECC_SECTOR_SIZE];//ECC读取的值缓冲区
}NAND_Attriute_T;

extern NAND_Attriute_T tNand_dev;				//nand重要参数结构体


//NAND FLASH 开启MPU配置
#define NAND_FLASH_MPU          1

//NAND Flash的闲/忙引脚
#define NAND_RB  	            (((GPIOB->IDR) >> 13) & 0x1U)  //NAND Flash的闲/忙引脚

#define NAND_ADDRESS			0x80000000	//nand flash的访问地址,接NCE3, 地址为:0x8000 0000
#define NAND_CMD				1<<16		//发送命令
#define NAND_ADDR				1<<17		//发送地址

//NAND FLASH命令
#define NAND_READID         	0x90    	//读ID指令
#define NAND_FEATURE			0xEF    	//设置特性指令
#define NAND_RESET          	0xFF    	//复位NAND
#define NAND_READSTA        	0x70   	 	//读状态
#define NAND_AREA_A         	0x00
#define NAND_AREA_TRUE1     	0x30
#define NAND_WRITE0        	 	0x80
#define NAND_WRITE_TURE1    	0x10
#define NAND_ERASE0        	 	0x60
#define NAND_ERASE1         	0xD0
#define NAND_MOVEDATA_CMD0  	0x00
#define NAND_MOVEDATA_CMD1  	0x35
#define NAND_MOVEDATA_CMD2  	0x85
#define NAND_MOVEDATA_CMD3  	0x10

//NAND FLASH状态
#define NSTA_READY       	   	0x40		//nand已经准备好
#define NSTA_ERROR				0x01		//nand错误
#define NSTA_TIMEOUT        	0x02		//超时
#define NSTA_ECC1BITERR       	0x03		//ECC 1bit错误
#define NSTA_ECC2BITERR       	0x04		//ECC 2bit以上错误


//NAND FLASH型号和对应的ID号
#define MT29F4G08ABADA			0xDC909556	//MT29F4G08ABADA
#define MT29F16G08ABABA			0x48002689	//MT29F16G08ABABA
#define W29N01GVSIAA			0xF1809500	//W29N01GVSIAA
#define W29N01HVSINA			0xF1009500	//W29N01HVSINA

//MPU相关设置
#define NAND_REGION_NUMBER      MPU_REGION_NUMBER3	    //NAND FLASH使用region0
#define NAND_ADDRESS_START      0x80000000              //NAND FLASH区的首地址
#define NAND_REGION_SIZE        MPU_REGION_SIZE_128MB   //NAND FLASH区大小

uint8_t NAND_Init(void);
uint8_t NAND_ModeSet(uint8_t mode);
uint32_t NAND_ReadID(void);
uint8_t NAND_ReadStatus(void);
uint8_t NAND_WaitForReady(void);
uint8_t NAND_Reset(void);
uint8_t NAND_WaitRB(volatile uint8_t rb);
void NAND_MPU_Config(void);
uint8_t NAND_ReadPage(uint32_t PageNum,uint16_t ColNum,uint8_t *pBuffer,uint16_t NumByteToRead);
uint8_t NAND_ReadPageComp(uint32_t PageNum,uint16_t ColNum,uint32_t CmpVal,uint16_t NumByteToRead,uint16_t *NumByteEqual);
uint8_t NAND_WritePage(uint32_t PageNum,uint16_t ColNum,uint8_t *pBuffer,uint16_t NumByteToWrite);
uint8_t NAND_WritePageConst(uint32_t PageNum,uint16_t ColNum,uint32_t cval,uint16_t NumByteToWrite);
uint8_t NAND_CopyPageWithoutWrite(uint32_t Source_PageNum,uint32_t Dest_PageNum);
uint8_t NAND_CopyPageWithWrite(uint32_t Source_PageNum,uint32_t Dest_PageNum,uint16_t ColNum,uint8_t *pBuffer,uint16_t NumByteToWrite);
uint8_t NAND_ReadSpare(uint32_t PageNum,uint16_t ColNum,uint8_t *pBuffer,uint16_t NumByteToRead);
uint8_t NAND_WriteSpare(uint32_t PageNum,uint16_t ColNum,uint8_t *pBuffer,uint16_t NumByteToRead);
uint8_t NAND_EraseBlock(uint32_t BlockNum);
void NAND_EraseChip(void);

uint16_t NAND_ECC_Get_OE(uint8_t oe,uint32_t eccval);
uint8_t NAND_ECC_Correction(uint8_t* data_buf,uint32_t eccrd,uint32_t ecccl);



#endif /* INC_BSP_BSP_NAND_H_ */
