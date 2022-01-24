/**
 ******************************************************************************
  * File Name          : ethernetif.h
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"
#include "cmsis_os.h"

/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN 0 */
/**ETH GPIO Configuration
PC1     ------> ETH_MDC
PA1     ------> ETH_REF_CLK
PA2     ------> ETH_MDIO
PA7     ------> ETH_CRS_DV
PC4     ------> ETH_RXD0
PC5     ------> ETH_RXD1
PB11     ------> ETH_TX_EN
PG13     ------> ETH_TXD0
PG14     ------> ETH_TXD1
*/

/* ETH_MDC */
#define ETH_MDC_GPIO_CLK_ENABLE()           __GPIOC_CLK_ENABLE()
#define ETH_MDC_PORT                        GPIOC
#define ETH_MDC_PIN                         GPIO_PIN_1
#define ETH_MDC_AF                          GPIO_AF11_ETH

/* ETH_RMII_REF_CLK */
#define ETH_RMII_REF_CLK_GPIO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()
#define ETH_RMII_REF_CLK_PORT               GPIOA
#define ETH_RMII_REF_CLK_PIN                GPIO_PIN_1
#define ETH_RMII_REF_CLK_AF                 GPIO_AF11_ETH

/* ETH_MDIO */
#define ETH_MDIO_GPIO_CLK_ENABLE()          __GPIOA_CLK_ENABLE()
#define ETH_MDIO_PORT                       GPIOA
#define ETH_MDIO_PIN                        GPIO_PIN_2
#define ETH_MDIO_AF                         GPIO_AF11_ETH

/* ETH_RMII_CRS_DV */
#define ETH_RMII_CRS_DV_GPIO_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define ETH_RMII_CRS_DV_PORT                GPIOA
#define ETH_RMII_CRS_DV_PIN                 GPIO_PIN_7
#define ETH_RMII_CRS_DV_AF                  GPIO_AF11_ETH

/* ETH_RMII_RXD0 */
#define ETH_RMII_RXD0_GPIO_CLK_ENABLE()     __GPIOC_CLK_ENABLE()
#define ETH_RMII_RXD0_PORT                  GPIOC
#define ETH_RMII_RXD0_PIN                   GPIO_PIN_4
#define ETH_RMII_RXD0_AF                    GPIO_AF11_ETH

/* ETH_RMII_RXD1 */
#define ETH_RMII_RXD1_GPIO_CLK_ENABLE()     __GPIOC_CLK_ENABLE()
#define ETH_RMII_RXD1_PORT                  GPIOC
#define ETH_RMII_RXD1_PIN                   GPIO_PIN_5
#define ETH_RMII_RXD1_AF                    GPIO_AF11_ETH

/* ETH_RMII_TX_EN */
#define ETH_RMII_TX_EN_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()
#define ETH_RMII_TX_EN_PORT                 GPIOB
#define ETH_RMII_TX_EN_PIN                  GPIO_PIN_11
#define ETH_RMII_TX_EN_AF                   GPIO_AF11_ETH

/* ETH_RMII_TXD0 */
#define ETH_RMII_TXD0_GPIO_CLK_ENABLE()     __GPIOG_CLK_ENABLE()
#define ETH_RMII_TXD0_PORT                  GPIOG
#define ETH_RMII_TXD0_PIN                   GPIO_PIN_13
#define ETH_RMII_TXD0_AF                    GPIO_AF11_ETH

/* ETH_RMII_TXD1 */
#define ETH_RMII_TXD1_GPIO_CLK_ENABLE()     __GPIOG_CLK_ENABLE()
#define ETH_RMII_TXD1_PORT                  GPIOG
#define ETH_RMII_TXD1_PIN                   GPIO_PIN_14
#define ETH_RMII_TXD1_AF                    GPIO_AF11_ETH

//MAC地址
#define MAC_ADDR0   0x02U
#define MAC_ADDR1   0x00U
#define MAC_ADDR2   0x00U
#define MAC_ADDR3   0x00U
#define MAC_ADDR4   0x00U
#define MAC_ADDR5   0x00U

/* USER CODE END 0 */

extern volatile uint8_t NetConfigFlag; //LWIP是否初始化标志

/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);

void ethernetif_input(void const * argument);
void ethernet_link_thread(void const * argument);

u32_t sys_jiffies(void);
u32_t sys_now(void);

/* USER CODE BEGIN 1 */
void PHY_Link_Check_TASK(void *parameter);
/* USER CODE END 1 */
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
