/*
 * user_protocol.c
 *
 *  Created on: 2021年10月15日
 *      Author: Lyric
 */
#ifdef USER_CONFIG
	#include "user_config.h"
#endif


/*-----------------------------------------------------------------------------
 * @name  : Comm_DataInit
 * @brief : 通信：数据初始化
 * @param :
 * @retval:
 * @date  : 2021/10/29
 * @note  :
 * ---------------------------------------------------------------------------*/
void Comm_DataInit(void)
{
	memset(&uExistClient, 0, sizeof(ExistClient_U));
	memset(&tMoreClient, 0, sizeof(MoreClient_T));

	memset(&tLowerPC_Rx, 0, sizeof(LowerPC_RxData_T)); //接收数据缓存区
	memset(&tCommAckSta, NACK, sizeof(CommAckStatus_T)); //通信应答状态结构体
	memset(&tExistCh, 0, sizeof(ExistCh_T)); //存在的通道数记录
	memset(&uExistLayer, 0, sizeof(ExistLayer_U)); //存在的层数记录
	memset(&tSetChCalData, 0, sizeof(tSetChCalData[LOWER_LAYER_TOTAL_NUM][LOWER_CH_TOTAL_NUM])); //单个通道总校准参数结构体
	memset(&tGetChCalData, 0, sizeof(tSetChCalData[LOWER_LAYER_TOTAL_NUM][LOWER_CH_TOTAL_NUM])); //单个通道总校准参数结构体
	memset(&tGlobalPramSet, 0, sizeof(GlobalPramSet_T)); //全局设置参数结构体
	memset(&tChStepLoad, 0, sizeof(tChStepLoad[LOWER_CH_TOTAL_NUM])); //单个通道工艺结构体
	memset(&tStepLoad, 0, sizeof(StepLoad_T)); //子工步结构体
	memset(&tChRawData, 0, sizeof(tChRawData[LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM])); //整板通道ADC原始数据
	memset(&tChRealTimeDat, 0, sizeof(tChRealTimeDat[LOWER_LAYER_TOTAL_NUM*LOWER_CH_TOTAL_NUM])); //单通道实时数据结构体
	memset(&uChAbnormalSta, 0, sizeof(uChAbnormalSta[LOWER_CH_TOTAL_NUM*LOWER_LAYER_TOTAL_NUM])); //通道异常标志联合体
	memset(&uLayerAbnormalSta, 0, sizeof(uLayerAbnormalSta[LOWER_LAYER_TOTAL_NUM])); //层异常标志联合体
	memset(&uSysAbnormalSta, 0, sizeof(SysAbnormalStatus_U)); //系统异常标志联合体
	memset(&tLosePowerData, 0, sizeof(LosePowerData_T)); //断电续接
	memset(&tCommPro, 0, sizeof(CommProtect_T)); //通信保护
	memset(&tCommLayerCnt, 0, sizeof(CommLayerCnt_T)); //要通信的层数
	memset(&uLowerAckCommSta, COMM_OK, sizeof(uLowerAckCommSta[LOWER_LAYER_TOTAL_NUM])); //通信指令状态
	memset(&uLowerCommSta, COMM_OK, sizeof(uLowerCommSta[LOWER_LAYER_TOTAL_NUM])); //通信指令状态

	UpperPc_DeviceInfoInit(); //设备信息初始化
}

/*-----------------------------------------------------------------------------
 * @name  : CheckSum
 * @brief : 校验和分析函数，中上通信校验
 * @param : buff校验缓冲区首地址  len校验长度
 * @retval: uint16_t 校验和
 * @date  : 2021/10/26
 * @note  : 累加和
 * ---------------------------------------------------------------------------*/
uint16_t CheckSum(const uint8_t* buff, uint16_t len)
{
	uint16_t src = 0;

	while(len != 0)
	{
		src += (*buff);                     //16位数高8位
		buff++;                             //buff移位
		len--;                              //长度递减
	}

	return src;
}

/*-----------------------------------------------------------------------------
 * @name  : CheckSum16
 * @brief : 校验和分析函数，中下通信校验
 * @param : buff校验缓冲区首地址  len校验长度
 * @retval: uint16_t 校验和
 * @date  : 2021/10/26
 * @note  :
 *  *       ① 先将需要计算checksum数据中的checksum设为0；
 *          ② 计算checksum的数据按2byte划分开来，每2byte组成一个16bit的值，如果最后有单个byte的数据，补一个byte的0组成2byte；
 *          ③ 将所有的16bit值累加到一个32bit的值中；
 *          ④ 将32bit值的高16bit与低16bit相加到一个新的32bit值中，若新的32bit值大于0Xffff, 再将新值的高16bit与低16bit相加；
 *          ⑤ 将上一步计算所得的16bit值按位取反，即得到checksum值，存入数据的checksum字段即可。
 * ---------------------------------------------------------------------------*/
uint16_t CheckSum16(const uint8_t* buff, uint16_t len)
{
	uint32_t acc = 0;
	uint16_t src = 0;

	while(len > 1)
	{
		src = (*buff << 8);                     //16位数高8位
		buff++;                                 //buff移位
		src |= (*buff);                         //16位数低8位
		buff++;                                 //buff移位
		acc += src;                             //16位数累加
		len -= 2;                               //长度递减
	}

	if(len > 0)
	{
		src = (*buff) << 8;                     //16位数高8位，低8位补零
		acc += src;                             //16位数累加
	}

	acc = (acc >> 16) + (acc & 0x0000FFFF);     //32位数的高16位+低16位

	if((acc & 0xFFFF0000) != 0)                 //判断32位数是否大于0xFFFF
	{
		acc = (acc >> 16) + (acc & 0x0000FFFF); //32位数的高16位+低16位
	}

	src = (uint16_t)acc;                        //获取32位数低16位

	return src;
}
