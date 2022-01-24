/*
 * bsp_nand.c
 *
 *  Created on: 2021年9月2日
 *      Author: Administrator
 */


#include "bsp_nand.h"
#ifdef USER_CONFIG
	#include "user_config.h"
#endif


NAND_HandleTypeDef NAND_Handler;    //NAND FLASH句柄
NAND_Attriute_T tNand_dev;           //nand重要参数结构体

/**
  * @brief  延迟一段时间
  * @param  延迟的时间长度
  * @retval None
  */
static void NAND_Delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0;
  for(index = ( nCount); index != 0; index--)
  {
  }
}

//初始化NAND FLASH
uint8_t NAND_Init(void)
{
	uint8_t sta = 0;
    FMC_NAND_PCC_TimingTypeDef ComSpaceTiming,AttSpaceTiming;

    NAND_Handler.Instance = FMC_NAND_DEVICE;
    /* hnand1.Init */
    NAND_Handler.Init.NandBank = FMC_NAND_BANK3;                         //NAND挂在BANK3上
    NAND_Handler.Init.Waitfeature = FMC_NAND_PCC_WAIT_FEATURE_DISABLE;   //关闭等待特性
    NAND_Handler.Init.MemoryDataWidth = FMC_NAND_PCC_MEM_BUS_WIDTH_8;    //8位数据宽度
    NAND_Handler.Init.EccComputation = FMC_NAND_ECC_DISABLE;             //禁止ECC
    NAND_Handler.Init.ECCPageSize = FMC_NAND_ECC_PAGE_SIZE_512BYTE;      //ECC页大小为512字节
    NAND_Handler.Init.TCLRSetupTime = 10;                                 //设置TCLR(tCLR=CLE到RE的延时)=(TCLR+TSET+2)*THCLK,THCLK=1/180M=5.5ns
    NAND_Handler.Init.TARSetupTime = 10;                                  //设置TAR(tAR=ALE到RE的延时)=(TAR+TSET+2)*THCLK,THCLK=1/180M=5.5ns

    ComSpaceTiming.SetupTime = 10;       //建立时间
    ComSpaceTiming.WaitSetupTime = 10;   //等待时间
    ComSpaceTiming.HoldSetupTime = 10;   //保持时间
    ComSpaceTiming.HiZSetupTime = 10;    //高阻态时间

    AttSpaceTiming.SetupTime = 10;       //建立时间
    AttSpaceTiming.WaitSetupTime = 10;   //等待时间
    AttSpaceTiming.HoldSetupTime = 10;   //保持时间
    AttSpaceTiming.HiZSetupTime = 10;    //高阻态时间

    if (HAL_NAND_Init(&NAND_Handler, &ComSpaceTiming, &AttSpaceTiming) != HAL_OK)
    {
    	Error_Handler( );
    }

    sta = NAND_Reset();       		    //复位NAND
    if(sta) DEBUG_PRINTF("复位NAND失败\r\n", tNand_dev.id);
    NAND_Delay(1000);
    tNand_dev.id = NAND_ReadID();	    //读取ID
    DEBUG_PRINTF("NAND ID:%#x\r\n", tNand_dev.id);
    sta = NAND_ModeSet(4);			    //设置为MODE4,高速模式
    if(sta) DEBUG_PRINTF("设置模式失败\r\n", tNand_dev.id);

    if(tNand_dev.id==MT29F4G08ABADA)     //NAND为MT29F4G08ABADA
    {
        tNand_dev.page_totalsize = 2112; //NAND一个page的总大小（包括spare区）
        tNand_dev.page_mainsize  = 2048; //NAND一个page的有效数据区大小
        tNand_dev.page_sparesize = 64;   //NAND一个page的spare区大小
        tNand_dev.block_pagenum  = 64;   //NAND一个block所包含的page数目
        tNand_dev.plane_blocknum = 2048; //NAND一个plane所包含的block数目
        tNand_dev.block_totalnum = 4096; //NAND的总block数目
    }
	else if(tNand_dev.id==W29N01GVSIAA)  //NAND为W29N01GVSIAA
	{
		tNand_dev.page_totalsize=2112;
		tNand_dev.page_mainsize=2048;
		tNand_dev.page_sparesize=64;
		tNand_dev.block_pagenum=64;
		tNand_dev.plane_blocknum=1024;
		tNand_dev.block_totalnum=1024;
	}else if (tNand_dev.id==W29N01HVSINA)
	{
		tNand_dev.page_totalsize=2112;
		tNand_dev.page_mainsize=2048;
		tNand_dev.page_sparesize=64;
		tNand_dev.block_pagenum=64;
		tNand_dev.plane_blocknum=1024;
		tNand_dev.block_totalnum=1024;
	}
    else return 1;	//错误，返回

    tNand_dev.page_totalnum  = tNand_dev.block_pagenum*tNand_dev.block_totalnum-1; //NAND的总page数目，减一是因为page从0开始算起
    tNand_dev.block_totalsectors = tNand_dev.block_pagenum*(tNand_dev.page_mainsize/NAND_ECC_SECTOR_SIZE); //计算每个块的总扇区数量

    return 0;
}

