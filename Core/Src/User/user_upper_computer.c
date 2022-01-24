/*-----------------------------------------------------------------------------
 * @file        : user_upper_computer.c
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2019/10/18
 * @date changed: 2021/10/22
 * @brief       : 中上位机通信处理程序
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#ifdef USER_CONFIG
	#include "user_config.h"
#endif



//-----------------------------------------------------------------------------
//                              TODO:发送处理
//-----------------------------------------------------------------------------


/*-----------------------------------------------------------------------------
 * @name  : UpperPc_DeviceInfoInit
 * @brief : 上位机通信：设备信息初始化
 * @param :
 * @retval:
 * @date  : 2021/10/20
 * @note  :
 * ---------------------------------------------------------------------------*/
void UpperPc_DeviceInfoInit(void)
{
	uint32_t wSize = 0, rSize = 0;

	memset(&tDeviceInfo, 0, sizeof(DeviceInfo_T));

	NAND_Read_File(NAND_EQUIPMENTINFO, &tDeviceInfo, sizeof(DeviceInfo_T), &rSize); //读NAND设备信息

	if(memcmp(&tDeviceInfo.midDriverVersion, (char*)MID_COMPUTER_DRIVERVERSION, sizeof(MID_COMPUTER_DRIVERVERSION)) != 0) //有改动则进行修改
	{
		if(rSize != sizeof(DeviceInfo_T))
		{
			memcpy(&tDeviceInfo.deviceSPEC, (char*)DEVICE_SPEC, sizeof(DEVICE_SPEC)); //设备规格
			memcpy(&tDeviceInfo.deviceSN, (char*)DEVICE_SN, sizeof(DEVICE_SN)); //设备序列号
			memcpy(&tDeviceInfo.midDriverVersion, (char*)MID_COMPUTER_DRIVERVERSION, sizeof(MID_COMPUTER_DRIVERVERSION)); //中位机驱动版本描述
			memcpy(&tDeviceInfo.midDescription, (char*)MID_COMPUTER_DESCRIPTION, sizeof(MID_COMPUTER_DESCRIPTION)); //中位机板卡硬件版本描述
			tDeviceInfo.layerTotalNum = LOWER_LAYER_TOTAL_NUM; //总层数
			//每层信息
			tDeviceInfo.chTotalNum = LOWER_CH_TOTAL_NUM; //每层通道数

			/* IP 192.168.0.100 */
			tDeviceInfo.ipAddr[0]  = DECToASC(1);
			tDeviceInfo.ipAddr[1]  = DECToASC(9);
			tDeviceInfo.ipAddr[2]  = DECToASC(2);
			tDeviceInfo.ipAddr[3]  = '.';
			tDeviceInfo.ipAddr[4]  = DECToASC(1);
			tDeviceInfo.ipAddr[5]  = DECToASC(6);
			tDeviceInfo.ipAddr[6]  = DECToASC(8);
			tDeviceInfo.ipAddr[7]  = '.';
			tDeviceInfo.ipAddr[8]  = DECToASC(0);
			tDeviceInfo.ipAddr[9]  = DECToASC(0);
			tDeviceInfo.ipAddr[10] = DECToASC(0);
			tDeviceInfo.ipAddr[11] = '.';
			tDeviceInfo.ipAddr[12] = DECToASC(1);
			tDeviceInfo.ipAddr[13] = DECToASC(0);
			tDeviceInfo.ipAddr[14] = DECToASC(0);
			tDeviceInfo.ipAddr[15] = 0x00;
			/* 子网掩码 */
			tDeviceInfo.maskAddr[0]  = DECToASC(2);
			tDeviceInfo.maskAddr[1]  = DECToASC(5);
			tDeviceInfo.maskAddr[2]  = DECToASC(5);
			tDeviceInfo.maskAddr[3]  = '.';
			tDeviceInfo.maskAddr[4]  = DECToASC(2);
			tDeviceInfo.maskAddr[5]  = DECToASC(5);
			tDeviceInfo.maskAddr[6]  = DECToASC(5);
			tDeviceInfo.maskAddr[7]  = '.';
			tDeviceInfo.maskAddr[8]  = DECToASC(2);
			tDeviceInfo.maskAddr[9]  = DECToASC(5);
			tDeviceInfo.maskAddr[10] = DECToASC(5);
			tDeviceInfo.maskAddr[11] = '.';
			tDeviceInfo.maskAddr[12] = DECToASC(0);
			tDeviceInfo.maskAddr[13] = DECToASC(0);
			tDeviceInfo.maskAddr[14] = DECToASC(0);
			tDeviceInfo.maskAddr[15] = 0x00;
			/* 网关 */
			tDeviceInfo.gwAddr[0]  = DECToASC(1);
			tDeviceInfo.gwAddr[1]  = DECToASC(9);
			tDeviceInfo.gwAddr[2]  = DECToASC(2);
			tDeviceInfo.gwAddr[3]  = '.';
			tDeviceInfo.gwAddr[4]  = DECToASC(1);
			tDeviceInfo.gwAddr[5]  = DECToASC(6);
			tDeviceInfo.gwAddr[6]  = DECToASC(8);
			tDeviceInfo.gwAddr[7]  = '.';
			tDeviceInfo.gwAddr[8]  = DECToASC(0);
			tDeviceInfo.gwAddr[9]  = DECToASC(0);
			tDeviceInfo.gwAddr[10] = DECToASC(0);
			tDeviceInfo.gwAddr[11] = '.';
			tDeviceInfo.gwAddr[12] = DECToASC(0);
			tDeviceInfo.gwAddr[13] = DECToASC(0);
			tDeviceInfo.gwAddr[14] = DECToASC(1);
			tDeviceInfo.gwAddr[15] = 0x00;

			memcpy(&tDeviceInfo.description, (char*)BI_Version, sizeof(BI_Version)); //双向电源型号
			tDeviceInfo.deviceID = 0x0001; //设备ID

			NAND_Write_File(NAND_EQUIPMENTINFO, &tDeviceInfo, sizeof(DeviceInfo_T), &wSize); //保存设备信息

		    //TODO:修改IP后保存至flash内，下次上电读flash后直接使用保存的数据
			/* IP 192.168.0.100 */
		    tNetConfigSet.user_ip[0] = IP_ADDR0;
		    tNetConfigSet.user_ip[1] = IP_ADDR1;
		    tNetConfigSet.user_ip[2] = IP_ADDR2;
		    tNetConfigSet.user_ip[3] = IP_ADDR3;
		    tNetConfigSet.user_mask[0] = NETMASK_ADDR0;
		    tNetConfigSet.user_mask[1] = NETMASK_ADDR1;
		    tNetConfigSet.user_mask[2] = NETMASK_ADDR2;
		    tNetConfigSet.user_mask[3] = NETMASK_ADDR3;
		    tNetConfigSet.user_gw[0] = GW_ADDR0;
		    tNetConfigSet.user_gw[1] = GW_ADDR1;
		    tNetConfigSet.user_gw[2] = GW_ADDR2;
		    tNetConfigSet.user_gw[3] = GW_ADDR3;
		}
	}

	/* 网络接口配置更新 */
	tNetConfigSet.user_ip[0] =
    		ASCToDEC(tDeviceInfo.ipAddr[0])*100 +
			ASCToDEC(tDeviceInfo.ipAddr[1])*10 +
			ASCToDEC(tDeviceInfo.ipAddr[2]); //ASCII码转十进制、转IP配置值
	tNetConfigSet.user_ip[1] =
    		ASCToDEC(tDeviceInfo.ipAddr[4])*100 +
			ASCToDEC(tDeviceInfo.ipAddr[5])*10 +
			ASCToDEC(tDeviceInfo.ipAddr[6]); //ASCII码转十进制、转IP配置值
	tNetConfigSet.user_ip[2] =
    		ASCToDEC(tDeviceInfo.ipAddr[8])*100 +
			ASCToDEC(tDeviceInfo.ipAddr[9])*10 +
			ASCToDEC(tDeviceInfo.ipAddr[10]); //ASCII码转十进制、转IP配置值
	tNetConfigSet.user_ip[3] =
    		ASCToDEC(tDeviceInfo.ipAddr[12])*100 +
			ASCToDEC(tDeviceInfo.ipAddr[13])*10 +
			ASCToDEC(tDeviceInfo.ipAddr[14]); //ASCII码转十进制、转IP配置值
	tNetConfigSet.user_mask[0] = 255;
	tNetConfigSet.user_mask[1] = 255;
	tNetConfigSet.user_mask[2] = 255;
	tNetConfigSet.user_mask[3] = 0;
    tNetConfigSet.user_gw[0] = tNetConfigUpd.user_ip[0];
    tNetConfigSet.user_gw[1] = tNetConfigUpd.user_ip[1];
    tNetConfigSet.user_gw[2] = tNetConfigUpd.user_ip[2];
    tNetConfigSet.user_gw[3] = 1;
}

