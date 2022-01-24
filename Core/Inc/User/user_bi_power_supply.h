/*-----------------------------------------------------------------------------
 * @file        : user_bi_power_supply.h
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2021/09/30
 * @date changed: 2021/09/30
 * @brief       : 图为科技 TBM系列三相AC-DC电源模块
 *              : 波特率：500K
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/

#ifndef INC_USER_USER_BI_POWER_SUPPLY_H_
#define INC_USER_USER_BI_POWER_SUPPLY_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "fdcan.h"

//指令枚举
typedef enum
{
	ALARM_CMD       = 0x00, //告警信息指令
	STATUS_CMD      = 0x01, //状态信息指令
	ANALOG_DC_CMD   = 0x02, //模拟量信息直流侧信息指令
	ANALOG_AC_V_CMD = 0x03, //模拟量信息交流侧电压信息指令
	ANALOG_AC_C_CMD = 0x04, //模拟量信息交流侧电流信息指令
	NUMBER_CMD      = 0x05  //指令数量
}PowerSupply_CMD_E;

//帧ID枚举 使用：FrameID + 电源ID
typedef enum
{
	ALARM_FRAME_ID  = 0x380, //告警信息   0x380
	STATUS_FRAME_ID = 0x480, //状态信息   0x480
	ANALOG_FRAME_ID = 0x580  //模拟量信息 0x580
}FrameID_E;

//源ID划分
typedef enum
{
	BRD_FRAME = 0x7F, //广播帧 0x7F
	M0N_FRAME = 0xFE  //监控   0xFE
}SourceID_E;

//Rs/Rq枚举 使用：Frame + ServiceCode + 电源号
typedef enum
{
	RQ_FRAME = 0x00, //请求帧 0x00
	RS_FRAME = 0x80  //响应帧 0x80
}TypeFrame_E;

//服务码 使用：ServiceCode + 电源号
typedef enum
{
	ALARM_SERVICE_CODE       = 0x10, //告警信息服务码 0x10
	STATUS_SERVICE_CODE      = 0x10, //状态信息服务码 0x10
	ANALOG_DC_SERVICE_CODE   = 0x10, //模拟量信息直流侧服务码 0x10
	ANALOG_AC_V_SERVICE_CODE = 0x11, //模拟量信息交流侧电压信息服务码 0x11
	ANALOG_AC_C_SERVICE_CODE = 0x12  //模拟量信息交流侧电流信息服务码 0x12
}ServiceCode_E;


#pragma  pack(push)  	//保存对齐状态
#pragma  pack(1)		//改为1字节对齐，防止内存对齐导致的错误

//-----------------用户数据结构------------------------

//告警信息
typedef union
{
	struct
	{
		uint8_t bit0_fanFault :1;				// 风扇故障			0 正常 1 异常
		uint8_t bit1_busBarFault :1;	    	// 母线故障			0 正常 1 异常
		uint8_t bit2_rectifierFault :1;		    // 整流器故障			0 正常 1 异常
		uint8_t bit3_overTempFault :1;	    	// 过温故障			0 正常 1 异常

		uint8_t bit4_noTempSensorFault :1;	    // 无温度传感器故障	0 正常 1 异常
		uint8_t bit5_chargerHighVol :1;		    // 充电器高压			0 正常 1 异常
		uint8_t bit6_chargerLowVol :1;			// 充电器低压			0 正常 1 异常
		uint8_t bit7_chargerShortCircuit :1;	// 充电器短路			0 正常 1 异常

		uint8_t bit8_15_reserved :8;			// 预留
	}AlarmMsgBit;
	uint16_t alarmMsg;
}PowerSupplyAlarmMessage_U;

PowerSupplyAlarmMessage_U uPowerAlarmData;

//状态信息
typedef union
{
	struct
	{
		uint8_t bit0_electriCity :1;				// 市电OK			0 异常 1 正常
		uint8_t bit1_workDirection :1;				// 工作方向			0 正向 1 异反向
		uint8_t bit2_freqAdaptiveFlag :1;			// 频率自适应标志		0 自适应中 1 自适应结束
		uint8_t bit3_machineMasterSlaveFlag :1;	    // 并机主机标志		0 从机 1 主机
		uint8_t bit4_5_sysSupplyStatus :2;			// 整机侧供应状态		0 均不供电 1 硬件软起 2 主动软起 3 正常输出
		uint8_t bit6_7_chargeStatus :2;			    // 充电器状态			0 均不供电 1 软起 3 正常输出
		uint8_t bit8_11_SlaveNum :4;				// 并机台数
		uint8_t bit12_phaseLockStatus :1;			// 锁相状态			0 锁相异常 1 锁相OK
		uint8_t bit13_CurrentLimitedStatus :1;		// 限流状态			0 不限流 1 限流
		uint8_t bit14_reserved :1;					// 预留
		uint8_t bit15_reserved :1;					// 预留

		uint8_t bit16_19_deviceID :4;				// 模块ID设备
		uint8_t bit20_deviceFreq :1;				// 频率体系			0 频率50Hz 1 频率60Hz
		uint8_t bit21_autoBurnIn :1;	    		// 是否自老化过		0 未自老化 1 自老化过
		uint8_t bit22_25_swVerLowFour :4;			// 软件版本低4位
		uint8_t bit26_inputThreePhasePositive :1;	// 输入三相正负信息	0 正序 1 负序
		uint8_t bit27_inputPhaseNum :1;			    // 单相/三相输入		0 三相 1 单相
		uint8_t bit28_reserved :1;	    			// 预留
		uint8_t bit29_31_swVerHighThree :4;		    // 软件版本高3位
	}StatusMsgBit;
	uint32_t statusMsg;
}PowerSupplyStatusMessage_U;

PowerSupplyStatusMessage_U uPowerStatusData;

//模拟量信息
typedef struct
{
	//直流侧信息
	uint16_t DCVol;   //组内直流平均电压 /100
	uint16_t DCCur;   //组内直流电流总和 /100
	uint16_t DCPower; //组内直流侧功率   /1

	//交流侧
	uint16_t ACVolA;  //组内交流侧A相电压的均值 /10
	uint16_t ACVolB;  //组内交流侧B相电压的均值 /10
	uint16_t ACVolC;  //组内交流侧C相电压的均值 /10

	uint16_t ACCurA;  //组内交流侧A相电流的总和 /100
	uint16_t ACCurB;  //组内交流侧B相电流的总和 /100
	uint16_t ACCurC;  //组内交流侧C相电流的总和 /100
}PowerSupplyAnalogMessage_T;

PowerSupplyAnalogMessage_T tPowerAnalogData;

#pragma pack(pop)


uint8_t PowerSupplySend(PowerSupply_CMD_E eCmd, uint8_t id);
void PowerSupplyDataAnalyse(canID *canRxID);

#ifdef __cplusplus
}
#endif


#endif /* INC_USER_USER_BI_POWER_SUPPLY_H_ */
