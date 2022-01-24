/*-----------------------------------------------------------------------------
 * @file        : user_circle_queue.c
 * @author      : Lyric-L
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2021/10/06
 * @date changed: 2021/10/06
 * @brief       : 环形队列
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#include "user_circle_queue.h"


/*-----------------------------------------------------------------------------
 * @name  : pQueueMemcpy
 * @brief : 指定队列内存拷贝
 * @param : dst 目标地址
 *          src 源地址
 *          n   拷贝长度
 * @retval:
 * @date  : 2021/10/06
 * @note  :
 * ---------------------------------------------------------------------------*/
static void *pQueueMemcpy(void *dst, const void *src, size_t n)
{
	return memcpy(dst, src, n);
}

/*-----------------------------------------------------------------------------
 * @name  : pQueueMemset
 * @brief : 将某一块内存中的内容全部设置为指定的值
 * @param : dst 目标地址
 *          src 源地址
 *          n   拷贝长度
 * @retval:
 * @date  : 2021/10/06
 * @note  :
 * ---------------------------------------------------------------------------*/
static void *pQueueMemset(void *dst, int data, size_t n)
{
	return memset(dst, data, n);
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueInit
 * @brief : FIFO初始化
 * @param : pQueue：缓存区结构体句柄
 *          OneSize：单条数据大小
*           MaxCnt：限制缓冲区总数据容量(条数)
 * @retval: QUEUE_OK 正常
 *          QUEUE_ERROR 异常
 * @date  : 2021/10/06
 * @note  : 初始化 FIFO
 *          if (uQueueInit(&tCtrlQueue, QUEUE_OneSize, QUEUE_MaxCnt) == QUEUE_OK)
 *              printf("FIFO was init successfully! \r\n");
 * ---------------------------------------------------------------------------*/
uint8_t uQueueInit(QueueHandle_T * pQueue, uint16_t OneSize, uint16_t MaxCnt)
{
	pQueue->InitStatus = 0; //初始化成功状态无效

#if QUEUE_USE_MALLOC
	pQueue->pQueueBuff = (uint8_t *)mymalloc(SDRAM_POOL, OneSize * MaxCnt); //申请内存缓冲区指针 - 用户动态申请内存函数
#else
	pQueue->pQueueBuff = (uint8_t *)pvPortMalloc(OneSize * MaxCnt); //申请内存缓冲区指针 - FreeRTOS动态申请内存函数
#endif

	if (pQueue->pQueueBuff == NULL)
	{
#if QUEUE_USE_ASSERT
		assert(pQueue->pQueueBuff); //申请失败会有断言
#else
		return QUEUE_ERROR; //返回
#endif
	}

#if QUEUE_USE_MALLOC
	pQueue->pByteCntBuff = (uint16_t *)mymalloc(SDRAM_POOL, sizeof(uint16_t) * MaxCnt); //申请内存数据大小记录缓冲区 - 用户动态申请内存函数
#else
	pQueue->pByteCntBuff = (uint16_t *)pvPortMalloc(sizeof(uint16_t) * MaxCnt); //申请内存数据大小记录缓冲区 - FreeRTOS动态申请内存函数
#endif

	if (pQueue->pByteCntBuff == NULL)
	{
#if QUEUE_USE_MALLOC
		myfree(SDRAM_POOL, pQueue->pQueueBuff); //申请内存失败，释放之前申请的内存
#else
		vPortFree(pQueue->pQueueBuff); //申请内存失败，释放之前申请的内存
#endif

#if QUEUE_USE_ASSERT
		assert(pQueue->pByteCntBuff); //申请失败会有断言
#else
		return QUEUE_ERROR; //返回
#endif
	}

	pQueue->OneSize = OneSize;         //单条数据大小
	pQueue->Cnt = MaxCnt;              //缓冲区总数据容量(条数)
	pQueue->ReadCnt = 0;               //读取位置
	pQueue->WriteCnt = 0;              //写位置
	pQueue->NotReadCnt = 0;            //未读取数量
	pQueue->DataLen = 0;               //读取到数据的长度
	pQueue->InitStatus = QUEUE_STATUS; //初始化标志

	QUEUE_API_CREATELOCK(pQueue); //创建互斥量
	QUEUE_API_UNLOCK(pQueue);     //解锁

	return QUEUE_OK;
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueDelete
 * @brief : 删除队列：释放队列内存、释放队列句柄内存
 * @param : pQueue：缓存区结构体句柄
 * @retval: QUEUE_OK 正常
 *          QUEUE_ERROR 异常
 * @date  : 2021/10/06
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t uQueueDelete(QueueHandle_T *pQueue)
{
	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return QUEUE_ERROR; //没有初始化

	QUEUE_API_LOCK(pQueue); //上锁

#if QUEUE_USE_MALLOC
	myfree(SDRAM_POOL, pQueue->pQueueBuff);   //释放队列内存
	myfree(SDRAM_POOL, pQueue->pByteCntBuff); //释放队列内存
#else
	vPortFree(pQueue->pQueueBuff);        //释放队列内存
	vPortFree(pQueue->pByteCntBuff);      //释放队列内存
#endif

	//清除数据及标志
	pQueue->pQueueBuff = NULL;
	pQueue->pByteCntBuff = NULL;
	pQueue->OneSize = 0;     //单条数据大小
	pQueue->Cnt = 0;         //缓冲区总数据容量(条数)
	pQueue->ReadCnt = 0;     //读取位置
	pQueue->WriteCnt = 0;    //写位置
	pQueue->NotReadCnt = 0;  //未读取数量
	pQueue->DataLen = 0;     //读取到数据的长度
	pQueue->InitStatus = 0;  //初始化标志

	QUEUE_API_UNLOCK(pQueue); //解锁

	QUEUE_API_DELETELOCK(pQueue); //删除互斥量

	return QUEUE_OK;
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueWrite
 * @brief : 写入环形队列
 * @param : pQueue：缓存区结构体句柄
 *          pBuf：数据指针
 *          len：写入队列数据的字节长度
 * @retval: QUEUE_OK 正常
 *          QUEUE_ERROR 异常
 * @date  : 2021/10/06
 * @note  : uQueueWrite(&tCtrlQueue, (uint8_t *)wruteDatabuf, wruteDatalen); //写入环形队列
 * ---------------------------------------------------------------------------*/
uint8_t uQueueWrite(QueueHandle_T *pQueue, uint8_t *pBuf, uint16_t len)
{
	QueueStatusTypeDef_E sta = QUEUE_OK;
	uint16_t cnt = 0; //在环形队列中指针位置

	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return QUEUE_ERROR; //没有初始化

	QUEUE_API_LOCK(pQueue); //上锁

	if (pQueue->NotReadCnt >= pQueue->Cnt) //发送溢出
	{
		cnt = pQueue->WriteCnt;                                                //先将写指针后移，占位，防止多线程写冲突
		if (len > pQueue->OneSize) len = pQueue->OneSize;                      //限制单条数据大小
		pQueue->WriteCnt = (pQueue->WriteCnt + 1) % pQueue->Cnt;               //写入数量增加，环形FIFO
		pQueue->ReadCnt = (pQueue->ReadCnt + 1) % pQueue->Cnt;                 //读取数量增加，环形FIFO，把读写指针都增加，但是剩余数据数量不变
		pQueue->pByteCntBuff[cnt] = len;                                       //记录数据大小
		pQueueMemcpy(&pQueue->pQueueBuff[cnt * pQueue->OneSize], pBuf, len);   //拷贝数据到缓冲区

		sta = QUEUE_ISFULL; //数据已经满了
	}
	else
	{
		if (len > pQueue->OneSize) len = pQueue->OneSize;                      //限制单条数据大小
		cnt = pQueue->WriteCnt;                                                //先将写指针后移，占位，防止多线程写冲突
		pQueue->WriteCnt = (pQueue->WriteCnt + 1) % pQueue->Cnt;               //写入数量增加，环形FIFO
		pQueue->pByteCntBuff[cnt] = len;                                       //记录数据大小
		pQueueMemcpy(&pQueue->pQueueBuff[cnt * pQueue->OneSize], pBuf, len);   //拷贝数据到缓冲区
		pQueue->NotReadCnt ++;                                                 //没有读取的数量增加

		sta = QUEUE_OK; //数据写入成功
	}

	QUEUE_API_UNLOCK(pQueue); //解锁

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueWriteNoCover
 * @brief : 写入环形队列，写满不覆盖
 * @param : pQueue：缓存区结构体句柄
 *          pBuf：数据指针
 *          len：写入队列数据的字节长度
 * @retval: QUEUE_OK 正常
 *          QUEUE_ERROR 异常
 * @date  : 2021/10/06
 * @note  : uQueueWrite(&tCtrlQueue, (uint8_t *)wruteDatabuf, wruteDatalen); //写入环形队列
 * ---------------------------------------------------------------------------*/
uint8_t uQueueWriteNoCover(QueueHandle_T *pQueue, uint8_t *pBuf, uint16_t len)
{
	QueueStatusTypeDef_E sta = QUEUE_OK;
	uint16_t cnt = 0; //在环形队列中指针位置

	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return QUEUE_ERROR; //没有初始化

	QUEUE_API_LOCK(pQueue); //上锁

	if (pQueue->NotReadCnt >= pQueue->Cnt) //发送溢出
	{
		sta = QUEUE_ISFULL; //数据已经满了
	}
	else
	{
		if (len > pQueue->OneSize) len = pQueue->OneSize;                      //限制单条数据大小
		cnt = pQueue->WriteCnt;                                                //先将写指针后移，占位，防止多线程写冲突
		pQueue->WriteCnt = (pQueue->WriteCnt + 1) % pQueue->Cnt;               //写入数量增加，环形FIFO
		pQueue->pByteCntBuff[cnt] = len;                                       //记录数据大小
		pQueueMemcpy(&pQueue->pQueueBuff[cnt * pQueue->OneSize], pBuf, len);   //拷贝数据到缓冲区
		pQueue->NotReadCnt ++;                                                 //没有读取的数量增加

		sta = QUEUE_OK; //数据写入成功
	}

	QUEUE_API_UNLOCK(pQueue); //解锁

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueRead
 * @brief : 读取一条数据，并清除该条数据
 * @param : pQueue：缓存区结构体句柄
 *          pBuf：获取接收到数据的缓存区
 *          len：获取接收到数据的长度
 * @retval: QUEUE_OK 正常
 *          QUEUE_ERROR 异常
 * @date  : 2021/10/06
 * @note  : uQueueRead(&tCtrlQueue, (uint8_t *)readDataBuf, &readDataLen); //从FIFO读取数据
 * ---------------------------------------------------------------------------*/
uint8_t uQueueRead(QueueHandle_T *pQueue, uint8_t *pBuf, uint16_t *len)
{
	QueueStatusTypeDef_E sta = QUEUE_OK;
	uint16_t cnt = 0; //在环形队列中指针位置

	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return QUEUE_ERROR; //没有初始化
	if(pQueue->NotReadCnt == 0)  return QUEUE_ERROR; //数据为空

	QUEUE_API_LOCK(pQueue); //上锁

	cnt = pQueue->pByteCntBuff[pQueue->ReadCnt];       //获取数据大小
	if (cnt > pQueue->OneSize) cnt = pQueue->OneSize;  //限制单条数据大小

	pQueueMemcpy(pBuf, &pQueue->pQueueBuff[pQueue->ReadCnt * pQueue->OneSize], cnt); //拷贝缓冲区数据

	pQueueMemset(&pQueue->pQueueBuff[pQueue->ReadCnt * pQueue->OneSize], 0, cnt); //清除已拷贝缓冲区数据

	*len = pQueue->DataLen = cnt;                           //获取接收到数据的长度
	pQueue->ReadCnt = (pQueue->ReadCnt + 1) % pQueue->Cnt;  //读取数量增加，环形FIFO
	pQueue->NotReadCnt--;                                   //没有读取的数量减少

	QUEUE_API_UNLOCK(pQueue); //解锁

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueReadNoDel
 * @brief : 读取一条数据，不清除该条数据
 * @param : pQueue：缓存区结构体句柄
 *          pBuf：获取接收到数据的缓存区
 *          len：获取接收到数据的长度
 * @retval: QUEUE_OK 正常
 *          QUEUE_ERROR 异常
 * @date  : 2021/10/06
 * @note  : uQueueReadNoDel(&tCtrlQueue, (uint8_t *)readDataBuf, &readDataLen); //从FIFO读取数据
 * ---------------------------------------------------------------------------*/
uint8_t uQueueReadNoDel(QueueHandle_T *pQueue, uint8_t *pBuf, uint16_t *len)
{
	QueueStatusTypeDef_E sta = QUEUE_OK;
	uint16_t cnt = 0; //在环形队列中指针位置

	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return QUEUE_ERROR; //没有初始化
	if (pQueue->NotReadCnt == 0)  return QUEUE_ERROR; //数据为空

	QUEUE_API_LOCK(pQueue); //上锁

	cnt = pQueue->pByteCntBuff[pQueue->ReadCnt];      //获取数据大小
	if (cnt > pQueue->OneSize) cnt = pQueue->OneSize; //限制单条数据大小

	pQueueMemcpy(pBuf, &pQueue->pQueueBuff[pQueue->ReadCnt * pQueue->OneSize], cnt); //拷贝缓冲区数据

	*len = pQueue->DataLen = cnt; //获取接收到数据的长度

	QUEUE_API_UNLOCK(pQueue); //解锁

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueClear
 * @brief : 清除指定个数的数据
 * @param : pQueue：缓存区结构体句柄
 *          delCnt：清除的个数
 * @retval: QUEUE_OK 正常
 *          QUEUE_ERROR 异常
 * @date  : 2021/10/06
 * @note  : uQueueReadNoDelete(&tCtrlQueue, (uint8_t *)readDataBuf, &readDataLen); //从FIFO读取数据
 * ---------------------------------------------------------------------------*/
uint8_t uQueueClear(QueueHandle_T *pQueue, uint16_t delCnt)
{
	QueueStatusTypeDef_E sta = QUEUE_OK;
	uint16_t cnt = 0; //在环形队列中指针位置

	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return QUEUE_ERROR; //没有初始化
	if (pQueue->NotReadCnt == 0)  return QUEUE_ERROR; //数据为空

	QUEUE_API_LOCK(pQueue); //上锁

	if (delCnt < pQueue->NotReadCnt)
	{
		cnt = pQueue->pByteCntBuff[delCnt + pQueue->ReadCnt];                              //获取数据大小
		if (cnt > pQueue->OneSize) cnt = pQueue->OneSize;                                  //限制单条数据大小
		pQueueMemset(&pQueue->pQueueBuff[(delCnt + pQueue->ReadCnt) * pQueue->OneSize], 0, cnt); //清除已拷贝缓冲区数据

		pQueue->ReadCnt = ((delCnt + pQueue->ReadCnt) + 1) % pQueue->Cnt;                  //读取数量增加，环形FIFO
		pQueue->NotReadCnt -= delCnt;                                                      //没有读取的数量减少
	}
	else
	{
		sta = QUEUE_ERROR; //超过剩余数量
	}

	QUEUE_API_UNLOCK(pQueue); //解锁

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueClearALL
 * @brief : 清除FIFO
 * @param : pQueue：缓存区结构体句柄
 * @retval: QUEUE_OK 正常
 *          QUEUE_ERROR 异常
 * @date  : 2021/10/06
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t uQueueClearALL(QueueHandle_T *pQueue)
{
	QueueStatusTypeDef_E sta = QUEUE_OK;
	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return QUEUE_ERROR; //没有初始化

	QUEUE_API_LOCK(pQueue); //上锁

	pQueue->ReadCnt = 0;    //FIFO读取位置
	pQueue->WriteCnt = 0;   //FIFO写入位置
	pQueue->NotReadCnt = 0; //FIFO内没有读取的数据数量为0

	QUEUE_API_UNLOCK(pQueue); //解锁

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueGetDataNum
 * @brief : 获取FIFO中已存数据数量
 * @param : pQueue：缓存区结构体句柄
 * @retval: num：FIFO内没有读取的数据数量
 * @date  : 2021/10/06
 * @note  :
 * ---------------------------------------------------------------------------*/
uint16_t uQueueGetDataNum(QueueHandle_T *pQueue)
{
	uint16_t num = 0;
	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return num; //没有初始化

	QUEUE_API_LOCK(pQueue); //上锁

//	num = (pQueue->WriteCnt + pQueue->Cnt - pQueue->ReadCnt) % pQueue->Cnt;
	num = pQueue->NotReadCnt;

	QUEUE_API_UNLOCK(pQueue); //解锁

	return num; //FIFO内没有读取的数据数量
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueLeftNum
 * @brief : 获取FIFO剩余可存放数据个数
 * @param : pQueue：缓存区结构体句柄
 * @retval: num：FIFO内剩余可存放数据个数
 * @date  : 2021/10/06
 * @note  :
 * ---------------------------------------------------------------------------*/
uint16_t uQueueLeftNum(QueueHandle_T *pQueue)
{
	uint16_t num = 0;
	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return num; //没有初始化

	QUEUE_API_LOCK(pQueue); //上锁

//	num = (pQueue->Cnt - pQueue->WriteCnt);
	num = (pQueue->Cnt - pQueue->NotReadCnt);

	QUEUE_API_UNLOCK(pQueue); //解锁

	return num; //FIFO内剩余可存放数据个数
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueCapacity
 * @brief : 获取FIFO总容量
 * @param : pQueue：缓存区结构体句柄
 * @retval: num：FIFO总数据容量（条数）
 * @date  : 2021/10/06
 * @note  :
 * ---------------------------------------------------------------------------*/
uint16_t uQueueCapacity(QueueHandle_T *pQueue)
{
	uint16_t num = 0;
	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return num; //没有初始化

	QUEUE_API_LOCK(pQueue); //上锁

	num = pQueue->Cnt;

	QUEUE_API_UNLOCK(pQueue); //解锁

	return num; //FIFO总数据容量（条数）
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueIsFull
 * @brief : FIFO是否已满
 * @param : pQueue：缓存区结构体句柄
 * @retval: QUEUE_TRUE 已满
 *          QUEUE_FALSE 未满
 * @date  : 2021/10/06
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t uQueueIsFull(QueueHandle_T *pQueue)
{
	QueueStatusTypeDef_E sta = QUEUE_TRUE;
	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return sta; //没有初始化

	sta = (uQueueGetDataNum(pQueue) == (pQueue->Cnt)) ? QUEUE_TRUE : QUEUE_FALSE;

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : uQueueIsEmpty
 * @brief : FIFO是否已空
 * @param : pQueue：缓存区结构体句柄
 * @retval: QUEUE_TRUE 为空
 *          QUEUE_FALSE 不为空
 * @date  : 2021/10/06
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t uQueueIsEmpty(QueueHandle_T *pQueue)
{
	QueueStatusTypeDef_E sta = QUEUE_TRUE;
	if((pQueue == NULL) || (pQueue->InitStatus != QUEUE_STATUS)) return sta; //没有初始化

	sta = (uQueueGetDataNum(pQueue) == 0) ? QUEUE_TRUE : QUEUE_FALSE;

	return sta;
}
