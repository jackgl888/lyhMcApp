/**
  ******************************************************************************
  * @file    fdcan.c
  * @brief   This file provides code for the configuration
  *          of the FDCAN instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#ifdef USER_CONFIG
	#include "user_config.h"
#endif

/* FDCAN 句柄 */
FDCAN_HandleTypeDef FDCAN1_Handler;
FDCAN_RxHeaderTypeDef FDCAN1_RxHeader;
FDCAN_TxHeaderTypeDef FDCAN1_TxHeader;

/* FDCAN数据缓存 */
uint8_t can1RxRTR = 0;			//CAN1远程帧标志缓存
canID can1RxID;					//CAN1帧ID缓存
canData can1RxData;				//CAN1数据帧缓存
canData can1TxData;				//CAN1数据帧缓存


/* FDCAN 句柄 */
FDCAN_HandleTypeDef FDCAN2_Handler;
FDCAN_RxHeaderTypeDef FDCAN2_RxHeader;
FDCAN_TxHeaderTypeDef FDCAN2_TxHeader;

/* FDCAN数据缓存 */
uint8_t can2RxRTR = 0;			//CAN2远程帧标志缓存
canID can2RxID;					//CAN2帧ID缓存
canData can2RxData;				//CAN2数据帧缓存
canData can2TxData;				//CAN2数据帧缓存

/* USER CODE BEGIN 1 */

/*-----------------------------------------------------------------------------
 * @name  : FDCAN1_Mode_Init(uint16_t presc, uint8_t ntsjw, uint16_t ntsg1,
        					 uint8_t ntsg2, uint32_t mode)
 * @brief :
 * @param : presc：分频值，取值范围1~512
			ntsjw：重新同步跳跃时间单元.范围:1~128
			ntsg1：取值范围2~256
			ntsg2：取值范围2~128
			mode：FDCAN_MODE_NORMAL 普通模式;FDCAN_MODE_EXTERNAL_LOOPBACK 回环模式
 * @retval: 0：初始化OK；其他：初始化失败
 * @date  : 2020/07/20
 * @note  : 配置FDCAN1的时钟源为PLL1Q=200Mhz
 * ---------------------------------------------------------------------------*/
