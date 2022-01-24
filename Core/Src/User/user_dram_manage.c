
#ifdef USER_CONFIG
	#include "user_config.h"
#endif
#include "user_dram_manage.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef __OS_ENABLE
#define	MyDramQueue_API_CREATELOCK(v)		do{(v)->lock = xSemaphoreCreateMutex();assert((v)->lock);}while(0);
#define MyDramQueue_API_DELETELOCK(v)		do{((v)->lock!=NULL) ? vSemaphoreDelete((v)->lock) : pdFAIL;}while(0);
#define	MyDramQueue_API_LOCK(v)				do{((v)->lock!=NULL) ? xSemaphoreTake((v)->lock, portMAX_DELAY) : pdFAIL;}while(0);
#define MyDramQueue_API_UNLOCK(v)			do{((v)->lock!=NULL) ? xSemaphoreGive((v)->lock) : pdFAIL;}while(0);
#else
#define	MyDramQueue_API_CREATELOCK(v)
#define MyDramQueue_API_DELETELOCK(v)
#define	MyDramQueue_API_LOCK(v)
#define MyDramQueue_API_UNLOCK(v)
#endif

/* 队列结构 */
struct myDramQueue
{
	void                *buffer;        //数据缓冲区
	size_t              len;            //队列长度
	size_t              size;           //单个数据大小(单位 字节)
	size_t              front;          //数据头,指向下一个空闲存放地址
	size_t              rear;           //数据尾，指向第一个数据
#ifdef __OS_ENABLE
	SemaphoreHandle_t   lock;           //保护锁
#endif
};

static myDramQueueHandle_t nandSaveFIFO;


/* 目前队列中已经缓存的数据长度 */
#define NUM_IN_QUEUE(v)					((((v)->front) <= ((v)->rear))?(((v)->rear)-((v)->front)):(((v)->len)-((v)->front)+((v)->rear)))
/* 获取队列剩余可存放数据个数 */
#define LFET_NUM_IN_QUEUE(v)			((((v)->front) <= ((v)->rear))?((((v)->len)-1)-(((v)->rear)-((v)->front))):(((v)->front)-((v)->rear)-1))


/**
 * @name  : myDramQueueCreate
 * @brief : 创建队列：申请队列句柄、申请队列内存
 * @param : queue_len ：队列长度
 *          item_size ：单个数据大小(单位 字节)
 * @retval: 队列句柄
 * @date  : 2021/09/24
 * @note  :
 */
myDramQueueHandle_t myDramQueueCreate(size_t queue_len,size_t item_size)
{
	if((0==queue_len) || (0==item_size))
	{
		DEBUG_PRINTF("create queue,len=%d,ItemSize=%d",queue_len,item_size);
		return NULL;
	}
	myDramQueueHandle_t queue = mymalloc(SDRAM_POOL, sizeof(struct myDramQueue));  //申请一个队列结构所需要的内存，并将queue指向该内存
	assert(queue);  //申请失败会有断言
	queue->size = item_size;  //队列长度
	queue->len = queue_len;   //单个数据大小(单位 字节)
	queue->buffer = mymalloc(SDRAM_POOL, (queue->size*queue->len));  //申请内存
	assert(queue->buffer);  //申请失败会有断言
	queue->front = queue->rear = 0;  //初始化头尾
	MyDramQueue_API_CREATELOCK(queue);   //创建互斥量
	MyDramQueue_API_UNLOCK(queue);  //解锁
	return queue;
}

/**
 * @name  : myDramQueueDelete
 * @brief : 删除队列：释放队列内存、释放队列句柄内存
 * @param : queue ：队列句柄
 * @retval:
 * @date  : 2021/09/24
 * @note  :
 */
void myDramQueueDelete(myDramQueueHandle_t queue)
{
	if(NULL == queue)
	{
		return;
	}
	MyDramQueue_API_LOCK(queue);  //上锁
	myfree(SDRAM_POOL,queue->buffer);      //释放队列内存
	MyDramQueue_API_UNLOCK(queue);//解锁
	MyDramQueue_API_DELETELOCK(queue);  //删除互斥量
	myfree(SDRAM_POOL,queue);  //释放队列句柄内存
}

/**
  * @brief	获取队列已存放数据个数
  * @param	queue：队列句柄
  *
  * @return	size_t:队列数据个数
  * @remark
  */
