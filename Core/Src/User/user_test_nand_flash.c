/*
 * user_nand_flash.c
 *
 *  Created on: 2021年9月3日
 *      Author: Administrator
 */

#include <user_test_nand_flash.h>
#ifdef USER_CONFIG
	#include "user_config.h"
#endif


//向NAND某一页写入指定大小的数据
//pagenum:要写入的页地址
//colnum:要写入的开始列地址(页内地址)
//writebytes:要写入的数据大小，MT29F16G最大为4320，MT29F4G最大为2112
uint8_t test_writepage(uint32_t pagenum,uint16_t colnum,uint16_t writebytes, uint8_t CNT)
{
	uint8_t *pbuf;
	uint8_t sta = 0;
    uint16_t i = 0;
	pbuf = pvPortMalloc(5000);
    for(i=0; i<writebytes; i++) //准备要写入的数据,填充数据,从0开始增大
    {
        pbuf[i]=CNT;
    }
	sta = NAND_WritePage(pagenum,colnum,pbuf,writebytes); //向nand写入数据
	vPortFree(pbuf); //释放内存
	return sta;
}

//读取NAND某一页指定大小的数据
//pagenum:要读取的页地址
//colnum:要读取的开始列地址(页内地址)
//readbytes:要读取的数据大小，MT29F16G最大为4320，MT29F4G最大为2112
uint8_t test_readpage(uint32_t pagenum,uint16_t colnum,uint16_t readbytes)
{
	uint8_t *pbuf;
	uint8_t sta = 0;
    uint16_t i = 0;
	pbuf = pvPortMalloc(5000);
	sta = NAND_ReadPage(pagenum,colnum,pbuf,readbytes);	//读取数据

	if(sta==0 || sta==NSTA_ECC1BITERR || sta==NSTA_ECC2BITERR) //读取成功
	{
		DEBUG_PRINTF("read page data is:\r\n");
		for(i=0; i<readbytes; i++)
		{
			DEBUG_PRINTF("%d ",pbuf[i]); //串口打印读取到的数据
		}
		DEBUG_PRINTF("\r\nend\r\n");
	}
	vPortFree(pbuf); //释放内存
	return sta;
}

//将一页数据拷贝到另外一页,并写入一部分内容.
//注意:源页和目标页要在同一个Plane内！(同为奇数/同为偶数)
//spnum:源页地址
//epnum:目标页地址
//colnum:要写入的开始列地址(页内地址)
//writebytes:要写入的数据大小，不能超过页大小
uint8_t test_copypageandwrite(uint32_t spnum,uint32_t dpnum,uint16_t colnum,uint16_t writebytes)
{
	uint8_t *pbuf;
	uint8_t sta = 0;
    uint16_t i = 0;
	pbuf = pvPortMalloc(5000);
    for(i=0; i<writebytes; i++) //准备要写入的数据,填充数据,从0X80开始增大
    {
        pbuf[i] = i+0X80;
    }
	sta = NAND_CopyPageWithWrite(spnum,dpnum,colnum,pbuf,writebytes); //向nand写入数据
	vPortFree(pbuf); //释放内存
	return sta;
}

//读取NAND某一页Spare区指定大小的数据
//pagenum:要读取的页地址
//colnum:要读取的spare区开始地址
//readbytes:要读取的数据大小，MT29F16G最大为64，MT29F4G最大为224
uint8_t test_readspare(uint32_t pagenum,uint16_t colnum,uint16_t readbytes)
{
	uint8_t *pbuf;
	uint8_t sta = 0;
    uint16_t i = 0;
	pbuf = pvPortMalloc(512);
	memset(pbuf, 0, 512);
	sta = NAND_ReadSpare(pagenum,colnum,pbuf,readbytes); //读取数据
	if(sta == 0)//读取成功
	{
		DEBUG_PRINTF("read spare data is:\r\n");
		for(i=0; i<readbytes; i++)
		{
			DEBUG_PRINTF("%x ",pbuf[i]); //串口打印读取到的数据
		}
		DEBUG_PRINTF("\r\nend\r\n");
	}
	vPortFree(pbuf); //释放内存
	return sta;
}