uint8_t FDCAN1_Mode_Init(uint16_t presc, uint8_t ntsjw, uint16_t ntsg1,
        uint8_t ntsg2, uint32_t mode)
{
	uint8_t err = 0;
    //初始化FDCAN1
    HAL_FDCAN_DeInit(&FDCAN1_Handler); 							   //先清除以前的设置
    FDCAN1_Handler.Instance = FDCAN1;
    FDCAN1_Handler.Init.FrameFormat = FDCAN_FRAME_CLASSIC;         //传统模式
    FDCAN1_Handler.Init.Mode = mode;                               //模式选择
    FDCAN1_Handler.Init.AutoRetransmission = DISABLE;     		   //关闭自动重传！传统模式下一定要关闭！！！
    FDCAN1_Handler.Init.TransmitPause = DISABLE;                   //关闭传输暂停
    FDCAN1_Handler.Init.ProtocolException = DISABLE;               //关闭协议异常处理
    FDCAN1_Handler.Init.NominalPrescaler = presc;                  //分频系数
    FDCAN1_Handler.Init.NominalSyncJumpWidth = ntsjw;              //重新同步跳跃宽度
    FDCAN1_Handler.Init.NominalTimeSeg1 = ntsg1;                   //tsg1范围:2~256
    FDCAN1_Handler.Init.NominalTimeSeg2 = ntsg2;                   //tsg2范围:2~128
    FDCAN1_Handler.Init.MessageRAMOffset = 0;                      //信息RAM偏移
    FDCAN1_Handler.Init.StdFiltersNbr = 12;                        //标准信息ID滤波器数量
    FDCAN1_Handler.Init.ExtFiltersNbr = 0;                         //扩展信息ID滤波器数量
    FDCAN1_Handler.Init.RxFifo0ElmtsNbr = 1;                       //接收FIFO0元素编号
    FDCAN1_Handler.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;  	   //接收FIFO0元素大小：8字节
    FDCAN1_Handler.Init.RxBuffersNbr = 0;                          //接收缓冲编号
    FDCAN1_Handler.Init.TxEventsNbr = 0;                           //发送事件编号
    FDCAN1_Handler.Init.TxBuffersNbr = 0;                          //发送缓冲编号
    FDCAN1_Handler.Init.TxFifoQueueElmtsNbr = 2;                   //发送FIFO序列元素编号
    FDCAN1_Handler.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION; //发送FIFO序列模式
    FDCAN1_Handler.Init.TxElmtSize = FDCAN_DATA_BYTES_8;           //发送大小:8字节

    if (HAL_FDCAN_Init(&FDCAN1_Handler) != HAL_OK)
        return 1; //初始化FDCAN

    /* CAN硬件滤波DEBUG */
    err = CAN1_Filter_Init(&FDCAN1_Handler, 0, 0x313, 0x313);		//初始化滤波器  TPDO1
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 1, 0x281, 0x282);		//初始化滤波器  TPDO2
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 2, 0x381, 0x382);		//初始化滤波器  TPDO3
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 3, 0x481, 0x482);		//初始化滤波器  TPDO4
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 4, 0x581, 0x582);		//初始化滤波器   TSDO
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 5, 0x701, 0x702);		//初始化滤波器   NMT
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 6, 0x183, 0x184);		//初始化滤波器  TPDO1
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 7, 0x283, 0x284);		//初始化滤波器  TPDO2
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 8, 0x383, 0x384);		//初始化滤波器  TPDO3
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 9, 0x483, 0x484);		//初始化滤波器  TPDO4
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 10, 0x583, 0x584);		//初始化滤波器   TSDO
    if (err != HAL_OK)
        return err;

    err = CAN1_Filter_Init(&FDCAN1_Handler, 11, 0x703, 0x704);		//初始化滤波器   NMT
    if (err != HAL_OK)
        return err;

    //使能全局滤波器,2 3参数表示对不匹配的11位、29位ID的帧作的处理，我们选择过滤丢弃；45参数表示对远程帧11位、29位ID的处理，程序不会接收到远程帧，所以直接拒绝即可
    if( HAL_FDCAN_ConfigGlobalFilter(&FDCAN1_Handler, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK) 	//全局滤波器开启
        return 3;

    HAL_FDCAN_Start(&FDCAN1_Handler); //开启FDCAN
    HAL_FDCAN_ActivateNotification(&FDCAN1_Handler,FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);  //开启CAN接收中断

    return 0;
}

/*-----------------------------------------------------------------------------
 * @name  : CAN1_Filter_Init(FDCAN_HandleTypeDef *hfdcan, uint32_t FilterIndex,
 * 							 uint32_t FilterID1, uint32_t FilterID2)
 * @brief : 双ID滤波器初始化，主站不需要过滤器，从站需要
 * @param : hfdcan：CAN通信句柄
			FilterID1：需要过滤出来的ID1
			FilterID2：需要过滤出来的ID2
 * @retval: 0：初始化 OK；其他：初始化失败
 * @date  : 2020/07/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t CAN1_Filter_Init(FDCAN_HandleTypeDef *hfdcan, uint32_t FilterIndex, uint32_t FilterID1, uint32_t FilterID2)
{
    FDCAN_FilterTypeDef FDCAN1_RXFilter;

    //配置RX滤波器
    FDCAN1_RXFilter.IdType = FDCAN_STANDARD_ID; 				//FDCAN_STANDARD_ID 标准ID, FDCAN_EXTENDED_ID 拓展ID
    FDCAN1_RXFilter.FilterIndex = FilterIndex;                  //滤波器索引,128个滤波器，0~127
    FDCAN1_RXFilter.FilterType = FDCAN_FILTER_DUAL;        		//滤波器类型:双ID过滤
    FDCAN1_RXFilter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; 	//过滤器0关联到FIFO0
    FDCAN1_RXFilter.FilterID1 = FilterID1;                      //32位ID
    FDCAN1_RXFilter.FilterID2 = FilterID2;                      //32位ID
    //滤波器初始化
    if(HAL_FDCAN_ConfigFilter(hfdcan,&FDCAN1_RXFilter)!=HAL_OK) //滤波器初始化
    	return 2;

    return 0;
}

/*-----------------------------------------------------------------------------
 * @name  : FDCAN1_Send_Msg(uint8_t *msg, uint32_t len, uint32_t id)
 * @brief : CAN数据发送数据帧
 * @param : msg：发送数据指针
			len：发送数据长度(最大为8),可设置为FDCAN_DLC_BYTES_2~FDCAN_DLC_BYTES_8
			id：CAN通信ID
 * @retval: 0：初始化OK；其他：初始化失败
 * @date  : 2020/07/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t FDCAN1_Send_Msg(uint8_t *msg, uint32_t len, uint32_t id)
{
    FDCAN1_TxHeader.Identifier = id;             //11位ID
    FDCAN1_TxHeader.IdType = FDCAN_STANDARD_ID;  //FDCAN_STANDARD_ID 标准ID, FDCAN_EXTENDED_ID 拓展ID
    FDCAN1_TxHeader.TxFrameType = FDCAN_DATA_FRAME; //FDCAN_DATA_FRAME 数据帧, FDCAN_REMOTE_FRAME 远程帧
    FDCAN1_TxHeader.DataLength = len;               //数据长度
    FDCAN1_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    FDCAN1_TxHeader.BitRateSwitch = FDCAN_BRS_OFF;  //关闭速率切换
    FDCAN1_TxHeader.FDFormat = FDCAN_CLASSIC_CAN; 	//传统的CAN模式（8字节数据）/FDCAN模式（64字节数据）
    FDCAN1_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; //无发送事件
    FDCAN1_TxHeader.MessageMarker = 0;

    if (HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN1_Handler, &FDCAN1_TxHeader, msg) != HAL_OK)
    {
    	/* 错误重启 */
    	FDCAN1_Mode_Init(12, 8, 31, 8, FDCAN_MODE_NORMAL); //500kbps:12, 8, 31, 8; 1000kbps:6, 8, 31, 8
    	__HAL_FDCAN_ENABLE_IT(&FDCAN1_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
    	return 1; //发送
    }
    return 0;
}