/*-----------------------------------------------------------------------------
 * @name  : UpperPc_Send_DeviceInfoPack
 * @brief : 上位机通信：设备信息数据帧打包
 * @param : eCmd：指令码
 *          pBuf：帧缓存区指针
 *          len：获取帧长度
 * @retval:
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
static void UpperPc_Send_DeviceInfoPack(UpperControlCmd_E eCmd, uint8_t* pBuf, uint16_t* len)
{
	uint8_t i = 0;
	uint8_t layerID = 0;
    uint16_t checkSum = 0; //校验和
    uint32_t addr = 0;     //数据长度的位置
    uint32_t tempLen = 0;  //帧长度

	pBuf[tempLen++] = UPPER_FRAME_HEAD_L;       //包头
	pBuf[tempLen++] = UPPER_FRAME_HEAD_H;       //包头

	memcpy(pBuf+tempLen, &addr, sizeof(addr));  //数据长度，初始为0
	addr = tempLen;                             //记录数据长度的位置
	tempLen += sizeof(addr);

	pBuf[tempLen++] = tDeviceInfo.deviceID;     //设备ID
	pBuf[tempLen++] = DataU16L_ToU8(eCmd);      //指令码（LSB）
	pBuf[tempLen++] = DataU16H_ToU8(eCmd);      //指令码（MSB）

	/* 数据段 */
	if(tCommAckSta.ReadDeviceInfoSta == ACK)
	{
		pBuf[tempLen++] = ACK; //响应标志位
		memcpy(pBuf+tempLen, &tDeviceInfo.deviceSPEC, 89);
		tempLen += 89;
		for(i=0; i<LOWER_LAYER_TOTAL_NUM; i++)
		{
			layerID = i + 1;
			if(LowerPC_QueryLayerIsNULL(&uExistLayer, layerID) == 0)
			{
				continue; //查询不到层存在则跳过该次循环
			}

			pBuf[tempLen++] = tDeviceInfo.tlayerInfo[INDEX(layerID)].layerID; //层号
			memcpy(pBuf+tempLen, &tDeviceInfo.tlayerInfo[INDEX(layerID)].DriverVersion[0], 56);
			tempLen += 56;
		}
		memcpy(pBuf+tempLen, &tDeviceInfo.chTotalNum, 67);
		tempLen += 67;
	}
	else if(tCommAckSta.ReadDeviceInfoSta == NACK)
	{
		pBuf[tempLen++] = NACK; //响应标志位
		tempLen += 1;
	}

	memcpy(pBuf+addr, &tempLen, sizeof(tempLen));  //数据长度

	checkSum = CheckSum(&pBuf[2], tempLen-2);   //校验和
	pBuf[tempLen++] = DataU16L_ToU8(checkSum);  //校验和（LSB）
	pBuf[tempLen++] = DataU16H_ToU8(checkSum);  //校验和（MSB）
	pBuf[tempLen++] = UPPER_FRAME_END_L;        //包尾
	pBuf[tempLen++] = UPPER_FRAME_END_H;        //包尾

	*len = tempLen;                             //帧长度
}

