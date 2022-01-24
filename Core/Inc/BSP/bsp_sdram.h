/*-----------------------------------------------------------------------------
 * @file        : bsp_sdram.h
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2021/09/1
 * @date changed: 2021/09/1
 * @brief       : 外扩SDRAM应用函数接口
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/
#ifndef INC_BSP_BSP_SDRAM_H_
#define INC_BSP_BSP_SDRAM_H_

#include "main.h"
#include "stm32h7xx.h"
#include <stdio.h>


//#define IS42S16400J_SIZE  0x800000   //400000*16bits = 0x800000  ，8M字节
//#define IS42S16400J_SIZE  0x1000000  //16M字节
#define IS42S16400J_SIZE    (32*1024*1024)  //32M字节，W9825G6

/* SDRAM 的bank选择*/
#define FMC_BANK_SDRAM            FMC_SDRAM_BANK2
#define FMC_COMMAND_TARGET_BANK   FMC_SDRAM_CMD_TARGET_BANK2

/* FMC SDRAM 数据基地址 */
#define SDRAM_BANK_ADDR           ((uint32_t)0xD0000000)

/* FMC SDRAM 数据宽度 */
#if 0
#define SDRAM_MEMORY_WIDTH   FMC_SDRAM_MEM_BUS_WIDTH_8
#else
#define SDRAM_MEMORY_WIDTH    FMC_SDRAM_MEM_BUS_WIDTH_16
#endif

/* FMC SDRAM CAS Latency */
#if 0
#define SDRAM_CAS_LATENCY   FMC_SDRAM_CAS_LATENCY_2
#else
#define SDRAM_CAS_LATENCY   FMC_SDRAM_CAS_LATENCY_3
#endif

/* FMC SDRAM SDCLK 时钟分频因子 */
#if 0
#define SDCLOCK_PERIOD    FMC_SDRAM_CLOCK_PERIOD_2        /* Default configuration used with LCD */
#else
#define SDCLOCK_PERIOD    FMC_SDRAM_CLOCK_PERIOD_3
#endif

/* FMC SDRAM RPIPE DELAY 读通道延时 */
#if 0
#define SDRAM_RPIPE_DELAY    FMC_SDRAM_RPIPE_DELAY_0
#else
#define SDRAM_RPIPE_DELAY    FMC_SDRAM_RPIPE_DELAY_1
#endif

#define SDRAM_TIMEOUT        ((uint32_t)0xFFFF)


//SDRAM 开启MPU配置
#define SDRAM_MPU          1

//MPU相关设置
#define SDRAM_REGION_NUMBER      MPU_REGION_NUMBER2	     //SDRAMH使用region0
#define SDRAM_ADDRESS_START      SDRAM_BANK_ADDR         //SDRAM区的首地址
#define SDRAM_REGION_SIZE        MPU_REGION_SIZE_32MB    //SDRAM区大小


/**
  * @brief  FMC SDRAM 模式配置的寄存器相关定义
  */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)      


/*信息输出*/
#define SDRAM_DEBUG_ON         1

