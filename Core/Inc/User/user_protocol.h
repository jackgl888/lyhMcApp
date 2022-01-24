/*
 * user_protocol.h
 *
 *  Created on: 2021年10月14日
 *      Author: Lyric
 */

#ifndef INC_USER_USER_PROTOCOL_H_
#define INC_USER_USER_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


//------------------------------------- 通信宏定义 -------------------------------------
#define STEP_LOAD_MAX                  64U                       //每工艺最大工步数
#define DEVICE_SPEC                    "5V12A-1978"              //设备规格
#define DEVICE_SN                      "SN 0000 0001"            //设备规格
#define MID_COMPUTER_DRIVERVERSION     "V1.0.0"                  //中位机驱动版本描述
#define MID_COMPUTER_DESCRIPTION       "DES20210910-PCB20210925" //中位机板卡硬件版本描述
#define BI_Version                     "TBM220-10KTI"            //双向电源型号


//-------------------------------- 与下位机通信宏定义 -----------------------------------
#define LOWER_FRAME_HEAD               0x5A       //包头
#define LOWER_FRAME_END                0xA5       //包尾
#define LOWER_CH_TOTAL_NUM             8U         //板卡通道数
#define LOWER_LAYER_TOTAL_NUM          8U         //板卡层数


//-------------------------------- 与上位机通信宏定义 -----------------------------------
#define UPPER_FRAME_HEAD_L             0xEF       //包头
#define UPPER_FRAME_HEAD_H             0xEF       //包头
#define UPPER_FRAME_END_L              0xEE       //包尾
#define UPPER_FRAME_END_H              0xEE       //包尾
#define UPPER_FRAME_HEAD               ((uint16_t)UPPER_FRAME_HEAD_H << 8 | UPPER_FRAME_HEAD_L)    //包头（全）
#define UPPER_FRAME_END                ((uint16_t)UPPER_FRAME_END_H << 8 | UPPER_FRAME_END_L)      //包尾（全）



/* 宏定义 数据转数组索引 接口 */
#define INDEX(a)                       (a-1)


/* 宏定义 数据减一 接口 */
#define MINUS_ONE(a)                   a = ((a>0) ? (a-1) : (0))


/* 宏定义 判断数据是否为零 接口 */
#define JUDGE_ZERO(a)                  ((a==0) ? (1) : (0))



//------------------------------------- 通信枚举 ---------------------------------------

/* 下位机板卡地址枚举 */
typedef enum
{
	COMM_ERROR = 0x00, //通信异常
	COMM_OK    = 0x01, //通信正常
}CommStatus_E;


/* 下位机板卡地址枚举 */
typedef enum
{
	LOWER_COMPUTER_ADDR1    = 0x01, //下位机板卡地址
	LOWER_COMPUTER_ADDR2    = 0x02, //下位机板卡地址
	LOWER_COMPUTER_ADDR3    = 0x03, //下位机板卡地址
	LOWER_COMPUTER_ADDR4    = 0x04, //下位机板卡地址
	LOWER_COMPUTER_ADDR5    = 0x05, //下位机板卡地址
	LOWER_COMPUTER_ADDR6    = 0x06, //下位机板卡地址
	LOWER_COMPUTER_ADDR7    = 0x07, //下位机板卡地址
	LOWER_COMPUTER_ADDR8    = 0x08  //下位机板卡地址
}LowerComputerAddr_E;


/* 控制指令枚举 */
typedef enum
{
	CMD_LOWER_READ_BOARD_INFO    = 0x11, //读取板卡信息
	CMD_LOWER_GET_BOARD_CH_DATA  = 0x12, //获取整板通道实时数据
	CMD_LOWER_GET_BOARD_CH_ADC   = 0x13, //获取整板通道ADC原始数据
	CMD_LOWER_GET_CH_CAL_DATA    = 0x14, //获取单个通道校准参数
	CMD_LOWER_SET_CH_CAL_DATA    = 0x21, //设置单个通道校准参数
	CMD_LOWER_RESET_CH_CAL_DATA  = 0x22, //重置单个通道校准参数
	CMD_LOWER_GLOBAL_SETTING     = 0x23, //全局设置
	CMD_LOWER_CH_STEP_LOADING    = 0x24, //通道工步加载
	CMD_LOWER_POWER_OFF_CONNECT  = 0x31, //断电续接
	CMD_LOWER_START_STEP         = 0x32, //开始工步
	CMD_LOWER_SUSPEND_STEP       = 0x33, //暂停工步
	CMD_LOWER_STOP_STEP          = 0x34, //停止工步
	CMD_LOWER_CONTINUE_STEP	     = 0x35, //继续工步
	CMD_LOWER_JUMP_STEP          = 0x36, //跳转工步
	CMD_LOWER_START_C_D          = 0x37, //下发开始充放电指令
	CMD_LOWER_STOP_C_D           = 0x38, //下发停止充放电指令
	CMD_LOWER_CLEAR_WARNING      = 0x41, //清除警告
	CMD_LOWER_RESET              = 0x42, //复位指令
	CMD_LOWER_CH_ENABLE          = 0x43  //通道禁用/启用
}LowerControlCmd_E;