//NAND FALSH底层驱动,引脚配置，时钟使能
//此函数会被HAL_NAND_Init()调用
void HAL_NAND_MspInit(NAND_HandleTypeDef *hnand)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_FMC_CLK_ENABLE();     //使能FMC时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();   //使能GPIOD时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();   //使能GPIOE时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();   //使能GPIOG时钟

	//初始化PB13 R/B引脚
	GPIO_Initure.Pin = GPIO_PIN_13;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;            //输入
    GPIO_Initure.Pull = GPIO_PULLUP;                //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //高速
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);

	//初始化PG9 NCE3引脚
    GPIO_Initure.Pin = GPIO_PIN_9;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;            //输入
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //高速
	GPIO_Initure.Alternate = GPIO_AF12_FMC;         //复用为FMC
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);

    //初始化PD0,1,4,5,11,12,14,15
    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 |\
                       GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_Initure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);

    //初始化PE7,8,9,10
    GPIO_Initure.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);
}

//读取NAND FLASH的ID
//返回值:0,成功;
//    其他,失败
uint8_t NAND_ModeSet(uint8_t mode)
{
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_FEATURE; //发送设置特性命令
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = 0x01;		//地址为0x01,设置mode
 	*(__IO uint8_t*)NAND_ADDRESS = mode;					//P1参数,设置mode
	*(__IO uint8_t*)NAND_ADDRESS = 0;
	*(__IO uint8_t*)NAND_ADDRESS = 0;
	*(__IO uint8_t*)NAND_ADDRESS = 0;
    if(NAND_WaitForReady()==NSTA_READY) return 0; //成功
    else return 1;								  //失败
}

//读取NAND FLASH的ID
//不同的NAND略有不同，请根据自己所使用的NAND FALSH数据手册来编写函数
//返回值:NAND FLASH的ID值
uint32_t NAND_ReadID(void)
{
    uint8_t deviceid[5];
    uint32_t id;
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_READID; //发送读取ID命令
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = 0x00;
	//ID一共有5个字节
    deviceid[0] = *(__IO uint8_t*)NAND_ADDRESS;
    deviceid[1] = *(__IO uint8_t*)NAND_ADDRESS;
    deviceid[2] = *(__IO uint8_t*)NAND_ADDRESS;
    deviceid[3] = *(__IO uint8_t*)NAND_ADDRESS;
    deviceid[4] = *(__IO uint8_t*)NAND_ADDRESS;
    //镁光的NAND FLASH的ID一共5个字节，但是为了方便我们只取4个字节组成一个32位的ID值
    //根据NAND FLASH的数据手册，只要是镁光的NAND FLASH，那么一个字节ID的第一个字节都是0x2C
    //所以我们就可以抛弃这个0x2C，只取后面四字节的ID值。
    id = ((uint32_t)deviceid[1])<<24 | ((uint32_t)deviceid[2])<<16 | ((uint32_t)deviceid[3])<<8 | deviceid[4];
    return id;
}

//读NAND状态
//返回值:NAND状态值
//bit0:0,成功;1,错误(编程/擦除/READ)
//bit6:0,Busy;1,Ready
uint8_t NAND_ReadStatus(void)
{
    __IO uint8_t data = 0;
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_READSTA; //发送读状态命令
	data++;data++;data++;data++;data++;	 //加延时,防止-O2优化,导致的错误.
 	data = *(__IO uint8_t*)NAND_ADDRESS; //读取状态值
    return data;
}

//等待NAND准备好
//返回值:NSTA_TIMEOUT 等待超时了
//      NSTA_READY    已经准备好
uint8_t NAND_WaitForReady(void)
{
    uint8_t status = 0;
    __IO uint32_t time = 0;
	while(1)						//等待ready
	{
		status = NAND_ReadStatus();	//获取状态值
		if(status & NSTA_READY) break;
		time++;
		if(time >= 0x1FFFF) return NSTA_TIMEOUT;//超时
	}
    return NSTA_READY;//准备好
}

//复位NAND
//返回值:0,成功;
//    其他,失败
uint8_t NAND_Reset(void)
{
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_RESET; //复位NAND
    if(NAND_WaitForReady() == NSTA_READY) return 0; //复位成功
    else return 1;                                  //复位失败
}

