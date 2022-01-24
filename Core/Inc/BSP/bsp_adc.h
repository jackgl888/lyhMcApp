/*-----------------------------------------------------------------------------
 * @file        : bsp_adc.h
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2020/09/15
 * @date changed: 2020/09/17
 * @brief       : 板卡级ADC通道配置文件
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/

#ifndef INC_BSP_BSP_ADC_H_
#define INC_BSP_BSP_ADC_H_


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx.h"

/* USER CODE BEGIN Includes */


/* USER CODE END Includes */

#define ADC_DATA_BUFFER_SIZE 2U

#pragma  pack(push)  	//保存对齐状态
#pragma  pack(1)		//改为1字节对齐，防止内存对齐导致的错误

//-----------------用户数据结构------------------------

//ADC信息
typedef struct
{
	uint16_t adc; //采样值
	float adcVol; //转换电压值
	float adcKalmanVol; //卡尔曼滤波后电压值
}AdcData_T;

AdcData_T tADC;


#pragma pack(pop)


extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc3;
/* USER CODE BEGIN Private defines */
extern uint32_t ADCxConvertedData;
/* USER CODE END Private defines */

void MX_ADC3_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* INC_BSP_BSP_ADC_H_ */
