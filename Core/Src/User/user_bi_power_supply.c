/*-----------------------------------------------------------------------------
 * @file        : user_bi_power_supply.c
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2021/09/30
 * @date changed: 2021/09/30
 * @brief       : 图为科技 TBM系列三相AC-DC电源模块
 *              : 波特率：500K
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#ifdef USER_CONFIG
	#include "user_config.h"
#endif


/*-----------------------------------------------------------------------------
 * @name  : PowerSupplySendMsg
 * @brief : 发送函数
 * @param : pBuf：发送数据指针
			len：发送数据长度(最大为8),可设置为FDCAN_DLC_BYTES_2~FDCAN_DLC_BYTES_8
			id：CAN通信ID
 * @retval: OK：0    Error：1
 * @date  : 2021/09/30
 * @note  : 一问一答，带错误重发
 * ---------------------------------------------------------------------------*/
uint8_t PowerSupplySendMsg(uint8_t* pBuf, uint32_t len, uint32_t id)
{
    uint8_t TransmitFLAG = 0, TranCNT = 0;

    /* 错误重发 */
	do{
		TransmitFLAG = FDCAN1_Send_Msg(pBuf, len, id);
		TranCNT++;
	}while(((xSemaphoreTake(PowerSupplySem, 50) != pdTRUE) && TranCNT < 5) || TransmitFLAG != HAL_OK);

	//DEBUG：查看失败次数
	if(TranCNT >= 5)
	{
		return HAL_ERROR ;
	}
	else
	{
		return HAL_OK;
	}
}

/*-----------------------------------------------------------------------------
 * @name  : PowerSupplySend
 * @brief : 指令发送函数
 * @param : eCmd：指令码
			id：电源号
 * @retval: OK：0    Error：1
 * @date  : 2021/09/30
 * @note  : 一问一答，带错误重发
 *          TODO:2021/09/30 未测试
 * ---------------------------------------------------------------------------*/
