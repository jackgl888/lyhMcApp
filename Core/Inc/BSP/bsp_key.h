/*
 * key.h
 *
 *  Created on: Aug 16, 2021
 *      Author: Lyric
 */

#ifndef INC_KEY_H_
#define INC_KEY_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define KEY_0_PIN             GPIO_PIN_4
#define KEY_0_PORT            GPIOE
#define KEY_0_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()

#define KEY_1_PIN             GPIO_PIN_3
#define KEY_1_PORT            GPIOE
#define KEY_1_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()

#define KEY_2_PIN             GPIO_PIN_2
#define KEY_2_PORT            GPIOE
#define KEY_2_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()

#define KEY_UP_PIN            GPIO_PIN_0
#define KEY_UP_PORT           GPIOA
#define KEY_UP_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()



void KEY_Init(void);
void KEY_Ctrl_Task(void *parameter);



#ifdef __cplusplus
}
#endif
#endif /* INC_KEY_H_ */