/* 控制指令枚举 */
typedef enum
{
	CMD_UPPER_READ_DEVICE_INFO      = 0x0101, //读取设备信息
	CMD_UPPER_UPDATE_DEVICE_INFO    = 0x0102, //修改设备信息
	CMD_UPPER_ADV_SET               = 0x0103, //高级设置
	CMD_UPPER_GLOBAL_SETTING        = 0x0201, //全局设置
	CMD_UPPER_STEP_LOADING          = 0x0202, //工步加载
	CMD_UPPER_START_STEP            = 0x0301, //开始工步
	CMD_UPPER_STOP_STEP             = 0x0302, //停止工步
	CMD_UPPER_SUSPEND_STEP          = 0x0303, //暂停工步
	CMD_UPPER_CONTINUE_STEP         = 0x0304, //继续工步
	CMD_UPPER_JUMP_STEP             = 0x0305, //跳转工步
	CMD_UPPER_CH_ENABLE             = 0x0306, //通道禁用/启用
	CMD_UPPER_CLEAR_WARNING         = 0x0307, //清除警告
	CMD_UPPER_RESET                 = 0x0308, //复位
	CMD_UPPER_GET_CH_REAL_TIME_DATA = 0x0401, //获取各通道实时数据
	CMD_UPPER_GET_CH_RECORD_DATA    = 0x0402  //获取各通道记录数据
}UpperControlCmd_E;


/* 工步名称枚举 */
typedef enum
{
	STEP_SET_STANDBY  = 0x00, //工步待机
	STEP_SET_ASIDE    = 0x01, //工步搁置
	STEP_CC_CHARGE    = 0x02, //恒流充电
	STEP_CC_CV_CHARGE = 0x03, //恒流恒压充电
	STEP_CC_DISCHARGE = 0x04, //恒流放电
	STEP_END          = 0x05, //结束
	STEP_CYCLE        = 0x06  //循环
}StepName_E;


/* 通道使能枚举 */
typedef enum
{
	ENABLE_CHANNEL  = 0x01, //使能通道
	DISABLE_CHANNEL = 0x00  //失能通道
}Channel_EN_Status_E;


/* 通道状态枚举 */
typedef enum
{
	CH_STA_OFF_LINE  = 0x00, //离线
	CH_STA_STANDBY   = 0x01, //待机
	CH_STA_SHELVE    = 0x02, //通道搁置
	CH_STA_CHARGE    = 0x03, //充电
	CH_STA_DISCHARGE = 0x04, //充电
	CH_STA_SUSPEND   = 0x05, //暂停
	CH_STA_STOP      = 0x06, //停止
	CH_STA_FINISH    = 0x07, //完成
	CH_STA_PRORECT   = 0x08, //保护
	CH_STA_DISABLE   = 0x09  //禁用
}ChannelStatus_E;


/* 应答标识枚举 */
typedef enum
{
	ACK  = 0x00, //接收指令成功
	NACK = 0x01  //接收指令失败
}ResponseFlag_E;


/* 充放电模式枚举 */
typedef enum
{
	CHARGE_MODE    = 0, //充电
	DISCHARGE_MODE = 1  //放电
}Channel_C_D_Mode_E;


/* 校准点类型枚举 */
typedef enum
{
	CHARGE_CUR_ADC_CAL    = 0x01, //充电电流ADC校准
	DISCHARGE_CUR_ADC_CAL = 0x02, //放电电流ADC校准

	CHARGE_CUR_DAC_CAL    = 0x03, //充电电流DAC校准
	DISCHARGE_CUR_DAC_CAL = 0x04, //放电电流DAC校准

	CHARGE_VOL_ADC_CAL    = 0x05, //充电电压ADC校准
	DISCHARGE_VOL_ADC_CAL = 0x06, //放电电压ADC校准

	CHARGE_VOL_DAC_CAL    = 0x07, //充电电压DAC校准
	DISCHARGE_VOL_DAC_CAL = 0x08, //放电电压DAC校准
}CailPointsType_E;