static size_t myDramQueueNum(const myDramQueueHandle_t queue)
{
	if(NULL == queue)
	{
		return 0;
	}
	MyDramQueue_API_LOCK(queue);
	size_t num_in_queue = NUM_IN_QUEUE(queue);
	MyDramQueue_API_UNLOCK(queue);
	return num_in_queue;
}

/**
  * @brief	获取队列剩余可存放数据个数
  * @param	queue：队列句柄
  *
  * @return	size_t:队列剩余可存放数据个数
  * @remark
  */
size_t myDramQueueLeftNum(const myDramQueueHandle_t queue)
{
	if(NULL == queue)
	{
		return 0;
	}
	MyDramQueue_API_LOCK(queue);
	size_t left_num_in_queue = LFET_NUM_IN_QUEUE(queue);
	MyDramQueue_API_UNLOCK(queue);
	return left_num_in_queue;
}




static  __attribute__((always_inline)) inline size_t minn(size_t a, size_t b)
{
	return a<b ? a : b;
}

/**
  * @brief	：指定队列内存拷贝
  * @param	：dst 目标地址
  *           src 源地址
  *           n   拷贝长度
  * @return
  * @remark
  */
static void *myDramQueue_memcpy(void *dst, const void *src, size_t n)
{
	return memcpy(dst,src,n);
}

/**
  * @brief	将指定个数的数据放入队列
  * @param	queue：队列句柄
  * @param	*buf：数据指针
  * @param	num：数据个数
  *
  * @return	uint8_t：是否放入成功   0 、放入成功   1、初始化异常  2、写入数据大于剩余空闲长度
  * @remark
  */
static uint8_t myDramQueuePut(myDramQueueHandle_t queue,const void *buf,size_t num)
{
	/* 错误参数检测 */
	if((NULL==queue) || (NULL==queue->buffer) || (NULL==buf) || (0==num))
	{
		DEBUG_PRINTF("put queue=%p,buf=%p,num=%d", queue, buf, num);
		return 1;
	}
	uint8_t rt = 2;  //入队是否成功标志
	MyDramQueue_API_LOCK(queue);  //上锁
	size_t left_num_in_queue = LFET_NUM_IN_QUEUE(queue);  //获取队列剩余可存放数据个数
	if (num <= left_num_in_queue)  //只有放入数据的长度小于队列中剩余数据个数才可执行入队操作
	{
		/* 以下是按照队列是否回环入队分情况 */
		size_t templen = minn((queue->len) - (queue->rear), num);  //队尾到队列结束的长度与本次入队长度中的最小值
		/* 由于最小值≤队尾到队列结束的长度，因此不用担心野指针 */
		if (templen > 0)
		{
			myDramQueue_memcpy((char *)(queue->buffer) + (queue->rear)*(queue->size),
							   buf,
							   templen*(queue->size));
		}
		/* 如果本次入队长度大于队尾到队列结束的长度，说明需要回环入队 */
		if (num > templen)
		{
			myDramQueue_memcpy((char *)(queue->buffer),
							   (char *)buf + templen*(queue->size),
							   (num - templen)*(queue->size));
		}
		queue->rear = (queue->rear + num) % (queue->len);  //更新队尾
		rt = 0;
	}else
	{
		DEBUG_PRINTF("put failed, num=%d,left=%d,front=%d,rear=%d,len=%d", num, left_num_in_queue, queue->front, queue->rear, queue->len);
	}
	MyDramQueue_API_UNLOCK(queue);  //解锁
	return rt;
}

/**
  * @brief	从队列中取出指定个数的数据
  * @param	queue：队列句柄
  * @param	*buf：数据指针
  * @param	num：取出的队列个数
  *
  * @return	uint8_t：是否取出成功     0、 读出成功   1、初始化异常  2、读出数据大于当前已经存储的数据长度
  * @remark
  */