/*-----------------------------------------------------------------------------
 * @name  : EmergencyDirective1(uint32_t id)
 * @brief : CAN发送远程帧
 * @param : id CAN远程帧ID
 * @retval: 0：初始化OK；其他：初始化失败
 * @date  : 2020/07/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t EmergencyDirective1(uint32_t id)
{
    uint8_t *msg = NULL;
    FDCAN1_TxHeader.Identifier = id;               			 	//32位ID
    FDCAN1_TxHeader.IdType = FDCAN_STANDARD_ID; 				//FDCAN_STANDARD_ID 标准ID, FDCAN_EXTENDED_ID 拓展ID
    FDCAN1_TxHeader.TxFrameType = FDCAN_REMOTE_FRAME; 			//FDCAN_DATA_FRAME 数据帧, FDCAN_REMOTE_FRAME 远程帧
    FDCAN1_TxHeader.DataLength = FDCAN_DLC_BYTES_8;          	//数据长度
    FDCAN1_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    FDCAN1_TxHeader.BitRateSwitch = FDCAN_BRS_OFF;           	//关闭速率切换
    FDCAN1_TxHeader.FDFormat = FDCAN_CLASSIC_CAN; 				//传统的CAN模式（8字节数据）/FDCAN模式（64字节数据）
    FDCAN1_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; 	//无发送事件
    FDCAN1_TxHeader.MessageMarker = 0;

    if (HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN1_Handler, &FDCAN1_TxHeader, msg) != HAL_OK)
    {
    	/* 错误重启 */
    	FDCAN1_Mode_Init(12, 8, 31, 8, FDCAN_MODE_NORMAL); //500kbps:12, 8, 31, 8; 1000kbps:6, 8, 31, 8
    	__HAL_FDCAN_ENABLE_IT(&FDCAN1_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
    	return 1; //发送
    }
    return 0;
}

/*-----------------------------------------------------------------------------
 * @name  : FDCAN1_Receive_Msg(uint8_t *buf)
 * @brief :
 * @param : buf 接收数据缓冲区指针
 * @retval: 0：初始化OK；其他：初始化失败
 * @date  : 2020/07/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t FDCAN1_Receive_Msg(uint8_t *buf)
{
    if (HAL_FDCAN_GetRxMessage(&FDCAN1_Handler, FDCAN_RX_FIFO0, &FDCAN1_RxHeader, buf) != HAL_OK)
        return 0; //接收数据
    return FDCAN1_RxHeader.DataLength >> 16;
}

/*-----------------------------------------------------------------------------
 * @name  : FDCAN2_Mode_Init(uint16_t presc, uint8_t ntsjw, uint16_t ntsg1,
        					 uint8_t ntsg2, uint32_t mode)
 * @brief :
 * @param : presc：分频值，取值范围1~512
			ntsjw：重新同步跳跃时间单元.范围:1~128
			ntsg1：取值范围2~256
			ntsg2：取值范围2~128
			mode：FDCAN_MODE_NORMAL 普通模式;FDCAN_MODE_EXTERNAL_LOOPBACK 回环模式
 * @retval: 0：初始化OK；其他：初始化失败
 * @date  : 2020/07/20
 * @note  : 配置FDCAN2的时钟源为PLL1Q=200Mhz
 * ---------------------------------------------------------------------------*/
