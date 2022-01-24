/*
 * user_lose_power.c
 *
 *  Created on: 2021年11月8日
 *      Author: Lyric
 */

#ifdef USER_CONFIG
	#include "user_config.h"
#endif

/*-----------------------------------------------------------------------------
 * @name  : LosePower_SaveData
 * @brief : 断电续接保存信息
 * @param :
 * @retval: 0:保存正常，1:保存错误
 * @date  : 2021/11/10
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t LosePower_SaveData(void)
{
	uint8_t i = 0;
	uint8_t sta = 1;
	uint32_t wSize = 0;

	xEventGroupClearBits(TimingCommEvent, TIMING_COMM_SYNC_EVENT); //TODO:清除通信同步事件

	uQueueClearALL(&tLowerPcTxQueue); //清空队列

	tLosePowerData.flag = 1; //断电标志

	for(i=0; i<LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM; i++)
	{
		if(LowerPC_QueryChIsNULL(&tExistCh, i) == 0)
		{
			continue; //查询不到通道存在则跳过该次循环
		}

		tLosePowerData.tInfo[i].chID          = tChRecordTestDat[i].chID;          //通道号
		tLosePowerData.tInfo[i].chStatus      = tChRecordTestDat[i].chStatus;      //通道状态
		tLosePowerData.tInfo[i].stepSerialNum = tChRecordTestDat[i].stepSerialNum; //工步序号
		tLosePowerData.tInfo[i].stepID        = tChRecordTestDat[i].stepID;        //当前工步号
		tLosePowerData.tInfo[i].cycleNum      = tChRecordTestDat[i].cycleNum;      //循环次数
		tLosePowerData.tInfo[i].stepName      = tChRecordTestDat[i].stepName;      //工步名称
		tLosePowerData.tInfo[i].stepRuntime   = tChRecordTestDat[i].stepRuntime;   //当前工步运行时间
		tLosePowerData.tInfo[i].capacity      = tChRecordTestDat[i].capacity;      //容量
		tLosePowerData.tInfo[i].energy        = tChRecordTestDat[i].energy;        //能量
		tLosePowerData.tInfo[i].chWarnStatus  = tChRecordTestDat[i].chWarnStatus;  //通道异常状态
	}

	memcpy(&tLosePowerData.tCh, &tExistCh, sizeof(ExistCh_T)); //复制存在的通道

	NAND_Write_File(NAND_STEPLOADINFO, &tStepLoadBuf, sizeof(StepLoadBuf_T), &wSize); //保存工步加载信息

	if(wSize == sizeof(StepLoadBuf_T))
		sta = 0;

	NAND_Write_File(NAND_LOSEPOWERINFO, &tLosePowerData, sizeof(LosePowerData_T), &wSize); //保存断电续接信息

	if(wSize == sizeof(LosePowerData_T))
		sta = 0;

	uRecordDataSaveLose(); //断电时保存记录数据

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : LosePower_ReadData
 * @brief : 断电续接读取信息
 * @param :
 * @retval:
 * @date  : 2021/11/10
 * @note  :
 * ---------------------------------------------------------------------------*/
