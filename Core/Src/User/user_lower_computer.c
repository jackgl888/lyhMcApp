/*-----------------------------------------------------------------------------
 * @file        : user_lower_computer.c
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2019/10/18
 * @date changed: 2021/10/22
 * @brief       : 中下位机通信处理程序
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#ifdef USER_CONFIG
	#include "user_config.h"
#endif


/*-----------------------------------------------------------------------------
 * @name  : LowerPC_QueryLayerIsNULL
 * @brief : 下位机通信：查询层是否存在
 * @param : pLayer：存在的层记录结构体句柄
 *          num：查询的层数
 * @retval: 0 不存在 1 存在
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPC_QueryLayerIsNULL(ExistLayer_U* pLayer, uint8_t num)
{
	uint8_t sta = 0;

	if(((pLayer->layerNum >> (num-1)) & 0x01) == 0x01)
	{
		sta  = 1;
	}

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPC_DeleteLayer
 * @brief : 下位机通信：删除层
 * @param : pLayer：存在的层记录结构体句柄
 *          num：删除的层数
 * @retval: 0 失败 1 成功
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPC_DeleteLayer(ExistLayer_U* pLayer, uint8_t num)
{
	uint8_t sta = 0;

	if(num <= LOWER_LAYER_TOTAL_NUM) //在默认层数之内
	{
		pLayer->layerNum &= ~(0x01 << (num-1));

		sta  = 1;
	}

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPC_QueryChIsNULL
 * @brief : 下位机通信：查询通道是否存在
 * @param : pLayer：存在的层记录结构体句柄
 *          num：查询的通道数
 * @retval: 0 不存在 1 存在
 * @date  : 2021/10/27
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPC_QueryChIsNULL(ExistCh_T* pCh, uint8_t num)
{
	uint8_t sta = 0;
	uint8_t layerID = (num / LOWER_CH_TOTAL_NUM); //计算出所在层号
	uint8_t chID = (num % LOWER_CH_TOTAL_NUM);    //计算出所在层的通道号

	if(pCh->existCh[layerID][chID] == 0x01)
	{
		sta  = 1;
	}

	return sta;
}


//-----------------------------------------------------------------------------
//                              TODO:发送处理
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * @name  : LowerPC_CommStaJudge
 * @brief : 通信状态判断
 * @param : pBuf：发送数据指针
			bufLen：发送数据长度
 * @retval:
 * @date  : 2021/11/02
 * @note  :
 * ---------------------------------------------------------------------------*/