uint8_t  FDCAN2_Mode_Init(uint16_t presc, uint8_t ntsjw, uint16_t ntsg1,
                          uint8_t ntsg2, uint32_t mode)
{
	uint8_t err = 0;
    //初始化FDCAN2
    HAL_FDCAN_DeInit(&FDCAN2_Handler); 							   //先清除以前的设置
    FDCAN2_Handler.Instance = FDCAN2;
    FDCAN2_Handler.Init.FrameFormat = FDCAN_FRAME_CLASSIC;         //传统模式
    FDCAN2_Handler.Init.Mode = mode;                               //模式选择
    FDCAN2_Handler.Init.AutoRetransmission = DISABLE;     		   //关闭自动重传！传统模式下一定要关闭！！！
    FDCAN2_Handler.Init.TransmitPause = DISABLE;                   //关闭传输暂停
    FDCAN2_Handler.Init.ProtocolException = DISABLE;               //关闭协议异常处理
    FDCAN2_Handler.Init.NominalPrescaler = presc;                  //分频系数
    FDCAN2_Handler.Init.NominalSyncJumpWidth = ntsjw;              //重新同步跳跃宽度
    FDCAN2_Handler.Init.NominalTimeSeg1 = ntsg1;                   //tsg1范围:2~256
    FDCAN2_Handler.Init.NominalTimeSeg2 = ntsg2;                   //tsg2范围:2~128
    FDCAN2_Handler.Init.MessageRAMOffset = 1000;                   //信息RAM偏移
    FDCAN2_Handler.Init.StdFiltersNbr = 12;                        //标准信息ID滤波器数量
    FDCAN2_Handler.Init.ExtFiltersNbr = 0;                         //扩展信息ID滤波器数量
    FDCAN2_Handler.Init.RxFifo0ElmtsNbr = 32;                      //接收FIFO0元素编号
    FDCAN2_Handler.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;  	   //接收FIFO0元素大小：8字节
    FDCAN2_Handler.Init.RxBuffersNbr = 0;                          //接收缓冲编号
    FDCAN2_Handler.Init.TxEventsNbr = 0;                           //发送事件编号
    FDCAN2_Handler.Init.TxBuffersNbr = 0;                          //发送缓冲编号
    FDCAN2_Handler.Init.TxFifoQueueElmtsNbr = 2;                   //发送FIFO序列元素编号
    FDCAN2_Handler.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION; //发送FIFO序列模式
    FDCAN2_Handler.Init.TxElmtSize = FDCAN_DATA_BYTES_8;           //发送大小:8字节

    if (HAL_FDCAN_Init(&FDCAN2_Handler) != HAL_OK)
        return 1; //初始化FDCAN

    /* CAN硬件滤波DEBUG */
    err = CAN2_Filter_Init(&FDCAN2_Handler, 0, 0x181, 0x182);		//初始化滤波器  TPDO1
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 1, 0x281, 0x282);		//初始化滤波器  TPDO2
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 2, 0x381, 0x382);		//初始化滤波器  TPDO3
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 3, 0x481, 0x482);		//初始化滤波器  TPDO4
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 4, 0x581, 0x582);		//初始化滤波器   TSDO
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 5, 0x701, 0x702);		//初始化滤波器   NMT
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 6, 0x183, 0x184);		//初始化滤波器  TPDO1
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 7, 0x283, 0x284);		//初始化滤波器  TPDO2
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 8, 0x383, 0x384);		//初始化滤波器  TPDO3
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 9, 0x483, 0x484);		//初始化滤波器  TPDO4
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 10, 0x583, 0x584);		//初始化滤波器   TSDO
    if (err != HAL_OK)
        return err;

    err = CAN2_Filter_Init(&FDCAN2_Handler, 11, 0x703, 0x704);		//初始化滤波器   NMT
    if (err != HAL_OK)
        return err;

    //使能全局滤波器,2 3参数表示对不匹配的11位、29位ID的帧作的处理，我们选择过滤丢弃；45参数表示对远程帧11位、29位ID的处理，程序不会接收到远程帧，所以直接拒绝即可
    if( HAL_FDCAN_ConfigGlobalFilter(&FDCAN2_Handler, FDCAN_REJECT,
    		                         FDCAN_REJECT, FDCAN_REJECT_REMOTE,
									 FDCAN_REJECT_REMOTE) != HAL_OK) 	//全局滤波器开启
        return 3;

    HAL_FDCAN_Start(&FDCAN2_Handler); //开启FDCAN
    HAL_FDCAN_ActivateNotification(&FDCAN2_Handler,FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);  //开启CAN接收中断

    return 0;
}

