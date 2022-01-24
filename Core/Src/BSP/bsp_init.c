/*-----------------------------------------------------------------------------
 * @file        : board_init.c
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2019/10/10
 * @date changed: 2021/10/22
 * @brief       : 板级硬件初始化文件
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#ifdef USER_CONFIG
	#include "user_config.h"
#endif

/*--------------------------------OS_IPC--------------------------------------*/
static TaskHandle_t Test_Handle               = NULL; //任务句柄

static TaskHandle_t LowerPc_RxDataTran_Handle = NULL; //任务句柄
static TaskHandle_t LowerPc_RxTask_Handle     = NULL; //任务句柄

static TaskHandle_t LowerPc_TxDataTran_Handle = NULL; //任务句柄
static TaskHandle_t LowerPc_TxTask_Handle     = NULL; //任务句柄

static TaskHandle_t LowerPc_DataAnalyzeTask_Handle = NULL; //任务句柄

static TaskHandle_t UpperPc_RxTask_Handle     = NULL; //任务句柄
static TaskHandle_t UpperPc_TxTask_Handle     = NULL; //任务句柄

static TaskHandle_t TCP_CtrlTask_Handle       = NULL; //任务句柄


#if __USER_LOSE_POWER
static TaskHandle_t LosePower_Handle          = NULL; //任务句柄
#endif

#if __TEST_NAND_FLASH
static TaskHandle_t NAND_Test_Handle          = NULL; //任务句柄
#endif


//LWIP
TaskHandle_t PHY_Link_Check_Handle            = NULL; //查询以太网连接状态任务句柄
TaskHandle_t TCP_TASK_Handle                  = NULL; //TCP任务句柄

#if __SYS_IWDG
static TaskHandle_t Watchdog_Task_Handle      = NULL; //看门狗
#endif


//串口发送DMA处理
SemaphoreHandle_t uart1DMATxCpltSem = NULL;	//串口1 DMA 发送完成信号
SemaphoreHandle_t uart2DMATxCpltSem = NULL;	//串口2 DMA 发送完成信号
SemaphoreHandle_t uart3DMATxCpltSem = NULL;	//串口3 DMA 发送完成信号
SemaphoreHandle_t uart4DMATxCpltSem = NULL;	//串口4 DMA 发送完成信号
SemaphoreHandle_t uart5DMATxCpltSem = NULL;	//串口5 DMA 发送完成信号
SemaphoreHandle_t uart6DMATxCpltSem = NULL;	//串口6 DMA 发送完成信号
SemaphoreHandle_t uart7DMATxCpltSem = NULL;	//串口7 DMA 发送完成信号
SemaphoreHandle_t uart8DMATxCpltSem = NULL;	//串口8 DMA 发送完成信号
SemaphoreHandle_t uart1DMATxMutex = NULL;   //串口1 通信保护（多个线程调用锁）
SemaphoreHandle_t uart2DMATxMutex = NULL;   //串口2 通信保护（多个线程调用锁）
SemaphoreHandle_t uart3DMATxMutex = NULL;   //串口3 通信保护（多个线程调用锁）
SemaphoreHandle_t uart4DMATxMutex = NULL;   //串口4 通信保护（多个线程调用锁）
SemaphoreHandle_t uart5DMATxMutex = NULL;   //串口5 通信保护（多个线程调用锁）
SemaphoreHandle_t uart6DMATxMutex = NULL;   //串口6 通信保护（多个线程调用锁）
SemaphoreHandle_t uart7DMATxMutex = NULL;   //串口7 通信保护（多个线程调用锁）
SemaphoreHandle_t uart8DMATxMutex = NULL;   //串口8 通信保护（多个线程调用锁）

//线程保护
SemaphoreHandle_t  ReadFlashMutex     = NULL; //读flash保护互斥量
SemaphoreHandle_t  WriteFlashMutex    = NULL; //写flash保护互斥量

