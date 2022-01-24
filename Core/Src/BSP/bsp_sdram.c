/*-----------------------------------------------------------------------------
 * @file        : bsp_sdram.c
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2021/09/1
 * @date changed: 2021/09/1
 * @brief       : 外扩SDRAM应用函数接口
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#include "bsp_sdram.h"

SDRAM_HandleTypeDef hsdram1;
#define sdramHandle hsdram1


/*-----------------------------------------------------------------------------
 * @name  : SDRAM_delay
 * @brief : 延迟一段时间
 * @param : 延迟的时间长度
 * @retval:
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
static void SDRAM_delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}

/*-----------------------------------------------------------------------------
 * @name  : SDRAM_GPIO_Config
 * @brief : 初始化控制SDRAM的IO
 * @param :
 * @retval:
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
static void SDRAM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	/* 使能SDRAM相关的GPIO时钟 */

	/*地址信号线*/
	FMC_A0_GPIO_CLK(); FMC_A1_GPIO_CLK(); FMC_A2_GPIO_CLK();
	FMC_A3_GPIO_CLK(); FMC_A4_GPIO_CLK(); FMC_A5_GPIO_CLK();
	FMC_A6_GPIO_CLK(); FMC_A7_GPIO_CLK(); FMC_A8_GPIO_CLK();
	FMC_A9_GPIO_CLK(); FMC_A10_GPIO_CLK();FMC_A11_GPIO_CLK();
	FMC_A12_GPIO_CLK();
	/*数据信号线*/
	FMC_D0_GPIO_CLK(); FMC_D1_GPIO_CLK() ; FMC_D2_GPIO_CLK();
	FMC_D3_GPIO_CLK(); FMC_D4_GPIO_CLK() ; FMC_D5_GPIO_CLK();
	FMC_D6_GPIO_CLK(); FMC_D7_GPIO_CLK() ; FMC_D8_GPIO_CLK();
	FMC_D9_GPIO_CLK(); FMC_D10_GPIO_CLK(); FMC_D11_GPIO_CLK();
	FMC_D12_GPIO_CLK();FMC_D13_GPIO_CLK(); FMC_D14_GPIO_CLK();
	FMC_D15_GPIO_CLK();
	/*控制信号线*/
	FMC_CS_GPIO_CLK() ; FMC_BA0_GPIO_CLK(); FMC_BA1_GPIO_CLK();
	FMC_WE_GPIO_CLK() ; FMC_RAS_GPIO_CLK(); FMC_CAS_GPIO_CLK();
	FMC_CLK_GPIO_CLK(); FMC_CKE_GPIO_CLK(); FMC_UDQM_GPIO_CLK();
	FMC_LDQM_GPIO_CLK();


	/*-- GPIO 配置 -----------------------------------------------------*/

	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;//配置为复用功能
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF12_FMC;

	/*地址信号线 针对引脚配置*/
	GPIO_InitStructure.Pin = FMC_A0_GPIO_PIN;
	HAL_GPIO_Init(FMC_A0_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A1_GPIO_PIN;
	HAL_GPIO_Init(FMC_A1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A2_GPIO_PIN;
	HAL_GPIO_Init(FMC_A2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A3_GPIO_PIN;
	HAL_GPIO_Init(FMC_A3_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A4_GPIO_PIN;
	HAL_GPIO_Init(FMC_A4_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A5_GPIO_PIN;
	HAL_GPIO_Init(FMC_A5_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A6_GPIO_PIN;
	HAL_GPIO_Init(FMC_A6_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A7_GPIO_PIN;
	HAL_GPIO_Init(FMC_A7_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A8_GPIO_PIN;
	HAL_GPIO_Init(FMC_A8_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A9_GPIO_PIN;
	HAL_GPIO_Init(FMC_A9_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A10_GPIO_PIN;
	HAL_GPIO_Init(FMC_A10_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A11_GPIO_PIN;
	HAL_GPIO_Init(FMC_A11_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_A12_GPIO_PIN;
	HAL_GPIO_Init(FMC_A12_GPIO_PORT, &GPIO_InitStructure);

	/*数据信号线 针对引脚配置*/
	GPIO_InitStructure.Pin = FMC_D0_GPIO_PIN;
	HAL_GPIO_Init(FMC_D0_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D1_GPIO_PIN;
	HAL_GPIO_Init(FMC_D1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D2_GPIO_PIN;
	HAL_GPIO_Init(FMC_D2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D3_GPIO_PIN;
	HAL_GPIO_Init(FMC_D3_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D4_GPIO_PIN;
	HAL_GPIO_Init(FMC_D4_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D5_GPIO_PIN;
	HAL_GPIO_Init(FMC_D5_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D6_GPIO_PIN;
	HAL_GPIO_Init(FMC_D6_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D7_GPIO_PIN;
	HAL_GPIO_Init(FMC_D7_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D8_GPIO_PIN;
	HAL_GPIO_Init(FMC_D8_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D9_GPIO_PIN;
	HAL_GPIO_Init(FMC_D9_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D10_GPIO_PIN;
	HAL_GPIO_Init(FMC_D10_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D11_GPIO_PIN;
	HAL_GPIO_Init(FMC_D11_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D12_GPIO_PIN;
	HAL_GPIO_Init(FMC_D12_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D13_GPIO_PIN;
	HAL_GPIO_Init(FMC_D13_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D14_GPIO_PIN;
	HAL_GPIO_Init(FMC_D14_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_D15_GPIO_PIN;
	HAL_GPIO_Init(FMC_D15_GPIO_PORT, &GPIO_InitStructure);

	/*控制信号线*/
	GPIO_InitStructure.Pin = FMC_CS_GPIO_PIN;
	HAL_GPIO_Init(FMC_CS_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_BA0_GPIO_PIN;
	HAL_GPIO_Init(FMC_BA0_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_BA1_GPIO_PIN;
	HAL_GPIO_Init(FMC_BA1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_WE_GPIO_PIN;
	HAL_GPIO_Init(FMC_WE_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_RAS_GPIO_PIN;
	HAL_GPIO_Init(FMC_RAS_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_CAS_GPIO_PIN;
	HAL_GPIO_Init(FMC_CAS_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_CLK_GPIO_PIN;
	HAL_GPIO_Init(FMC_CLK_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_CKE_GPIO_PIN;
	HAL_GPIO_Init(FMC_CKE_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_UDQM_GPIO_PIN;
	HAL_GPIO_Init(FMC_UDQM_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = FMC_LDQM_GPIO_PIN;
	HAL_GPIO_Init(FMC_LDQM_GPIO_PORT, &GPIO_InitStructure);

}

/*-----------------------------------------------------------------------------
 * @name  : SDRAM_InitSequence
 * @brief : 对SDRAM芯片进行初始化配置
 * @param :
 * @retval:
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
static void SDRAM_InitSequence(void)
{
	static FMC_SDRAM_CommandTypeDef Command;
	uint32_t tmpr = 0;

	/* Step 1 ----------------------------------------------------------------*/
	/* 配置命令：开启提供给SDRAM的时钟 */
	Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 2: 延时100us */
	SDRAM_delay(1); //因为只需要初始化的时候延时，无伤大雅，因此不将其改成阻塞延迟

	/* Step 3 ----------------------------------------------------------------*/
	/* 配置命令：对所有的bank预充电 */
	Command.CommandMode = FMC_SDRAM_CMD_PALL;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 4 ----------------------------------------------------------------*/
	/* 配置命令：自动刷新 */
	Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 4;
	Command.ModeRegisterDefinition = 0;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 5 ----------------------------------------------------------------*/
	/* 设置sdram寄存器配置 */
	tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
				     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
				     SDRAM_MODEREG_CAS_LATENCY_3           |
				     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
				     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	/* 配置命令：设置SDRAM寄存器 */
	Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = tmpr;
	/* 发送配置命令 */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 6 ----------------------------------------------------------------*/
	/* 设置刷新计数器 */
	/* 刷新周期=64ms/4096行=15.625us */
	/* COUNT=(15.625us x Freq) - 20 */
	/* 设置自刷新速率 */
	HAL_SDRAM_ProgramRefreshRate(&sdramHandle, 1855);

}

/*-----------------------------------------------------------------------------
 * @name  : SDRAM_Init
 * @brief : 初始化配置使用SDRAM的FMC及GPIO接口，
 * @param :
 * @retval:
 * @date  : 2021/09/01
 * @note  : 本函数在SDRAM读写操作前需要被调用
 * ---------------------------------------------------------------------------*/
void SDRAM_Init(void)
{
	FMC_SDRAM_TimingTypeDef SdramTiming;
	RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

	/* 配置FMC接口相关的 GPIO*/
	SDRAM_GPIO_Config();

	/* 配置SDRAM时钟源*/
	RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FMC;
	RCC_PeriphClkInit.PLL2.PLL2M = 5;
	RCC_PeriphClkInit.PLL2.PLL2N = 144;
	RCC_PeriphClkInit.PLL2.PLL2P = 2;
	RCC_PeriphClkInit.PLL2.PLL2Q = 2;
	RCC_PeriphClkInit.PLL2.PLL2R = 3;
	RCC_PeriphClkInit.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
	RCC_PeriphClkInit.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
	RCC_PeriphClkInit.PLL2.PLL2FRACN = 0;
	RCC_PeriphClkInit.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
	if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
	{
		while(1);
	}
	/* 使能 FMC 时钟 */
	__FMC_CLK_ENABLE();

	/*执行SDRAM1的内存初始化序列 */
	hsdram1.Instance = FMC_SDRAM_DEVICE;
	/* hsdram1结构体初始化*/
	hsdram1.Init.SDBank = FMC_BANK_SDRAM;							   // SDRAM 的bank选择
	hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;       // SDRAM 列数
	hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;            // SDRAM 行数
	hsdram1.Init.MemoryDataWidth = SDRAM_MEMORY_WIDTH;                 // 总线数据宽度为 16 位
	hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;    // 4 个扇区
	hsdram1.Init.CASLatency = SDRAM_CAS_LATENCY;                       // 列地址选通信延时
	hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE; // 禁止写保护
	hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;             // SDRAM 时钟
	hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;                  // 使能突发传输模式
	hsdram1.Init.ReadPipeDelay = SDRAM_RPIPE_DELAY;                    // 读通道延时
	/* SDRAM时序 */
	SdramTiming.LoadToActiveDelay = 2;     // 加载模式寄存器命令与行有效或刷新命令之间的延迟
	SdramTiming.ExitSelfRefreshDelay = 8;  // 退出自我刷新到行有效命令之间的延迟
	SdramTiming.SelfRefreshTime = 5;       // 行有效与预充电命令之间的延迟
	SdramTiming.RowCycleDelay = 8;         // 两个刷新命令或两个行有效命令之间的延迟
	SdramTiming.WriteRecoveryTime = 2;     // 写入命令到预充电命令之间的延迟
	SdramTiming.RPDelay = 2;               // 预充电与行有效命令之间的延迟
	SdramTiming.RCDDelay = 2;              // 行有效与列读写命令之间的延迟

	if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
	{
		Error_Handler( );
	}

	/* FMC SDRAM 设备时序初始化 */
	SDRAM_InitSequence();
}

/*-----------------------------------------------------------------------------
 * @name  : SDRAM_WriteBuffer
 * @brief : 以“字”为单位向sdram写入数据
 * @param : pBuffer: 指向数据的指针
 *          uwWriteAddress: 要写入的SDRAM内部地址
 *          uwBufferSize: 要写入数据大小
 * @retval:
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
void SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwWriteAddress;

  /* 禁止写保护 */
  HAL_SDRAM_WriteProtection_Disable(&hsdram1);
  /* 检查SDRAM标志，等待至SDRAM空闲 */
  while(HAL_SDRAM_GetState(&hsdram1) != HAL_SDRAM_STATE_RESET)
  {
  }

  /* 循环写入数据 */
  for (; uwBufferSize != 0; uwBufferSize--) 
  {
    /* 发送数据到SDRAM */
    *(uint32_t *) (SDRAM_BANK_ADDR + write_pointer) = *pBuffer++;

    /* 地址自增*/
    write_pointer += 4;
  }
}

/*-----------------------------------------------------------------------------
 * @name  : SDRAM_ReadBuffer
 * @brief : 从SDRAM中读取数据
 * @param : pBuffer: 指向存储数据的buffer
 *          ReadAddress: 要读取数据的地址
 *          uwBufferSize: 要读取的数据大小
 * @retval:
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
void SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwReadAddress;
  
   
  /* 检查SDRAM标志，等待至SDRAM空闲 */
  while ( HAL_SDRAM_GetState(&hsdram1) != HAL_SDRAM_STATE_RESET)
  {
  }
  
  /*读取数据 */
  for(; uwBufferSize != 0x00; uwBufferSize--)
  {
   *pBuffer++ = *(__IO uint32_t *)(SDRAM_BANK_ADDR + write_pointer );
    
   /* 地址自增*/
    write_pointer += 4;
  } 
}

/*-----------------------------------------------------------------------------
 * @name  : SDRAM_Test
 * @brief : 测试函数，测试SDRAM是否正常
 * @param :
 * @retval:
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t SDRAM_Test(void)
{
  /*写入数据计数器*/
  uint32_t counter=0;
  
  /* 8位的数据 */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16位的数据 */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  SDRAM_INFO("正在检测SDRAM，以8位、16位的方式读写sdram...");

  /*按8位格式读写数据，并校验*/
  
  /* 把SDRAM数据全部重置为0 ，IS42S16400J_SIZE是以8位为单位的 */
  for (counter = 0x00; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* 向整个SDRAM写入数据  8位 */
  for (counter = 0; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<IS42S16400J_SIZE; counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(SDRAM_BANK_ADDR + counter);  //从该地址读出数据
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SDRAM_ERROR("8位数据读写错误！");
      return 0;
    }
  }
  
  /*按16位格式读写数据，并检测*/
  
  /* 把SDRAM数据全部重置为0 */
  for (counter = 0x00; counter < IS42S16400J_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* 向整个SDRAM写入数据  16位 */
  for (counter = 0; counter < IS42S16400J_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<IS42S16400J_SIZE/2; counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(SDRAM_BANK_ADDR + 2*counter);  //从该地址读出数据
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SDRAM_ERROR("16位数据读写错误！");
      return 0;
    }
  }

  SDRAM_INFO("SDRAM读写测试正常！");
  /*检测正常，return 1 */
  return 1;
  
}


/*********************************************END OF FILE**********************/
