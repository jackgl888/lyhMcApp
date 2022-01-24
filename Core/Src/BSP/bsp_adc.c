/*-----------------------------------------------------------------------------
 * @file        : bsp_adc.c
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2020/09/15
 * @date changed: 2020/09/17
 * @brief       : 板卡级ADC通道配置文件
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#ifdef USER_CONFIG
	#include "user_config.h"
#endif

#if 0
ALIGN_32BYTES (uint32_t ADCxConvertedData) __attribute__((section(".ARM.__at_0x24000000")));
#else
ALIGN_32BYTES (uint32_t ADCxConvertedData) __attribute__((section(".adc_membase")));
#endif

ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc3;


/* ADC2 init function */
void MX_ADC3_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  ADCxConvertedData = 0;
  memset(&tADC, 0, sizeof(AdcData_T));

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
  hadc3.Init.Resolution = ADC_RESOLUTION_16B; //16位模式
  hadc3.Init.ScanConvMode = ADC_SCAN_ENABLE;  //扫描模式
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV; //关闭 EOC 中断
  hadc3.Init.LowPowerAutoWait = DISABLE; //自动低功耗关闭
  hadc3.Init.ContinuousConvMode = ENABLE; //连续转换
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE; //禁止不连续采样模式
  hadc3.Init.NbrOfDiscConversion = 0; //不连续采样通道数为 0
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START; //软件触发
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc3.Init.OversamplingMode = DISABLE; //过采样关闭
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;                        /* 配置使用的ADC通道 */
  sConfig.Rank = ADC_REGULAR_RANK_1;                      /* 采样序列里的第1个 */
  sConfig.SamplingTime = LL_ADC_SAMPLINGTIME_387CYCLES_5; /* 采样周期 */
  sConfig.SingleDiff = ADC_SINGLE_ENDED;                  /* 单端输入 */
  sConfig.OffsetNumber = ADC_OFFSET_NONE;                 /* 无偏移 */
  sConfig.Offset = 0;                                     /* 无偏移的情况下，此参数忽略 */
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
	  Error_Handler();
  }

  /* 开启回调 */
  if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
  {
	  Error_Handler();
  }

  /* 开启DMA传输 */
  if (HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&ADCxConvertedData, 1) != HAL_OK)
  {
	  Error_Handler();
  }

  /* 卡尔曼滤波初始化 */
  vKalmanInit(&tAdcVol, 0.0001, 0.6);

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC2_MspInit 0 */

  /* USER CODE END ADC2_MspInit 0 */
  /** Initializes the peripherals clock
  */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInitStruct.PLL3.PLL3M = 4;
	PeriphClkInitStruct.PLL3.PLL3N = 9;
	PeriphClkInitStruct.PLL3.PLL3P = 2;
	PeriphClkInitStruct.PLL3.PLL3Q = 2;
	PeriphClkInitStruct.PLL3.PLL3R = 2;
	PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
	PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
	PeriphClkInitStruct.PLL3.PLL3FRACN = 3072.0;
	PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
	  Error_Handler();
	}

    /* ADC3 clock enable */
    __HAL_RCC_ADC3_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**ADC3 GPIO Configuration
    PF10     ------> ADC3_INP6
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* ADC3 DMA Init */
    /* ADC3 Init */
	hdma_adc3.Instance = DMA1_Stream6;
	hdma_adc3.Init.Request = DMA_REQUEST_ADC3;
	hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_adc3.Init.Mode = DMA_CIRCULAR;
	hdma_adc3.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_adc3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_adc3) != HAL_OK)
	{
	  Error_Handler();
	}

	__HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc3);

  /* USER CODE BEGIN ADC2_MspInit 1 */

  /* USER CODE END ADC2_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC2_MspDeInit 0 */

  /* USER CODE END ADC2_MspDeInit 0 */
    /* Peripheral clock disable */
	__HAL_RCC_ADC3_CLK_DISABLE();

	/**ADC3 GPIO Configuration
	PF10     ------> ADC3_INP6
	*/
	HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10);

    /* ADC2 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC2_MspDeInit 1 */

  /* USER CODE END ADC2_MspDeInit 1 */
  }
}


/**
  * @brief  Conversion DMA half-transfer callback in non-blocking mode
  * @param  hadc: ADC handle
  * @retval None
  */
/* 结束后回调 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	tADC.adc = ADCxConvertedData;

	tADC.adcVol = (float)tADC.adc / 65536 * 3.3; //转换后电压值

	tADC.adcKalmanVol = fKalmanFilter(&tAdcVol, tADC.adcVol); //卡尔曼滤波

	/* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */
	/* DMA访问的ram，CPU访问的是cache，使用下面函数使ram和cache一致 */
	SCB_InvalidateDCache_by_Addr((uint32_t*)&ADCxConvertedData, 1);
}


/*----------------- (C) COPYRIGHT Lyric Robot-----END OF FILE-----------------*/