//AC-DC双向电源模块
SemaphoreHandle_t  PowerSupplyMutex   = NULL; //双向电源通信保护互斥量
SemaphoreHandle_t  PowerSupplySem     = NULL; //双向电源错误重发管理

//下位机通信
SemaphoreHandle_t LowerPcSyncSem      = NULL; //创建二值信号量，用于通信同步
SemaphoreHandle_t LowerPcRxSem        = NULL; //创建二值信号量，用于接收数据
SemaphoreHandle_t LowerPcSem          = NULL; //创建二值信号量，用于错误重发
SemaphoreHandle_t LowerPcMutex        = NULL; //保护互斥量，用于通信数据处理保护
SemaphoreHandle_t LowerPcCaliMutex    = NULL; //保护互斥量，用于校准数据处理保护

//上位机位机通信
SemaphoreHandle_t UpperPcSem          = NULL; //创建二值信号量，用于下位机应答同步
SemaphoreHandle_t UpperPcMutex        = NULL; //保护互斥量，用于通信数据处理保护

//事件
EventGroupHandle_t WatchDogEvent     = NULL; //用于独立开门狗的喂狗事件
EventGroupHandle_t TimingCommEvent   = NULL; //用于下位机定时通信处理事件

/*-----------------------------------------------------------------------------
 * @name  : BSP_Init
 * @brief : 板卡级硬件初始化接口
 * @param : None
 * @retval: None
 * @date  : 2021/10/27
 * @note  :
 * ---------------------------------------------------------------------------*/
void BSP_Init(void)
{
#if 1
	//防止硬件上电跟不上软件初始化
	uint32_t hardwardInitTime = 0;
	for(uint32_t i = 0; i < 20000; i++) //20000
		hardwardInitTime++;
	hardwardInitTime = 0;
#endif

	/* MPU Configuration -*/
	MPU_Config();

	/* Enable I-Cache--Enable D-Cache */
	Cache_Enable();

	/* MCU Configuration */
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

	/* 打开FPU（硬件浮点运算） */
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
#endif

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
#if 0
	/* 复位标志判断 */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET) //上电复位
    {
    	__HAL_RCC_CLEAR_RESET_FLAGS(); //清除标志
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET) //按键或者STM32CubeIDE Debug复位
    {
    	__HAL_RCC_CLEAR_RESET_FLAGS(); //清除标志
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) != RESET) //软件复位
    {
    	__HAL_RCC_CLEAR_RESET_FLAGS(); //清除标志
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST) != RESET) //看门狗复位
    {
    	__HAL_RCC_CLEAR_RESET_FLAGS();  //清除标志 //如果一直不喂狗，会一直复位
    }
#endif

	/* CRC校验初始化 */
//    MX_CRC_Init();

	/* RTC初始化 */
    MX_RTC_Init();

	MX_GPIO_Init(); //野火开发板LED、KEY

	/* 485初始化 */
	RS485_Init();

	/* DMA初始化 */
	MX_DMA_Init();

	/* 串口初始化 */
	MX_USART_UART_Init();

#if __BSP_ADC
	/* ADC初始化 */
	MX_ADC3_Init(); //要在SDRAM初始化之后初始化ADC_DMA
#endif

	/* 定时器初始化
	   定时器时钟为240M，分频系数为24000-1
	   频率为240M/24000=10K，自动重装载为1000-1，那么定时器周期就是1/(10)=100ms */
	MX_TIM3_Init(1000-1, 24000-1 );

	/* CAN1 初始化 500kbps:12, 8, 31, 8 */
	FDCAN1_Mode_Init(12, 8, 31, 8, FDCAN_MODE_NORMAL); //2021/09/27 未测试

	/* CAN2 初始化 1000kbps:6, 8, 31, 8 */
