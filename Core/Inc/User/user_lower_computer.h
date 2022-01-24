/*-----------------------------------------------------------------------------
 * @file        : user_lower_computer.h
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2019/10/18
 * @date changed: 2021/10/22
 * @brief       : 中下位机通信处理程序
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#ifndef INC_USER_USER_LOWER_COMPUTER_H_
#define INC_USER_USER_LOWER_COMPUTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USER_CONFIG
	#include "user_config.h"
#endif


#define LOWER_PC_TEST           0 //中位机单独与上位机测试用


/* 缓存区配置 */
#define LOWER_PC_BUF_MAX_LEN    (2048) //限制单条数据大小
#define LOWER_PC_BUF_MAX_CNT    (64)   //限制缓冲区总数据容量(条数)


/* 锁保护 */
#define	LOWER_PC_LOCK()         do{(LowerPcMutex==NULL) ? xSemaphoreTake(LowerPcMutex, portMAX_DELAY) : pdFAIL;}while(0);
#define LOWER_PC_UNLOCK()       do{(LowerPcMutex==NULL) ? xSemaphoreGive(LowerPcMutex) : pdFAIL;}while(0);

/* 锁保护 */
#define	LOWER_PC_CALI_LOCK()    do{(LowerPcCaliMutex==NULL) ? xSemaphoreTake(LowerPcCaliMutex, portMAX_DELAY) : pdFAIL;}while(0);
#define LOWER_PC_CALI_UNLOCK()  do{(LowerPcCaliMutex==NULL) ? xSemaphoreGive(LowerPcCaliMutex) : pdFAIL;}while(0);


#pragma  pack(push)  	//保存对齐状态
#pragma  pack(1)		//改为1字节对齐，防止内存对齐导致的错误

//-----------------用户数据结构------------------------

/* 接收数据缓存区 */
typedef struct
{
	uint8_t RxDataBuf[UART2_RX_SIZE];
	uint16_t RxDataLen;
}LowerPC_RxData_T;

LowerPC_RxData_T tLowerPC_Rx;


/* 存在的层数记录 */
typedef union
{
	struct
	{
		uint8_t existLayer1  :1; // bit0 第1层
		uint8_t existLayer2  :1; // bit1 第2层
		uint8_t existLayer3  :1; // bit2 第3层
		uint8_t existLayer4  :1; // bit3 第4层
		uint8_t existLayer5  :1; // bit4 第5层
		uint8_t existLayer6  :1; // bit5 第6层
		uint8_t existLayer7  :1; // bit6 第7层
		uint8_t existLayer8  :1; // bit7 第8层
	}LayerBit;
	uint8_t layerNum;
}ExistLayer_U;

ExistLayer_U uExistLayer;


/* 存在的通道数记录 */
typedef union
{
	uint16_t existCh[LOWER_LAYER_TOTAL_NUM][LOWER_CH_TOTAL_NUM]; //通道记录，（层：1~8，通道：1~16）
}ExistCh_T;

ExistCh_T tExistCh;


/* 要通信的层数 */
typedef struct
{
	__IO uint8_t cnt_GlobalSetting;   // 全局设置
	__IO uint8_t cnt_ChStepLoading;   // 通道工步加载
	__IO uint8_t cnt_StartStep;       // 开始工步
	__IO uint8_t cnt_SuspendStep;     // 暂停工步
	__IO uint8_t cnt_StopStep;        // 停止工步
	__IO uint8_t cnt_ContinueStep;    // 继续工步
	__IO uint8_t cnt_JumpStep;        // 跳转工步
	__IO uint8_t cnt_ClearWarning;    // 清除警告
	__IO uint8_t cnt_Reset;           // 复位指令
	__IO uint8_t cnt_Enable;          // 通道禁用/启用指令
}CommLayerCnt_T;

CommLayerCnt_T tCommLayerCnt;


/* 校准点类型设置联合体 */
typedef union
{
	struct
	{
		uint8_t chargeCurAdc    :1; // bit0 充电电流ADC校准
		uint8_t dischargeCurAdc :1; // bit1 放电电流ADC校准
		uint8_t chargeCurDac    :1; // bit2 充电电流DAC校准
		uint8_t dischargeCurDac :1; // bit3 放电电流DAC校准
		uint8_t chargeVolAdc    :1; // bit4 充电电压ADC校准
		uint8_t dischargeVolAdc :1; // bit5 放电电压ADC校准
		uint8_t chargeVolDac    :1; // bit6 充电电压DAC校准
		uint8_t dischargeVolDac :1; // bit7 放电电压DAC校准
	}StaBit;
	uint8_t sta;
}CailPointsType_U;

CailPointsType_U uCalType;