/*-----------------------------------------------------------------------------
 * @name  : CAN2_Filter_Init(FDCAN_HandleTypeDef *hfdcan, uint32_t FilterIndex,
 * 							 uint32_t FilterID1, uint32_t FilterID2)
 * @brief : 双ID滤波器初始化，主站不需要过滤器，从站需要
 * @param : hfdcan：CAN通信句柄
			FilterID1：需要过滤出来的ID1
			FilterID2：需要过滤出来的ID2
 * @retval: 0：初始化 OK；其他：初始化失败
 * @date  : 2020/07/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t CAN2_Filter_Init(FDCAN_HandleTypeDef *hfdcan, uint32_t FilterIndex,
		                 uint32_t FilterID1, uint32_t FilterID2)
{
    FDCAN_FilterTypeDef FDCAN2_RXFilter;

    //配置RX滤波器
    FDCAN2_RXFilter.IdType = FDCAN_STANDARD_ID; 				//FDCAN_STANDARD_ID 标准ID, FDCAN_EXTENDED_ID 拓展ID
    FDCAN2_RXFilter.FilterIndex = FilterIndex;                  //滤波器索引,128个滤波器，0~127
    FDCAN2_RXFilter.FilterType = FDCAN_FILTER_DUAL;        		//滤波器类型:双ID过滤
    FDCAN2_RXFilter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; 	//过滤器0关联到FIFO0
    FDCAN2_RXFilter.FilterID1 = FilterID1;                      //32位ID
    FDCAN2_RXFilter.FilterID2 = FilterID2;                      //32位ID
    //滤波器初始化
    if(HAL_FDCAN_ConfigFilter(hfdcan,&FDCAN2_RXFilter)!=HAL_OK) //滤波器初始化
    	return 2;

    return 0;
}

/*-----------------------------------------------------------------------------
 * @name  : FDCAN2_Send_Msg(uint8_t *msg, uint32_t len, uint32_t id)
 * @brief : CAN数据发送数据帧
 * @param : msg：发送数据指针
			len：发送数据长度(最大为8),可设置为FDCAN_DLC_BYTES_2~FDCAN_DLC_BYTES_8
			id：CAN通信ID
 * @retval: 0：初始化OK；其他：初始化失败
 * @date  : 2020/07/20
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t FDCAN2_Send_Msg(uint8_t *msg, uint32_t len, uint32_t id)
{
    FDCAN2_TxHeader.Identifier = id;                 //11位ID
    FDCAN2_TxHeader.IdType = FDCAN_STANDARD_ID;      //FDCAN_STANDARD_ID 标准ID, FDCAN_EXTENDED_ID 拓展ID
    FDCAN2_TxHeader.TxFrameType = FDCAN_DATA_FRAME;  //FDCAN_DATA_FRAME 数据帧, FDCAN_REMOTE_FRAME 远程帧
    FDCAN2_TxHeader.DataLength = len;                //数据长度
    FDCAN2_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    FDCAN2_TxHeader.BitRateSwitch = FDCAN_BRS_OFF;   //关闭速率切换
    FDCAN2_TxHeader.FDFormat = FDCAN_CLASSIC_CAN; 	 //传统的CAN模式（8字节数据）/FDCAN模式（64字节数据）
    FDCAN2_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; //无发送事件
    FDCAN2_TxHeader.MessageMarker = 0;

    if (HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN2_Handler, &FDCAN2_TxHeader, msg) != HAL_OK)
    {
    	/* 错误重启 */
    	FDCAN2_Mode_Init(6, 8, 31, 8, FDCAN_MODE_NORMAL); //500kbps:12, 8, 31, 8; 1000kbps:6, 8, 31, 8
    	__HAL_FDCAN_ENABLE_IT(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
    	return 1; //发送
    }
    return 0;
}