#define SDRAM_INFO(fmt,arg...)           printf("<<-SDRAM-INFO->> "fmt"\n",##arg)
#define SDRAM_ERROR(fmt,arg...)          printf("<<-SDRAM-ERROR->> "fmt"\n",##arg)
#define SDRAM_DEBUG(fmt,arg...)          do{\
                                          if(SDRAM_DEBUG_ON)\
                                          printf("<<-SDRAM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

  
/*A行列地址信号线*/
#define FMC_A0_GPIO_PORT         GPIOF
#define FMC_A0_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A0_GPIO_PIN          GPIO_PIN_0

#define FMC_A1_GPIO_PORT         GPIOF
#define FMC_A1_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A1_GPIO_PIN          GPIO_PIN_1

#define FMC_A2_GPIO_PORT         GPIOF
#define FMC_A2_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A2_GPIO_PIN          GPIO_PIN_2

#define FMC_A3_GPIO_PORT         GPIOF
#define FMC_A3_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A3_GPIO_PIN          GPIO_PIN_3

#define FMC_A4_GPIO_PORT         GPIOF
#define FMC_A4_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A4_GPIO_PIN          GPIO_PIN_4

#define FMC_A5_GPIO_PORT         GPIOF
#define FMC_A5_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A5_GPIO_PIN          GPIO_PIN_5

#define FMC_A6_GPIO_PORT         GPIOF
#define FMC_A6_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A6_GPIO_PIN          GPIO_PIN_12

#define FMC_A7_GPIO_PORT         GPIOF
#define FMC_A7_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A7_GPIO_PIN          GPIO_PIN_13

#define FMC_A8_GPIO_PORT         GPIOF
#define FMC_A8_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A8_GPIO_PIN          GPIO_PIN_14

#define FMC_A9_GPIO_PORT         GPIOF
#define FMC_A9_GPIO_CLK()        __GPIOF_CLK_ENABLE()
#define FMC_A9_GPIO_PIN          GPIO_PIN_15

#define FMC_A10_GPIO_PORT        GPIOG
#define FMC_A10_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_A10_GPIO_PIN         GPIO_PIN_0

#define FMC_A11_GPIO_PORT        GPIOG
#define FMC_A11_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_A11_GPIO_PIN         GPIO_PIN_1

#define FMC_A12_GPIO_PORT        GPIOG
#define FMC_A12_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_A12_GPIO_PIN         GPIO_PIN_2

/*BA地址线*/
#define FMC_BA0_GPIO_PORT        GPIOG
#define FMC_BA0_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_BA0_GPIO_PIN         GPIO_PIN_4

#define FMC_BA1_GPIO_PORT        GPIOG
#define FMC_BA1_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_BA1_GPIO_PIN         GPIO_PIN_5

/*数据信号线*/
#define FMC_D0_GPIO_PORT         GPIOD
#define FMC_D0_GPIO_CLK()        __GPIOD_CLK_ENABLE()
#define FMC_D0_GPIO_PIN          GPIO_PIN_14

#define FMC_D1_GPIO_PORT         GPIOD
#define FMC_D1_GPIO_CLK()        __GPIOD_CLK_ENABLE()
#define FMC_D1_GPIO_PIN          GPIO_PIN_15

#define FMC_D2_GPIO_PORT         GPIOD
#define FMC_D2_GPIO_CLK()        __GPIOD_CLK_ENABLE()
#define FMC_D2_GPIO_PIN          GPIO_PIN_0

#define FMC_D3_GPIO_PORT         GPIOD
#define FMC_D3_GPIO_CLK()        __GPIOD_CLK_ENABLE()
#define FMC_D3_GPIO_PIN          GPIO_PIN_1

#define FMC_D4_GPIO_PORT         GPIOE
#define FMC_D4_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D4_GPIO_PIN          GPIO_PIN_7

#define FMC_D5_GPIO_PORT         GPIOE
#define FMC_D5_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D5_GPIO_PIN          GPIO_PIN_8

#define FMC_D6_GPIO_PORT         GPIOE
#define FMC_D6_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D6_GPIO_PIN          GPIO_PIN_9

#define FMC_D7_GPIO_PORT         GPIOE
#define FMC_D7_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D7_GPIO_PIN          GPIO_PIN_10

#define FMC_D8_GPIO_PORT         GPIOE
#define FMC_D8_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D8_GPIO_PIN          GPIO_PIN_11

#define FMC_D9_GPIO_PORT         GPIOE
#define FMC_D9_GPIO_CLK()        __GPIOE_CLK_ENABLE()
#define FMC_D9_GPIO_PIN          GPIO_PIN_12

#define FMC_D10_GPIO_PORT        GPIOE
#define FMC_D10_GPIO_CLK()       __GPIOE_CLK_ENABLE()
#define FMC_D10_GPIO_PIN         GPIO_PIN_13

#define FMC_D11_GPIO_PORT        GPIOE
#define FMC_D11_GPIO_CLK()       __GPIOE_CLK_ENABLE()
#define FMC_D11_GPIO_PIN         GPIO_PIN_14

#define FMC_D12_GPIO_PORT        GPIOE
#define FMC_D12_GPIO_CLK()       __GPIOE_CLK_ENABLE()
#define FMC_D12_GPIO_PIN         GPIO_PIN_15

#define FMC_D13_GPIO_PORT        GPIOD
#define FMC_D13_GPIO_CLK()       __GPIOD_CLK_ENABLE()
#define FMC_D13_GPIO_PIN         GPIO_PIN_8

#define FMC_D14_GPIO_PORT        GPIOD
#define FMC_D14_GPIO_CLK()       __GPIOD_CLK_ENABLE()
#define FMC_D14_GPIO_PIN         GPIO_PIN_9

#define FMC_D15_GPIO_PORT        GPIOD
#define FMC_D15_GPIO_CLK()       __GPIOD_CLK_ENABLE()
#define FMC_D15_GPIO_PIN         GPIO_PIN_10


/*控制信号线*/
/*CS片选*/
#define FMC_CS_GPIO_PORT         GPIOH
#define FMC_CS_GPIO_CLK()        __GPIOH_CLK_ENABLE()
#define FMC_CS_GPIO_PIN          GPIO_PIN_6

/*WE写使能*/
#define FMC_WE_GPIO_PORT         GPIOC
#define FMC_WE_GPIO_CLK()        __GPIOC_CLK_ENABLE()
#define FMC_WE_GPIO_PIN          GPIO_PIN_0

/*RAS行选通*/
#define FMC_RAS_GPIO_PORT        GPIOF
#define FMC_RAS_GPIO_CLK()       __GPIOF_CLK_ENABLE()
#define FMC_RAS_GPIO_PIN         GPIO_PIN_11

/*CAS列选通*/
#define FMC_CAS_GPIO_PORT        GPIOG
#define FMC_CAS_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_CAS_GPIO_PIN         GPIO_PIN_15

/*DQM1数据掩码*/
#define FMC_UDQM_GPIO_PORT        GPIOE
#define FMC_UDQM_GPIO_CLK()       __GPIOE_CLK_ENABLE()
#define FMC_UDQM_GPIO_PIN         GPIO_PIN_1

/*DQM0数据掩码*/
#define FMC_LDQM_GPIO_PORT        GPIOE
#define FMC_LDQM_GPIO_CLK()       __GPIOE_CLK_ENABLE()
#define FMC_LDQM_GPIO_PIN         GPIO_PIN_0

/* TODO:
 *
 * SDCKE0： SDRAM 存储区域 1 时钟使能
 * SDCKE1： SDRAM 存储区域 2 时钟使能
 *
 * SDNE0： SDRAM 存储区域 1 芯片使能
 * SDNE1： SDRAM 存储区域 2 芯片使能
 *
 */

/*CLK同步时钟，存储区域2*/
#define FMC_CLK_GPIO_PORT        GPIOG
#define FMC_CLK_GPIO_CLK()       __GPIOG_CLK_ENABLE()
#define FMC_CLK_GPIO_PIN         GPIO_PIN_8

/*CKE时钟使能，存储区域2*/
#define FMC_CKE_GPIO_PORT        GPIOH
#define FMC_CKE_GPIO_CLK()       __GPIOH_CLK_ENABLE()
#define FMC_CKE_GPIO_PIN         GPIO_PIN_7


/** @defgroup STM32429 SDRAM函数
  * @{
  */ 
void  SDRAM_Init(void);
void  SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
void  SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);
uint8_t SDRAM_Test(void);


#endif /* INC_BSP_BSP_SDRAM_H_ */