static void LowerPC_CommStaJudge(uint8_t layer, LowerControlCmd_E eCmd, CommStatus_E eSta)
{
	if(eCmd == CMD_LOWER_READ_BOARD_INFO) //读取板卡信息
	{
		uLowerCommSta[layer].CommStaBit.sta_ReadBoardInfo = eSta;
	}

	if(tCommPro.Lose_Power_Comm_Flag == 1)
	{
		if(eCmd == CMD_LOWER_GET_BOARD_CH_DATA) //获取整板通道实时数据
		{
			uLowerCommSta[layer].CommStaBit.sta_GetBoardChData = eSta;
		}
		else if(eCmd == CMD_LOWER_GET_BOARD_CH_ADC) //获取整板通道ADC原始数据
		{
			uLowerCommSta[layer].CommStaBit.sta_GetBoardChAdc = eSta;
		}
		else if(eCmd == CMD_LOWER_GET_CH_CAL_DATA) //获取单个通道校准参数
		{
			uLowerCommSta[layer].CommStaBit.sta_GetChCalData = eSta;
		}
		else if(eCmd == CMD_LOWER_SET_CH_CAL_DATA) //设置单个通道校准参数
		{
			uLowerCommSta[layer].CommStaBit.sta_SetChCalData = eSta;
		}
		else if(eCmd == CMD_LOWER_RESET_CH_CAL_DATA) //重置单个通道校准参数
		{
			uLowerCommSta[layer].CommStaBit.sta_ResetChCalData = eSta;
		}
		else if(eCmd == CMD_LOWER_GLOBAL_SETTING) //全局设置
		{
			MINUS_ONE(tCommLayerCnt.cnt_GlobalSetting);
			if(JUDGE_ZERO(tCommLayerCnt.cnt_GlobalSetting))
			{
				/* 下位机通信结束才应答给上位机 */
				xSemaphoreGive(UpperPcSem); //释放信号量
			}

			uLowerCommSta[layer].CommStaBit.sta_GlobalSetting = eSta;
		}
		else if(eCmd == CMD_LOWER_CH_STEP_LOADING) //通道工步加载
		{
			MINUS_ONE(tCommLayerCnt.cnt_ChStepLoading);
			if(JUDGE_ZERO(tCommLayerCnt.cnt_ChStepLoading))
			{
				/* 下位机通信结束才应答给上位机 */
				xSemaphoreGive(UpperPcSem); //释放信号量
			}

			uLowerCommSta[layer].CommStaBit.sta_ChStepLoading = eSta;
		}
		else if(eCmd == CMD_LOWER_POWER_OFF_CONNECT) //断电续接
		{
			uLowerCommSta[layer].CommStaBit.sta_PowerOffConnect = eSta;
		}
		else if(eCmd == CMD_LOWER_START_STEP) //开始工步
		{
			MINUS_ONE(tCommLayerCnt.cnt_StartStep);
			if(JUDGE_ZERO(tCommLayerCnt.cnt_StartStep))
			{
				/* 下位机通信结束才应答给上位机 */
				xSemaphoreGive(UpperPcSem); //释放信号量
			}

			uLowerCommSta[layer].CommStaBit.sta_StartStep = eSta;
		}
		else if(eCmd == CMD_LOWER_SUSPEND_STEP) //暂停工步
		{
			MINUS_ONE(tCommLayerCnt.cnt_SuspendStep);
			if(JUDGE_ZERO(tCommLayerCnt.cnt_SuspendStep))
			{
				/* 下位机通信结束才应答给上位机 */
				xSemaphoreGive(UpperPcSem); //释放信号量
			}

			uLowerCommSta[layer].CommStaBit.sta_SuspendStep = eSta;
		}
		else if(eCmd == CMD_LOWER_STOP_STEP) //停止工步
		{
			MINUS_ONE(tCommLayerCnt.cnt_StopStep);
			if(JUDGE_ZERO(tCommLayerCnt.cnt_StopStep))
			{
				/* 下位机通信结束才应答给上位机 */
				xSemaphoreGive(UpperPcSem); //释放信号量
			}

			uLowerCommSta[layer].CommStaBit.sta_StopStep = eSta;
		}
		else if(eCmd == CMD_LOWER_CONTINUE_STEP) //继续工步
		{
			MINUS_ONE(tCommLayerCnt.cnt_ContinueStep);
			if(JUDGE_ZERO(tCommLayerCnt.cnt_ContinueStep))
			{
				/* 下位机通信结束才应答给上位机 */
				xSemaphoreGive(UpperPcSem); //释放信号量
			}

			uLowerCommSta[layer].CommStaBit.sta_ContinueStep = eSta;
		}
		else if(eCmd == CMD_LOWER_JUMP_STEP) //跳转工步
		{
			MINUS_ONE(tCommLayerCnt.cnt_JumpStep);
			if(JUDGE_ZERO(tCommLayerCnt.cnt_JumpStep))
			{
				/* 下位机通信结束才应答给上位机 */
				xSemaphoreGive(UpperPcSem); //释放信号量
			}

			uLowerCommSta[layer].CommStaBit.sta_JumpStep = eSta;
		}
		else if(eCmd == CMD_LOWER_START_C_D) //下发开始充放电指令
		{
			uLowerCommSta[layer].CommStaBit.sta_Start_C_D = eSta;
		}
		else if(eCmd == CMD_LOWER_STOP_C_D) //下发停止充放电指令
		{
			uLowerCommSta[layer].CommStaBit.sta_Stop_C_D = eSta;
		}
		else if(eCmd == CMD_LOWER_CLEAR_WARNING) //清除警告
		{
			MINUS_ONE(tCommLayerCnt.cnt_ClearWarning);
			if(JUDGE_ZERO(tCommLayerCnt.cnt_ClearWarning))
			{
				/* 下位机通信结束才应答给上位机 */
				xSemaphoreGive(UpperPcSem); //释放信号量
			}

			uLowerCommSta[layer].CommStaBit.sta_ClearWarning = eSta;
		}
		else if(eCmd == CMD_LOWER_RESET) //复位指令
		{
			MINUS_ONE(tCommLayerCnt.cnt_Reset);
			if(JUDGE_ZERO(tCommLayerCnt.cnt_Reset))
			{
				/* 下位机通信结束才应答给上位机 */
				xSemaphoreGive(UpperPcSem); //释放信号量
			}

			uLowerCommSta[layer].CommStaBit.sta_Reset = eSta;
		}
		else if(eCmd == CMD_LOWER_CH_ENABLE) //通道禁用/启用
		{
			MINUS_ONE(tCommLayerCnt.cnt_Enable);
			if(JUDGE_ZERO(tCommLayerCnt.cnt_Enable))
			{
				/* 下位机通信结束才应答给上位机 */
				xSemaphoreGive(UpperPcSem); //释放信号量
			}

			uLowerCommSta[layer].CommStaBit.sta_Enable = eSta;
		}
	}
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPC_SendData
 * @brief : 发送函数
 * @param : pBuf：发送数据指针
			bufLen：发送数据长度
 * @retval: 发送失败或者发送1次都不成功就返回 HAL_ERROR
 * @date  : 2021/10/21
 * @note  : 一问一答，带错误重发
 * ---------------------------------------------------------------------------*/
static HAL_StatusTypeDef LowerPC_SendData(uint8_t* pBuf, uint16_t bufLen)
{
	const static uint8_t tranNum = 1; //错误重发次数限制
	HAL_StatusTypeDef sta = HAL_ERROR;
	uint8_t err = 0, tranCnt = 0;

sendAgain:

	err = UART1_Send(pBuf, bufLen);

	if(err != HAL_OK)
	{
		 return sta;
	}

	/* 区分不同指令应答等待时间 */
	if((pBuf[2] == CMD_LOWER_GET_BOARD_CH_DATA) ||  //获取整板通道实时数据
	   (pBuf[2] == CMD_LOWER_CH_STEP_LOADING))      //通道工步加载
	{
		if(xSemaphoreTake(LowerPcSem, 150) == errQUEUE_EMPTY) //150ms没有应答
		{
			tranCnt++;

			if(tranCnt < tranNum)
			{
				goto sendAgain;
			}
		}
	}
	else
	{
		if(xSemaphoreTake(LowerPcSem, 100) == errQUEUE_EMPTY) //100ms没有应答
		{
			tranCnt++;

			if(tranCnt < tranNum)
			{
				goto sendAgain;
			}
		}
	}

	//DEBUG：查看失败次数
	if(tranCnt >= tranNum) //通信异常
	{
		LowerPC_CommStaJudge(INDEX(pBuf[1]), pBuf[2], COMM_ERROR); //通信异常

		sta = HAL_ERROR;
	}
	else
	{
		LowerPC_CommStaJudge(INDEX(pBuf[1]), pBuf[2], COMM_OK); //通信正常

		sta = HAL_OK;
	}

    return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_TxFramePack
 * @brief : 下位机通信：发送数据打包
 * @param : eCmd：指令码
 *          eAddr：目标板卡地址
 *          ... 可变参数
 * @retval: QUEUE_OK：0    QUEUE_ERROR：1
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
static uint8_t LowerPc_TxFramePack(LowerControlCmd_E eCmd, LowerComputerAddr_E eAddr, ...)
{
    QueueStatusTypeDef_E sta = QUEUE_OK;

    uint8_t* pBuf = (uint8_t*)pvPortMalloc(LOWER_PC_BUF_MAX_LEN); //申请内存缓冲区指针
	if(pBuf == NULL)
	{
		return QUEUE_ERROR; //返回
	}
	memset(pBuf, 0, LOWER_PC_BUF_MAX_LEN); //清零

	LOWER_PC_LOCK(); //互斥量闭锁

	uint16_t addr = 0;        //数据长度的位置
    uint16_t checkSum = 0;    //校验和
    uint16_t tempDataLen = 0; //数据段长度
	uint16_t len = 0;         //帧长度

	pBuf[len++] = LOWER_FRAME_HEAD;         //包头
	pBuf[len++] = eAddr;                    //通信目标地址
	pBuf[len++] = eCmd;                     //指令码

	memcpy(pBuf+len, &addr, sizeof(addr));  //数据长度，初始为0
	addr = len;                             //记录数据长度的位置
	len += sizeof(addr);

	if((eCmd == CMD_LOWER_READ_BOARD_INFO)   || //读取板卡信息
	   (eCmd == CMD_LOWER_GET_BOARD_CH_DATA) || //获取整板通道实时数据
	   (eCmd == CMD_LOWER_GET_BOARD_CH_ADC))    //获取整板通道ADC原始数据
	{
		//不需处理
	}
	else if((eCmd == CMD_LOWER_SET_CH_CAL_DATA)    || //设置单个通道校准参数
			(eCmd == CMD_LOWER_GLOBAL_SETTING)     || //全局设置
			(eCmd == CMD_LOWER_CH_STEP_LOADING)    || //通道工步加载
			(eCmd == CMD_LOWER_POWER_OFF_CONNECT)  || //断电续接
		    (eCmd == CMD_LOWER_START_C_D)          || //下发开始充放电指令
		    (eCmd == CMD_LOWER_STOP_C_D))             //下发停止充放电指令
	{
		va_list args;
		va_start(args, eAddr);
		uint8_t* dataBuf = (uint8_t*)va_arg(args, char*); //数组指针
		uint16_t dataLen = (uint16_t)va_arg(args, int);    //数组长度
		va_end(args);

		/* 数据段 */
		memcpy(pBuf+len, dataBuf, dataLen);
		len += dataLen;
	}
	else if(eCmd == CMD_LOWER_GET_CH_CAL_DATA)  //获取单个通道校准参数
	{
		va_list args;
		va_start(args, eAddr);
		uint8_t chID = (uint8_t)va_arg(args, int); //通道号
		va_end(args);

		/* 数据段 */
		pBuf[len++] = chID;                     //通道号
	}
	else if((eCmd == CMD_LOWER_RESET_CH_CAL_DATA)  || //重置单个通道校准参数
			(eCmd == CMD_LOWER_START_STEP)         || //开始工步
			(eCmd == CMD_LOWER_SUSPEND_STEP)       || //暂停工步
			(eCmd == CMD_LOWER_STOP_STEP)          || //停止工步
			(eCmd == CMD_LOWER_CONTINUE_STEP)      || //继续工步
			(eCmd == CMD_LOWER_CLEAR_WARNING)      || //清除警告
			(eCmd == CMD_LOWER_RESET))                //复位指令
	{
		va_list args;
		va_start(args, eAddr);
		uint8_t chNum = (uint8_t)va_arg(args, int);    //通道数
		uint8_t* chID = (uint8_t*)va_arg(args, char*); //通道号
		va_end(args);

		/* 数据段 */
		pBuf[len++] = chNum;           //通道数量
		memcpy(pBuf+len, chID, chNum); //通道号数组
		len += chNum;
	}
	else if((eCmd == CMD_LOWER_JUMP_STEP)  || //跳转工步
			(eCmd == CMD_LOWER_CH_ENABLE))    //通道禁用/启用
	{
		va_list args;
		va_start(args, eAddr);
		uint8_t chNum = (uint8_t)va_arg(args, int);    //通道数
		uint8_t* chID = (uint8_t*)va_arg(args, char*); //通道号
		uint8_t data  = (uint8_t)va_arg(args, int);    //跳转的工步or通道禁用/启用
		va_end(args);

		/* 数据段 */
		pBuf[len++] = chNum;                    //通道数量
		memcpy(pBuf+len, chID, chNum);          //通道号数组
		len += chNum;
		pBuf[len++] = data;
	}

	tempDataLen = len - (addr + 2);             //计算数据段的总字节长度
	memcpy(pBuf+addr, &tempDataLen, sizeof(tempDataLen)); //数据长度

	checkSum = CheckSum16(&pBuf[1], len-1);     //校验和
	pBuf[len++] = DataU16L_ToU8(checkSum);      //校验和（LSB）
	pBuf[len++] = DataU16H_ToU8(checkSum);      //校验和（MSB）
	pBuf[len++] = LOWER_FRAME_END;              //包尾

    sta = uQueueWrite(&tLowerPcTxQueue, (uint8_t*)pBuf, len); //写入环形队列

    vPortFree(pBuf); //释放之前申请的内存

    LOWER_PC_UNLOCK(); //互斥量开锁

    return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_StartStep
 * @brief : 下位机通信：通道分析
 * @param : eCtrlCmd：指令码（开始工步、暂停工步、停止工步、继续工步、跳转工步、清除警告、复位指令等）
 *          pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/20
 * @note  : 用于工艺、工步控制，在此函数内进行通道划分
 * ---------------------------------------------------------------------------*/
static uint8_t LowerPc_ChAnalyze(LowerControlCmd_E eCtrlCmd, const uint8_t* pBuf, uint16_t len)
{
	uint8_t i = 0;
	uint8_t layerID = 0, lastLayerID = 0;   //层号、上一次层号
	uint8_t chNum = 0;                      //通道数
	uint8_t chID[LOWER_CH_TOTAL_NUM] = {0}; //通道号
	uint8_t cnt = 0; //要通信的层数

	if(pBuf[0] == 0x01) //全部通道进行操作
	{
		for(i=0; i<LOWER_CH_TOTAL_NUM; i++)
		{
			chID[i] = i+1; //每层所有的通道号
		}

		chNum = LOWER_CH_TOTAL_NUM;

		for(i=0; i<LOWER_LAYER_TOTAL_NUM; i++)
		{
			layerID = i + 1;
			if(LowerPC_QueryLayerIsNULL(&uExistLayer, layerID) == 0)
			{
				continue; //查询不到层存在则跳过该次循环循环
			}

			cnt++;
			if((eCtrlCmd == CMD_LOWER_JUMP_STEP) || (eCtrlCmd == CMD_LOWER_CH_ENABLE)) //跳转工步、通道禁用/启用另作打包
			{
				LowerPc_TxFramePack(eCtrlCmd, layerID, chNum, chID, pBuf[len-1]); //打包
			}
			else
			{
				LowerPc_TxFramePack(eCtrlCmd, layerID, chNum, chID); //打包
			}
		}
	}
	else if(pBuf[0] == 0x00) //非全部通道进行操作，对指定通道操作，需划分每层所对应的通道数及通道号
	{
		for(i=0; i<pBuf[1]; i++)
		{
			layerID = (INDEX(pBuf[i+2]) / LOWER_CH_TOTAL_NUM) + 1; //计算所在的层

			if((lastLayerID != 0) && (lastLayerID != layerID)) //通道所对应的层发生改变，既转移至下一层
			{
				if(LowerPC_QueryLayerIsNULL(&uExistLayer, lastLayerID) != 0) //层存在
				{
					cnt++;
					//发送上一层的通道
					if((eCtrlCmd == CMD_LOWER_JUMP_STEP) || (eCtrlCmd == CMD_LOWER_CH_ENABLE)) //跳转工步、通道禁用/启用另作打包
					{
						LowerPc_TxFramePack(eCtrlCmd, lastLayerID, chNum, chID, pBuf[len-1]); //打包
					}
					else
					{
						LowerPc_TxFramePack(eCtrlCmd, lastLayerID, chNum, chID); //打包
					}
				}

				chNum = 0; //清零
			}

			chID[chNum] = (INDEX(pBuf[i+2]) % LOWER_CH_TOTAL_NUM) + 1; //计算所在的层的通道号

			chNum++; //通道号计数自增

			lastLayerID = layerID; //记录上一次层号

			if(i == INDEX(pBuf[1])) //若为最后一层，最后一个
			{
				if(LowerPC_QueryLayerIsNULL(&uExistLayer, lastLayerID) != 0) //层存在
				{
					cnt++;
					if((eCtrlCmd == CMD_LOWER_JUMP_STEP) || (eCtrlCmd == CMD_LOWER_CH_ENABLE)) //跳转工步、通道禁用/启用另作打包
					{
						LowerPc_TxFramePack(eCtrlCmd, lastLayerID, chNum, chID, pBuf[len-1]); //打包
					}
					else
					{
						LowerPc_TxFramePack(eCtrlCmd, lastLayerID, chNum, chID); //打包
					}
				}
			}
		}
	}

	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_ReadBoardInfo
 * @brief : 下位机通信：读取板卡信息（默认所有层数都会轮询一遍）
 * @param : NULL
 * @retval: NULL
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_ReadBoardInfo(void)
{
	uint8_t i = 0;
	uint8_t layerID = 0; //层号

	for(i=0; i<LOWER_LAYER_TOTAL_NUM; i++)
	{
		layerID = i + 1;
		LowerPc_TxFramePack(CMD_LOWER_READ_BOARD_INFO, layerID); //依次读取每层板卡信息
	}
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_ReadBoardInfo
 * @brief : 下位机通信：获取整板通道实时数据
 * @param : NULL
 * @retval: NULL
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_GetBoardChData(void)
{
	uint8_t i = 0;
	uint8_t layerID = 0; //层号

	for(i=0; i<LOWER_LAYER_TOTAL_NUM; i++)
	{
		layerID = i + 1;
		if(LowerPC_QueryLayerIsNULL(&uExistLayer, layerID) == 0)
		{
			continue; //查询不到层存在则跳过该次循环
		}

		LowerPc_TxFramePack(CMD_LOWER_GET_BOARD_CH_DATA, layerID); //依次获取每层通道实时数据
	}
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_GetBoardChAdc
 * @brief : 下位机通信：获取整板通道ADC原始数据
 * @param : NULL
 * @retval: NULL
 * @date  : 2021/10/19
 * @note  : TODO:在触控屏校准界面时要发送该指令
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_GetBoardChAdc(void)
{
	uint8_t i = 0;
	uint8_t layerID = 0; //层号

	for(i=0; i<LOWER_LAYER_TOTAL_NUM; i++)
	{
		layerID = i + 1;
		if(LowerPC_QueryLayerIsNULL(&uExistLayer, layerID) == 0)
		{
			continue; //查询不到层存在则跳过该次循环
		}

		LowerPc_TxFramePack(CMD_LOWER_GET_BOARD_CH_ADC, layerID); //依次获取每层通道ADC原始数据
	}
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_GetChCalData
 * @brief : 下位机通信：获取单个通道校准参数
 * @param : ch：通道号（范围：1~144）
 * @retval: NULL
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_GetChCalData(uint16_t ch)
{
	uint8_t layerID = (INDEX(ch) / LOWER_CH_TOTAL_NUM); //计算出通道号所在层号
	uint8_t chID    = (INDEX(ch) % LOWER_CH_TOTAL_NUM); //计算出所在层的具体通道号

	LowerPc_TxFramePack(CMD_LOWER_GET_CH_CAL_DATA, layerID, chID); //获取单个通道校准参数
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_SetChCalData
 * @brief : 下位机通信：设置单个通道校准参数
 * @param : ch：通道号（范围：1~144）
 *          pType：校准类型句柄
 *          pol：通道极性，校准极性类型（0为正极性、1为负极性）
 * @retval: NULL
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_SetChCalData(uint16_t ch, CailPointsType_U* pType, uint8_t pol)
{
	uint8_t i = 0;
	uint8_t layerID = (INDEX(ch) / LOWER_CH_TOTAL_NUM); //计算出通道号所在层号
	uint8_t chID    = (INDEX(ch) % LOWER_CH_TOTAL_NUM); //计算出所在层的具体通道号

    uint8_t* pBuf = (uint8_t*)pvPortMalloc(LOWER_PC_BUF_MAX_LEN); //申请内存缓冲区指针
	if(pBuf == NULL)
	{
		DEBUG_PRINTF("\r\nFailure!\r\n"); //申请失败
	}
	memset(pBuf, 0, LOWER_PC_BUF_MAX_LEN); //清零

	uint16_t tempLen = 0; //数据长度
	uint8_t typeAddr = 0; //校准类型总数的位置

	pBuf[tempLen++] = tSetChCalData[layerID][chID].chID = chID + 1;       //通道号
	pBuf[tempLen++] = tSetChCalData[layerID][chID].calPolarity = pol; //校准极性类型
	pBuf[tempLen++] = tSetChCalData[layerID][chID].calTypeNum = 0x00; //校准类型总数，初始为0，方便后续计数用
	typeAddr = tempLen;

	if(pType->StaBit.chargeCurAdc == 0x01) //充电电流ADC校准
	{
		tSetChCalData[layerID][chID].calTypeNum++;
		pBuf[tempLen++] = CHARGE_CUR_ADC_CAL; //校准类型
		pBuf[tempLen++] = tSetChCalData[layerID][chID].chargeCurAdcPointsNum; //校准点数量

		for(i=0; i<tSetChCalData[layerID][chID].chargeCurAdcPointsNum; i++)
		{
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].chargeCurAdcPoints[i].currentSamp); //采样电流值
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].chargeCurAdcPoints[i].currentTrue); //真实电流值
		}
	}
	if(pType->StaBit.dischargeCurAdc == 0x01) //放电电流ADC校准
	{
		tSetChCalData[layerID][chID].calTypeNum++;
		pBuf[tempLen++] = DISCHARGE_CUR_ADC_CAL; //校准类型
		pBuf[tempLen++] = tSetChCalData[layerID][chID].dischargeCurAdcPointsNum; //校准点数量

		for(i=0; i<tSetChCalData[layerID][chID].dischargeCurAdcPointsNum; i++)
		{
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].dischargeCurAdcPoints[i].currentSamp); //采样电流值
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].dischargeCurAdcPoints[i].currentTrue); //真实电流值
		}
	}
	if(pType->StaBit.chargeCurDac == 0x01) //充电电流DAC校准
	{
		tSetChCalData[layerID][chID].calTypeNum++;
		pBuf[tempLen++] = CHARGE_CUR_DAC_CAL; //校准类型
		pBuf[tempLen++] = tSetChCalData[layerID][chID].chargeCurDacPointsNum; //校准点数量

		for(i=0; i<tSetChCalData[layerID][chID].chargeCurDacPointsNum; i++)
		{
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].chargeCurDacPoints[i].currentSet);  //设定电流值
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].chargeCurDacPoints[i].currentTrue); //真实电流值
		}
	}
	if(pType->StaBit.dischargeCurDac == 0x01) //充电电流DAC校准
	{
		tSetChCalData[layerID][chID].calTypeNum++;
		pBuf[tempLen++] = DISCHARGE_CUR_DAC_CAL; //校准类型
		pBuf[tempLen++] = tSetChCalData[layerID][chID].dischargeCurDacPointsNum; //校准点数量

		for(i=0; i<tSetChCalData[layerID][chID].dischargeCurDacPointsNum; i++)
		{
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].dischargeCurDacPoints[i].currentSet);  //设定电流值
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].dischargeCurDacPoints[i].currentTrue); //真实电流值
		}
	}
	if(pType->StaBit.chargeVolAdc == 0x01) //充电电压ADC校准
	{
		tSetChCalData[layerID][chID].calTypeNum++;
		pBuf[tempLen++] = CHARGE_VOL_ADC_CAL; //校准类型
		pBuf[tempLen++] = tSetChCalData[layerID][chID].chargeVolAdcPointsNum; //校准点数量

		for(i=0; i<tSetChCalData[layerID][chID].chargeVolAdcPointsNum; i++)
		{
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].chargeVolAdcPoints[i].voltageSamp); //采样电压值
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].chargeVolAdcPoints[i].voltageTrue); //真实电压值
		}
	}
	if(pType->StaBit.dischargeVolAdc == 0x01) //放电电压ADC校准
	{
		tSetChCalData[layerID][chID].calTypeNum++;
		pBuf[tempLen++] = DISCHARGE_VOL_ADC_CAL; //校准类型
		pBuf[tempLen++] = tSetChCalData[layerID][chID].dischargeVolAdcPointsNum; //校准点数量

		for(i=0; i<tSetChCalData[layerID][chID].dischargeVolAdcPointsNum; i++)
		{
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].dischargeVolAdcPoints[i].voltageSamp); //采样电压值
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].dischargeVolAdcPoints[i].voltageTrue); //真实电压值
		}
	}
	if(pType->StaBit.chargeVolDac == 0x01) //充电电压DAC校准
	{
		tSetChCalData[layerID][chID].calTypeNum++;
		pBuf[tempLen++] = CHARGE_VOL_DAC_CAL; //校准类型
		pBuf[tempLen++] = tSetChCalData[layerID][chID].chargeVolDacPointsNum; //校准点数量

		for(i=0; i<tSetChCalData[layerID][chID].chargeVolDacPointsNum; i++)
		{
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].chargeVololDacPoints[i].voltageSet);  //设定电压值
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].chargeVololDacPoints[i].voltageTrue); //真实电压值
		}
	}
	if(pType->StaBit.dischargeVolDac == 0x01) //放电电压DAC校准
	{
		tSetChCalData[layerID][chID].calTypeNum++;
		pBuf[tempLen++] = DISCHARGE_VOL_DAC_CAL; //校准类型
		pBuf[tempLen++] = tSetChCalData[layerID][chID].dischargeVolDacPointsNum; //校准点数量

		for(i=0; i<tSetChCalData[layerID][chID].dischargeVolDacPointsNum; i++)
		{
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].dischargeVolDacPoints[i].voltageSet);  //设定电压值
			pBuf[tempLen++] = fabs(tSetChCalData[layerID][chID].dischargeVolDacPoints[i].voltageTrue); //真实电压值
		}
	}

	pBuf[typeAddr] = tSetChCalData[layerID][chID].calTypeNum; //校准类型总数

	LowerPc_TxFramePack(CMD_LOWER_SET_CH_CAL_DATA, layerID, pBuf, tempLen); //设置单个通道校准参数

	vPortFree(pBuf); //释放之前申请的内存
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_ChangeCurCal
 * @brief : 下位机通信：设置单个通道充电电流单个校准点参数
 * @param : ch：通道号（范围：1~144）
 * @retval: NULL
 * @date  : 2021/10/21
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_ChangeCurCal(uint16_t ch)
{
	LOWER_PC_CALI_LOCK(); //互斥锁上锁

	uint8_t layerID = (INDEX(ch) / LOWER_CH_TOTAL_NUM); //计算出通道号所在层号
	uint8_t chID    = (INDEX(ch) % LOWER_CH_TOTAL_NUM); //计算出所在层的具体通道号

	uCalType.sta = 0x05; //校准类型

	tSetChCalData[layerID][chID].chargeCurAdcPointsNum = 0x01; //校准点数量

	tSetChCalData[layerID][chID].chargeCurAdcPoints[0].currentSamp = tChRawData[ch].current; //采样电流值，要获取下位机整板通道ADC原始数据
	tSetChCalData[layerID][chID].chargeCurAdcPoints[0].currentTrue = 0x00; //真实电流值，该值为触控屏输入的校准值


	tSetChCalData[layerID][chID].chargeCurDacPointsNum = 0x01; //校准点数量

	tSetChCalData[layerID][chID].chargeCurDacPoints[0].currentSet = 0x00;  //设定电流值，该值为设定的电流值
	tSetChCalData[layerID][chID].chargeCurDacPoints[0].currentTrue = 0x00; //真实电流值，该值为触控屏输入的校准值

	LowerPc_Send_SetChCalData(ch, &uCalType, CH_POSITIVE_POL);

	LOWER_PC_CALI_UNLOCK(); //解锁
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_DischangeCurCal
 * @brief : 下位机通信：设置单个通道放电电流单个校准点参数
 * @param : ch：通道号（范围：1~144）
 * @retval: NULL
 * @date  : 2021/10/21
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_DischangeCurCal(uint16_t ch)
{
	LOWER_PC_CALI_LOCK(); //互斥锁上锁

	uint8_t layerID = (INDEX(ch) / LOWER_CH_TOTAL_NUM); //计算出通道号所在层号
	uint8_t chID    = (INDEX(ch) % LOWER_CH_TOTAL_NUM); //计算出所在层的具体通道号

	uCalType.sta = 0x0A; //校准类型

	tSetChCalData[layerID][chID].dischargeCurAdcPointsNum = 0x01; //校准点数量

	tSetChCalData[layerID][chID].dischargeCurAdcPoints[0].currentSamp = tChRawData[ch].current; //采样电流值，要获取下位机整板通道ADC原始数据
	tSetChCalData[layerID][chID].dischargeCurAdcPoints[0].currentTrue = 0x00; //真实电流值，该值为触控屏输入的校准值


	tSetChCalData[layerID][chID].dischargeCurDacPointsNum = 0x01; //校准点数量

	tSetChCalData[layerID][chID].dischargeCurDacPoints[0].currentSet = 0x00;  //设定电流值，该值为设定的电流值
	tSetChCalData[layerID][chID].dischargeCurDacPoints[0].currentTrue = 0x00; //真实电流值，该值为触控屏输入的校准值

	LowerPc_Send_SetChCalData(ch, &uCalType, CH_POSITIVE_POL);

	LOWER_PC_CALI_UNLOCK(); //解锁
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_ChangeVolCal
 * @brief : 下位机通信：设置单个通道充电电压单个校准点参数
 * @param : ch：通道号（范围：1~144）
 * @retval: NULL
 * @date  : 2021/10/21
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_ChangeVolCal(uint16_t ch)
{
	LOWER_PC_CALI_LOCK(); //互斥锁上锁

	uint8_t layerID = (INDEX(ch) / LOWER_CH_TOTAL_NUM); //计算出通道号所在层号
	uint8_t chID    = (INDEX(ch) % LOWER_CH_TOTAL_NUM); //计算出所在层的具体通道号

	uCalType.sta = 0x50; //校准类型

	tSetChCalData[layerID][chID].chargeVolAdcPointsNum = 0x01; //校准点数量

	tSetChCalData[layerID][chID].chargeVolAdcPoints[0].voltageSamp = tChRawData[ch].voltage; //采样电压值，要获取下位机整板通道ADC原始数据
	tSetChCalData[layerID][chID].chargeVolAdcPoints[0].voltageTrue = 0x00; //真实电压值，该值为触控屏输入的校准值


	tSetChCalData[layerID][chID].chargeVolDacPointsNum = 0x01; //校准点数量

	tSetChCalData[layerID][chID].chargeVololDacPoints[0].voltageSet = 0x00;  //设定电压值，该值为设定的电流值
	tSetChCalData[layerID][chID].chargeVololDacPoints[0].voltageTrue = 0x00; //真实电压值，该值为触控屏输入的校准值

	LowerPc_Send_SetChCalData(ch, &uCalType, CH_POSITIVE_POL);

	LOWER_PC_CALI_UNLOCK(); //解锁
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_DischangeVolCal
 * @brief : 下位机通信：设置单个通道放电电压单个校准点参数
 * @param : ch：通道号（范围：1~144）
 * @retval: NULL
 * @date  : 2021/10/21
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_DischangeVolCal(uint16_t ch)
{
	LOWER_PC_CALI_LOCK(); //互斥锁上锁

	uint8_t layerID = (INDEX(ch) / LOWER_CH_TOTAL_NUM); //计算出通道号所在层号
	uint8_t chID    = (INDEX(ch) % LOWER_CH_TOTAL_NUM); //计算出所在层的具体通道号

	uCalType.sta = 0xA0; //校准类型

	tSetChCalData[layerID][chID].dischargeVolAdcPointsNum = 0x01; //校准点数量

	tSetChCalData[layerID][chID].dischargeVolAdcPoints[0].voltageSamp = tChRawData[ch].voltage; //采样电压值，要获取下位机整板通道ADC原始数据
	tSetChCalData[layerID][chID].dischargeVolAdcPoints[0].voltageTrue = 0x00; //真实电压值，该值为触控屏输入的校准值


	tSetChCalData[layerID][chID].dischargeVolDacPointsNum = 0x01; //校准点数量

	tSetChCalData[layerID][chID].dischargeVolDacPoints[0].voltageSet = 0x00;  //设定电压值，该值为设定的电流值
	tSetChCalData[layerID][chID].dischargeVolDacPoints[0].voltageTrue = 0x00; //真实电压值，该值为触控屏输入的校准值

	LowerPc_Send_SetChCalData(ch, &uCalType, CH_POSITIVE_POL);

	LOWER_PC_CALI_UNLOCK(); //解锁
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_ResetChCalData
 * @brief : 下位机通信：重置单个通道校准参数
 * @param : layerID：通道号所在层号
 *          chNum：通道数
 *          chID：通导号数组
 * @retval: NULL
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_ResetChCalData(uint8_t layerID, uint8_t chNum, uint8_t* chID)
{
	LowerPc_TxFramePack(CMD_LOWER_RESET_CH_CAL_DATA, layerID, chNum, chID); //重置单个通道校准参数
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_GlobalSetting
 * @brief : 下位机通信：全局设置
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPc_Send_GlobalSetting(const uint8_t* pBuf, uint16_t len)
{
	uint8_t i = 0;
	uint8_t layerID = 0; //层号
	uint8_t cnt = 0; //要通信的层数

	for(i=0; i<LOWER_LAYER_TOTAL_NUM; i++)
	{
		layerID = i + 1;
		if(LowerPC_QueryLayerIsNULL(&uExistLayer, layerID) == 0)
		{
			continue; //查询不到层存在则跳过该次循环
		}

		cnt++;
		LowerPc_TxFramePack(CMD_LOWER_GLOBAL_SETTING, layerID, pBuf, len); //整柜全局设置
	}

	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_StepLoading
 * @brief : 下位机通信：通道工步加载
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPc_Send_StepLoading(const uint8_t* pBuf, uint16_t len)
{
	uint8_t i = 0;
	uint8_t layerID = 0, lastLayerID = 0;   //层号、上一次层号
	uint8_t chNum = 0;                      //通道数
	uint8_t chID[LOWER_CH_TOTAL_NUM] = {0}; //通道号
	uint8_t cnt = 0; //要通信的层数

	tLosePowerData.chTotalNum = 0;

	if(pBuf[1] == 0x01) //加载全部通道
	{
		tLosePowerData.chTotalNum = tDeviceInfo.layerTotalNum*tDeviceInfo.chTotalNum; //断电续接加载的通道数
		for(i=0; i<LOWER_LAYER_TOTAL_NUM; i++)
		{
			layerID = i + 1;
			if(LowerPC_QueryLayerIsNULL(&uExistLayer, layerID) == 0)
			{
				continue; //查询不到层存在则跳过该次循环
			}

			cnt++;
			LowerPc_TxFramePack(CMD_LOWER_CH_STEP_LOADING, layerID, pBuf, len); //通道工步加载
		}
	}
	else if(pBuf[1] == 0x00) //加载指定通道，需划分每层所对应的通道数及通道号
	{
	    uint8_t* pTempBuf = (uint8_t*)pvPortMalloc(LOWER_PC_BUF_MAX_LEN); //申请内存缓冲区指针
		if(pTempBuf == NULL)
		{
			vTaskDelay(10);
			DEBUG_PRINTF("\r\n ERROR! \r\n");
		}
		memset(pTempBuf, 0, LOWER_PC_BUF_MAX_LEN); //清零
		uint16_t tempLen = 0, dataLen = 0; //数据长度

		for(i=0; i<pBuf[2]; i++) //下发的通道数量
		{
			layerID = (INDEX(pBuf[i+3]) / LOWER_CH_TOTAL_NUM) + 1; //计算所在的层

			if((lastLayerID != 0) && (lastLayerID != layerID)) //通道所对应的层发生改变，既转移至下一层
			{
				if(LowerPC_QueryLayerIsNULL(&uExistLayer, lastLayerID) != 0) //层存在，发送上一层数据
				{
					pTempBuf[0] = pBuf[0];
					pTempBuf[1] = pBuf[1];
					pTempBuf[2] = chNum;
					memcpy(&pTempBuf[3], chID, chNum);
					dataLen = 3+chNum;
					tempLen = (len-dataLen);
					memcpy(&pTempBuf[dataLen], &pBuf[dataLen], tempLen); //拷贝工步信息
					dataLen += tempLen; //计算这一次发送所需的数据长度

					cnt++;
					LowerPc_TxFramePack(CMD_LOWER_CH_STEP_LOADING, lastLayerID, pTempBuf, dataLen); //通道工步加载
				}

				chNum = 0; //清零
			}

			chID[chNum] = (INDEX(pBuf[i+3]) % LOWER_CH_TOTAL_NUM) + 1; //计算所在的层的通道号

			chNum++; //通道号计数自增
			tLosePowerData.chTotalNum++; //断电续接加载的通道数

			lastLayerID = layerID; //记录上一次层号

			if(i == INDEX(pBuf[2])) //若为最后一层，最后一个，则发送该层的数据
			{
				if(LowerPC_QueryLayerIsNULL(&uExistLayer, lastLayerID) != 0) //层存在
				{
					pTempBuf[0] = pBuf[0];
					pTempBuf[1] = pBuf[1];
					pTempBuf[2] = chNum;
					memcpy(&pTempBuf[3], chID, chNum);
					dataLen = 3+chNum;
					tempLen = (len-dataLen);
					memcpy(&pTempBuf[dataLen], &pBuf[dataLen], tempLen); //拷贝工步信息
					dataLen += tempLen; //计算这一次发送所需的数据长度

					cnt++;
					LowerPc_TxFramePack(CMD_LOWER_CH_STEP_LOADING, lastLayerID, pTempBuf, dataLen); //通道工步加载
				}
			}
		}

	    vPortFree(pTempBuf); //释放之前申请的内存
	}

	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_LosePowerConnect
 * @brief : 下位机通信：断电续接
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: NULL
 * @date  : 2021/10/20
 * @note  : TODO:从FLASH内读取断电续接数据进行打包发送
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_LosePowerConnect(uint8_t layerID, const uint8_t* pBuf, uint16_t len)
{
	LowerPc_TxFramePack(CMD_LOWER_POWER_OFF_CONNECT, layerID, pBuf, len); //断电续接
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_StartStep
 * @brief : 下位机通信：开始工步
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPc_Send_StartStep(const uint8_t* pBuf, uint16_t len)
{
	uint8_t cnt = 0; //要通信的层数
	cnt = LowerPc_ChAnalyze(CMD_LOWER_START_STEP, pBuf, len); //通道分析及打包
	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_SuspendStep
 * @brief : 下位机通信：暂停工步
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPc_Send_SuspendStep(const uint8_t* pBuf, uint16_t len)
{
	uint8_t cnt = 0; //要通信的层数
	cnt = LowerPc_ChAnalyze(CMD_LOWER_SUSPEND_STEP, pBuf, len); //通道分析及打包
	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_StopStep
 * @brief : 下位机通信：停止工步
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPc_Send_StopStep(const uint8_t* pBuf, uint16_t len)
{
	uint8_t cnt = 0; //要通信的层数
	cnt = LowerPc_ChAnalyze(CMD_LOWER_STOP_STEP, pBuf, len); //通道分析及打包
	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_ContinueStep
 * @brief : 下位机通信：继续工步
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPc_Send_ContinueStep(const uint8_t* pBuf, uint16_t len)
{
	uint8_t cnt = 0; //要通信的层数
	cnt = LowerPc_ChAnalyze(CMD_LOWER_CONTINUE_STEP, pBuf, len); //通道分析及打包
	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_JumpStep
 * @brief : 下位机通信：跳转工步
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPc_Send_JumpStep(const uint8_t* pBuf, uint16_t len)
{
	uint8_t cnt = 0; //要通信的层数
	cnt = LowerPc_ChAnalyze(CMD_LOWER_JUMP_STEP, pBuf, len); //通道分析及打包
	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_Start_C_D
 * @brief : 下位机通信：下发开始充放电指令
 * @param : ch：通道号（范围：1~144）
 *          mode：充放电标志，充电 0，放电 1
 *          adjust：DAC是否需要校准再输出标志位，若要校准输出，则为0；若不需要，则为1
 *          setV：设置电压
 *          setC：设置电流
 * @retval: NULL
 * @date  : 2021/10/20
 * @note  : TODO:用于触控屏通道开启充放电
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_Start_C_D(uint16_t ch, uint8_t mode, uint8_t adjust, float setV, float setC)
{
	uint8_t pBuf[sizeof(C_D_Cmd_T)+2] = {0};         //缓存区
	uint8_t chNum = 1;                               //通道数 - 单通道控制
	uint8_t layerID = 0;                             //计算出通道号所在层号
	uint8_t chID[LOWER_CH_TOTAL_NUM] = {0};          //通道号
	uint8_t tempLen = 0;                             //数据长度

	layerID = (INDEX(ch) / LOWER_CH_TOTAL_NUM); //计算出通道号所在层号
	chID[0] = (INDEX(ch) % LOWER_CH_TOTAL_NUM); //计算出所在层的具体通道号

	pBuf[tempLen++] = chNum;
	pBuf[tempLen++] = chID[0] + 1;
	tCD_Cmd[layerID][chID[0]].mode = mode;
	tCD_Cmd[layerID][chID[0]].adjustFlag = adjust;
	tCD_Cmd[layerID][chID[0]].setVoltage = fabs(setV);
	tCD_Cmd[layerID][chID[0]].setCurrent = fabs(setC);

	memcpy(pBuf+tempLen, &tCD_Cmd[layerID][chID[0]].mode, sizeof(C_D_Cmd_T));
	tempLen += sizeof(C_D_Cmd_T);

	LowerPc_TxFramePack(CMD_LOWER_START_C_D, layerID + 1, pBuf, tempLen); //设置单个通道校准参数
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_Stop_C_D
 * @brief : 下位机通信：下发停止充放电指令 - 单通道
 * @param : ch：通道号（范围：1~144）
 * @retval: NULL
 * @date  : 2021/10/20
 * @note  : TODO:用于触控屏通道关闭充放电
 * ---------------------------------------------------------------------------*/
void LowerPc_Send_Stop_C_D(uint16_t ch)
{
	uint8_t layerID = (INDEX(ch) / LOWER_CH_TOTAL_NUM); //计算出通道号所在层号
	uint8_t pBuf[2] = {0};                           //缓存区
	uint8_t tempLen = 2;                             //数据长度
	uint8_t chNum = 1;                               //通道数 - 单通道控制
	uint8_t chID[LOWER_CH_TOTAL_NUM] = {0};          //通道号

	pBuf[0] = chNum;
	pBuf[1] = chID[0] = (ch % LOWER_CH_TOTAL_NUM); //计算出所在层的具体通道号

	LowerPc_TxFramePack(CMD_LOWER_STOP_C_D, layerID + 1, pBuf, tempLen); //下发停止充放电指令
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_ClearWarning
 * @brief : 下位机通信：清除警告
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPc_Send_ClearWarning(const uint8_t* pBuf, uint16_t len)
{
	uint8_t cnt = 0; //要通信的层数
	cnt = LowerPc_ChAnalyze(CMD_LOWER_CLEAR_WARNING, pBuf, len); //通道分析及打包
	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_Reset
 * @brief : 下位机通信：复位指令
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPc_Send_Reset(const uint8_t* pBuf, uint16_t len)
{
	uint8_t cnt = 0; //要通信的层数
	cnt = LowerPc_ChAnalyze(CMD_LOWER_RESET, pBuf, len); //通道分析及打包
	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Send_ChEnanble
 * @brief : 下位机通信：通道禁用/启用
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: cnt：要通信的层数
 * @date  : 2021/10/21
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LowerPc_Send_ChEnanble(const uint8_t* pBuf, uint16_t len)
{
	uint8_t cnt = 0; //要通信的层数
	cnt = LowerPc_ChAnalyze(CMD_LOWER_CH_ENABLE, pBuf, len); //通道分析及打包
	return cnt;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_TxDataTranTask
 * @brief : 与下位机通信发送数据传输任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/10/21
 * @note  : 将要发送的数据写入环形队列
 * ---------------------------------------------------------------------------*/
void LowerPc_TxDataTranTask(void *parameter)
{
	EventBits_t uxBitsToWaitFor = 0;

#if (LOWER_PC_TEST == 0)
	//TODO:这里要注意下位机上电初始化完成后的时间，可延时或者多次轮询已确定是否读取正常
	vTaskDelay(2000); //等待2秒
	while(xSemaphoreTake(LowerPcSyncSem, 200) == errQUEUE_EMPTY) //等待下位机上电初始化成功
	{
		ThreadTime(4);
		uExistLayer.layerNum = 0x00; //初始值，全部层不存在
		LowerPc_Send_ReadBoardInfo(); //读取板卡信息（默认所有层数都会轮询一遍）
		vTaskDelay(1000);
	}

	/* 再次确认存在层 */
	LowerPc_Send_ReadBoardInfo(); //读取板卡信息（默认所有层数都会轮询一遍）
	if(xSemaphoreTake(LowerPcSyncSem, 200) != errQUEUE_EMPTY) //确认成功
	{
		DEBUG_PRINTF("\r\n QUERY LAYER OK! \r\n");
	}
#endif

#if __USER_LOSE_POWER
	/* 断电续接 */
	LosePower_ReadData(); //断电续接判断
	vTaskDelay(100);
#endif

	tCommPro.Comm_Sync_Flag = COMM_OK; //通信同步

	tCommPro.Lose_Power_Comm_Flag = 1;

	while(1)
	{
		ThreadTime(5);

	    //节点可操作事件触发
	    //逻辑或等待事件，退出时清除事件
	    //永久阻塞等待事件
    	uxBitsToWaitFor = xEventGroupWaitBits(TimingCommEvent,
    			                              TIMING_COMM_EVENT,
											  pdTRUE, pdFALSE,
											  portMAX_DELAY);

		/* 获取整板通道实时数据 */
	    if((TIMING_COMM_GET_BOARD_DATA_EVENT & uxBitsToWaitFor) == TIMING_COMM_GET_BOARD_DATA_EVENT)
	    {
	    	LowerPc_Send_GetBoardChData();
	    }
	}
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_TxTask
 * @brief : 与下位机通信发送任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_TxTask(void *parameter)
{
    uint8_t* txBuf = (uint8_t*)pvPortMalloc(LOWER_PC_BUF_MAX_LEN); //申请内存缓冲区指针
	if(txBuf == NULL)
	{
		vTaskDelay(10);
		DEBUG_PRINTF("\r\n ERROR! \r\n");
	}
	memset(txBuf, 0, LOWER_PC_BUF_MAX_LEN); //清零

	uint16_t txLen = 0;
	QueueStatusTypeDef_E sta = QUEUE_OK;

	/* 用于保存上次时间，调用后系统自动更新 */
	static portTickType PreviousWakeTime;
	/* 设置延时时间，将时间转为节拍数 */
	const portTickType TimeIncrement = pdMS_TO_TICKS(1); //1ms
	const portTickType sendTimeIncrement = pdMS_TO_TICKS(20); //20ms
	/* 获取当前系统时间 */
	PreviousWakeTime = xTaskGetTickCount();

	while(1)
	{
		if(uQueueGetDataNum(&tLowerPcTxQueue) != 0)
		{
			while(uQueueGetDataNum(&tLowerPcTxQueue) != 0)
			{
				ThreadTime(6);

				//从FIFO读取数据
				sta = uQueueRead(&tLowerPcTxQueue, (uint8_t*)txBuf, &txLen); //从FIFO读取数据

				if(sta == QUEUE_OK)
				{
					//发送数据
					LowerPC_SendData(txBuf, txLen);
				}

			    /* 调用绝对延时函数,任务时间间隔为20个tick */
				vTaskDelayUntil(&PreviousWakeTime, sendTimeIncrement);
			}
		}

	    /* 调用绝对延时函数,任务时间间隔为1个tick */
		vTaskDelayUntil(&PreviousWakeTime, TimeIncrement);
	}
}


//-----------------------------------------------------------------------------
//                              TODO:接收处理
//-----------------------------------------------------------------------------


/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Analyze_ReadBoardInfo
 * @brief : 下位机通信：分析 - 读取板卡信息
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: NULL
 * @date  : 2021/10/21
 * @note  :
 * ---------------------------------------------------------------------------*/
static void LowerPc_Analyze_ReadBoardInfo(const uint8_t* pBuf, uint16_t len)
{
	uint8_t layerID = INDEX(pBuf[1]); //所在层号减1为数组索引

	if(layerID == 0)
	{
		char buf[16] = {0};

		uExistLayer.LayerBit.existLayer1 = 1;

		tLayerInfo.rateVoltage = Data2U8ToU16(pBuf[6], pBuf[7]); //标称充放电电压规格
		tLayerInfo.rateCurrent = Data2U8ToU16(pBuf[8], pBuf[9]); //标称充放电电流规格

		sprintf((char*)buf,"%dV%dA-1978", tLayerInfo.rateVoltage, tLayerInfo.rateCurrent);
		DEBUG_PRINTF((char*)buf);
		memcpy(&tDeviceInfo.deviceSPEC[0], (uint8_t*)buf, sizeof(buf));
	}
	else if(layerID == 1)
	{
		uExistLayer.LayerBit.existLayer2 = 1;
	}
	else if(layerID == 2)
	{
		uExistLayer.LayerBit.existLayer3 = 1;
	}
	else if(layerID == 3)
	{
		uExistLayer.LayerBit.existLayer4 = 1;
	}
	else if(layerID == 4)
	{
		uExistLayer.LayerBit.existLayer5 = 1;
	}
	else if(layerID == 5)
	{
		uExistLayer.LayerBit.existLayer6 = 1;
	}
	else if(layerID == 6)
	{
		uExistLayer.LayerBit.existLayer7 = 1;
	}
	else if(layerID == 7)
	{
		uExistLayer.LayerBit.existLayer8 = 1;
	}

	tChNum.chTotalNum = pBuf[5]; //每层通道数
	tDeviceInfo.chTotalNum = tLayerInfo.chTotalNum = tChNum.chTotalNum; //每层通道数

	tDeviceInfo.tlayerInfo[layerID].layerID = layerID+1; //记录层号

	memcpy(&tDeviceInfo.tlayerInfo[layerID].DriverVersion, &pBuf[10], 16);
	memcpy(&tDeviceInfo.tlayerInfo[layerID].Description, &pBuf[25], 40);
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Analyze_GetBoardChData
 * @brief : 下位机通信：分析 - 获取整板通道实时数据
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: NULL
 * @date  : 2021/10/21
 * @note  :
 * ---------------------------------------------------------------------------*/
static void LowerPc_Analyze_GetBoardChData(const uint8_t* pBuf, uint16_t len)
{
	const uint16_t tempLen = sizeof(ChannelRecordData_T);
	uint8_t layerID = INDEX(pBuf[1]); //所在层号减1为数组索引
	uint8_t tempChID = 0; //通道号
	uint8_t chID = 0;
	uint32_t packLen = sizeof(ChannelRealTimeData_T); //包长度

	uLayerAbnormalSta[layerID].sta = Data2U8ToU16(pBuf[5], pBuf[6]); //层异常状态

	tChRecordData.chTotalDataNum[layerID] = pBuf[7]; //通道数量

	for(uint8_t i=0; i<tChRecordData.chTotalDataNum[layerID]; i++)
	{
		chID = pBuf[9+(i*tempLen)] + ((tChNum.chTotalNum * (layerID)) - 1); //更新通道号给上位机
		memcpy(&tChRecordData.tChReData[chID].chDataType, &pBuf[8+(i*tempLen)], tempLen); //拷贝数据
		tempChID = INDEX(tChRecordData.tChReData[chID].tChData.chID);
		tChRecordData.tChReData[chID].tChData.chID = chID + 1;

		if(tChRecordData.tChReData[chID].chDataType == 0x00) //实时数据
		{
			//记录通道是否存在
			tExistCh.existCh[layerID][tempChID] = 0x01;

			memcpy(&tChRealTimeDat[chID].chID, &tChRecordData.tChReData[chID].tChData.chID, packLen);
		}
		else if(tChRecordData.tChReData[chID].chDataType == 0x01) //记录数据
		{
			memcpy(&tChRecordTestDat[chID].chID, &tChRecordData.tChReData[chID].tChData.chID, packLen); //测试用

			uQueueWrite(&tRecordDataQueue, (uint8_t*)&tChRecordData.tChReData[chID].tChData.chID, packLen); //写入队列
			uRecordDataSave((uint8_t*)&tChRecordData.tChReData[chID].tChData.chID, packLen);
		}
	}
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Analyze_GetBoardChAdc
 * @brief : 下位机通信：分析 - 获取整板通道ADC原始数据
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: NULL
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
static void LowerPc_Analyze_GetBoardChAdc(const uint8_t* pBuf, uint16_t len)
{
	uint8_t layerID = INDEX(pBuf[1]); //所在层号减1为数组索引
	uint16_t chTempCnt = (tChNum.chTotalNum * (layerID));
	uint32_t packLen = sizeof(ChannelRawData_T); //包长度

	for(uint8_t i=0; i<pBuf[5]; i++)
	{
		memcpy(&tChRawData[i], &pBuf[6], packLen);
		tChRawData[i].chID += chTempCnt;  //更新通道号
	}
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_Analyze_GetChCalData
 * @brief : 下位机通信：分析 - 获取单个通道校准参数，暂存于 tGetChCalData 结构体中
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval: NULL
 * @date  : 2021/10/21
 * @note  :
 * ---------------------------------------------------------------------------*/
static void LowerPc_Analyze_GetChCalData(const uint8_t* pBuf, uint16_t len)
{
	uint8_t i = 0;
	uint16_t tempLen = 5;                  //数据长度存放区
	uint8_t layerID = INDEX(pBuf[1]);      //所在层号减1为数组索引
	uint8_t chID = INDEX(pBuf[tempLen++]); //通道号减1为数组索引

	tGetChCalData[layerID][chID].chID = chID + 1; //通道号
	tGetChCalData[layerID][chID].calPolarity = pBuf[tempLen++]; //校准极性类型（0为正极性、1为负极性）

	tGetChCalData[layerID][chID].chargeCurAdcPointsNum = pBuf[tempLen++]; //充电电流ADC校准点数
	for(i=0; i<tGetChCalData[layerID][chID].chargeCurAdcPointsNum; i++)
	{
		tGetChCalData[layerID][chID].chargeCurAdcPoints[i].currentSamp = pBuf[tempLen++]; //采样电流值
		tGetChCalData[layerID][chID].chargeCurAdcPoints[i].currentTrue = pBuf[tempLen++]; //真实电流值
	}

	tGetChCalData[layerID][chID].dischargeCurAdcPointsNum = pBuf[tempLen++]; //放电电流ADC校准点数
	for(i=0; i<tGetChCalData[layerID][chID].dischargeCurAdcPointsNum; i++)
	{
		tGetChCalData[layerID][chID].dischargeCurAdcPoints[i].currentSamp = pBuf[tempLen++]; //采样电流值
		tGetChCalData[layerID][chID].dischargeCurAdcPoints[i].currentTrue = pBuf[tempLen++]; //真实电流值
	}

	tGetChCalData[layerID][chID].chargeCurDacPointsNum = pBuf[tempLen++]; //充电电流DAC校准点数
	for(i=0; i<tGetChCalData[layerID][chID].chargeCurDacPointsNum; i++)
	{
		tGetChCalData[layerID][chID].chargeCurDacPoints[i].currentSet = pBuf[tempLen++];  //设定电流值
		tGetChCalData[layerID][chID].chargeCurDacPoints[i].currentTrue = pBuf[tempLen++]; //真实电流值
	}

	tGetChCalData[layerID][chID].dischargeCurDacPointsNum = pBuf[len++]; //放电电流DAC校准点数
	for(i=0; i<tGetChCalData[layerID][chID].dischargeCurDacPointsNum; i++)
	{
		tGetChCalData[layerID][chID].dischargeCurDacPoints[i].currentSet = pBuf[tempLen++];  //设定电流值
		tGetChCalData[layerID][chID].dischargeCurDacPoints[i].currentTrue = pBuf[tempLen++]; //真实电流值
	}

	tGetChCalData[layerID][chID].chargeVolAdcPointsNum = pBuf[tempLen++]; //充电电压ADC校准点数
	for(i=0; i<tGetChCalData[layerID][chID].chargeVolAdcPointsNum; i++)
	{
		tGetChCalData[layerID][chID].chargeVolAdcPoints[i].voltageSamp = pBuf[tempLen++]; //采样电压值
		tGetChCalData[layerID][chID].chargeVolAdcPoints[i].voltageTrue = pBuf[tempLen++]; //真实电压值
	}

	tGetChCalData[layerID][chID].dischargeVolAdcPointsNum = pBuf[tempLen++]; //放电电压ADC校准点数
	for(i=0; i<tGetChCalData[layerID][chID].dischargeVolAdcPointsNum; i++)
	{
		tGetChCalData[layerID][chID].dischargeVolAdcPoints[i].voltageSamp = pBuf[tempLen++]; //采样电压值
		tGetChCalData[layerID][chID].dischargeVolAdcPoints[i].voltageTrue = pBuf[tempLen++]; //真实电压值
	}

	tGetChCalData[layerID][chID].chargeVolDacPointsNum = pBuf[tempLen++]; //充电电压DAC校准点数
	for(i=0; i<tGetChCalData[layerID][chID].chargeVolDacPointsNum; i++)
	{
		tGetChCalData[layerID][chID].chargeVololDacPoints[i].voltageSet = pBuf[tempLen++];  //设定电压值
		tGetChCalData[layerID][chID].chargeVololDacPoints[i].voltageTrue = pBuf[tempLen++]; //真实电压值
	}

	tGetChCalData[layerID][chID].dischargeVolDacPointsNum = pBuf[tempLen++]; //充电电压DAC校准点数
	for(i=0; i<tGetChCalData[layerID][chID].dischargeVolDacPointsNum; i++)
	{
		tGetChCalData[layerID][chID].dischargeVolDacPoints[i].voltageSet = pBuf[tempLen++];  //设定电压值
		tGetChCalData[layerID][chID].dischargeVolDacPoints[i].voltageTrue = pBuf[tempLen++]; //真实电压值
	}
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_RxFrameAnalyze
 * @brief : 下位机通信：接收数据解析
 * @param : pBuf：数据缓存区指针
 *          len：数据长度
 * @retval:
 * @date  : 2021/10/18
 * @note  :
 * ---------------------------------------------------------------------------*/
static uint8_t LowerPc_RxFrameAnalyze(const uint8_t* pBuf, uint16_t len)
{
	CommStatus_E sta = COMM_OK;
	uint16_t checkSum = 0;

	/* 包头包尾判断 */
	if((pBuf[0] == LOWER_FRAME_HEAD) && (pBuf[len-1] == LOWER_FRAME_END))
	{
		/* 校验和判断 */
	    checkSum = Data2U8ToU16(pBuf[len-3], pBuf[len-2]);
		if(checkSum == CheckSum16(&pBuf[1], len-4))
		{
			switch(pBuf[2])
			{
				case CMD_LOWER_READ_BOARD_INFO: //读取板卡信息
				{
					LowerPc_Analyze_ReadBoardInfo(pBuf, len);

					if(uExistLayer.layerNum != 0x00) //有层存在才释放信号量
					{
						xSemaphoreGive(LowerPcSyncSem); //释放信号量
					}

					uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_ReadBoardInfo = COMM_OK; //通信正常
				} break;
				case CMD_LOWER_GET_BOARD_CH_DATA: //获取整板通道实时数据
				{
					LowerPc_Analyze_GetBoardChData(pBuf, len);

					uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_GetBoardChData = COMM_OK; //通信正常
				} break;
				case CMD_LOWER_GET_BOARD_CH_ADC: //获取整板通道ADC原始数据
				{
					LowerPc_Analyze_GetBoardChAdc(pBuf, len);

					uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_GetBoardChAdc = COMM_OK; //通信正常
				} break;
				case CMD_LOWER_GET_CH_CAL_DATA: //获取单个通道校准参数
				{
					LowerPc_Analyze_GetChCalData(pBuf, len);

					uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_GetChCalData = COMM_OK; //通信正常
				} break;
				case CMD_LOWER_SET_CH_CAL_DATA: //设置单个通道校准参数
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_SetChCalData = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_SetChCalData = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_RESET_CH_CAL_DATA: //重置单个通道校准参数
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_ResetChCalData = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_ResetChCalData = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_GLOBAL_SETTING: //全局设置
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_GlobalSetting = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_GlobalSetting = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_CH_STEP_LOADING: //通道工步加载
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_ChStepLoading = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_ChStepLoading = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_POWER_OFF_CONNECT: //断电续接
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_PowerOffConnect = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_PowerOffConnect = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_START_STEP: //开始工步
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_StartStep = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_StartStep = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_SUSPEND_STEP: //暂停工步
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_SuspendStep = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_SuspendStep = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_STOP_STEP: //停止工步
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_StopStep = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_StopStep = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_CONTINUE_STEP: //继续工步
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_ContinueStep = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_ContinueStep = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_JUMP_STEP: //跳转工步
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_JumpStep = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_JumpStep = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_START_C_D: //下发开始充放电指令
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_Start_C_D = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_Start_C_D = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_STOP_C_D: //下发停止充放电指令
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_Stop_C_D = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_Stop_C_D = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_CLEAR_WARNING: //清除警告
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_ClearWarning = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_ClearWarning = COMM_OK; //通信正常
					}
				} break;
				case CMD_LOWER_RESET: //复位指令
				{
					if(pBuf[5] == NACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_Reset = COMM_ERROR; //通信异常
					}
					else if(pBuf[5] == ACK)
					{
						uLowerAckCommSta[INDEX(pBuf[1])].CommStaBit.sta_Reset = COMM_OK; //通信正常
					}
				} break;
				default: break;
			}

			xSemaphoreGive(LowerPcSem); //释放信号量
		}
	}

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_RxDataTranTask
 * @brief : 与下位机通信接收数据传输任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/10/21
 * @note  : 将中断接收的数据入环形队列
 * ---------------------------------------------------------------------------*/
void LowerPc_RxDataTranTask(void *parameter)
{
	while(1)
	{
		xSemaphoreTake(LowerPcRxSem, portMAX_DELAY); //等待信号量

		ThreadTime(2);

		uQueueWrite(&tLowerPcRxQueue, (uint8_t*)&tLowerPC_Rx.RxDataBuf, tLowerPC_Rx.RxDataLen); //写入环形队列

		memset(&tLowerPC_Rx, 0, sizeof(LowerPC_RxData_T)); //清空
	}
}

/*-----------------------------------------------------------------------------
 * @name  : LowerPc_RxTask
 * @brief : 与下位机通信接收数据处理任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_RxTask(void *parameter)
{
    uint8_t* rxBuf = (uint8_t*)pvPortMalloc(LOWER_PC_BUF_MAX_LEN); //申请内存缓冲区指针
	if(rxBuf == NULL)
	{
		vTaskDelay(10);
		DEBUG_PRINTF("\r\n ERROR! \r\n");
	}
	memset(rxBuf, 0, LOWER_PC_BUF_MAX_LEN); //清零

	uint16_t rxLen = 0;
	QueueStatusTypeDef_E sta = QUEUE_OK;

	/* 用于保存上次时间，调用后系统自动更新 */
	static portTickType PreviousWakeTime;
	/* 设置延时时间，将时间转为节拍数 */
	const portTickType TimeIncrement = pdMS_TO_TICKS(1); //1ms
	const portTickType cycleTimeIncrement = pdMS_TO_TICKS(2); //2ms
	/* 获取当前系统时间 */
	PreviousWakeTime = xTaskGetTickCount();

	while(1)
	{
		if(uQueueGetDataNum(&tLowerPcRxQueue) != 0)
		{
			while(uQueueGetDataNum(&tLowerPcRxQueue) != 0)
			{
				ThreadTime(3);
				//从FIFO读取数据
				sta = uQueueRead(&tLowerPcRxQueue, (uint8_t*)rxBuf, &rxLen); //从FIFO读取数据

				if(sta == QUEUE_OK)
				{
					LowerPc_RxFrameAnalyze(rxBuf, rxLen);
				}

				/* 调用绝对延时函数,任务时间间隔为2个tick */
				vTaskDelayUntil(&PreviousWakeTime, cycleTimeIncrement);
			}
		}

		/* 调用绝对延时函数,任务时间间隔为1个tick */
		vTaskDelayUntil(&PreviousWakeTime, TimeIncrement);
	}
}


//-----------------------------------------------------------------------------
//                              TODO:其他处理
//-----------------------------------------------------------------------------


/*-----------------------------------------------------------------------------
 * @name  : LowerPc_DataAnalyzeTask
 * @brief : 记录数据解析
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/10/22
 * @note  :
 * ---------------------------------------------------------------------------*/
void LowerPc_DataAnalyzeTask(void *parameter)
{
	uint8_t i = 0, chCnt = 0, layerCnt = 0; //计数
	uint8_t offLineCnt[8] = {0}; //计数
	uint8_t offLineSta[8] = {0}; //离线状态

	uint32_t packLen = (sizeof(ChannelRealTimeData_T) - 1); //包长度

	/* 用于保存上次时间，调用后系统自动更新 */
	static portTickType PreviousWakeTime;
	/* 设置延时时间，将时间转为节拍数 */
	const portTickType TimeIncrement = pdMS_TO_TICKS(200); //200ms
	/* 获取当前系统时间 */
	PreviousWakeTime = xTaskGetTickCount();

#if LOWER_PC_TEST
	tCommAckSta.ReadDeviceInfoSta = ACK;
	tCommAckSta.GetChRealTimeDataSta = ACK;

	tDeviceInfo.layerTotalNum = tChNum.layerTotalNum = 8; //层数
	tChNum.chTotalNum = 8; //每层通道数
	tDeviceInfo.chTotalNum = tLayerInfo.chTotalNum = tChNum.chTotalNum; //每层通道数

	tDeviceInfo.tlayerInfo[0].layerID = 1; //记录层号
	tDeviceInfo.tlayerInfo[1].layerID = 2; //记录层号
	tDeviceInfo.tlayerInfo[2].layerID = 3; //记录层号
	tDeviceInfo.tlayerInfo[3].layerID = 4; //记录层号
	tDeviceInfo.tlayerInfo[4].layerID = 5; //记录层号
	tDeviceInfo.tlayerInfo[5].layerID = 6; //记录层号
	tDeviceInfo.tlayerInfo[6].layerID = 7; //记录层号
	tDeviceInfo.tlayerInfo[7].layerID = 8; //记录层号

	uExistLayer.layerNum = 0xFF;
#endif

	while(1)
	{
    	/* 存在的通道判断 */
		for(i=0; i<LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM; i++)
		{
			if(LowerPC_QueryChIsNULL(&tExistCh, i) == 0)
			{
				continue; //查询不到通道存在则跳过该次循环
			}

			chCnt++;
		}
		if(chCnt == 0)
		{
			//不存在通道
#if (LOWER_PC_TEST == 0)
			tCommAckSta.GetChRealTimeDataSta = NACK;
#endif
		}
		else
		{
			//存在通道
			tCommAckSta.GetChRealTimeDataSta = ACK;
			chCnt = 0; //清零，方便下一次计数
		}

		/* 存在的层判断 */
		for(uint8_t i=0; i<LOWER_LAYER_TOTAL_NUM; i++)
		{
			if(LowerPC_QueryLayerIsNULL(&uExistLayer, i+1) == 0)
			{
				continue; //查询不到层存在则跳过该次循环
			}

			layerCnt++; //层数计数
			tDeviceInfo.tlayerInfo[i].layerID = i+1; //层号

			/* 离线状态 */
			if(uLowerCommSta[i].CommStaBit.sta_GetBoardChData == 1)
			{
				if(offLineSta[i] < 0xFF)
				{
					offLineSta[i]++; //计时
				}
				else
				{
					offLineSta[i] = 0xFF;
				}
			}
			else
			{
				offLineCnt[i] = 0; //清零
				offLineSta[i] = 0; //清零
			}

			if(offLineCnt[i] == 0)
			{
				if(offLineSta[i] > 25) //超过5s则为离线状态
				{
					offLineCnt[i] = 1;
					/* 清除该层通道数据，保留通道ID */
					for(uint8_t a=0; a<LOWER_CH_TOTAL_NUM; a++)
					{
						memset(&tChRealTimeDat[i*LOWER_CH_TOTAL_NUM+a].chStatus, 0, packLen);
					}
				}
			}
		}
		if(layerCnt == 0)
		{
			//不存在层
#if (LOWER_PC_TEST == 0)
			tCommAckSta.ReadDeviceInfoSta = NACK;
#endif
		}
		else
		{
			//存在层
			tCommAckSta.ReadDeviceInfoSta = ACK;
			tDeviceInfo.layerTotalNum = tChNum.layerTotalNum = layerCnt; //层数
			layerCnt = 0; //清零，方便下一次计数
		}

		/* 调用绝对延时函数,任务时间间隔为200个tick */
		vTaskDelayUntil(&PreviousWakeTime, TimeIncrement);
	}
}