//等待RB信号为某个电平
//rb:0,等待RB==0
//   1,等待RB==1
//返回值:0,成功
//       1,超时
uint8_t NAND_WaitRB(__IO uint8_t rb)
{
    __IO uint16_t time = 0;
	while(time < 0xFFFF)
	{
		time++;
		if(NAND_RB == rb) return 0;
	}
	return 1;
}

//读取NAND Flash的指定页指定列的数据(main区和spare区都可以使用此函数)
//PageNum:要读取的页地址,范围:0~(block_pagenum*block_totalnum-1)
//ColNum:要读取的列开始地址(也就是页内地址),范围:0~(page_totalsize-1)
//*pBuffer:指向数据存储区
//NumByteToRead:读取字节数(不能跨页读)
//返回值:0,成功
//    其他,错误代码
uint8_t NAND_ReadPage(uint32_t PageNum,uint16_t ColNum,uint8_t *pBuffer,uint16_t NumByteToRead)
{
    __IO uint16_t i = 0;
	uint8_t res = 0;
	uint8_t eccnum = 0;		//需要计算的ECC个数，每NAND_ECC_SECTOR_SIZE字节计算一个ecc
	uint8_t eccstart = 0;	//第一个ECC值所属的地址范围
	uint8_t errsta = 0;
	uint8_t *p;

	/* 区分FLASH是否大于128M */
	if(tNand_dev.page_totalnum <= 65535U)
	{
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_AREA_A;
	    //发送地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>8);
	}
	else
	{
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_AREA_A;
	    //发送地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>16);
	}
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_AREA_TRUE1;

    //下面两行代码是等待R/B引脚变为低电平，其实主要起延时作用的，等待NAND操作R/B引脚。因为我们是通过
    //将STM32的NWAIT引脚(NAND的R/B引脚)配置为普通IO，代码中通过读取NWAIT引脚的电平来判断NAND是否准备
    //就绪的。这个也就是模拟的方法，所以在速度很快的时候有可能NAND还没来得及操作R/B引脚来表示NAND的忙
    //闲状态，结果我们就读取了R/B引脚,这个时候肯定会出错的，事实上确实是会出错!大家也可以将下面两行
    //代码换成延时函数,只不过这里我们为了效率所以没有用延时函数。
	res = NAND_WaitRB(0);	     //等待RB=0
    if(res) return NSTA_TIMEOUT; //超时退出
    //下面2行代码是真正判断NAND是否准备好的
	res = NAND_WaitRB(1);		 //等待RB=1
    if(res) return NSTA_TIMEOUT; //超时退出
	if(NumByteToRead % NAND_ECC_SECTOR_SIZE)//不是NAND_ECC_SECTOR_SIZE的整数倍，不进行ECC校验
	{
		//读取NAND FLASH中的值
		for(i=0; i<NumByteToRead; i++)
		{
			*(__IO uint8_t*)pBuffer++ = *(__IO uint8_t*)NAND_ADDRESS;
		}
	}
	else
	{
		eccnum = NumByteToRead/NAND_ECC_SECTOR_SIZE; //得到ecc计算次数
		eccstart = ColNum/NAND_ECC_SECTOR_SIZE;
		p = pBuffer;
		for(res=0; res<eccnum; res++)
		{
			FMC_NAND_DEVICE->PCR |= 1<<6;          //使能ECC校验
			for(i=0; i<NAND_ECC_SECTOR_SIZE; i++)  //读取NAND_ECC_SECTOR_SIZE个数据
			{
				*(__IO uint8_t*)pBuffer++ = *(__IO uint8_t*)NAND_ADDRESS;
			}
			while(!(FMC_NAND_DEVICE->SR&(1<<6)));                     //等待 FIFO 空
			tNand_dev.ecc_hdbuf[res+eccstart] = FMC_NAND_DEVICE->ECCR; //读取硬件计算后的ECC值
			FMC_NAND_DEVICE->PCR &= ~(1<<6);						  //禁止ECC校验
		}
		i = tNand_dev.page_mainsize+0x10+eccstart*4; //从spare区的0x10位置开始读取之前存储的ecc值
		NAND_Delay(30);//等待tADL
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = 0x05; //随机读指令
		//发送地址
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(i);
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(i>>8);
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = 0xE0; //开始读数据
		NAND_Delay(30);//等待tADL
		pBuffer = (uint8_t*)&tNand_dev.ecc_rdbuf[eccstart];
		for(i=0; i<4*eccnum; i++) //读取保存的ECC值
		{
			*(__IO uint8_t*)pBuffer++= *(__IO uint8_t*)NAND_ADDRESS;
		}
		for(i=0; i<eccnum; i++)								//检验ECC
		{
			if(tNand_dev.ecc_rdbuf[i+eccstart] != tNand_dev.ecc_hdbuf[i+eccstart])//不相等,需要校正
			{
				DEBUG_PRINTF("err hd,rd:0x%x,0x%x\r\n",tNand_dev.ecc_hdbuf[i+eccstart],tNand_dev.ecc_rdbuf[i+eccstart]);
 				DEBUG_PRINTF("eccnum,eccstart:%d,%d\r\n",eccnum,eccstart);
				DEBUG_PRINTF("PageNum,ColNum:%d,%d\r\n",PageNum,ColNum);
				res = NAND_ECC_Correction(p+NAND_ECC_SECTOR_SIZE*i,tNand_dev.ecc_rdbuf[i+eccstart],tNand_dev.ecc_hdbuf[i+eccstart]);//ECC校验
				if(res)errsta = NSTA_ECC2BITERR;			//标记2BIT及以上ECC错误
				else errsta = NSTA_ECC1BITERR;				//标记1BIT ECC错误
			}
		}
	}
    if(NAND_WaitForReady() != NSTA_READY) errsta = NSTA_ERROR;	//失败
    return errsta;	//成功
}