/* 是否校准输出标志位枚举 */
typedef enum
{
	NEED_ADJUST = 0x00, //需要校准输出
	NO_ADJUST   = 0x01  //不需要校准输出
}AdjustADDA_E;


/* 是否校准输出标志位枚举 */
typedef enum
{
	CH_POSITIVE_POL = 0x00, //正极性
	NO_NEGATIVE_POL = 0x01  //负极性
}ChannelPolarity_E;


//------------------------------------ 通信数据结构 ------------------------------------

#pragma  pack(push)  	//保存对齐状态
#pragma  pack(1)		//改为1字节对齐，防止内存对齐导致的错误


/* 通信保护结构体 */
typedef struct
{
	__IO uint8_t Upper_PC_Comm_Flag;   //上位机正常通信标志，在获取设置信息指令后才可与其他指令进行通信
	__IO uint8_t Lose_Power_Comm_Flag; //断电续接通信标志，在同步通信后才可与上位机进行通信
	__IO uint8_t Comm_Sync_Flag;       //通信同步标志
	__IO uint8_t Tcp_Comm_Flag[3];     //TCP连接标志
	__IO uint32_t Tcp_Timeout_MS[3];   //TCP保活时间-毫秒 ms
	__IO uint32_t Tcp_Timeout_S[3];    //TCP保活时间-秒 s
	uint32_t tempReg;                  //以太网芯片BSR寄存器缓存，判断掉线
}CommProtect_T;

CommProtect_T tCommPro;


/* 通道变量结构体 */
typedef struct
{
	uint8_t layerTotalNum; //设备总层数 如：LOWER_CH_TOTAL_NUM
	uint8_t	chTotalNum;    //每层通道数 如：LOWER_LAYER_TOTAL_NUM
}ChannelTotalNum_T;

ChannelTotalNum_T tChNum; //总通道数量 = 设备总层数 * 每层通道数;


/* 通信应答状态结构体 */
typedef struct
{
	ResponseFlag_E ReadDeviceInfoSta;    //读取设备信息的通信状态
	ResponseFlag_E GetChRealTimeDataSta; //获取各通道实时数据的通信状态
}CommAckStatus_T;

CommAckStatus_T tCommAckSta; //通信应答状态


/* 通道异常标志联合体 */
typedef union
{
	struct
	{
		uint8_t volUpCutoff         :1; // bit0 电压上限截止，检测电压超过设置全局上限电压
		uint8_t volDownCutoff       :1; // bit1 电压下限截止 检测电压低于设置全局下限电压
		uint8_t volReverseInc       :1; // bit2 电压反向增长保护 检测电压变化状态与当前设置趋势相反
		uint8_t volGradientAbnormal :1; // bit3 电压变化率异常保护 检测电压变化速度偏离与电流/时间设定范围
		uint8_t curUpCutoff         :1; // bit4 电流上限截止，检测电流超过设置全局上限电压
		uint8_t curDownCutoff       :1; // bit5 电流下限截止 检测电流低于设置全局下限电压
		uint8_t curReverseInc       :1; // bit6 电流反向增长保护 检测电流变化状态与当前设置趋势相反
		uint8_t curGradientAbnormal :1;	// bit7 电流变化率异常保护 检测电流变化速度偏离与电流/时间设定范围

		uint8_t curAppearUnworked   :1;	// bit8 电流异常-通道未运行产生电流，检测通道静止条件下电流动态，出现电流数值
		uint8_t curDisappearWorked  :1;	// bit9 电流异常-通道运行未产生电流，检测通道充放电条件下电流动态，出现电流数值
		uint8_t tempUpCutoff        :1; // bit10 温度上限截止，检测点的温度超过设置上限温度值
		uint8_t tempDownCutoff      :1; // bit11 温度下限截止，检测点的温度超过设置下限温度值
		uint8_t capUpCutoff         :1; // bit12 充电容量上限截止，检测电池充电容量达到设置全局上限容量
		uint8_t capDownCutoff       :1; // bit13 放电容量下限截止，检测电池充电容量达到设置全局下限容量
		uint8_t batNotAccessed      :1; // bit14 电池未接入，检测电池端是否有电压为判断电池接⼊条件
		uint8_t batOverVol          :1; // bit15 电池过压，检测电池电压超过设置值上限电压

		uint8_t batUnderVol         :1; // bit16 电池欠压，检测电池电压低于设置值下限电压
		uint8_t batUnderCur         :1; // bit17 电池过流，检测通过电池的电流超过设置值上限电流
		uint8_t relayOpenAbnormal   :1;	// bit18 电池继电器开路故障，检测通道的继电器⼯作状态处于失控状态
		uint8_t startAbnormal       :1; // bit19 启动失败，检查通道控制状态，⽆法进行对应的设置动作
		uint8_t SamVolLowCutOffVol  :1; // bit20 电压低于截止电压
		uint8_t SamVolHighCutOffVol :1; // bit21 电压高于截止电压
		uint8_t curUnderCutoff      :1; // bit22 预留位
		uint8_t reserved1           :1; // bit23 预留位

		uint8_t reserved2           :1; // bit24 预留位
		uint8_t reserved3           :1; // bit25 预留位
		uint8_t reserved4           :1; // bit26 预留位
		uint8_t reserved5           :1; // bit27 预留位
		uint8_t reserved6           :1; // bit28 预留位
		uint8_t reserved7           :1; // bit29 预留位
		uint8_t reserved8           :1; // bit30 预留位
		uint8_t reserved9           :1; // bit31 预留位
	}StaBit;
	uint32_t Sta;
}ChannelAbnormalStatus_U;