//	FDCAN2_Mode_Init(6, 8, 31, 8, FDCAN_MODE_NORMAL); //2021/09/27 未测试

	/* SDRAM初始化 */
	SDRAM_Init();
	my_mem_init(SDRAM_POOL);

	/* NAND FLASH 初始化 */
    MX_FATFS_Init();         //FATFS init
    uRecordDataMemoryInit(); //创建SDRAM存储队列

	/* 环形队列创建及初始化 */
	if (uQueueInit(&tLowerPcRxQueue, LOWER_PC_BUF_MAX_LEN, LOWER_PC_BUF_MAX_CNT) == QUEUE_OK) //下位机接收环形队列
		printf("FIFO1 was init successfully! \r\n");
	if (uQueueInit(&tLowerPcTxQueue, LOWER_PC_BUF_MAX_LEN, LOWER_PC_BUF_MAX_CNT) == QUEUE_OK) //下位机发送环形队列
		printf("FIFO2 was init successfully! \r\n");
	if (uQueueInit(&tUpperPcRxQueue, UPPER_PC_BUF_MAX_LEN, LOWER_PC_BUF_MAX_CNT) == QUEUE_OK) //上位机接收环形队列
		printf("FIFO3 was init successfully! \r\n");
	if (uQueueInit(&tUpperPcTxQueue, UPPER_PC_BUF_MAX_LEN, UPPER_PC_BUF_MAX_CNT) == QUEUE_OK) //上位机发送环形队列
		printf("FIFO4 was init successfully! \r\n");
	if (uQueueInit(&tRecordDataQueue, UPPER_PC_BUF_MAX_LEN, UPPER_PC_BUF_MAX_CNT) == QUEUE_OK)   //通道记录数据环形队列
		printf("FIFO5 was init successfully! \r\n");

	/* 通信数据初始化 */
	Comm_DataInit();

    setvbuf(stdout, NULL, _IONBF, 0);  //将缓冲区禁止，直接输出printf

	/* 进入任务创建和调度，开始打印调试 */
    DEBUG_PRINTF("----------- LYH_DEBUG ------------\r\n");
}

/*-----------------------------------------------------------------------------
 * @name  : AppTaskCreate
 * @brief : 为了方便管理，所有的任务创建函数都放在这个函数里面
 * @param :
 * @retval:
 * @date  : 2021/8/26
 * @note  :
 * ---------------------------------------------------------------------------*/