//读取NAND Flash的指定页指定列的数据(main区和spare区都可以使用此函数),并对比(FTL管理时需要)
//PageNum:要读取的页地址,范围:0~(block_pagenum*block_totalnum-1)
//ColNum:要读取的列开始地址(也就是页内地址),范围:0~(page_totalsize-1)
//CmpVal:要对比的值,以uint32_t为单位
//NumByteToRead:读取字数(以4字节为单位,不能跨页读)
//NumByteEqual:从初始位置持续与CmpVal值相同的数据个数
//返回值:0,成功
//    其他,错误代码
uint8_t NAND_ReadPageComp(uint32_t PageNum,uint16_t ColNum,uint32_t CmpVal,uint16_t NumByteToRead,uint16_t *NumByteEqual)
{
    uint16_t i = 0;
	uint8_t res = 0;

	if(tNand_dev.page_totalnum <= 65535U)
	{
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_AREA_A;
	    //发送地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>8);
	}
	else
	{
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_AREA_A;
	    //发送地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>16);
	}
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_AREA_TRUE1;

    //下面两行代码是等待R/B引脚变为低电平，其实主要起延时作用的，等待NAND操作R/B引脚。因为我们是通过
    //将STM32的NWAIT引脚(NAND的R/B引脚)配置为普通IO，代码中通过读取NWAIT引脚的电平来判断NAND是否准备
    //就绪的。这个也就是模拟的方法，所以在速度很快的时候有可能NAND还没来得及操作R/B引脚来表示NAND的忙
    //闲状态，结果我们就读取了R/B引脚,这个时候肯定会出错的，事实上确实是会出错!大家也可以将下面两行
    //代码换成延时函数,只不过这里我们为了效率所以没有用延时函数。
	res = NAND_WaitRB(0);		    //等待RB=0
	if(res)
		return NSTA_TIMEOUT;	//超时退出
    //下面2行代码是真正判断NAND是否准备好的
	res = NAND_WaitRB(1);			//等待RB=1
    if(res)
    	return NSTA_TIMEOUT;	//超时退出
    for(i=0; i<NumByteToRead; i++)  //读取数据,每次读4字节
    {
		if(*(__IO uint32_t*)NAND_ADDRESS != CmpVal) break;	 //如果有任何一个值,与CmpVal不相等,则退出.
    }
	*NumByteEqual = i; //与CmpVal值相同的个数
    if(NAND_WaitForReady() != NSTA_READY) return NSTA_ERROR; //失败
    return 0;	//成功
}