ChannelAbnormalStatus_U uChAbnormalSta[LOWER_CH_TOTAL_NUM*LOWER_LAYER_TOTAL_NUM];


/* 层异常标志联合体 */
typedef union
{
	struct
	{
		uint8_t busOverVol         :1; // bit0 母线过压，检测双向电源输入的母线电压超过了设置上限电压
		uint8_t busUnderVol        :1; // bit1 母线欠压，检测双向电源输入的母线电压低于设置下限电压
		uint8_t busOverCur         :1; // bit2 母线过流，检测双向电源输入的母线电流高于设置上限电流
		uint8_t layerResetAbnormal :1; // bit3 层复位失败
		uint8_t reserved0          :1; // bit4 预留位
		uint8_t reserved1          :1; // bit5 预留位
		uint8_t reserved2          :1; // bit6 预留位
		uint8_t reserved3          :1; // bit7 预留位

		uint8_t reserved4          :1; // bit8 预留位
		uint8_t reserved5          :1; // bit9 预留位
		uint8_t reserved6          :1; // bit10 预留位
		uint8_t reserved7          :1; // bit11 预留位
		uint8_t reserved8          :1; // bit12 预留位
		uint8_t reserved9          :1; // bit13 预留位
		uint8_t reserved10		   :1; // bit14 预留位
		uint8_t reserved11         :1; // bit15 预留位
	}StaBit;
	uint16_t sta;
}LayerAbnormalStatus_U;

LayerAbnormalStatus_U uLayerAbnormalSta[LOWER_LAYER_TOTAL_NUM];


/* 系统异常标志联合体 */
typedef union
{
	struct
	{
		uint8_t fanAbnormal   :1; // bit0 风扇故障
		uint8_t reserved0     :1; // bit1 预留位
		uint8_t reserved1     :1; // bit2 预留位
		uint8_t reserved2     :1; // bit3 预留位
		uint8_t reserved3     :1; // bit4 预留位
		uint8_t reserved4     :1; // bit5 预留位
		uint8_t reserved5     :1; // bit6 预留位
		uint8_t reserved6     :1; // bit7 预留位
		uint8_t reserved7     :1; // bit8 预留位
		uint8_t reserved8     :1; // bit9 预留位
		uint8_t reserved9     :1; // bit10 预留位
		uint8_t reserved10    :1; // bit11 预留位
		uint8_t reserved11    :1; // bit12 预留位
		uint8_t reserved12    :1; // bit13 预留位
		uint8_t reserved13    :1; // bit14 预留位
		uint8_t reserved14    :1; // bit15 预留位
	}StaBit;
	uint16_t sta;
}SysAbnormalStatus_U;

SysAbnormalStatus_U uSysAbnormalSta;