static uint8_t myDramQueueGet(myDramQueueHandle_t queue, void *buf, size_t num)
{
	if((NULL==queue) || (NULL==queue->buffer) || (NULL==buf) || (0==num))
	{
		DEBUG_PRINTF("get queue=%p,buf=%p,num=%d", queue, buf, num);
		return 1;
	}
	uint8_t rt = 1;
	MyDramQueue_API_LOCK(queue);  //上锁
	size_t num_in_queue = NUM_IN_QUEUE(queue);  //目前队列中已缓存的数据长度
	if (num <= num_in_queue)
	{
		/* 以下是按照队列是否回环出队分情况 */
		size_t templen = minn((queue->len) - (queue->front), num);
		if (templen > 0)
		{
			myDramQueue_memcpy((char *)buf,
							   (char *)(queue->buffer) + (queue->front)*(queue->size),
							   templen*(queue->size));
		}
		if (num > templen)
		{
			myDramQueue_memcpy((char *)buf + templen*(queue->size),
							   queue->buffer,
							   (num - templen)*(queue->size));
		}
		queue->front = (queue->front + num) % (queue->len);
		rt = 0;
	}else
	{
		DEBUG_PRINTF("get failed, num=%d,inQueue=%d,front=%d,rear=%d,len=%d", num, num_in_queue, queue->front, queue->rear, queue->len);
	}
	MyDramQueue_API_UNLOCK(queue);  //解锁
	return rt;
}

/**
  * @brief	弹出指定个数的数据
  * @param	queue：队列句柄
  * @param	num：长度
  *
  * @return	uint8_t：是否弹出成功
  * @remark
  */
uint8_t myDramQueuePop(myDramQueueHandle_t queue, size_t num)
{
	if((NULL==queue) || (0==num))
	{
		DEBUG_PRINTF("pop queue=%p,num=%d", queue, num);
		return 1;
	}
	uint8_t rt = 0;
	MyDramQueue_API_LOCK(queue);
	size_t num_in_queue = NUM_IN_QUEUE(queue);
	if(num <= num_in_queue)
	{
		queue->front = (queue->front + num) % (queue->len);
	}else
	{
		rt = 1;
		DEBUG_PRINTF("pop failed, num=%d,inQueue=%d,front=%d,rear=%d,len=%d", num,num_in_queue, queue->front, queue->rear, queue->len);
	}
	MyDramQueue_API_UNLOCK(queue);
	return rt;
}


/**
  * @brief	从队列中取出指定个数的数据并写入NAND FLASH
  * @param	queue：队列句柄
  * @param	fileName：写入的文件名称
  * @param	num：写入FLASH的队列个数
  *
  * @return	uint8_t：是否取出成功   0、取出成功   1、 初始化异常  2、写入FLASH异常  3、SDRAM当前存储的数据不足以读出
  * @remark
  */
uint8_t myDramQueueToNand(BTE_File_Name_E fileName, myDramQueueHandle_t queue, size_t num)
{
	uint8_t rt = 0, res = 0;
	uint32_t wLen = 0, wLenTemp = 0;
	if((NULL==queue) || (NULL==queue->buffer) || (0==num))
	{
		DEBUG_PRINTF("get queue=%p,num=%d", queue, num);
		return 1;
	}
	MyDramQueue_API_LOCK(queue);  //上锁
	size_t num_in_queue = NUM_IN_QUEUE(queue);  //目前队列中已缓存的数据长度
	if (num <= num_in_queue)
	{
		/* 以下是按照队列是否回环出队分情况 */
		size_t templen = minn((queue->len) - (queue->front), num);
		if (templen > 0)
		{
			res = NAND_Write_File(fileName,
					             (char *)(queue->buffer) + (queue->front)*(queue->size),
								 templen*(queue->size),
							     &wLenTemp);
			wLen += wLenTemp; //累计写入的数据长度 bytes
		}
		if (num > templen)
		{
			res = NAND_Write_File(fileName,
					              queue->buffer,
								  (num - templen)*(queue->size),
							      &wLenTemp);
			wLen += wLenTemp; //累计写入的数据长度 bytes
		}
		/* 校验写入FLASH的数据个数是否准确 */
		if(res != FR_OK || wLen != num*(queue->size))
		{
			rt = 2;
		}
		else
		{
			queue->front = (queue->front + num) % (queue->len);
			rt = 0;
		}
	}else
	{
		rt = 3;
		DEBUG_PRINTF("get failed, num=%d,inQueue=%d,front=%d,rear=%d,len=%d", num, num_in_queue, queue->front, queue->rear, queue->len);
	}
	MyDramQueue_API_UNLOCK(queue);  //解锁
	return rt;
}


/**
  * @brief	掉电时使用记录数据存：将当前DRAM中所有的数据写入NAND FLASH
  * @param	queue：队列句柄
  *
  * @return	uint8_t：是否写入成功   0、写入成功   1、 初始化异常  2、写入FLASH异常
  * @remark
  */
