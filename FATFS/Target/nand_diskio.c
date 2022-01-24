/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sd_diskio.c
  * @brief   SD Disk I/O driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Note: code generation based on sd_diskio_dma_rtos_template_bspv1.c v2.1.4
   as FreeRTOS is enabled. */

/* USER CODE BEGIN firstSection */
/* can be used to modify / undefine following code or add new definitions */
/* USER CODE END firstSection*/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "ff_gen_drv.h"
#include "nand_diskio.h"
#include "bsp_nand.h"
#include "user_ftl.h"


#define NAND_TIMEOUT  1000
#define NAND_DEFAULT_BLOCK_SIZE 512

/* Private variables ---------------------------------------------------------*/

/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

#ifdef NAND_DISK_USE_OS
static SemaphoreHandle_t nandDiskLock = NULL;       //保护锁
#endif

/* Private function prototypes -----------------------------------------------*/
static DSTATUS NAND_CheckStatus(BYTE lun);
DSTATUS NAND_initialize (BYTE);
DSTATUS NAND_status (BYTE);
DRESULT NAND_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
DRESULT NAND_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
DRESULT NAND_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  NAND_Driver =
{
	NAND_initialize,
	NAND_status,
	NAND_read,
#if  _USE_WRITE == 1
	NAND_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
	NAND_ioctl,
#endif /* _USE_IOCTL == 1 */

};


/* Private functions ---------------------------------------------------------*/
static int NAND_CheckStatusWithTimeout(uint32_t timeout)
{
#ifdef NAND_DISK_USE_OS
	BYTE lun = 0;  //no use
	uint32_t timer;
	timer = osKernelSysTick();
	while( osKernelSysTick() - timer < timeout)
	{
		if (NAND_CheckStatus(lun) == RES_OK)
		{
			return 0;
		}
	}
#endif
	return -1;
}


/**
  * @brief  读NAND状态
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
static DSTATUS NAND_CheckStatus(BYTE lun)
{
//TODO：20211013测试异常
//  Stat = STA_NOINIT;
//  uint8_t status = NAND_ReadStatus();
//  if(status == NSTA_READY)
//  {
//    Stat &= ~STA_NOINIT;
//  }
//  return Stat;
	return RES_OK;
}

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS NAND_initialize(BYTE lun)
{
	Stat = STA_NOINIT;
	if(FTL_Init() == 0)
	{
		Stat = NAND_CheckStatus(lun);
	}
	/* 如果初始化成功则开始创建互斥量 */
	if (Stat != STA_NOINIT)
	{
		if (nandDiskLock == NULL)
		{
			NAND_DISK_API_CREATELOCK(nandDiskLock);
		}
		if (nandDiskLock == NULL)
		{
			Stat |= STA_NOINIT;
		}
	}
	return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS NAND_status(BYTE lun)
{
	return NAND_CheckStatus(lun);
}

/* USER CODE BEGIN beforeReadSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END beforeReadSection */
/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT NAND_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
	DRESULT res = RES_ERROR;
    if (!count)
    	return RES_PARERR;//count不能等于0，否则返回参数错误
    if (NAND_CheckStatusWithTimeout(NAND_TIMEOUT) < 0)
      return res;
    NAND_DISK_API_LOCK(nandDiskLock);
	res=FTL_ReadSectors(buff,sector,NAND_DEFAULT_BLOCK_SIZE,count);	//读取数据
	NAND_DISK_API_UNLOCK(nandDiskLock);
    if(res == 0x00)
    	return RES_OK;
    else
    	return RES_ERROR;
}

/* USER CODE BEGIN beforeWriteSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END beforeWriteSection */
/**
  * @brief  Writes Sector(s)
  * @param  lun : not used
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1

DRESULT NAND_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
	DRESULT res = RES_ERROR;
	if(!count)
		return RES_PARERR;//count不能等于0，否则返回参数错误
	if (NAND_CheckStatusWithTimeout(NAND_TIMEOUT) < 0)
		return res;
	NAND_DISK_API_LOCK(nandDiskLock);
	res = FTL_WriteSectors((uint8_t*)buff,sector,NAND_DEFAULT_BLOCK_SIZE,count);//写入数据
	NAND_DISK_API_UNLOCK(nandDiskLock);
	//处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
	if(res == 0x00)
		return RES_OK;
	else
		return RES_ERROR;
}

#endif /* _USE_WRITE == 1 */

/* USER CODE BEGIN beforeIoctlSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END beforeIoctlSection */
/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT NAND_ioctl(BYTE lun, BYTE cmd, void *buff)
{
	DRESULT res = RES_ERROR;
	if (Stat & STA_NOINIT)
		return RES_NOTRDY;
	switch(cmd)
	{
	case CTRL_SYNC:
		res = RES_OK;
		break;
	case GET_SECTOR_SIZE:
		*(WORD*)buff = NAND_DEFAULT_BLOCK_SIZE;	//NAND FLASH扇区强制为512字节大小
		res = RES_OK;
		break;
	case GET_BLOCK_SIZE:
		*(WORD*)buff = tNand_dev.page_mainsize / NAND_DEFAULT_BLOCK_SIZE;//block大小,定义成一个page的大小
		res = RES_OK;
		break;
	case GET_SECTOR_COUNT:
		*(DWORD*)buff = tNand_dev.valid_blocknum * tNand_dev.block_pagenum *
		                tNand_dev.page_mainsize / NAND_DEFAULT_BLOCK_SIZE;//NAND FLASH的总扇区大小
		res = RES_OK;
		break;
	default:
		res = RES_PARERR;
		break;
	}
	return res;
}
#endif /* _USE_IOCTL == 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