/* 单通道实时数据结构体 */
typedef struct
{
	uint8_t chID;          //通道号
	uint8_t	chStatus;      //通道状态
	uint8_t	chPolarity;	   //通道极性
	uint8_t	stepTAG;       //工步标识符
	uint8_t	stepSerialNum; //工步序号
	uint8_t	stepID;        //当前工步号
	uint8_t	cycleNum;      //循环次数
	uint8_t	stepName;      //工步名称
	uint32_t stepRuntime;  //当前工步运行时间
	float voltage;         //电压
	float current;         //电流
	float capacity;        //容量
	float energy;          //能量
	uint16_t temperature;  //温度
	uint32_t chWarnStatus; //通道异常状态
}ChannelRealTimeData_T;

ChannelRealTimeData_T tChRealTimeDat[LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM]; //通道实时数据 TODO:用于触控屏启停机界面显示
ChannelRealTimeData_T tChRecordTestDat[LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM]; //通道记录数据 测试用


/* 单通道回传数据结构体 */
typedef struct
{
	uint8_t chDataType;            //通道数据类型
	ChannelRealTimeData_T tChData; //通道数据
}ChannelRecordData_T;


/* 通道回传数据结构体缓冲区 */
typedef struct
{
	uint8_t	chTotalDataNum[LOWER_LAYER_TOTAL_NUM]; //每层通道数量
	ChannelRecordData_T tChReData[LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM]; //结构体数组缓冲区
}ChannelRecordDataBuff_T;

ChannelRecordDataBuff_T tChRecordData;


/* 整板通道ADC原始数据 结构体 */
typedef struct
{
	uint8_t chID;			//通道号
	float voltage;			//通道未校准的电压值
	float current;			//通道未校准的电流值
}ChannelRawData_T;

ChannelRawData_T tChRawData[LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM]; //TODO:在校准界面需获取整板通道ADC原始数据，待添加


/* 电流ADC校准点结构体 */
typedef struct
{
	float currentSamp;		//采样电流值
	float currentTrue;		//真实电流值
}CurAdcPoints_T;


/* 电流DAC校准点结构体 */
typedef struct
{
	float currentSet;		//设定电流值
	float currentTrue;		//真实电流值
}CurDacPoints_T;


/* 电压ADC校准点结构体 */
typedef struct
{
	float voltageSamp;		//采样电压值
	float voltageTrue;		//真实电压值
}VolAdcPoints_T;


/* 电压DAC校准点结构体 */
typedef struct
{
	float voltageSet;		//设定电压值
	float voltageTrue;		//真实电压值
}VolDacPoints_T;


/* 单个通道总校准参数结构体 */
typedef struct
{
	uint8_t chID;          //通道号
	uint8_t calPolarity;   //校准极性类型（0为正极性、1为负极性）
	uint8_t calTypeNum;    //校准类型总数

	uint8_t chargeCurAdcPointsNum;            //充电电流ADC校准点数
	CurAdcPoints_T chargeCurAdcPoints[10];
	uint8_t dischargeCurAdcPointsNum;         //放电电流ADC校准点数
	CurAdcPoints_T dischargeCurAdcPoints[10];

	uint8_t chargeCurDacPointsNum;            //充电电流DAC校准点数
	CurDacPoints_T chargeCurDacPoints[10];
	uint8_t dischargeCurDacPointsNum;         //放电电流DAC校准点数
	CurDacPoints_T dischargeCurDacPoints[10];

	uint8_t chargeVolAdcPointsNum;            //充电电压ADC校准点数
	VolAdcPoints_T chargeVolAdcPoints[10];
	uint8_t dischargeVolAdcPointsNum;         //放电电压ADC校准点数
	VolAdcPoints_T dischargeVolAdcPoints[10];

	uint8_t chargeVolDacPointsNum;            //充电电压DAC校准点数
	VolDacPoints_T chargeVololDacPoints[10];
	uint8_t dischargeVolDacPointsNum;         //放电电压DAC校准点数
	VolDacPoints_T dischargeVolDacPoints[10];
}ChannelCaliData_T;

ChannelCaliData_T tGetChCalData[LOWER_LAYER_TOTAL_NUM][LOWER_CH_TOTAL_NUM];
ChannelCaliData_T tSetChCalData[LOWER_LAYER_TOTAL_NUM][LOWER_CH_TOTAL_NUM];