/*-----------------------------------------------------------------------------
 * @name  : EmergencyDirective2(uint32_t id)
 * @brief : CAN发送远程帧
 * @param : id CAN远程帧ID
 * @retval: 0：初始化OK；其他：初始化失败
 * @date  : 2020/07/20
 * @note  : 用于NMT管理、节点守护
 * ---------------------------------------------------------------------------*/
uint8_t EmergencyDirective2(uint32_t id)
{
    uint8_t *msg = NULL;
    FDCAN2_TxHeader.Identifier = id;               			 	//32位ID
    FDCAN2_TxHeader.IdType = FDCAN_STANDARD_ID; 				//FDCAN_STANDARD_ID 标准ID, FDCAN_EXTENDED_ID 拓展ID
    FDCAN2_TxHeader.TxFrameType = FDCAN_REMOTE_FRAME; 			//FDCAN_DATA_FRAME 数据帧, FDCAN_REMOTE_FRAME 远程帧
    FDCAN2_TxHeader.DataLength = FDCAN_DLC_BYTES_8;          	//数据长度
    FDCAN2_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    FDCAN2_TxHeader.BitRateSwitch = FDCAN_BRS_OFF;           	//关闭速率切换
    FDCAN2_TxHeader.FDFormat = FDCAN_CLASSIC_CAN; 				//传统的CAN模式（8字节数据）/FDCAN模式（64字节数据）
    FDCAN2_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; 	//无发送事件
    FDCAN2_TxHeader.MessageMarker = 0;

    if (HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN2_Handler, &FDCAN2_TxHeader, msg) != HAL_OK)
    {
    	/* 错误重启 */
    	FDCAN2_Mode_Init(6, 8, 31, 8, FDCAN_MODE_NORMAL); //500kbps:12, 8, 31, 8; 1000kbps:6, 8, 31, 8
    	__HAL_FDCAN_ENABLE_IT(&FDCAN2_Handler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
    	return 1; //发送
    }
    return 0;
}

/*-----------------------------------------------------------------------------
 * @name  : FDCAN2_Receive_Msg(uint8_t *buf)
 * @brief :
 * @param : buf 接收数据缓冲区指针
 * @retval: 0：初始化OK；其他：初始化失败
 * @date  : 2020/12/31
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t FDCAN2_Receive_Msg(uint8_t *buf)
{
    if (HAL_FDCAN_GetRxMessage(&FDCAN2_Handler, FDCAN_RX_FIFO0, &FDCAN2_RxHeader, buf) != HAL_OK)
        return 0; //接收数据
    return FDCAN2_RxHeader.DataLength >> 16;
}

/*-----------------------------------------------------------------------------
 * @name  : HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
 * @brief : 底层驱动，引脚配置，时钟使能
 * @param : hfdcan：CAN通信句柄
 * @retval:
 * @date  : 2020/07/20
 * @note  : 此函数会被HAL_FDCAN_DeInit调用
 * ---------------------------------------------------------------------------*/
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspInit 0 */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
	/* FDCAN 时钟源配置为 PLL1Q */
	PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
	  Error_Handler();
	}
  /* USER CODE END FDCAN1_MspInit 0 */
    /* FDCAN1 clock enable */
    __HAL_RCC_FDCAN_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN1 GPIO Configuration
    PB9     ------> FDCAN1_TX
    PB8     ------> FDCAN1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* FDCAN1 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspInit 1 */

  /* USER CODE END FDCAN1_MspInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN2)
  {
  /* USER CODE BEGIN FDCAN2_MspInit 0 */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
	PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
	  Error_Handler();
	}
  /* USER CODE END FDCAN2_MspInit 0 */
    /* FDCAN2 clock enable */
    __HAL_RCC_FDCAN_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN2 GPIO Configuration
    PB12     ------> FDCAN2_RX
    PB13     ------> FDCAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* FDCAN2 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
  /* USER CODE BEGIN FDCAN2_MspInit 1 */

  /* USER CODE END FDCAN2_MspInit 1 */
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_FDCAN_CLK_DISABLE();

    /**FDCAN1 GPIO Configuration
    PH13     ------> FDCAN1_TX
    PH14     ------> FDCAN1_RX
    */
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_13|GPIO_PIN_14);

    /* FDCAN1 interrupt Deinit */
  /* USER CODE BEGIN FDCAN1:FDCAN_CAL_IRQn disable */
    /**
    * Uncomment the line below to disable the "FDCAN_CAL_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE END FDCAN1:FDCAN_CAL_IRQn disable */

  }
  else if(fdcanHandle->Instance==FDCAN2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_FDCAN_CLK_DISABLE();

    /**FDCAN2 GPIO Configuration
    PB12     ------> FDCAN2_RX
    PB13     ------> FDCAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* FDCAN2 interrupt Deinit */
  /* USER CODE BEGIN FDCAN2:FDCAN_CAL_IRQn disable */
    /**
    * Uncomment the line below to disable the "FDCAN_CAL_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
    HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
  /* USER CODE END FDCAN2:FDCAN_CAL_IRQn disable */

  }
}