//在NAND一页中写入指定个字节的数据(main区和spare区都可以使用此函数)
//PageNum:要写入的页地址,范围:0~(block_pagenum*block_totalnum-1)
//ColNum:要写入的列开始地址(也就是页内地址),范围:0~(page_totalsize-1)
//pBbuffer:指向数据存储区
//NumByteToWrite:要写入的字节数，该值不能超过该页剩余字节数！！！
//返回值:0,成功
//    其他,错误代码
uint8_t NAND_WritePage(uint32_t PageNum,uint16_t ColNum,uint8_t *pBuffer,uint16_t NumByteToWrite)
{
	__IO uint16_t i = 0;
	uint8_t res = 0;
	uint8_t eccnum = 0;		//需要计算的ECC个数，每NAND_ECC_SECTOR_SIZE字节计算一个ecc
	uint8_t eccstart = 0;	//第一个ECC值所属的地址范围

	if(tNand_dev.page_totalnum <= 65535U)
	{
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_WRITE0;
		//发送地址
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum);
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum>>8);
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum);
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>8);
	}
	else
	{
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_WRITE0;
		//发送地址
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum);
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum>>8);
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum);
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>8);
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>16);
	}
	NAND_Delay(30); //等待 tADL
	if(NumByteToWrite%NAND_ECC_SECTOR_SIZE) //不是 NAND_ECC_SECTOR_SIZE 的整数倍，不进行 ECC 校验
	{
		for(i=0; i<NumByteToWrite; i++) //写入数据
		{
			*(__IO uint8_t*)NAND_ADDRESS = *(__IO uint8_t*)pBuffer++;
		}
	}
	else
	{
		eccnum = NumByteToWrite/NAND_ECC_SECTOR_SIZE; //得到 ecc 计算次数
		eccstart = ColNum/NAND_ECC_SECTOR_SIZE;
		for(res=0; res<eccnum; res++)
		{
			FMC_NAND_DEVICE->PCR |= 1<<6; //使能 ECC 校验
			for(i=0; i<NAND_ECC_SECTOR_SIZE; i++) //写入 NAND_ECC_SECTOR_SIZE 个数据
			{
				*(__IO uint8_t*)NAND_ADDRESS = *(__IO uint8_t*)pBuffer++;
			}
			while(!(FMC_NAND_DEVICE->SR&(1<<6))); //等待 FIFO 空
			tNand_dev.ecc_hdbuf[res+eccstart] = FMC_NAND_DEVICE->ECCR; //读取硬件计算后的 ECC 值
			FMC_NAND_DEVICE->PCR&=~(1<<6); //禁止 ECC 校验
		}
		i = tNand_dev.page_mainsize+0x10+eccstart*4; //计算写入 ECC 的 spare 区地址
		NAND_Delay(30); //等待
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = 0x85; //随机写指令
		//发送地址
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(i);
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(i>>8);
		NAND_Delay(30); //等待 tADL
		pBuffer = (uint8_t*)&tNand_dev.ecc_hdbuf[eccstart];
		for(i=0; i<eccnum; i++) //写入 ECC
		{
			for(res=0; res<4; res++)
			{
				*(__IO uint8_t*)NAND_ADDRESS = *(__IO uint8_t*)pBuffer++;
			}
		}
	}

	*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD)=  NAND_WRITE_TURE1;

	res = NAND_WaitRB(0);	     //等待RB=0
	if(res) return NSTA_TIMEOUT; //超时退出
	//下面2行代码是真正判断NAND是否准备好的
	res = NAND_WaitRB(1);		 //等待RB=1
	if(res) return NSTA_TIMEOUT; //超时退出

	if(NAND_WaitForReady()!=NSTA_READY) return NSTA_ERROR;//失败

	return 0;//成功
}

//在NAND一页中的指定地址开始,写入指定长度的恒定数字
//PageNum:要写入的页地址,范围:0~(block_pagenum*block_totalnum-1)
//ColNum:要写入的列开始地址(也就是页内地址),范围:0~(page_totalsize-1)
//cval:要写入的指定常数
//NumByteToWrite:要写入的字数(以4字节为单位)
//返回值:0,成功
//    其他,错误代码
uint8_t NAND_WritePageConst(uint32_t PageNum,uint16_t ColNum,uint32_t cval,uint16_t NumByteToWrite)
{
	uint8_t res = 0;
    uint16_t i = 0;

	if(tNand_dev.page_totalnum <= 65535U)
	{
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_WRITE0;
	    //发送地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>8);
	}
	else
	{
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_WRITE0;
	    //发送地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(PageNum>>16);
	}
	NAND_Delay(30);//等待tADL
	for(i=0; i<NumByteToWrite; i++)	  //写入数据,每次写4字节
	{
		*(__IO uint32_t*)NAND_ADDRESS = cval;
	}
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_WRITE_TURE1;

	res = NAND_WaitRB(0);	     //等待RB=0
	if(res) return NSTA_TIMEOUT; //超时退出
	//下面2行代码是真正判断NAND是否准备好的
	res = NAND_WaitRB(1);		 //等待RB=1
	if(res) return NSTA_TIMEOUT; //超时退出

    if(NAND_WaitForReady() != NSTA_READY) return NSTA_ERROR;//失败
    return 0;//成功
}