extern TaskHandle_t AppTaskCreate_Handle; //创建任务句柄
void AppTaskCreate(void *parameter)
{
	//定义一个创建信息返回值，默认为pdPASS
	BaseType_t xReturn = pdPASS;

	//看门狗事件组
#if __SYS_IWDG
	WatchDogEvent = xEventGroupCreate();
#endif

	TimingCommEvent = xEventGroupCreate();

	//串口 DMA 发送控制
	//同步信号量
	uart1DMATxCpltSem = xSemaphoreCreateBinary();
	uart2DMATxCpltSem = xSemaphoreCreateBinary();
	uart3DMATxCpltSem = xSemaphoreCreateBinary();
	uart4DMATxCpltSem = xSemaphoreCreateBinary();
	uart5DMATxCpltSem = xSemaphoreCreateBinary();
	uart6DMATxCpltSem = xSemaphoreCreateBinary();
	uart7DMATxCpltSem = xSemaphoreCreateBinary();
	uart8DMATxCpltSem = xSemaphoreCreateBinary();
    //保护互斥量
	uart1DMATxMutex = xSemaphoreCreateMutex();
	uart2DMATxMutex = xSemaphoreCreateMutex();
	uart3DMATxMutex = xSemaphoreCreateMutex();
	uart4DMATxMutex = xSemaphoreCreateMutex();
	uart5DMATxMutex = xSemaphoreCreateMutex();
	uart6DMATxMutex = xSemaphoreCreateMutex();
	uart7DMATxMutex = xSemaphoreCreateMutex();
	uart8DMATxMutex = xSemaphoreCreateMutex();

	//任务线程保护互斥量
	ReadFlashMutex = xSemaphoreCreateMutex();  //读内部flash保护
	WriteFlashMutex = xSemaphoreCreateMutex(); //写内部flash保护

	//AC-DC双向电源模块
	PowerSupplySem = xSemaphoreCreateBinary();  //信号量
	PowerSupplyMutex = xSemaphoreCreateMutex(); //互斥量

	//下位机通信
	LowerPcSyncSem = xSemaphoreCreateBinary();  //信号量
	LowerPcRxSem = xSemaphoreCreateBinary();    //信号量
	LowerPcSem = xSemaphoreCreateBinary();      //信号量
	LowerPcMutex = xSemaphoreCreateMutex();     //互斥量
	LowerPcCaliMutex = xSemaphoreCreateMutex(); //互斥量

	//上位机位机通信
	UpperPcSem = xSemaphoreCreateBinary();  //信号量
	UpperPcMutex = xSemaphoreCreateMutex(); //互斥量


	taskENTER_CRITICAL(); //进入临界区

//-------------------------------------------------------------------------------------
#if __SYS_IWDG
    /* 系统看门狗喂狗任务 */
    xReturn = xTaskCreate((TaskFunction_t) Watchdog_Task,
    		              (const char*   ) "Watchdog_Task",
			              (uint16_t      ) 256,
			              (void*         ) NULL,
			              (UBaseType_t   ) 9,
			              (TaskHandle_t* ) &Watchdog_Task_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create Watchdog_Task sucess...\r\n");
#endif
//-------------------------------------------------------------------------------------
#if __LWIP_ENABLE
    /* LWIP任务，网线插入后才初始化LWIP */
    xReturn = xTaskCreate((TaskFunction_t) PHY_Link_Check_TASK,
    		              (const char*   ) "PHY_Link_Check_TASK",
			              (uint16_t      ) 256,
			              (void*         ) NULL,
			              (UBaseType_t   ) 5,
			              (TaskHandle_t* ) &PHY_Link_Check_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create PHY_Link_Check_TASK sucess...\r\n");
#endif
//-------------------------------------------------------------------------------------
    /* 与下位机通信接收数据任务 */
    xReturn = xTaskCreate((TaskFunction_t) LowerPc_RxDataTranTask,
    		              (const char*   ) "LowerPc_RxDataTranTask",
			              (uint16_t      ) 256,
			              (void*         ) NULL,
			              (UBaseType_t   ) 10,
			              (TaskHandle_t* ) &LowerPc_RxDataTran_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create LowerPc_RxDataTranTask sucess...\r\n");
    /* 与下位机通信接收数据处理任务 */
    xReturn = xTaskCreate((TaskFunction_t) LowerPc_RxTask,
    		              (const char*   ) "LowerPc_RxTask",
			              (uint16_t      ) 512,
			              (void*         ) NULL,
			              (UBaseType_t   ) 10,
			              (TaskHandle_t* ) &LowerPc_RxTask_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create LowerPc_RxTask sucess...\r\n");
//-------------------------------------------------------------------------------------
    /* 与下位机通信发送数据任务 */
    xReturn = xTaskCreate((TaskFunction_t) LowerPc_TxDataTranTask,
    		              (const char*   ) "LowerPc_TxDataTranTask",
			              (uint16_t      ) 256,
			              (void*         ) NULL,
			              (UBaseType_t   ) 8,
			              (TaskHandle_t* ) &LowerPc_TxDataTran_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create LowerPc_TxDataTranTask sucess...\r\n");
    /* 与下位机通信发送数据处理任务 */
    xReturn = xTaskCreate((TaskFunction_t) LowerPc_TxTask,
    		              (const char*   ) "LowerPc_TxTask",
			              (uint16_t      ) 512,
			              (void*         ) NULL,
			              (UBaseType_t   ) 10,
			              (TaskHandle_t* ) &LowerPc_TxTask_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create LowerPc_TxTask sucess...\r\n");
//-------------------------------------------------------------------------------------
    /* 与上位机通信接收数据处理任务 */
    xReturn = xTaskCreate((TaskFunction_t) UpperPc_RxTask,
    		              (const char*   ) "UpperPc_RxTask",
			              (uint16_t      ) 512,
			              (void*         ) NULL,
			              (UBaseType_t   ) 11,
			              (TaskHandle_t* ) &UpperPc_RxTask_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create UpperPc_RxTask sucess...\r\n");
    /* 与上位机通信发送数据处理任务 */
    xReturn = xTaskCreate((TaskFunction_t) UpperPc_TxTask,
    		              (const char*   ) "UpperPc_TxTask",
			              (uint16_t      ) 512,
			              (void*         ) NULL,
			              (UBaseType_t   ) 12,
			              (TaskHandle_t* ) &UpperPc_TxTask_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create UpperPc_TxTask sucess...\r\n");
    /* 以太网连接处理任务 */
    xReturn = xTaskCreate((TaskFunction_t) TCP_CtrlTask,
    		              (const char*   ) "UpperPc_TxTask",
			              (uint16_t      ) 256,
			              (void*         ) NULL,
			              (UBaseType_t   ) 10,
			              (TaskHandle_t* ) &TCP_CtrlTask_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create UpperPc_TxTask sucess...\r\n");
//-------------------------------------------------------------------------------------
    /* 与下位机通信数据处理任务 */
	xReturn = xTaskCreate((TaskFunction_t) LowerPc_DataAnalyzeTask,
						  (const char*   ) "LowerPc_DataAnalyzeTask",
						  (uint16_t      ) 256,
						  (void*         ) NULL,
						  (UBaseType_t   ) 8,
						  (TaskHandle_t* ) &LowerPc_DataAnalyzeTask_Handle);
	if (pdPASS == xReturn)
		DEBUG_PRINTF("Create LowerPc_DataAnalyzeTask sucess...\r\n");
//-------------------------------------------------------------------------------------
#if __USER_LOSE_POWER
    /* 断电续接处理任务 */
    xReturn = xTaskCreate((TaskFunction_t) LosePower_CtrlTask,
    		              (const char*   ) "LosePower_CtrlTask",
			              (uint16_t      ) 256,
			              (void*         ) NULL,
			              (UBaseType_t   ) 8,
			              (TaskHandle_t* ) &LosePower_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create LosePower_CtrlTask sucess...\r\n");
#endif
//-------------------------------------------------------------------------------------
    /* 测试任务 */
    xReturn = xTaskCreate((TaskFunction_t) Test_Task,
    		              (const char*   ) "Test_Task",
			              (uint16_t      ) 256,
			              (void*         ) NULL,
			              (UBaseType_t   ) 6,
			              (TaskHandle_t* ) &Test_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create Test_Task sucess...\r\n");
//-------------------------------------------------------------------------------------
#if __TEST_NAND_FLASH
    xReturn = xTaskCreate((TaskFunction_t) NAND_Test_Task,
    		              (const char*   ) "NAND_Test_Task",
			              (uint16_t      ) 512,
			              (void*         ) NULL,
			              (UBaseType_t   ) 8,
			              (TaskHandle_t* ) &NAND_Test_Handle);
    if (pdPASS == xReturn)
    	DEBUG_PRINTF("Create NAND_Test_Task sucess...\r\n");
#endif
//-------------------------------------------------------------------------------------

    vTaskDelete(AppTaskCreate_Handle); 	//删除AppTaskCreate任务

    taskEXIT_CRITICAL();	//退出临界区
}

/*-----------------------------------------------------------------------------
 * @name  : Test_Task
 * @brief : 测试任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/9/29
 * @note  :
 * ---------------------------------------------------------------------------*/
void Test_Task(void *parameter)
{

	while(1)
	{
		ThreadTime(11);

#if 0
		/* 按键触发修改IP地址 */
    	/* 检测是否有按键按下 */
    	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET )
    	{
    		/* 等待按键释放 */
    		while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) vTaskDelay(10);
    	    tNetConfigUpd.user_ip[0] = 192;
    	    tNetConfigUpd.user_ip[1] = 168;
    	    tNetConfigUpd.user_ip[2] = 250;
    	    tNetConfigUpd.user_ip[3] = 221;
    	    tNetConfigUpd.user_mask[0] = 255;
    	    tNetConfigUpd.user_mask[1] = 255;
    	    tNetConfigUpd.user_mask[2] = 255;
    	    tNetConfigUpd.user_mask[3] = 0;
    	    tNetConfigUpd.user_gw[0] = tNetConfigUpd.user_ip[0];
    	    tNetConfigUpd.user_gw[1] = tNetConfigUpd.user_ip[1];
    	    tNetConfigUpd.user_gw[2] = tNetConfigUpd.user_ip[2];
    	    tNetConfigUpd.user_gw[3] = 1;

    	    if(NetConfigFlag == 1)
    	    {
        		NetConfigUpdate(&tNetConfigUpd);
    	    }
    	    else
    	    {
    	    	memcpy(&tNetConfigSet, &tNetConfigUpd, sizeof(NETCONFIG_T));
    	    }
    	}
#endif

    	RTC_Read_TimeDate(&RTC_TimeStruct, &RTC_DateStruct); //获取 RTC 时间

    	HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_10); //LED闪烁（野火开发板红灯亮）

		vTaskDelay(200);
	}
}

/*-----------------------------------------------------------------------------
 * @name  : NAND_Test_Task
 * @brief : NAND测试任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/9/29
 * @note  : 简单测试用（野火开发板）：
            按键 KEY 1 是写入，KEY 2 是读取。
 * ---------------------------------------------------------------------------*/
#define _NAND_INIT 0
#define _NAND_TEST 1
void NAND_Test_Task(void *parameter)
{
#if _NAND_TEST
	uint32_t wl = 0;

	ChannelRealTimeData_T tPTest;
	memset(&tPTest, 0, sizeof(ChannelRealTimeData_T));

    /* 测试buf */
    uint8_t *writebuf = (uint8_t *)mymalloc(SDRAM_POOL, sizeof(ChannelRealTimeData_T)*128*48);
	uint8_t *tempP = writebuf;
    memset(writebuf, 0, sizeof(ChannelRealTimeData_T)*128*48);

	for(uint16_t i=0; i<128; i++)
	{
		for(uint16_t j=0; j<48; j++)
		{
			tPTest.chID = j+1;
			tPTest.chStatus = CH_STA_SHELVE; //搁置

			memcpy(writebuf, &tPTest, sizeof(ChannelRealTimeData_T));
			writebuf += sizeof(ChannelRealTimeData_T);
		}
	}
#endif

    while(1)
    {
    	ThreadTime(19);

#if _NAND_TEST
    	/* 检测是否有按键按下 —— KEY1 */
    	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET )
    	{
    		/* 等待按键释放 */
    		while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
    			vTaskDelay(10);

    		for(uint8_t i=0; i<29; i++)
    		{
    			uRecordDataSave(tempP, sizeof(ChannelRealTimeData_T)*128*48);
    	    	vTaskDelay(2);
    		}
        }

    	/* 检测是否有按键按下 —— KEY2 */
    	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET )
    	{
    		/* 等待按键释放 */
    		while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
    			vTaskDelay(10);

        	uRecordDataPack(CMD_UPPER_GET_CH_RECORD_DATA, &tUpperPcTxQueue, &wl);
        }
#endif

#if _NAND_INIT
    	/* 检测是否有按键按下 —— KEY2 */
    	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET )
    	{
    		/* 等待按键释放 */
    		while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
    			vTaskDelay(10);

    		FTL_Format();
        }
#endif

    	vTaskDelay(20);
    }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

}

/*-----------------------------------------------------------------------------
 * @name  : Cache_Enable
 * @brief : 使能CPU的L1-Cache
 * @param : None
 * @retval: None
 * @date  : 2020/12/26
 * @note  :
 * ---------------------------------------------------------------------------*/
void Cache_Enable(void)
{
    SCB_EnableICache();//使能I-Cache
    SCB_EnableDCache();//使能D-Cache
	SCB->CACR|=1<<2;   //强制D-Cache透写,如不开启,实际使用中可能遇到各种问题
}

/* MPU Configuration */
void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;


  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x30040000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x30044000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

#if SDRAM_MPU

  //SDRAM MPU
  MPU_InitStruct.Number = SDRAM_REGION_NUMBER;      //设置region，NAND使用的region0
  MPU_InitStruct.BaseAddress = SDRAM_ADDRESS_START; //region基地址
  MPU_InitStruct.Size = SDRAM_REGION_SIZE;          //region大小
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;	  //此region可读写
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE; //允许读取此区域中的指令
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;      //禁止共享
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;          //允许cache
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;        //允许缓冲

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

#endif

#if NAND_FLASH_MPU

  //NAND FLASH MPU
  //配置RAM为region0，此区域可读写
  MPU_InitStruct.Number = NAND_REGION_NUMBER;      //设置region，NAND使用的region0
  MPU_InitStruct.BaseAddress = NAND_ADDRESS_START; //region基地址
  MPU_InitStruct.Size = NAND_REGION_SIZE;          //region大小
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;	  //此region可读写
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE; //允许读取此区域中的指令
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;      //禁止共享
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;      //禁止cache
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;        //允许缓冲

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

#endif

  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/*-----------------------------------------------------------------------------
 * @name  : ThreadTime
 * @brief : 任务测试数组
 * @param :
 * @retval:
 * @date  : 2021/01/10
 * @note  :
 * ---------------------------------------------------------------------------*/
uint32_t threadTimetest[20] = {0};
void ThreadTime(uint32_t num)
{
	threadTimetest[num]++;
	if(threadTimetest[num] > 4000000000)
	{
		threadTimetest[num] = 0;
	}
}

/*-----------------------------------------------------------------------------
 * @name  : DataU16H_ToU8
 * @brief : 返回参数的高8位
 * @param :
 * @retval: parm 16位参数
 * @date  : 2021/07/01
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t DataU16H_ToU8(uint16_t parm)
{
	return parm >> 8;
}

/*-----------------------------------------------------------------------------
 * @name  : DataU16L_ToU8
 * @brief : 返回参数的低8位
 * @param :
 * @retval: parm 16位参数
 * @date  : 2021/07/01
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t DataU16L_ToU8(uint16_t parm)
{
	return parm & 0xff;
}

/*-----------------------------------------------------------------------------
 * @name  : Data2U8ToU16
 * @brief : 两个U8转一个U16（将接收数据的字节数组转换成unsigned int类型）
 * @param : data_H：高八位， data_L：低八位
 * @retval: Data：unsigned int16 类型
 * @date  : 2021/01/10
 * @note  :
 * ---------------------------------------------------------------------------*/
uint16_t Data2U8ToU16(uint8_t data_L, uint8_t data_H)
{
	uint16_t Data = 0;
	Data = (data_H << 8) | data_L;
	return Data;
}

/*-----------------------------------------------------------------------------
 * @name  : Data4U8ToU32
 * @brief : 四个U8转一个U32（将接收数据的字节数组转换成unsigned int类型）
 * @param :
 * @retval: Data：unsigned int16 类型
 * @date  : 2021/01/10
 * @note  :
 * ---------------------------------------------------------------------------*/
uint32_t Data4U8ToU32(uint8_t data16L_L, uint8_t data16L_H, uint8_t data16H_L, uint8_t data16H_H)
{
	uint32_t Data = 0;
	Data = (data16H_H << 24) | (data16H_L << 16) | (data16L_H << 8) | data16L_L;
	return Data;
}

/*----------------- (C) COPYRIGHT Lyric Robot-----END OF FILE-----------------*/