/*-----------------------------------------------------------------------------
 * @name  : UpperPc_Send_AckPack
 * @brief : 上位机通信：应答帧打包
 * @param : eCmd：指令码
 *          pBuf：帧缓存区指针
 *          len：获取帧长度
 *          eAck：应答状态
 * @retval:
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
static void UpperPc_Send_AckPack(UpperControlCmd_E eCmd, uint8_t* pBuf, uint16_t* len, ResponseFlag_E eAck)
{
    uint16_t checkSum = 0; //校验和
    uint32_t addr = 0;     //数据长度的位置
    uint32_t tempLen = 0;  //帧长度

	pBuf[tempLen++] = UPPER_FRAME_HEAD_L;       //包头
	pBuf[tempLen++] = UPPER_FRAME_HEAD_H;       //包头

	memcpy(pBuf+tempLen, &addr, sizeof(addr));  //数据长度，初始为0
	addr = tempLen;                             //记录数据长度的位置
	tempLen += sizeof(addr);

	pBuf[tempLen++] = tDeviceInfo.deviceID;     //设备ID
	pBuf[tempLen++] = DataU16L_ToU8(eCmd);      //指令码（LSB）
	pBuf[tempLen++] = DataU16H_ToU8(eCmd);      //指令码（MSB）

	/* 数据段 */
	memcpy(pBuf+(tempLen), &eAck, 1);   //数据
	tempLen += 1;

	memcpy(pBuf+addr, &tempLen, sizeof(tempLen));  //数据长度

	checkSum = CheckSum(&pBuf[2], tempLen-2);   //校验和
	pBuf[tempLen++] = DataU16L_ToU8(checkSum);  //校验和（LSB）
	pBuf[tempLen++] = DataU16H_ToU8(checkSum);  //校验和（MSB）
	pBuf[tempLen++] = UPPER_FRAME_END_L;        //包尾
	pBuf[tempLen++] = UPPER_FRAME_END_H;        //包尾

	*len = tempLen;                             //帧长度
}

/*-----------------------------------------------------------------------------
 * @name  : UpperPc_Send_RealTimeDataPack
 * @brief : 上位机通信：实时数据数据帧打包
 * @param : eCmd：指令码
 *          pBuf：帧缓存区指针
 *          len：获取帧长度
 * @retval:
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
static void UpperPc_Send_RealTimeDataPack(UpperControlCmd_E eCmd, uint8_t* pBuf, uint16_t* len)
{
	uint8_t i = 0;
	uint8_t layerID = 0, chID = 0;    //层号、通道号
	uint16_t chNum = 0, layerCnt = 0; //通道数量、层数量
    uint16_t checkSum = 0;            //校验和
    uint16_t addrChNum = 0;           //通道数量的位置
    uint32_t addr = 0;                //数据长度的位置
    uint32_t tempLen = 0;             //帧长度
    uint32_t packLen = sizeof(ChannelRealTimeData_T); //拷贝包长度


	pBuf[tempLen++] = UPPER_FRAME_HEAD_L;       //包头
	pBuf[tempLen++] = UPPER_FRAME_HEAD_H;       //包头

	memcpy(pBuf+tempLen, &addr, sizeof(addr));  //数据长度，初始为0
	addr = tempLen;                             //记录数据长度的位置
	tempLen += sizeof(addr);

	pBuf[tempLen++] = tDeviceInfo.deviceID;     //设备ID
	pBuf[tempLen++] = DataU16L_ToU8(eCmd);      //指令码（LSB）
	pBuf[tempLen++] = DataU16H_ToU8(eCmd);      //指令码（MSB）

	/* 数据段 */
	//TODO:
	if(tCommAckSta.GetChRealTimeDataSta == ACK)
	{
		pBuf[tempLen++] = ACK; //响应标志位
		pBuf[tempLen++] = DataU16L_ToU8(uSysAbnormalSta.sta); //系统异常状态
		pBuf[tempLen++] = DataU16H_ToU8(uSysAbnormalSta.sta); //系统异常状态
		pBuf[tempLen++] = tDeviceInfo.layerTotalNum; //层数量
		for(i=0; i<LOWER_LAYER_TOTAL_NUM; i++)
		{
			layerID = i + 1;
			if(LowerPC_QueryLayerIsNULL(&uExistLayer, layerID) == 0)
			{
				continue; //查询不到层存在则跳过该次循环
			}

			layerCnt++; //层数量

			pBuf[tempLen++] = layerID; //层号
			pBuf[tempLen++] = DataU16L_ToU8(uLayerAbnormalSta[INDEX(layerID)].sta); //层异常状态
			pBuf[tempLen++] = DataU16H_ToU8(uLayerAbnormalSta[INDEX(layerID)].sta); //层异常状态
		}

		addrChNum = tempLen; //记录通道数量的位置
		tempLen += sizeof(chNum);


		for(i=0; i<LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM; i++)
		{
			chID = i;
			if(LowerPC_QueryChIsNULL(&tExistCh, chID) == 0)
			{
				continue; //查询不到通道存在则跳过该次循环
			}

			chNum++;

			memcpy(pBuf+tempLen, &tChRealTimeDat[chID].chID, packLen);
			tempLen += packLen;
		}

		memcpy(pBuf+addrChNum, &chNum, sizeof(chNum)); //通道数量
	}
	else if(tCommAckSta.GetChRealTimeDataSta == NACK)
	{
		pBuf[tempLen++] = NACK; //响应标志位
		tempLen += 1;
	}

	memcpy(pBuf+addr, &tempLen, sizeof(tempLen));  //数据长度

	checkSum = CheckSum(&pBuf[2], tempLen-2);   //校验和
	pBuf[tempLen++] = DataU16L_ToU8(checkSum);  //校验和（LSB）
	pBuf[tempLen++] = DataU16H_ToU8(checkSum);  //校验和（MSB）
	pBuf[tempLen++] = UPPER_FRAME_END_L;        //包尾
	pBuf[tempLen++] = UPPER_FRAME_END_H;        //包尾

	*len = tempLen;                             //帧长度
}