//将一页数据拷贝到另一页,不写入新数据
//注意:原页和目的页要在同一个Plane内！
//Source_PageNo:原页地址,范围:0~(block_pagenum*block_totalnum-1)
//Dest_PageNo:目的页地址,范围:0~(block_pagenum*block_totalnum-1)
//返回值:0,成功
//    其他,错误代码
uint8_t NAND_CopyPageWithoutWrite(uint32_t Source_PageNum,uint32_t Dest_PageNum)
{
	uint8_t res = 0;
    uint16_t source_block = 0,dest_block = 0;
    //判断原页和目的页是否在同一个plane中
    source_block = Source_PageNum/tNand_dev.block_pagenum;
    dest_block = Dest_PageNum/tNand_dev.block_pagenum;

    if(tNand_dev.id == W29N01GVSIAA || tNand_dev.id == W29N01HVSINA) //W29N01GVSIAA只有1个plane
    {
    	__NOP();
    }
    else
    {
    	if((source_block % 2) != (dest_block % 2)) return NSTA_ERROR; //不在同一个plane内
    }

	if(tNand_dev.page_totalnum <= 65535U)
	{
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD0;	//发送命令0x00
	    //发送原页地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Source_PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Source_PageNum>>8);
	}
	else
	{
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD0;	//发送命令0x00
	    //发送原页地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Source_PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Source_PageNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Source_PageNum>>16);
	}
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD1;//发送命令0x35

    //下面两行代码是等待R/B引脚变为低电平，其实主要起延时作用的，等待NAND操作R/B引脚。因为我们是通过
    //将STM32的NWAIT引脚(NAND的R/B引脚)配置为普通IO，代码中通过读取NWAIT引脚的电平来判断NAND是否准备
    //就绪的。这个也就是模拟的方法，所以在速度很快的时候有可能NAND还没来得及操作R/B引脚来表示NAND的忙
    //闲状态，结果我们就读取了R/B引脚,这个时候肯定会出错的，事实上确实是会出错!大家也可以将下面两行
    //代码换成延时函数,只不过这里我们为了效率所以没有用延时函数。
	res = NAND_WaitRB(0);		 //等待RB = 0
	if(res)return NSTA_TIMEOUT;	 //超时退出
    //下面2行代码是真正判断NAND是否准备好的
	res = NAND_WaitRB(1);		 //等待RB=1
    if(res) return NSTA_TIMEOUT; //超时退出
	if(tNand_dev.page_totalnum <= 65535U)
	{
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD2;  //发送命令0x85
	    //发送目的页地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Dest_PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Dest_PageNum>>8);
	}
	else
	{
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD2;  //发送命令0x85
	    //发送目的页地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Dest_PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Dest_PageNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Dest_PageNum>>16);
	}
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD3;	//发送命令0x10

	res = NAND_WaitRB(0);	     //等待RB=0
	if(res) return NSTA_TIMEOUT; //超时退出
	//下面2行代码是真正判断NAND是否准备好的
	res = NAND_WaitRB(1);		 //等待RB=1
	if(res) return NSTA_TIMEOUT; //超时退出

    if(NAND_WaitForReady() != NSTA_READY)
    	return NSTA_ERROR; //NAND未准备好

    return 0;//成功
}