uint8_t uRecordDataSaveLose(void)
{
	uint8_t rt = 0;
	/* 判断内存是否已经初始化(SDRAM & NAND FLASH) */
	if(tNand_dev.lut == NULL || nandSaveFIFO->buffer == NULL)
		return 1;

	/* 将DRAM中所有的数据写入NAND FALSH */
	rt = myDramQueueToNand(NAND_HISTORYRECORDINFO, nandSaveFIFO, (myDramQueueNum(nandSaveFIFO)*nandSaveFIFO->size));
	if(rt != 0 ) return 2;

	return rt;
}


/**
  * @brief	记录数据存:先入SDRAM，当SDRAM存储大于128K时存入NAND FLASH
  * @param	*buf：数据指针
  * @param	num：写入的队列个数
  *
  * @return	uint8_t：是否写入成功   0、写入成功   1、 初始化异常  2、写入FLASH异常
  * @remark
  */
uint8_t uRecordDataSave(const void *buf, size_t num)
{
	uint8_t rt = 0;
	rt = myDramQueuePut(nandSaveFIFO, buf, num);
	/* 判断内存是否已经初始化(SDRAM & NAND FLASH) */
	if(tNand_dev.lut == NULL || rt == 1)
		return 1;
	/* 判断目前已经存储的单元个数是否大于一个 NAND FALSH BLOCK */
	/* 若SDRAM中存储长度大于一个FLASH块的大小则从DRAM队列中读出并写入FLASH */
	while((myDramQueueNum(nandSaveFIFO)*nandSaveFIFO->size) >= (tNand_dev.block_pagenum*tNand_dev.page_mainsize))  //
	{
		rt = myDramQueueToNand(NAND_HISTORYRECORDINFO, nandSaveFIFO, (tNand_dev.block_pagenum*tNand_dev.page_mainsize));
		if(rt != 0 ) return 2;
	}
	return rt;
}


#pragma  pack(push)  	//保存对齐状态
#pragma  pack(1)		//改为1字节对齐，防止内存对齐导致的错误

typedef struct
{
	uint16_t header;         //包头
	uint32_t dataLen;        //数据长度
	uint8_t deviceID;        //设备ID
	uint16_t cmdFunc;        //功能码
	/* 数据域start... */
	uint32_t chTotalDataNum; //记录数据个数
	 //此处是不定长的记录单元...
	/* 数据域end... */
	uint16_t checkSum;       //累加和
	uint16_t tail;           //包尾
}PackageFrameHeader_T;

typedef union
{
	PackageFrameHeader_T frame;
	uint8_t data[sizeof(PackageFrameHeader_T)];
}PackageFrameHeader_U;

#pragma pack(pop)


/* 获取结构体成员在结构体中的位置：member 成员名称   type 结构体名称 */
#define MemberSiteOfPackage(type, member)  ((uint32_t)(&((type *)NULL)->member))


static uint16_t myCumsum(uint8_t *buf, uint32_t size)
{
	 uint16_t cumsum = 0;
	 for(uint32_t i = 0; i < size; i++)
		 cumsum += buf[i];
	 return cumsum;
}


/**
  * @brief	收到获取记录数据命令后调用：提取记录数据、计算长度、计算校验和、封包等
  * @param	queue：队列句柄
  * @param	*wl：提取长度
  * @param	eCmd：指令功能码
  *
  * @return	uint8_t：是否取出成功   0、取出成功   1、初始化异常  2、读出FLASH异常  3、SDRAM读出异常  4、没有记录数据
  * @remark
  */