/* 板卡信息 结构体 */
typedef struct
{
	uint8_t chTotalNum;		//板子通道总数
	uint16_t rateVoltage;	//标称充放电电压规格
	uint16_t rateCurrent;	//标称充放电电流规格
	char driverVersion[16]; //板卡驱动版本描述
	char description[40];   //板卡硬件版本描述
}LayerInformation_T;

LayerInformation_T tLayerInfo;


/*下发充放电指令结构体*/
typedef struct
{
	uint8_t mode;	    //模式标志位
	uint8_t adjustFlag;	//DAC是否需要校准再输出标志位，若要校准输出，则为0；若不需要，则为1
	float setVoltage;	//设置电压值（V）
	float setCurrent;	//设置电流值（A）
}C_D_Cmd_T;

C_D_Cmd_T tCD_Cmd[LOWER_LAYER_TOTAL_NUM][LOWER_CH_TOTAL_NUM];


/* 每层驱动及PCB信息 */
typedef struct
{
	uint8_t layerID;        //层号
	char DriverVersion[16]; //下位机驱动版本描述
	char Description[40];   //下位机板卡硬件版本描述
}LayerInfo_T;


/* 设备信息 */
typedef struct
{
	char deviceSPEC[16];                           //设备规格
	char deviceSN[16];                             //设备序列号
	char midDriverVersion[16];                     //中位机驱动版本描述
	char midDescription[40];                       //中位机板卡硬件版本描述
	uint8_t layerTotalNum;                         //总层数
	LayerInfo_T tlayerInfo[LOWER_LAYER_TOTAL_NUM]; //每层信息
	uint8_t chTotalNum;                            //每层通道个数
	char ipAddr[16];                               //IP地址
	char maskAddr[16];                             //子网掩码
	char gwAddr[16];                               //网关
	char description[16];                          //双向电源型号
	uint16_t deviceID;                             //设备位于产线的ID
}DeviceInfo_T;

DeviceInfo_T tDeviceInfo;


/* 全局设置参数结构体 */
typedef struct
{
	uint16_t samPeriod;     //时间采样周期,单位：s
	uint8_t chPolarity;     //通道极性设置：0x01反接，0x00正接（默认）
	float maxChargeCap;	    //报警充电容量上限,单位：mAh
	float minDisChargeCap;  //报警放电容量下限，单位：mAh
	float maxVoltage;       //报警电压上限，单位：V，精度0.001
	float minVoltage;       //报警电压下限，单位：V，精度0.001
	float maxCurrent;       //报警电流上限，单位：A，精度0.001
	float minCurrent;       //报警电流下限，单位：A，精度0.001
	int16_t maxTemperature; //报警温度上限，单位：℃，精度0.01
	int16_t minTemperature; //报警温度下限，单位：℃，精度0.01
}GlobalPramSet_T;

GlobalPramSet_T tGlobalPramSet; //TODO:用于触控屏启停机界面显示


/* 子工步结构体 */
typedef struct
{
	uint8_t stepID;       //工步号
	uint8_t stepName;     //工步名称
	uint32_t stepTime;    //工步时间
	float setCurrent; 	  //充电电流（0.001A）
	float cutOffVoltage;  //截止电压（0.001V）
	float cutOffCurrent;  //截止电流（0.001A）
	uint8_t startStepID;  //起始工步号
	uint8_t cycleNum;	  //循环次数
	uint16_t stepMaxTemp; //工步温度上限（0.01℃）
	uint16_t stepMinTemp; //工步温度下限（0.01℃）
}StepLoad_T;

StepLoad_T tStepLoad;


/* 单个通道工艺结构体（1410Bytes 20211007）, 16通道就是23k */
typedef struct
{
	uint8_t stepTAG;                     //工步标识符
	uint8_t stepTotalNum;                //工步总数（实际载入的工步数）
    StepLoad_T tStepLoad[STEP_LOAD_MAX]; //子工步结构体数组（按恒流恒压充电参数为最大长度）
}StepDownload_T;

StepDownload_T tChStepLoad[LOWER_CH_TOTAL_NUM];




#pragma pack(pop)


//------------------------------------- 函数声明 --------------------------------------
void Comm_DataInit(void);

/* 中上通信校验 */
uint16_t CheckSum(const uint8_t* buff, uint16_t len);

/* 中下通信校验 */
uint16_t CheckSum16(const uint8_t *pData, uint16_t len);



#ifdef __cplusplus
}
#endif

#endif /* INC_USER_USER_PROTOCOL_H_ */