/*-----------------------------------------------------------------------------
 * @name  : UpperPc_Send_RecordDataPackTest
 * @brief : 上位机通信：记录数据数据帧打包，测试用
 * @param : eCmd：指令码
 *          pBuf：帧缓存区指针
 *          len：获取帧长度
 * @retval:
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
static void UpperPc_Send_RecordDataPackTest(UpperControlCmd_E eCmd, uint8_t* pBuf, uint16_t* len)
{
    uint16_t checkSum = 0; //校验和
    uint32_t addr = 0;     //数据长度的位置
    uint32_t tempLen = 0;  //帧长度
    uint32_t chNum = 0;    //通道数量为0

	pBuf[tempLen++] = UPPER_FRAME_HEAD_L;       //包头
	pBuf[tempLen++] = UPPER_FRAME_HEAD_H;       //包头

	memcpy(pBuf+tempLen, &addr, sizeof(addr));  //数据长度，初始为0
	addr = tempLen;                             //记录数据长度的位置
	tempLen += sizeof(addr);

	pBuf[tempLen++] = tDeviceInfo.deviceID;     //设备ID
	pBuf[tempLen++] = DataU16L_ToU8(eCmd);      //指令码（LSB）
	pBuf[tempLen++] = DataU16H_ToU8(eCmd);      //指令码（MSB）

	/* 数据段 */
	memcpy(pBuf+tempLen, &chNum, sizeof(chNum)); //通道数量为0
	tempLen += sizeof(chNum);

	memcpy(pBuf+addr, &tempLen, sizeof(tempLen)); //数据长度

	checkSum = CheckSum(&pBuf[2], tempLen-2);   //校验和
	pBuf[tempLen++] = DataU16L_ToU8(checkSum);  //校验和（LSB）
	pBuf[tempLen++] = DataU16H_ToU8(checkSum);  //校验和（MSB）
	pBuf[tempLen++] = UPPER_FRAME_END_L;        //包尾
	pBuf[tempLen++] = UPPER_FRAME_END_H;        //包尾

	*len = tempLen;                             //帧长度
}

/**-----------------------------------------------------------------------------
  * @brief	收到获取记录数据命令后调用：提取记录数据、计算长度、计算校验和、封包等
  * @param	eCmd：指令功能码
  * @param	queue：队列句柄
  * @param	*wl：提取长度（u32）
  * @return	uint8_t：是否取出成功   0、取出成功   1、初始化异常  2、读出FLASH异常  3、SDRAM读出异常  4、没有记录数据
  * @remark
  *---------------------------------------------------------------------------*/
#define UpperPc_Send_RecordDataPack(eCmd, pQueue, len)  uRecordDataPack(eCmd, pQueue, len)