uint8_t uRecordDataPack(UpperControlCmd_E eCmd, QueueHandle_T * pQueue, uint32_t *wl)
{
	uint8_t res = 0;
	uint8_t completeFlag = 0; //封包完成标志
    PackageFrameHeader_U recordDataFrame;  //包框架
    uint32_t OneSize = 0;  //写入队列单条数据大小
	uint32_t nandSize = 0;  //NAND FLASH中存储的字节数
	uint32_t dramSize = 0;  //SDRAM中存储的单元数
	uint32_t freeSize = 0;  //队列单元中空闲（未存储数据）的数据长度
	uint32_t rsize = 0;     //本次读出FLASH的数据长度
    uint32_t chTotalDataNumReal = 0;  //本次需要打包的记录数据的总字节数
    My_File_Format_T * pFile = &nandDiskHandle.nandFile.historyRecordInfo;  //NAND FLASH记录文件
    uint32_t wSize = 0;   //写入队列的次数
    uint32_t wLen = 0;    //写入队列的长度
    uint32_t tempLen = 0; //临时长度
    uint32_t rDataLen = 1000*UPPER_PC_BUF_MAX_LEN; //要一次处理的数据大小
    uint8_t *pBuf = mymalloc(SDRAM_POOL, rDataLen); //申请临时内存
//    uint8_t *pBuf = pvPortMalloc(rDataLen); //申请临时内存
    memset(pBuf, 0, rDataLen);
    *wl = 0;  //防止初始值非0

    /* 指针无效 */
	if(pFile == NULL || pBuf == NULL ||nandSaveFIFO->buffer == NULL)
	{
		res = 1; goto ret;
	}

	/* 获取已经存储的记录字节数（FLASH + SDRAM） */
	chTotalDataNumReal = mf_fileSize_get(pFile); //获取NAND FLASH中存储的字节个数
	chTotalDataNumReal += myDramQueueNum(nandSaveFIFO)*nandSaveFIFO->size; //加上SDRAM中新缓存的字节个数

	/* 包框架初始化 */
	recordDataFrame.frame.header = UPPER_FRAME_HEAD;
	recordDataFrame.frame.dataLen = (uint32_t)sizeof(recordDataFrame.frame) -
									(uint32_t)sizeof(recordDataFrame.frame.header) -
									(uint32_t)sizeof(recordDataFrame.frame.tail) +
									chTotalDataNumReal;
	recordDataFrame.frame.deviceID = tDeviceInfo.deviceID;  //设备ID
	recordDataFrame.frame.cmdFunc = eCmd;
	recordDataFrame.frame.checkSum = 0;
	recordDataFrame.frame.tail = UPPER_FRAME_END;

	/* 计算通道总量 */
	recordDataFrame.frame.chTotalDataNum = chTotalDataNumReal / sizeof(ChannelRealTimeData_T);

	//TODO:如果没有记录数据，则反馈数据长度为0的数据包
//	if(recordDataFrame.frame.chTotalDataNum == 0)  //如果没有记录数据
//	{
//		res = 4; goto ret;
//	}

	/* 封包拷贝 */
	/* 包头单独拷贝因为后面的需要计算校验和 */
	for(; OneSize < MemberSiteOfPackage(PackageFrameHeader_T, dataLen); OneSize++)
	{
		pBuf[OneSize] = recordDataFrame.data[OneSize];
	}

	/* 拷贝包头-》记录条目数 */
	for(; OneSize < MemberSiteOfPackage(PackageFrameHeader_T, checkSum); OneSize++)
	{
		pBuf[OneSize] = recordDataFrame.data[OneSize];
		recordDataFrame.frame.checkSum += pBuf[OneSize];
	}

	/* 拷入记录数据：最大不大于队列的最大单元长度 */
	while((OneSize < rDataLen && chTotalDataNumReal != 0) ||
		  (chTotalDataNumReal == 0 && completeFlag == 0))
	{
		nandSize = mf_fileSize_get(pFile);  //获取NAND FLASH中存储的字节数
		freeSize = rDataLen - OneSize;  //计算出还能存放几个字节
		if(chTotalDataNumReal < freeSize)
			freeSize = chTotalDataNumReal;
		/* 如果FLASH中的数据未全部取出且长度大于当前队列的剩余长度 */
		if(nandSize != 0 && nandSize > freeSize)
		{
			/* 读FLASH数据 */
			res = NAND_Read_File(NAND_HISTORYRECORDINFO,
					             &pBuf[OneSize],
					             freeSize,
								 &rsize);
			if(rsize == freeSize)  //读出成功
			{
				recordDataFrame.frame.checkSum += myCumsum(&pBuf[OneSize], rsize);
				OneSize += rsize;
				chTotalDataNumReal -= freeSize;  //计数更新
			}
			else
			{
				res = 2;
			}
		}
		/* 如果FLASH中的数据未全部取出且长度小于当前队列的剩余长度 */
		else if(nandSize != 0 && nandSize <= freeSize)
		{
			/* 读FLASH数据 */
			res = NAND_Read_File(NAND_HISTORYRECORDINFO,
					             &pBuf[OneSize],
					             nandSize,
								 &rsize);
			if(rsize == nandSize)
			{
				recordDataFrame.frame.checkSum += myCumsum(&pBuf[OneSize], rsize);
				OneSize += rsize;
				chTotalDataNumReal -= nandSize;  //计数更新
				//TODO：后续成熟了没必要每次读出文件都保存参数，而使用掉电前保存即可
				mf_format_save(&nandDiskHandle.nandFile.historyRecordInfo,"1:bte/mid/HistoryRecordInfo.text");
			}
			else
			{
				res = 2;
			}
		}
		/* 只剩下FLASH中的数据全部被取出的情况 */
		else
		{
			dramSize = freeSize/nandSaveFIFO->size;
			res = myDramQueueGet(nandSaveFIFO, &pBuf[OneSize], dramSize);
			if(0 == res)
			{
				recordDataFrame.frame.checkSum += myCumsum(&pBuf[OneSize], freeSize);
				OneSize += freeSize;
				chTotalDataNumReal -= freeSize;  //计数更新
			}
			else
			{
				res = 3;
			}
		}

		/* 最后是包尾封装 */
		if(chTotalDataNumReal == 0 &&
		  ((rDataLen - OneSize) >= (sizeof(recordDataFrame.frame.checkSum) + sizeof(recordDataFrame.frame.tail))))
		{
			uint16_t memberSite = MemberSiteOfPackage(PackageFrameHeader_T, checkSum);
			/* 拷贝校验和包尾 */
			for(uint8_t i = 0; i < 4; i++,OneSize++)
			{
				pBuf[OneSize] = recordDataFrame.data[memberSite + i];
			}

			completeFlag = 1;
		}

		/* buf封装完则入队
		 * 数据是否入以太网发送队：
		 * 1. 数据提取完、还没达到队列长度：入队
		 * 2. 数据没提取完、达到队列长度：入队
		 * 3. 其他：不入队
		 * 结论：数据提取完 || 队列单元满
		 */
		if(chTotalDataNumReal == 0 || (rDataLen - OneSize) == 0)
		{
#if 1
		    wSize = (OneSize/UPPER_PC_BUF_MAX_LEN + ((OneSize%UPPER_PC_BUF_MAX_LEN) ? 1:0)); //计算写入队列次数

		    //写入环形队列处理，队列满了则等待
		    for(uint32_t i=0; i<wSize; i++)
		    {
		    	tempLen = (i*UPPER_PC_BUF_MAX_LEN);
		    	wLen = OneSize-tempLen;
		    	if(wLen > UPPER_PC_BUF_MAX_LEN) wLen=UPPER_PC_BUF_MAX_LEN;
				do{
					res = uQueueWriteNoCover(pQueue, (uint8_t *)(pBuf+tempLen), (uint16_t)wLen);
					if(res == QUEUE_ERROR)
						return 1;
					if(res == QUEUE_ISFULL)
						vTaskDelay(1);
				} while(res == QUEUE_ISFULL); //1ms阻塞防止线程优先级过高永久阻塞导致队列无法出队
		    }
#else
		    wSize = (OneSize/(UPPER_PC_BUF_MAX_LEN*10) + ((OneSize%(UPPER_PC_BUF_MAX_LEN*10)) ? 1:0)); //计算写入队列次数

		    //不入队列，直接发送
		    for(uint32_t i=0; i<wSize; i++)
		    {
		    	tempLen = (i*(UPPER_PC_BUF_MAX_LEN*10));
		    	wLen = OneSize-tempLen;
		    	if(wLen > (UPPER_PC_BUF_MAX_LEN*10)) wLen=(UPPER_PC_BUF_MAX_LEN*10);

				if(TCP_Netconn != NULL)
				{
					netconn_write(TCP_Netconn, (uint8_t *)(pBuf+tempLen), wLen, NETCONN_COPY); //发送数据

					for(uint8_t a=0; a<3; a++)
					{
						tCommPro.Tcp_Timeout_MS[a] = 0; //超时时间清零
						tCommPro.Tcp_Timeout_S[a] = 0; //超时时间清零
					}
				}
				vTaskDelay(1);
		    }

			res = 0;
#endif

			*wl += OneSize; //更新写入长度
			OneSize = 0; //入队后队列长度复位
		}
	}

ret:
	myfree(SDRAM_POOL, pBuf);  //释放内存
//	vPortFree(pBuf);
	return res;
}


/**
  * @brief	工步记录数据内存管理初始化
  * @return	uint8_t：是否初始化成功   0、成功   1、初始化异常
  * @remark
  */
uint8_t uRecordDataMemoryInit(void)
{
    nandSaveFIFO = myDramQueueCreate(RECORD_SAVE_DRAM_LEN, RECORD_SAVE_DRAM_SIZE);  //创建SDRAM存储队列
    if(nandSaveFIFO == NULL) return 1;
    return 0;
}