//将一页数据拷贝到另一页,并且可以写入数据
//注意:原页和目的页要在同一个Plane内！
//Source_PageNo:原页地址,范围:0~(block_pagenum*block_totalnum-1)
//Dest_PageNo:目的页地址,范围:0~(block_pagenum*block_totalnum-1)
//ColNo:页内列地址,范围:0~(page_totalsize-1)
//pBuffer:要写入的数据
//NumByteToWrite:要写入的数据个数
//返回值:0,成功
//    其他,错误代码
uint8_t NAND_CopyPageWithWrite(uint32_t Source_PageNum,uint32_t Dest_PageNum,uint16_t ColNum,uint8_t *pBuffer,uint16_t NumByteToWrite)
{
	uint8_t res = 0;
    __IO uint16_t i = 0;
	uint16_t source_block = 0,dest_block = 0;
	uint8_t eccnum = 0;		//需要计算的ECC个数，每NAND_ECC_SECTOR_SIZE字节计算一个ecc
	uint8_t eccstart = 0;	//第一个ECC值所属的地址范围

    //判断原页和目的页是否在同一个plane中
    source_block = Source_PageNum/tNand_dev.block_pagenum;
    dest_block = Dest_PageNum/tNand_dev.block_pagenum;

    if(tNand_dev.id == W29N01GVSIAA  || tNand_dev.id == W29N01HVSINA) //W29N01GVSIAA只有1个plane
    {
    	__NOP();
    }
    else
    {
    	if((source_block % 2) != (dest_block % 2)) return NSTA_ERROR; //不在同一个plane内
    }

	if(tNand_dev.page_totalnum <= 65535U)
	{
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD0; //发送命令0x00
	    //发送原页地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Source_PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Source_PageNum>>8);
	}
	else
	{
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD0; //发送命令0x00
	    //发送原页地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)0;
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Source_PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Source_PageNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Source_PageNum>>16);
	}
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD1;  //发送命令0x35

    //下面两行代码是等待R/B引脚变为低电平，其实主要起延时作用的，等待NAND操作R/B引脚。因为我们是通过
    //将STM32的NWAIT引脚(NAND的R/B引脚)配置为普通IO，代码中通过读取NWAIT引脚的电平来判断NAND是否准备
    //就绪的。这个也就是模拟的方法，所以在速度很快的时候有可能NAND还没来得及操作R/B引脚来表示NAND的忙
    //闲状态，结果我们就读取了R/B引脚,这个时候肯定会出错的，事实上确实是会出错!大家也可以将下面两行
    //代码换成延时函数,只不过这里我们为了效率所以没有用延时函数。
	res = NAND_WaitRB(0);		 //等待RB = 0
	if(res) return NSTA_TIMEOUT; //超时退出
    //下面2行代码是真正判断NAND是否准备好的
	res = NAND_WaitRB(1);		 //等待RB=1
    if(res) return NSTA_TIMEOUT; //超时退出
	if(tNand_dev.page_totalnum <= 65535U)
	{
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD2; //发送命令0x85
	    //发送目的页地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Dest_PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Dest_PageNum>>8);
	}
	else
	{
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD2; //发送命令0x85
	    //发送目的页地址
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(ColNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Dest_PageNum);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Dest_PageNum>>8);
	    *(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(Dest_PageNum>>16);
	}
    //发送页内列地址
	NAND_Delay(30);//等待tADL
	if(NumByteToWrite % NAND_ECC_SECTOR_SIZE)//不是NAND_ECC_SECTOR_SIZE的整数倍，不进行ECC校验
	{
		for(i=0; i<NumByteToWrite; i++)		//写入数据
		{
			*(__IO uint8_t*)NAND_ADDRESS = *(__IO uint8_t*)pBuffer++;
		}
	}
	else
	{
		eccnum = NumByteToWrite/NAND_ECC_SECTOR_SIZE; //得到ecc计算次数
		eccstart = ColNum/NAND_ECC_SECTOR_SIZE;
 		for(res=0; res<eccnum; res++)
		{
			FMC_NAND_DEVICE->PCR |= 1<<6;			  //使能ECC校验
			for(i=0; i<NAND_ECC_SECTOR_SIZE; i++)	  //写入NAND_ECC_SECTOR_SIZE个数据
			{
				*(__IO uint8_t*)NAND_ADDRESS = *(__IO uint8_t*)pBuffer++;
			}
			while(!(FMC_NAND_DEVICE->SR&(1<<6)));					   //等待FIFO空
			tNand_dev.ecc_hdbuf[res+eccstart] = FMC_NAND_DEVICE->ECCR; //读取硬件计算后的ECC值
 			FMC_NAND_DEVICE->PCR &= ~(1<<6);						   //禁止ECC校验
		}
		i = tNand_dev.page_mainsize+0x10+eccstart*4; //计算写入ECC的spare区地址
		NAND_Delay(30); //等待
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = 0x85; //随机写指令
		//发送地址
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)i;
		*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(i>>8);
		NAND_Delay(30); //等待tADL
		pBuffer=(uint8_t*)&tNand_dev.ecc_hdbuf[eccstart];
		for(i=0; i<eccnum; i++) //写入ECC
		{
			for(res=0; res<4; res++)
			{
				*(__IO uint8_t*)NAND_ADDRESS = *(__IO uint8_t*)pBuffer++;
			}
		}
	}
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_MOVEDATA_CMD3; //发送命令0x10

	res = NAND_WaitRB(0);	     //等待RB=0
	if(res) return NSTA_TIMEOUT; //超时退出
	//下面2行代码是真正判断NAND是否准备好的
	res = NAND_WaitRB(1);		 //等待RB=1
	if(res) return NSTA_TIMEOUT; //超时退出

    if(NAND_WaitForReady() != NSTA_READY)
    	return NSTA_ERROR; //失败

    return 0;	//成功
}

//读取spare区中的数据
//PageNum:要写入的页地址,范围:0~(block_pagenum*block_totalnum-1)
//ColNum:要写入的spare区地址(spare区中哪个地址),范围:0~(page_sparesize-1)
//pBuffer:接收数据缓冲区
//NumByteToRead:要读取的字节数(不大于page_sparesize)
//返回值:0,成功
//    其他,错误代码
uint8_t NAND_ReadSpare(uint32_t PageNum,uint16_t ColNum,uint8_t *pBuffer,uint16_t NumByteToRead)
{
    uint8_t temp = 0;
    uint8_t remainbyte = 0;
    remainbyte = tNand_dev.page_sparesize-ColNum;
    if(NumByteToRead > remainbyte) NumByteToRead=remainbyte; //确保要读取的字节数不大于spare剩余的大小
    temp = NAND_ReadPage(PageNum, ColNum+tNand_dev.page_mainsize, pBuffer, NumByteToRead); //读取数据
    return temp;
}