/*-----------------------------------------------------------------------------
 * @name  : UpperPc_TxFramePack
 * @brief : 上位机通信：发送数据打包
 * @param : eCmd：指令码
 *          ...： 可变参数
 * @retval: 正常：QUEUE_OK 异常：QUEUE_ERROR
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
static uint8_t UpperPc_TxFramePack(UpperControlCmd_E eCmd, ...)
{
	uint8_t recordSta = 0;
    QueueStatusTypeDef_E sta = QUEUE_OK;

    uint8_t i = 0;
    uint16_t wSize = 0;   //写入队列的次数
    uint16_t wLen = 0;    //写入队列的长度
    uint16_t tempLen = 0; //临时长度

    uint8_t* pBuf = (uint8_t *)pvPortMalloc(TEMP_BUF_MAX_LEN); //申请内存缓冲区指针
	if(pBuf == NULL)
	{
		return QUEUE_ERROR; //返回
	}
	memset(pBuf, 0, TEMP_BUF_MAX_LEN); //清零

	UPPER_PC_LOCK(); //互斥量闭锁

    uint32_t len = 0; //数据长度

    if(eCmd == CMD_UPPER_READ_DEVICE_INFO) //读取设备信息
    {
    	UpperPc_Send_DeviceInfoPack(eCmd, pBuf, (uint16_t*)&len);
    }
    else if((eCmd == CMD_UPPER_UPDATE_DEVICE_INFO) || //修改设备信息
    		(eCmd == CMD_UPPER_ADV_SET)            || //高级设置
			(eCmd == CMD_UPPER_GLOBAL_SETTING)     || //全局设置
			(eCmd == CMD_UPPER_STEP_LOADING)       || //工步加载
			(eCmd == CMD_UPPER_START_STEP)         || //开始工步
			(eCmd == CMD_UPPER_STOP_STEP)          || //停止工步
			(eCmd == CMD_UPPER_SUSPEND_STEP)       || //暂停工步
			(eCmd == CMD_UPPER_CONTINUE_STEP)      || //继续工步
			(eCmd == CMD_UPPER_JUMP_STEP)          || //跳转工步
			(eCmd == CMD_UPPER_CH_ENABLE)          || //通道禁用/启用
			(eCmd == CMD_UPPER_CLEAR_WARNING)      || //清除警告
			(eCmd == CMD_UPPER_RESET))                //复位
    {
	    va_list args;
	    va_start(args, eCmd);
	    uint8_t ack = (uint8_t)va_arg(args, int);
	    va_end(args);

	    UpperPc_Send_AckPack(eCmd, pBuf, (uint16_t*)&len, ack);
    }
    else if(eCmd == CMD_UPPER_GET_CH_REAL_TIME_DATA) //获取各通道实时数据
    {
		UpperPc_Send_RealTimeDataPack(eCmd, pBuf, (uint16_t*)&len);
    }
    else if(eCmd == CMD_UPPER_GET_CH_RECORD_DATA) //获取各通道记录数据
    {
    	recordSta = UpperPc_Send_RecordDataPack(eCmd, &tUpperPcTxQueue, &len); //TODO：20211103新增接口

		if(recordSta == 0)
		{
			goto UpperPc_Tx_NO_Queue;
		}
		else
		{
			UpperPc_Send_RecordDataPackTest(eCmd, pBuf, (uint16_t*)&len);
		}
    }

#if 1
    wSize = (len/UPPER_PC_BUF_MAX_LEN + ((len%UPPER_PC_BUF_MAX_LEN) ? 1:0)); //计算写入队列次数

    //写入环形队列处理，队列满了则等待
    for(i=0; i<wSize; i++)
    {
    	tempLen = (i*UPPER_PC_BUF_MAX_LEN);
    	wLen = len-tempLen;
    	if(wLen > UPPER_PC_BUF_MAX_LEN) wLen=UPPER_PC_BUF_MAX_LEN;
		do{
			sta = uQueueWriteNoCover(&tUpperPcTxQueue, (uint8_t *)(pBuf+tempLen), (uint16_t)wLen);
			if(sta == QUEUE_ISFULL)
				vTaskDelay(1);
		} while(sta == QUEUE_ISFULL);  //1ms阻塞防止线程优先级过高永久阻塞导致队列无法出队
    }
#else
    wSize = (len/(UPPER_PC_BUF_MAX_LEN*2) + ((len%(UPPER_PC_BUF_MAX_LEN*2)) ? 1:0)); //计算写入队列次数

	//不入队列，直接发送
    for(i=0; i<wSize; i++)
    {
    	tempLen = (i*(UPPER_PC_BUF_MAX_LEN*2));
    	wLen = len-tempLen;
    	if(wLen > (UPPER_PC_BUF_MAX_LEN*2)) wLen=(UPPER_PC_BUF_MAX_LEN*2);

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
#endif

UpperPc_Tx_NO_Queue:
	vPortFree(pBuf); //释放之前申请的内存

    UPPER_PC_UNLOCK(); //互斥量开锁

    return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : UpperPc_TxTask
 * @brief : 与上位机通信发送任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
void UpperPc_TxTask(void *parameter)
{
    uint8_t* txBuf = (uint8_t *)pvPortMalloc(TEMP_BUF_MAX_LEN); //申请内存缓冲区指针
	if(txBuf == NULL)
	{
		vTaskDelay(10);
		DEBUG_PRINTF("\r\n ERROR! \r\n");
	}
	memset(txBuf, 0, TEMP_BUF_MAX_LEN); //清零

	uint16_t txLen = 0;
	QueueStatusTypeDef_E sta = QUEUE_OK;
	uint8_t i = 0;

	/* 用于保存上次时间，调用后系统自动更新 */
	static portTickType PreviousWakeTime;
	/* 设置延时时间，将时间转为节拍数 */
	const portTickType TimeIncrement = pdMS_TO_TICKS(1); //1ms
	const portTickType sendTimeIncrement = pdMS_TO_TICKS(1); //1ms
	/* 获取当前系统时间 */
	PreviousWakeTime = xTaskGetTickCount();

	while(1)
	{
		if(uQueueGetDataNum(&tUpperPcTxQueue) != 0)
		{
			while(uQueueGetDataNum(&tUpperPcTxQueue) != 0)
			{
				ThreadTime(8);

				//从FIFO读取数据
				sta = uQueueRead(&tUpperPcTxQueue, (uint8_t *)txBuf, &txLen); //从FIFO读取数据
				if(sta == QUEUE_OK)
				{
					if(TCP_Netconn != NULL)
					{
//						tcp_nagle_disable(TCP_Netconn->pcb.tcp); //禁用nagle算法
						netconn_write(TCP_Netconn, txBuf, txLen, NETCONN_COPY); //发送数据

						for(i=0; i<3; i++)
						{
							tCommPro.Tcp_Timeout_MS[i] = 0; //超时时间清零
							tCommPro.Tcp_Timeout_S[i] = 0; //超时时间清零
						}
					}
				}
			    /* 调用绝对延时函数,任务时间间隔为1个tick */
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
 * @name  : UpperPc_RxAck
 * @brief : 上位机通信：下位机应答解析
 * @param : eCmd：指令码
 *          commSta：通信状态
 *          time：等待时间
 * @retval:
 * @date  : 2021/11/15
 * @note  :
 * ---------------------------------------------------------------------------*/
static void UpperPc_RxAck(UpperControlCmd_E eCmd, uint32_t time)
{
	uint8_t commSta = COMM_ERROR;
	uint8_t i = 0;

	if(xSemaphoreTake(UpperPcSem, time) == errQUEUE_EMPTY) //若没有应答
	{
		UpperPc_TxFramePack(eCmd, NACK);
	}
	else //应答成功
	{
		for(i=0; i<LOWER_LAYER_TOTAL_NUM; i++)
		{
			if(LowerPC_QueryLayerIsNULL(&uExistLayer, i+1) == 0)
			{
				continue; //查询不到层存在则跳过该次循环
			}

			switch(eCmd)
			{
				case CMD_UPPER_GLOBAL_SETTING: //全局设置
				{
					if((uLowerCommSta[i].CommStaBit.sta_GlobalSetting == COMM_OK) &&
					   (uLowerAckCommSta[i].CommStaBit.sta_GlobalSetting == COMM_OK))
					{
						commSta = COMM_OK;
					}
				} break;
				case CMD_UPPER_STEP_LOADING: //工步加载
				{
					if((uLowerCommSta[i].CommStaBit.sta_ChStepLoading == COMM_OK) &&
					   (uLowerAckCommSta[i].CommStaBit.sta_ChStepLoading == COMM_OK))
					{
						commSta = COMM_OK;
					}
				} break;
				case CMD_UPPER_START_STEP: //开始工步
				{
					if((uLowerCommSta[i].CommStaBit.sta_StartStep == COMM_OK) &&
					   (uLowerAckCommSta[i].CommStaBit.sta_StartStep == COMM_OK))
					{
						commSta = COMM_OK;
					}
				} break;
				case CMD_UPPER_STOP_STEP: //停止工步
				{
					if((uLowerCommSta[i].CommStaBit.sta_StopStep == COMM_OK) &&
					   (uLowerAckCommSta[i].CommStaBit.sta_StopStep == COMM_OK))
					{
						commSta = COMM_OK;
					}
				} break;
				case CMD_UPPER_SUSPEND_STEP: //暂停工步
				{
					if((uLowerCommSta[i].CommStaBit.sta_SuspendStep == COMM_OK) &&
					   (uLowerAckCommSta[i].CommStaBit.sta_SuspendStep == COMM_OK))
					{
						commSta = COMM_OK;
					}
				} break;
				case CMD_UPPER_CONTINUE_STEP: //继续工步
				{
					if((uLowerCommSta[i].CommStaBit.sta_ContinueStep == COMM_OK) &&
					   (uLowerAckCommSta[i].CommStaBit.sta_ContinueStep == COMM_OK))
					{
						commSta = COMM_OK;
					}
				} break;
				case CMD_UPPER_JUMP_STEP: //跳转工步
				{
					if((uLowerCommSta[i].CommStaBit.sta_JumpStep == COMM_OK) &&
					   (uLowerAckCommSta[i].CommStaBit.sta_JumpStep == COMM_OK))
					{
						commSta = COMM_OK;
					}
				} break;
				case CMD_UPPER_CH_ENABLE: //通道禁用/启用
				{
					if((uLowerCommSta[i].CommStaBit.sta_Enable == COMM_OK) &&
					   (uLowerAckCommSta[i].CommStaBit.sta_Enable == COMM_OK))
					{
						commSta = COMM_OK;
					}
				} break;
				case CMD_UPPER_CLEAR_WARNING: //清除警告
				{
					if((uLowerCommSta[i].CommStaBit.sta_ClearWarning == COMM_OK) &&
					   (uLowerAckCommSta[i].CommStaBit.sta_ClearWarning == COMM_OK))
					{
						commSta = COMM_OK;
					}
				} break;
				case CMD_UPPER_RESET: //复位
				{
					if((uLowerCommSta[i].CommStaBit.sta_Reset == COMM_OK) &&
					   (uLowerAckCommSta[i].CommStaBit.sta_Reset == COMM_OK))
					{
						commSta = COMM_OK;
					}
				} break;
				default: break;
			}
		}

		if(commSta == COMM_ERROR)
		{
			UpperPc_TxFramePack(eCmd, NACK);
		}
		else if(commSta == COMM_OK)
		{
			UpperPc_TxFramePack(eCmd, ACK);
		}
	}
}

/*-----------------------------------------------------------------------------
 * @name  : UpperPc_RxFrameAnalyze
 * @brief : 上位机通信：接收数据解析
 * @param : pBuf：接收数据缓存区指针
 *          len：数据长度
 * @retval:
 * @date  : 2021/10/18
 * @note  :
 * ---------------------------------------------------------------------------*/
static uint8_t UpperPc_RxFrameAnalyze(const uint8_t* pBuf, uint16_t len)
{
	CommStatus_E sta = COMM_ERROR;
	UpperControlCmd_E eCmd = 0x0000;
	uint16_t checkSum = 0;
	uint16_t dataLen = 0;
	uint32_t wSize = 0;
	uint32_t waitTime = 800; //信号量等待时间，单位：ms

	/* 上位机正常通信标志，在获取设置信息指令后才可与其他指令进行通信 */
	if(tCommPro.Upper_PC_Comm_Flag == 0)
	{
		/* 包头包尾判断 */
		if((pBuf[0] == UPPER_FRAME_HEAD_L) &&
		   (pBuf[1] == UPPER_FRAME_HEAD_H) &&
		   (pBuf[len-2] == UPPER_FRAME_END_L) &&
		   (pBuf[len-1] == UPPER_FRAME_END_H))
		{
			/* 校验和判断 */
		    checkSum = Data2U8ToU16(pBuf[len-4], pBuf[len-3]);
			if(checkSum == CheckSum(&pBuf[2], len-6)) //校验通过
			{
				eCmd = Data2U8ToU16(pBuf[7], pBuf[8]);
				switch(eCmd)
				{
					case CMD_UPPER_READ_DEVICE_INFO: //读取设备信息
					{
						tCommPro.Upper_PC_Comm_Flag = 1;

						UpperPc_TxFramePack(eCmd);
					} break;
					default: break;
				}

				sta = COMM_OK; //校验通过
			}
			else
			{
				sta = COMM_ERROR; //校验失败
			}
		}
		else
		{
			sta = COMM_ERROR; //解析失败
		}
	}
	else if(tCommPro.Upper_PC_Comm_Flag == 1)
	{
		/* 包头包尾判断 */
		if((pBuf[0] == UPPER_FRAME_HEAD_L) &&
		   (pBuf[1] == UPPER_FRAME_HEAD_H) &&
		   (pBuf[len-2] == UPPER_FRAME_END_L) &&
		   (pBuf[len-1] == UPPER_FRAME_END_H))
		{
			/* 校验和判断 */
		    checkSum = Data2U8ToU16(pBuf[len-4], pBuf[len-3]);
			if(checkSum == CheckSum(&pBuf[2], len-6)) //校验通过
			{
				eCmd = Data2U8ToU16(pBuf[7], pBuf[8]);
				switch(eCmd)
				{
					case CMD_UPPER_READ_DEVICE_INFO: //读取设备信息
					{
						UpperPc_TxFramePack(eCmd);
					} break;
					case CMD_UPPER_UPDATE_DEVICE_INFO: //修改设备信息
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						if(dataLen == 18)
						{
							dataLen = 0;
							memcpy(&tDeviceInfo.deviceID, &pBuf[9], sizeof(tDeviceInfo.deviceID)); //设备ID
							dataLen += sizeof(tDeviceInfo.deviceID);
							memcpy(&tDeviceInfo.ipAddr, &pBuf[9+dataLen], sizeof(tDeviceInfo.ipAddr)); //IP地址

							UpperPc_TxFramePack(eCmd, ACK);

							/* 网络接口配置更新 */
				    	    tNetConfigUpd.user_ip[0] =
				    	    		ASCToDEC(tDeviceInfo.ipAddr[0])*100 +
									ASCToDEC(tDeviceInfo.ipAddr[1])*10 +
									ASCToDEC(tDeviceInfo.ipAddr[2]); //ASCII码转十进制、转IP配置值
				    	    tNetConfigUpd.user_ip[1] =
				    	    		ASCToDEC(tDeviceInfo.ipAddr[4])*100 +
									ASCToDEC(tDeviceInfo.ipAddr[5])*10 +
									ASCToDEC(tDeviceInfo.ipAddr[6]); //ASCII码转十进制、转IP配置值
				    	    tNetConfigUpd.user_ip[2] =
				    	    		ASCToDEC(tDeviceInfo.ipAddr[8])*100 +
									ASCToDEC(tDeviceInfo.ipAddr[9])*10 +
									ASCToDEC(tDeviceInfo.ipAddr[10]); //ASCII码转十进制、转IP配置值
				    	    tNetConfigUpd.user_ip[3] =
				    	    		ASCToDEC(tDeviceInfo.ipAddr[12])*100 +
									ASCToDEC(tDeviceInfo.ipAddr[13])*10 +
									ASCToDEC(tDeviceInfo.ipAddr[14]); //ASCII码转十进制、转IP配置值
				    	    tNetConfigUpd.user_mask[0] = 255;
				    	    tNetConfigUpd.user_mask[1] = 255;
				    	    tNetConfigUpd.user_mask[2] = 255;
				    	    tNetConfigUpd.user_mask[3] = 0;
				    	    tNetConfigUpd.user_gw[0] = tNetConfigUpd.user_ip[0];
				    	    tNetConfigUpd.user_gw[1] = tNetConfigUpd.user_ip[1];
				    	    tNetConfigUpd.user_gw[2] = tNetConfigUpd.user_ip[2];
				    	    tNetConfigUpd.user_gw[3] = 1;

				    	    if(memcmp(&tNetConfigSet, &tNetConfigUpd, sizeof(NETCONFIG_T)) != 0) //IP有改动则进行修改
				    	    {
								vTaskDelay(500); //延时，待上位机接收到应答

					    	    memcpy(&tNetConfigSet, &tNetConfigUpd, sizeof(NETCONFIG_T)); //更新IP，用于触控屏显示
					    	    //TODO: tNetConfigSet 保存在 NAND FLASH 中

					    	    NetConfigUpdate(&tNetConfigUpd); //网络接口配置更新
				    	    }

				    		NAND_Write_File(NAND_EQUIPMENTINFO, &tDeviceInfo, sizeof(DeviceInfo_T), &wSize); //保存设备信息
						}
						else
						{
							UpperPc_TxFramePack(eCmd, NACK);
						}
					} break;
					case CMD_UPPER_ADV_SET: //高级设置
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						if(dataLen == 16)
						{
							memcpy(&tDeviceInfo.deviceSN, &pBuf[9], sizeof(tDeviceInfo.deviceSN)); //设备序列号
							UpperPc_TxFramePack(eCmd, ACK);

							vTaskDelay(200); //延时，待上位机接收到应答

							NAND_Write_File(NAND_EQUIPMENTINFO, &tDeviceInfo, sizeof(DeviceInfo_T), &wSize); //保存设备信息
						}
						else
						{
							UpperPc_TxFramePack(eCmd, NACK);
						}
					} break;
					case CMD_UPPER_GLOBAL_SETTING: //全局设置
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						if(dataLen == sizeof(GlobalPramSet_T))
						{
							memcpy(&tStepLoadBuf.tGlobalPram, &pBuf[9], sizeof(GlobalPramSet_T)); //全局设置参数
							memcpy(&tGlobalPramSet.samPeriod, &pBuf[9], sizeof(GlobalPramSet_T)); //全局设置参数

							tCommLayerCnt.cnt_GlobalSetting = LowerPc_Send_GlobalSetting(&pBuf[9], dataLen);

#if(UPPER_PC_IMMEDIATE_ACK==0)
							UpperPc_RxAck(eCmd, waitTime);
#else
							UpperPc_TxFramePack(eCmd, ACK);
#endif
						}
						else
						{
							UpperPc_TxFramePack(eCmd, NACK);
						}
					} break;
					case CMD_UPPER_STEP_LOADING: //工步加载
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);

						memcpy(&tStepLoadBuf.buf, &pBuf[9], dataLen);
						tStepLoadBuf.len = dataLen;

						tCommLayerCnt.cnt_ChStepLoading = LowerPc_Send_StepLoading(&pBuf[9], dataLen);

#if(UPPER_PC_IMMEDIATE_ACK==0)
							UpperPc_RxAck(eCmd, waitTime);
#else
							UpperPc_TxFramePack(eCmd, ACK);
#endif
					} break;
					case CMD_UPPER_START_STEP: //开始工步
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						tCommLayerCnt.cnt_StartStep = LowerPc_Send_StartStep(&pBuf[9], dataLen);

#if(UPPER_PC_IMMEDIATE_ACK==0)
							UpperPc_RxAck(eCmd, waitTime);
#else
							UpperPc_TxFramePack(eCmd, ACK);
#endif
					} break;
					case CMD_UPPER_STOP_STEP: //停止工步
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						tCommLayerCnt.cnt_StopStep = LowerPc_Send_StopStep(&pBuf[9], dataLen);

#if(UPPER_PC_IMMEDIATE_ACK==0)
							UpperPc_RxAck(eCmd, waitTime);
#else
							UpperPc_TxFramePack(eCmd, ACK);
#endif
					} break;
					case CMD_UPPER_SUSPEND_STEP: //暂停工步
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						tCommLayerCnt.cnt_SuspendStep = LowerPc_Send_SuspendStep(&pBuf[9], dataLen);

#if(UPPER_PC_IMMEDIATE_ACK==0)
							UpperPc_RxAck(eCmd, waitTime);
#else
							UpperPc_TxFramePack(eCmd, ACK);
#endif
					} break;
					case CMD_UPPER_CONTINUE_STEP: //继续工步
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						tCommLayerCnt.cnt_ContinueStep = LowerPc_Send_ContinueStep(&pBuf[9], dataLen);