//从指定位置开始,读取整个NAND,每个BLOCK的第一个page的前5个字节
//sblock:指定开始的block编号
void test_readallblockinfo(uint32_t sblock)
{
    uint8_t j = 0;
    uint32_t i = 0;
	uint8_t sta;
    uint8_t buffer[5];
    for(i=sblock; i<tNand_dev.block_totalnum; i++)
    {
    	DEBUG_PRINTF("block %d info:", i);
        sta = NAND_ReadSpare(i*tNand_dev.block_pagenum,0,buffer,5); //读取每个block,第一个page的前5个字节

        if(sta) {DEBUG_PRINTF("failed\r\n");}

		for(j=0;j<5;j++)
        {
            DEBUG_PRINTF("%x ", buffer[j]);
        }
        DEBUG_PRINTF("\r\n");
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//FTL层测试代码

//从某个扇区开始,写入seccnt个扇区的数据
//secx:开始的扇区编号
//secsize:扇区大小
//seccnt:要写入的扇区个数
uint8_t test_ftlwritesectors(uint32_t secx,uint16_t secsize,uint16_t seccnt)
{
	uint8_t *pbuf;
	uint8_t sta = 0;
    uint32_t i = 0;

	static uint8_t cnt = 0; //随机数

    cnt++; if(cnt >= 255) cnt = 1;

    pbuf = pvPortMalloc(secsize*seccnt);
    if(NULL==pbuf)
    	return 3;
    memset(pbuf, 0, secsize*seccnt); //全部清理

    for(i=0; i<(secsize*seccnt); i++)	//准备要写入的数据,填充数据,从0开始增大
    {
        pbuf[i] = i + cnt;
    }

	sta = FTL_WriteSectors(pbuf, secx, secsize, seccnt); //向nand写入数据

	if(sta == 0)
	{
		DEBUG_PRINTF("\r\nwrite\r\n");
		for(i= 0; i<(secsize*seccnt); i++)
		{
			DEBUG_PRINTF("%x ",pbuf[i]); //串口打印数据
		}
		DEBUG_PRINTF("\r\nend\r\n");
	}
	else if(sta == 1)
	{
		DEBUG_PRINTF("\r\n物理块号大于NAND FLASH的有效总块数.\r\n");
	}
	else if(sta == 2)
	{
		DEBUG_PRINTF("\r\n读扇区数量错误!\r\n");
	}
	else if(sta == 3)
	{
		DEBUG_PRINTF("\r\nWrite failed!\r\n");
	}

	vPortFree(pbuf); //释放内存

	return sta;
}


//从某个扇区开始,读出seccnt个扇区的数据
//secx:开始的扇区编号
//secsize:扇区大小
//seccnt:要读取的扇区个数
uint8_t test_ftlreadsectors(uint32_t secx,uint16_t secsize,uint16_t seccnt)
{
	uint8_t *pbuf;
	uint8_t sta = 0;
    uint32_t i= 0;

	pbuf = pvPortMalloc(secsize*seccnt);
	memset(pbuf, 0, secsize*seccnt);

	sta = FTL_ReadSectors(pbuf, secx, secsize, seccnt); //读取数据

	if(sta == 0)
	{
		DEBUG_PRINTF("\r\nread sec %d data is:\r\n", secx);
		for(i= 0; i<(secsize*seccnt); i++) //准备要读取的数据,填充数据,从0开始增大
		{
			DEBUG_PRINTF("%x ",pbuf[i]); //串口打印读取到的数据
		}
		DEBUG_PRINTF("\r\nend\r\n");
	}
	else if(sta == 1)
	{
		DEBUG_PRINTF("\r\n物理块号大于NAND FLASH的有效总块数!\r\n");
	}
	else if(sta == 2)
	{
		DEBUG_PRINTF("\r\nRead failed!\r\n");
	}

	vPortFree(pbuf); //释放内存

	return sta;
}