void LosePower_ReadData(void)
{
	uint8_t i = 0;
	uint8_t layerID = 0, lastLayerID = 0;   //层号、上一次层号
	uint8_t chNum = 0;                      //通道数
	uint8_t chTotalNum = LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM; //总通道数
	uint32_t rSize = 0, wSize = 0;

	uint8_t* pBuf = (uint8_t *)mymalloc(SDRAM_POOL, LOWER_PC_BUF_MAX_LEN); //申请内存缓冲区指针 - 用户动态申请内存函数
	if(pBuf == NULL)
	{
		vTaskDelay(10);
		DEBUG_PRINTF("\r\n ERROR! \r\n");
	}
	memset(pBuf, 0, LOWER_PC_BUF_MAX_LEN); //清零
	uint16_t tempLen = 0;  //数据帧长度
	uint16_t tempAddr = 0; //位置记录


	NAND_Read_File(NAND_STEPLOADINFO, &tStepLoadBuf, sizeof(StepLoadBuf_T), &rSize); //读取工步加载信息

	LowerPc_Send_GlobalSetting((uint8_t*)&tStepLoadBuf.tGlobalPram, sizeof(GlobalPramSet_T)); //全局设置
	vTaskDelay(10);

	LowerPc_Send_StepLoading((uint8_t*)&tStepLoadBuf.buf, tStepLoadBuf.len); //工步加载
	vTaskDelay(10);

	NAND_Read_File(NAND_LOSEPOWERINFO, &tLosePowerData, sizeof(LosePowerData_T), &rSize); //读取断电续接信息

	if((rSize == sizeof(LosePowerData_T)) && (tLosePowerData.flag == 1))
	{
		for(i=0; i<chTotalNum; i++)
		{
			if(LowerPC_QueryChIsNULL(&tLosePowerData.tCh, i) == 0)
			{
				continue; //查询不到通道存在则跳过该次循环
			}

			layerID = (INDEX(tLosePowerData.tInfo[i].chID) / LOWER_CH_TOTAL_NUM) + 1; //计算所在的层

			if((lastLayerID != 0) && (lastLayerID != layerID)) //通道所对应的层发生改变，既转移至下一层
			{
				if(LowerPC_QueryLayerIsNULL(&uExistLayer, lastLayerID) != 0) //层存在
				{
					pBuf[tempLen] = chNum;
					tempAddr = INDEX(lastLayerID)*LOWER_CH_TOTAL_NUM;
					memcpy(pBuf+tempLen, &tLosePowerData.tInfo[tempAddr].chID, sizeof(LosePowerInfo_T)*chNum);
					tempLen += sizeof(LosePowerInfo_T)*chNum;

					LowerPc_Send_LosePowerConnect(lastLayerID, pBuf, tempLen); //打包
				}

				tempLen = 0; //清零
				chNum = 0; //清零
			}

			tLosePowerData.tInfo[i].chID = (INDEX(tLosePowerData.tInfo[i].chID) % LOWER_CH_TOTAL_NUM) + 1; //计算所在的层的通道号

			chNum++; //通道号计数自增

			lastLayerID = layerID; //记录上一次层号

			if(i == INDEX(chTotalNum)) //若为最后一层，最后一个
			{
				if(LowerPC_QueryLayerIsNULL(&uExistLayer, lastLayerID) != 0) //层存在
				{
					pBuf[tempLen] = chNum;
					tempAddr = INDEX(lastLayerID)*LOWER_CH_TOTAL_NUM;
					memcpy(pBuf+tempLen, &tLosePowerData.tInfo[tempAddr].chID, sizeof(LosePowerInfo_T)*chNum);
					tempLen += sizeof(LosePowerInfo_T)*chNum;

					LowerPc_Send_LosePowerConnect(lastLayerID, pBuf, tempLen); //打包
				}
			}
		}
	}

	tLosePowerData.flag = 0;

	NAND_Write_File(NAND_LOSEPOWERINFO, &tLosePowerData, sizeof(LosePowerData_T), &wSize); //保存断电续接信息

	vTaskDelay(10);
}

/*-----------------------------------------------------------------------------
 * @name  : LosePower_CtrlTask
 * @brief : 断电续接任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/11/10
 * @note  : 简单测试用（野火开发板）：
            按键 KEY 1 是写入，KEY 2 是读取。
 * ---------------------------------------------------------------------------*/
void LosePower_CtrlTask(void *parameter)
{

	while(1)
	{
		ThreadTime(10);
#if 0
    	/* 检测是否有按键按下 */
    	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET )
    	{
    		/* 等待按键释放 */
    		while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
    			vTaskDelay(10);

    		LosePower_SaveData();
        }

    	/* 检测是否有按键按下 */
    	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET )
    	{
    		/* 等待按键释放 */
    		while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
    			vTaskDelay(10);

    		LosePower_ReadData();
        }
#endif
    	vTaskDelay(50);
	}
}