#if(UPPER_PC_IMMEDIATE_ACK==0)
							UpperPc_RxAck(eCmd, waitTime);
#else
							UpperPc_TxFramePack(eCmd, ACK);
#endif
					} break;
					case CMD_UPPER_JUMP_STEP: //跳转工步
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						tCommLayerCnt.cnt_JumpStep = LowerPc_Send_JumpStep(&pBuf[9], dataLen);

#if(UPPER_PC_IMMEDIATE_ACK==0)
							UpperPc_RxAck(eCmd, waitTime);
#else
							UpperPc_TxFramePack(eCmd, ACK);
#endif
					} break;
					case CMD_UPPER_CH_ENABLE: //通道禁用/启用
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						tCommLayerCnt.cnt_Enable = LowerPc_Send_ChEnanble(&pBuf[9], dataLen);

#if(UPPER_PC_IMMEDIATE_ACK==0)
							UpperPc_RxAck(eCmd, waitTime);
#else
							UpperPc_TxFramePack(eCmd, ACK);
#endif
					} break;
					case CMD_UPPER_CLEAR_WARNING: //清除警告
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						tCommLayerCnt.cnt_ClearWarning = LowerPc_Send_ClearWarning(&pBuf[9], dataLen);

#if(UPPER_PC_IMMEDIATE_ACK==0)
							UpperPc_RxAck(eCmd, waitTime);