/* 通信指令状态 */
typedef union
{
	struct
	{
		uint8_t sta_ReadBoardInfo   :1; // bit0 读取板卡信息
		uint8_t sta_GetBoardChData  :1; // bit1 获取整板通道实时数据
		uint8_t sta_GetBoardChAdc   :1; // bit2 获取整板通道ADC原始数据
		uint8_t sta_GetChCalData    :1; // bit3 获取单个通道校准参数

		uint8_t sta_SetChCalData    :1; // bit4 设置单个通道校准参数
		uint8_t sta_ResetChCalData  :1; // bit5 重置单个通道校准参数
		uint8_t sta_GlobalSetting   :1; // bit6 全局设置
		uint8_t sta_ChStepLoading   :1; // bit7 通道工步加载

		uint8_t sta_PowerOffConnect :1; // bit8 断电续接
		uint8_t sta_StartStep       :1; // bit9 开始工步
		uint8_t sta_SuspendStep     :1; // bit10 暂停工步
		uint8_t sta_StopStep        :1; // bit11 停止工步

		uint8_t sta_ContinueStep    :1; // bit12 继续工步
		uint8_t sta_JumpStep        :1; // bit13 跳转工步
		uint8_t sta_Start_C_D       :1; // bit14 下发开始充放电指令
		uint8_t sta_Stop_C_D        :1; // bit15 下发停止充放电指令

		uint8_t sta_ClearWarning    :1; // bit16 清除警告
		uint8_t sta_Reset           :1; // bit17 复位指令
		uint8_t sta_Enable          :1; // bit18 通道禁用/启用指令
		uint8_t reserved2  :1; // bit19 预留

		uint8_t reserved3  :1; // bit20 预留
		uint8_t reserved4  :1; // bit21 预留
		uint8_t reserved5  :1; // bit22 预留
		uint8_t reserved6  :1; // bit23 预留

		uint8_t reserved7  :1; // bit24 预留
		uint8_t reserved8  :1; // bit25 预留
		uint8_t reserved9  :1; // bit26 预留
		uint8_t reserved10 :1; // bit27 预留

		uint8_t reserved11 :1; // bit28 预留
		uint8_t reserved12 :1; // bit29 预留
		uint8_t reserved13 :1; // bit30 预留
		uint8_t reserved14 :1; // bit31 预留
	}CommStaBit;
	uint32_t CommSta;
}CmdCommStatus_U;

CmdCommStatus_U uLowerAckCommSta[LOWER_LAYER_TOTAL_NUM];
CmdCommStatus_U uLowerCommSta[LOWER_LAYER_TOTAL_NUM];


/* 断电续接数据结构体 */
typedef struct
{
	uint8_t chID;          //通道号
	uint8_t chStatus;      //通道状态
	uint8_t stepSerialNum; //工步序号
	uint8_t stepID;        //工步号
	uint8_t cycleNum;      //循环次数
	uint8_t stepName;      //工步名称
	uint8_t stepRuntime;   //运行时间
	uint8_t capacity;      //当前容量
	uint8_t energy;        //当前能量
	uint8_t chWarnStatus;  //通道异常状态
}LosePowerInfo_T;


/* 断电续接 */
typedef struct
{
	uint8_t flag;                                                    //判断标志位
	uint8_t chTotalNum;                                              //加载通道数量
	LosePowerInfo_T tInfo[LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM]; //断电续接数据
	ExistCh_T tCh;                                                   //存在通道记录
}LosePowerData_T;

LosePowerData_T tLosePowerData;



#pragma pack(pop)


//----------------------函数声明-----------------------------
uint8_t LowerPC_QueryLayerIsNULL(ExistLayer_U* pLayer, uint8_t num);
uint8_t LowerPC_DeleteLayer(ExistLayer_U* pLayer, uint8_t num);
uint8_t LowerPC_QueryChIsNULL(ExistCh_T* pCh, uint8_t num);

void LowerPc_Send_ReadBoardInfo(void);
void LowerPc_Send_GetBoardChData(void);
void LowerPc_Send_GetBoardChAdc(void);
void LowerPc_Send_GetChCalData(uint16_t ch);
void LowerPc_Send_SetChCalData(uint16_t ch, CailPointsType_U* pType, uint8_t pol);
void LowerPc_Send_ChangeCurCal(uint16_t ch);
void LowerPc_Send_DischangeCurCal(uint16_t ch);
void LowerPc_Send_ChangeVolCal(uint16_t ch);
void LowerPc_Send_DischangeVolCal(uint16_t ch);
void LowerPc_Send_ResetChCalData(uint8_t layerID, uint8_t chNum, uint8_t* chID);
uint8_t LowerPc_Send_GlobalSetting(const uint8_t* pBuf, uint16_t len);
uint8_t LowerPc_Send_StepLoading(const uint8_t* pBuf, uint16_t len);
void LowerPc_Send_LosePowerConnect(uint8_t layerID, const uint8_t* pBuf, uint16_t len);
uint8_t LowerPc_Send_StartStep(const uint8_t* pBuf, uint16_t len);
uint8_t LowerPc_Send_SuspendStep(const uint8_t* pBuf, uint16_t len);
uint8_t LowerPc_Send_StopStep(const uint8_t* pBuf, uint16_t len);
uint8_t LowerPc_Send_ContinueStep(const uint8_t* pBuf, uint16_t len);
uint8_t LowerPc_Send_JumpStep(const uint8_t* pBuf, uint16_t len);
void LowerPc_Send_Start_C_D(uint16_t ch, uint8_t mode, uint8_t adjust, float setV, float setC);
void LowerPc_Send_Stop_C_D(uint16_t ch);
uint8_t LowerPc_Send_ClearWarning(const uint8_t* pBuf, uint16_t len);
uint8_t LowerPc_Send_Reset(const uint8_t* pBuf, uint16_t len);
uint8_t LowerPc_Send_ChEnanble(const uint8_t* pBuf, uint16_t len);

void LowerPc_TxDataTranTask(void *parameter);
void LowerPc_TxTask(void *parameter);

void LowerPc_RxDataTranTask(void *parameter);
void LowerPc_RxTask(void *parameter);


void LowerPc_DataAnalyzeTask(void *parameter);


#ifdef __cplusplus
}
#endif

#endif /* INC_USER_USER_LOWER_COMPUTER_H_ */