//向spare区中写数据
//PageNum:要写入的页地址,范围:0~(block_pagenum*block_totalnum-1)
//ColNum:要写入的spare区地址(spare区中哪个地址),范围:0~(page_sparesize-1)
//pBuffer:要写入的数据首地址
//NumByteToWrite:要写入的字节数(不大于page_sparesize)
//返回值:0,成功
//    其他,失败
uint8_t NAND_WriteSpare(uint32_t PageNum,uint16_t ColNum,uint8_t *pBuffer,uint16_t NumByteToWrite)
{
    uint8_t temp = 0;
    uint8_t remainbyte = 0;
    remainbyte = tNand_dev.page_sparesize-ColNum;
    if(NumByteToWrite > remainbyte) NumByteToWrite=remainbyte; //确保要写入 的字节数不大于spare剩余的大小
    temp = NAND_WritePage(PageNum, ColNum+tNand_dev.page_mainsize, pBuffer, NumByteToWrite); //写入数据
    return temp;
}

//擦除一个块
//BlockNum:要擦除的BLOCK编号,范围:0-(block_totalnum-1)
//返回值:0,擦除成功
//    其他,擦除失败
uint8_t NAND_EraseBlock(uint32_t BlockNum)
{
	uint8_t res = 0;

	//将块地址转换为页地址
	if(tNand_dev.id == MT29F16G08ABABA)     BlockNum<<=7;
    else if(tNand_dev.id == MT29F4G08ABADA) BlockNum<<=6; //块的数量乘于64
    else if(tNand_dev.id == W29N01GVSIAA)   BlockNum<<=6; //块的数量乘于64
    else if(tNand_dev.id == W29N01HVSINA)   BlockNum<<=6; //块的数量乘于64

	*(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_ERASE0;
	//发送块地址
	*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(BlockNum);
	*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(BlockNum>>8);
	*(__IO uint8_t*)(NAND_ADDRESS|NAND_ADDR) = (uint8_t)(BlockNum>>16);
    *(__IO uint8_t*)(NAND_ADDRESS|NAND_CMD) = NAND_ERASE1;

	res = NAND_WaitRB(0);	     //等待RB=0
	if(res) return NSTA_TIMEOUT; //超时退出
	//下面2行代码是真正判断NAND是否准备好的
	res = NAND_WaitRB(1);		 //等待RB=1
	if(res) return NSTA_TIMEOUT; //超时退出

	if(NAND_WaitForReady() != NSTA_READY) return NSTA_ERROR;//失败
    return 0;	//成功
}

//全片擦除NAND FLASH
void NAND_EraseChip(void)
{
    uint8_t status = 0;
    uint16_t i = 0;
    for(i = 0; i<tNand_dev.block_totalnum; i++) //循环擦除所有的块
    {
        status = NAND_EraseBlock(i);
        if(status) DEBUG_PRINTF("Erase %d block fail!!，错误码为%d\r\n", i, status);//擦除失败
    }
}

//获取ECC的奇数位/偶数位
//oe:0,偶数位
//   1,奇数位
//eccval:输入的ecc值
//返回值:计算后的ecc值(最多16位)
uint16_t NAND_ECC_Get_OE(uint8_t oe,uint32_t eccval)
{
	uint8_t i;
	uint16_t ecctemp = 0;
	for(i=0; i<24; i++)
	{
		if((i%2)==oe)
		{
			if((eccval>>i)&0x01) ecctemp += 1<<(i>>1);
		}
	}
	return ecctemp;
}

//ECC校正函数
//eccrd:读取出来,原来保存的ECC值
//ecccl:读取数据时,硬件计算的ECC只
//返回值:0,错误已修正
//    其他,ECC错误(有大于2个bit的错误,无法恢复)
uint8_t NAND_ECC_Correction(uint8_t* data_buf,uint32_t eccrd,uint32_t ecccl)
{
	uint16_t eccrdo, eccrde, eccclo, ecccle;
	uint16_t eccchk = 0;
	uint16_t errorpos = 0;
	uint32_t bytepos = 0;
	eccrdo = NAND_ECC_Get_OE(1,eccrd);	//获取eccrd的奇数位
	eccrde = NAND_ECC_Get_OE(0,eccrd);	//获取eccrd的偶数位
	eccclo = NAND_ECC_Get_OE(1,ecccl);	//获取ecccl的奇数位
	ecccle = NAND_ECC_Get_OE(0,ecccl); 	//获取ecccl的偶数位
	eccchk = eccrdo^eccrde^eccclo^ecccle;
	if(eccchk == 0xFFF)	//全1,说明只有1bit ECC错误
	{
		errorpos = eccrdo^eccclo;
		DEBUG_PRINTF("errorpos:%d\r\n",errorpos);
		bytepos = errorpos/8;
		data_buf[bytepos]^=1<<(errorpos%8);
	}else				//不是全1,说明至少有2bit ECC错误,无法修复
	{
		DEBUG_PRINTF("2bit ecc error or more\r\n");
		return 1;
	}
	return 0;
}