uint8_t PowerSupplySend(PowerSupply_CMD_E eCmd, uint8_t id)
{
    xSemaphoreTake(PowerSupplyMutex, portMAX_DELAY); //互斥量闭锁

    uint8_t sta = 0;

    switch(eCmd)
    {
		case ALARM_CMD: //告警信息
		{
		    can1TxData.data8[0] = BRD_FRAME;
		    can1TxData.data8[1] = RQ_FRAME + ALARM_SERVICE_CODE + id;
		    can1TxData.data8[2] = 0x00;
		    can1TxData.data8[3] = 0x00;
		    can1TxData.data8[4] = 0x00;
		    can1TxData.data8[5] = 0x00;
		    can1TxData.data8[6] = 0x00;
		    can1TxData.data8[7] = 0x00;
		    sta = PowerSupplySendMsg(can1TxData.data8, FDCAN_DLC_BYTES_8, ALARM_FRAME_ID + id);
		} break;
		case STATUS_CMD: //状态信息
		{
		    can1TxData.data8[0] = BRD_FRAME;
		    can1TxData.data8[1] = RQ_FRAME + STATUS_SERVICE_CODE + id;
		    can1TxData.data8[2] = 0x00;
		    can1TxData.data8[3] = 0x00;
		    can1TxData.data8[4] = 0x00;
		    can1TxData.data8[5] = 0x00;
		    can1TxData.data8[6] = 0x00;
		    can1TxData.data8[7] = 0x00;
		    sta = PowerSupplySendMsg(can1TxData.data8, FDCAN_DLC_BYTES_8, STATUS_FRAME_ID + id);
		} break;
		case ANALOG_DC_CMD: //模拟量信息直流侧信息
		{
		    can1TxData.data8[0] = BRD_FRAME;
		    can1TxData.data8[1] = RQ_FRAME + ANALOG_DC_SERVICE_CODE + id;
		    can1TxData.data8[2] = 0x00;
		    can1TxData.data8[3] = 0x00;
		    can1TxData.data8[4] = 0x00;
		    can1TxData.data8[5] = 0x00;
		    can1TxData.data8[6] = 0x00;
		    can1TxData.data8[7] = 0x00;
		    sta = PowerSupplySendMsg(can1TxData.data8, FDCAN_DLC_BYTES_8, ANALOG_FRAME_ID + id);
		} break;
		case ANALOG_AC_V_CMD: //模拟量信息交流侧电压信息
		{
		    can1TxData.data8[0] = BRD_FRAME;
		    can1TxData.data8[1] = RQ_FRAME + ANALOG_AC_V_SERVICE_CODE + id;
		    can1TxData.data8[2] = 0x00;
		    can1TxData.data8[3] = 0x00;
		    can1TxData.data8[4] = 0x00;
		    can1TxData.data8[5] = 0x00;
		    can1TxData.data8[6] = 0x00;
		    can1TxData.data8[7] = 0x00;
		    sta = PowerSupplySendMsg(can1TxData.data8, FDCAN_DLC_BYTES_8, ANALOG_FRAME_ID + id);
		} break;
		case ANALOG_AC_C_CMD: //模拟量信息交流侧电流信息
		{
		    can1TxData.data8[0] = BRD_FRAME;
		    can1TxData.data8[1] = RQ_FRAME + ANALOG_AC_C_SERVICE_CODE + id;
		    can1TxData.data8[2] = 0x00;
		    can1TxData.data8[3] = 0x00;
		    can1TxData.data8[4] = 0x00;
		    can1TxData.data8[5] = 0x00;
		    can1TxData.data8[6] = 0x00;
		    can1TxData.data8[7] = 0x00;
		    sta = PowerSupplySendMsg(can1TxData.data8, FDCAN_DLC_BYTES_8, ANALOG_FRAME_ID + id);
		} break;
		default: break;
    }

	xSemaphoreGive(PowerSupplyMutex); //互斥量开锁

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : PowerSupplyDataAnalyse
 * @brief : 接收数据解析函数
 * @param : canRxID CAN标准帧id
 * @retval:
 * @date  : 2021/09/30
 * @note  : 定时上传，无需下发询问指令
 *          TODO:2021/09/30 未测试
 * ---------------------------------------------------------------------------*/
void PowerSupplyDataAnalyse(canID *canRxID)
{
	uint16_t frameID = 0;    //帧 ID
	uint8_t powerNum = 0;    //电源号
	uint8_t rsFlag = 0;      //响应标志
	uint8_t serviceCode = 0; //服务码
	uint8_t sourceID = 0;    //源 ID

	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* 帧 ID */
	frameID = canRxID->id16 & 0xFFF0;
    /* 电源号 */
	powerNum = canRxID->id8[0] & 0x0F;
	/* 源 ID */
	sourceID = can2RxData.data8[0] & 0x7F;
	/* 响应标志 */
	rsFlag = can2RxData.data8[1] & 0x80;

	if((rsFlag == RS_FRAME) && (sourceID == BRD_FRAME))
	{
		switch(frameID)
		{
			case ALARM_FRAME_ID: //告警信息
			{
				/* 服务码 */
				serviceCode = (can2RxData.data8[1] & 0x7F) - powerNum;

				switch(serviceCode)
				{
					case ALARM_SERVICE_CODE: //告警信息服务码
					{
						uPowerAlarmData.alarmMsg = Data2U8ToU16(can2RxData.data8[3], can2RxData.data8[2]);

						xSemaphoreGiveFromISR(PowerSupplySem, &xHigherPriorityTaskWoken);
						portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
					} break;
					default: break;
				}
			} break;
			case STATUS_FRAME_ID: //状态信息
			{
				/* 服务码 */
				serviceCode = (can2RxData.data8[1] & 0x7F) - powerNum;

				switch(serviceCode)
				{
					case STATUS_SERVICE_CODE: //状态信息服务码
					{
						uPowerStatusData.statusMsg = Data4U8ToU32(can2RxData.data8[5], can2RxData.data8[4],
								                                  can2RxData.data8[3], can2RxData.data8[2]);

						xSemaphoreGiveFromISR(PowerSupplySem, &xHigherPriorityTaskWoken);
						portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
					} break;
					default: break;
				}
			} break;
			case ANALOG_FRAME_ID: //模拟量信息
			{
				/* 服务码 */
				serviceCode = (can2RxData.data8[1] & 0x7F);

				switch(serviceCode)
				{
					case ANALOG_DC_SERVICE_CODE: //模拟量信息直流侧服务码
					{
						memcpy(&tPowerAnalogData.DCVol, &can2RxData.data8[2], 6);

						xSemaphoreGiveFromISR(PowerSupplySem, &xHigherPriorityTaskWoken);
						portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
					} break;
					case ANALOG_AC_V_SERVICE_CODE: //模拟量信息交流侧电压信息服务码
					{
						memcpy(&tPowerAnalogData.ACVolA, &can2RxData.data8[2], 6);

						xSemaphoreGiveFromISR(PowerSupplySem, &xHigherPriorityTaskWoken);
						portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
					} break;
					case ANALOG_AC_C_SERVICE_CODE: //模拟量信息交流侧电流信息服务码
					{
						memcpy(&tPowerAnalogData.ACCurA, &can2RxData.data8[2], 6);

						xSemaphoreGiveFromISR(PowerSupplySem, &xHigherPriorityTaskWoken);
						portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
					} break;
					default: break;
				}
			} break;
			default: break;
		}
	}
}