//FDCAN1中断服务函数
void FDCAN1_IT0_IRQHandler(void)
{
    HAL_FDCAN_IRQHandler(&FDCAN1_Handler);
}

//FDCAN2中断服务函数
void FDCAN2_IT0_IRQHandler(void)
{
    HAL_FDCAN_IRQHandler(&FDCAN2_Handler);
}

/*-----------------------------------------------------------------------------
 * @name  : HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
 * @brief : FIFO0回调函数
 * @param : hfdcan CAN通信句柄
 *			RxFifo0ITs 指示哪个Rx FIFO 0中断是有信号的
 * @retval:
 * @date  : 2020/07/20
 * @note  :
 * ---------------------------------------------------------------------------*/
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) //FIFO0新数据中断
    {
    	if(hfdcan->Instance == FDCAN1)
    	{
            //提取FIFO0中接收到的数据
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN1_RxHeader, can1RxData.data8);

            //回传测试帧类型取值,中断时间间隔太短，不起作用
            can1RxRTR = (FDCAN1_RxHeader.RxFrameType) >> 11U; 	//1远程帧/0数据帧标志位
            can1RxID.id8[1] = FDCAN1_RxHeader.Identifier >> 8U; //标准帧ID高八位、拓展帧ID低8-11位
            can1RxID.id8[0] = FDCAN1_RxHeader.Identifier;       //标准帧ID低八位、拓展帧ID低0-7位

            PowerSupplyDataAnalyse(&can1RxID); //调用数据管理函数,接收反馈的数据

            HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    	}
    	else if(hfdcan->Instance == FDCAN2)
    	{
            //提取FIFO0中接收到的数据
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN2_RxHeader, can2RxData.data8);

            //回传测试帧类型取值,中断时间间隔太短，不起作用
            can2RxRTR = (FDCAN2_RxHeader.RxFrameType) >> 11U; 	//1远程帧/0数据帧标志位
            can2RxID.id8[1] = FDCAN2_RxHeader.Identifier >> 8U; //标准帧ID高八位、拓展帧ID低8-11位
            can2RxID.id8[0] = FDCAN2_RxHeader.Identifier;       //标准帧ID低八位、拓展帧ID低0-7位

            HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    	}
    }
}

/*-----------------------------------------------------------------------------
 * @name  : HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
 * @brief : FIFO0回调函数
 * @param : hfdcan CAN通信句柄
 *			RxFifo0ITs 指示哪个Rx FIFO 0中断是有信号的
 * @retval:
 * @date  : 2020/07/20
 * @note  :
 * ---------------------------------------------------------------------------*/
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
    if ((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != RESET) //FIFO1新数据中断
    {
    	if(hfdcan->Instance == FDCAN2)
    	{
            //提取FIFO1中接收到的数据
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN2_RxHeader, can2RxData.data8);

            //回传测试帧类型取值,中断时间间隔太短，不起作用
            can2RxRTR = (FDCAN2_RxHeader.RxFrameType) >> 11U; 	//1远程帧/0数据帧标志位
            can2RxID.id8[1] = FDCAN2_RxHeader.Identifier >> 8U; //标准帧ID高八位、拓展帧ID低8-11位
            can2RxID.id8[0] = FDCAN2_RxHeader.Identifier;       //标准帧ID低八位、拓展帧ID低0-7位

            HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    	}
    }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