#else
							UpperPc_TxFramePack(eCmd, ACK);
#endif
					} break;
					case CMD_UPPER_RESET: //复位
					{
						dataLen = (Data4U8ToU32(pBuf[2], pBuf[3], pBuf[4], pBuf[5]) - 9);
						tCommLayerCnt.cnt_Reset = LowerPc_Send_Reset(&pBuf[9], dataLen);

#if(UPPER_PC_IMMEDIATE_ACK==0)
							UpperPc_RxAck(eCmd, waitTime);
#else
							UpperPc_TxFramePack(eCmd, ACK);
#endif
					} break;
					case CMD_UPPER_GET_CH_REAL_TIME_DATA: //获取各通道实时数据
					{
						UpperPc_TxFramePack(eCmd);
					} break;
					case CMD_UPPER_GET_CH_RECORD_DATA: //获取各通道记录数据
					{
						UpperPc_TxFramePack(eCmd);
					} break;
					default: break;
				}

				sta = COMM_OK; //校验通过
			}
			else
			{
				sta = COMM_ERROR; //校验失败
			}
		}
		else
		{
			sta = COMM_ERROR; //解析失败
		}
	}

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : UpperPc_RxTask
 * @brief : 与上位机通信接收数据处理任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
void UpperPc_RxTask(void *parameter)
{
    uint8_t* rxBuf = (uint8_t *)pvPortMalloc(TEMP_BUF_MAX_LEN); //申请内存缓冲区指针
	if(rxBuf == NULL)
	{
		vTaskDelay(10);
		DEBUG_PRINTF("\r\n ERROR! \r\n");
	}
	memset(rxBuf, 0, TEMP_BUF_MAX_LEN); //清零

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
		if(uQueueGetDataNum(&tUpperPcRxQueue) != 0)
		{
			while(uQueueGetDataNum(&tUpperPcRxQueue) != 0)
			{
				ThreadTime(7);

				//从FIFO读取数据
				sta = uQueueRead(&tUpperPcRxQueue, (uint8_t *)rxBuf, &rxLen); //从FIFO读取数据

				if(sta == QUEUE_OK)
				{
					//解析数据
					UpperPc_RxFrameAnalyze(rxBuf, rxLen);
				}

				/* 调用绝对延时函数,任务时间间隔为2个tick */
				vTaskDelayUntil(&PreviousWakeTime, cycleTimeIncrement);
			}
		}

		/* 调用绝对延时函数,任务时间间隔为1个tick */
		vTaskDelayUntil(&PreviousWakeTime, TimeIncrement);
	}
}



