/*
 * key.c
 *
 *  Created on: Aug 16, 2021
 *      Author: Lyric
 */

#ifdef USER_CONFIG
	#include "user_config.h"
#endif


//申请按键结构
struct Button KEY_0;
struct Button KEY_1;
struct Button KEY_2;
struct Button KEY_UP;

/*-----------------------------------------------------------------------------
 * @name  : Read_KEY_0_GPIO
 * @brief : 获取按键状态
 * @param :
 * @retval:
 * @date  : 2021/08/16
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t Read_KEY_0_GPIO()
{
	return HAL_GPIO_ReadPin(KEY_0_PORT, KEY_0_PIN);
}

/*-----------------------------------------------------------------------------
 * @name  : Read_KEY_1_GPIO
 * @brief : 获取按键状态
 * @param :
 * @retval:
 * @date  : 2021/08/16
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t Read_KEY_1_GPIO()
{
	return HAL_GPIO_ReadPin(KEY_1_PORT, KEY_1_PIN);
}

/*-----------------------------------------------------------------------------
 * @name  : Read_KEY_2_GPIO
 * @brief : 获取按键状态
 * @param :
 * @retval:
 * @date  : 2021/08/16
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t Read_KEY_2_GPIO()
{
	return HAL_GPIO_ReadPin(KEY_2_PORT, KEY_2_PIN);
}

/*-----------------------------------------------------------------------------
 * @name  : Read_KEY_UP_GPIO
 * @brief : 获取按键状态
 * @param :
 * @retval:
 * @date  : 2021/08/16
 * @note  :
 * ---------------------------------------------------------------------------*/
uint8_t Read_KEY_UP_GPIO()
{
	return HAL_GPIO_ReadPin(KEY_UP_PORT, KEY_UP_PIN);
}

/*-----------------------------------------------------------------------------
 * @name  : KEY_GPIO_Init
 * @brief : 按键GPIO初始化
 * @param :
 * @retval:
 * @date  : 2021/08/16
 * @note  : 原子开发板上的四个按键初始化，需分上下拉
 * ---------------------------------------------------------------------------*/
static void KEY_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  KEY_0_CLK_ENABLE();
  KEY_1_CLK_ENABLE();
  KEY_2_CLK_ENABLE();
  KEY_UP_CLK_ENABLE();

  /*Configure GPIO pins : PE4 */
  GPIO_InitStruct.Pin = KEY_0_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_0_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : PE3 */
  GPIO_InitStruct.Pin = KEY_1_PIN;
  HAL_GPIO_Init(KEY_1_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : PE2 */
  GPIO_InitStruct.Pin = KEY_2_PIN;
  HAL_GPIO_Init(KEY_2_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = KEY_UP_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(KEY_UP_PORT, &GPIO_InitStruct);
}

/*-----------------------------------------------------------------------------
 * @name  : KEY_0_Button_Callback
 * @brief : 按键KEY_0回调函数
 * @param : button 结构体指针
 * @retval:
 * @date  : 2021/08/16
 * @note  :
 * ---------------------------------------------------------------------------*/
void KEY_0_Button_Callback(void *button)
{
    uint32_t btn_event_val;

    btn_event_val = get_button_event((struct Button *)button);

    switch(btn_event_val)
    {
	    case PRESS_DOWN:
	        printf("---> KEY_0 press down! <---\r\n");
	    	break;

	    case PRESS_UP:
	        printf("***> KEY_0 press up! <***\r\n");
	    	break;

	    case PRESS_REPEAT:
	        printf("---> KEY_0 press repeat! <---\r\n");
	    	break;

	    case SINGLE_CLICK:
	        printf("---> KEY_0 single click! <---\r\n");
	    	break;

	    case DOUBLE_CLICK:
	        printf("***> KEY_0 double click! <***\r\n");
	    	break;

	    case LONG_PRESS_START:
	        printf("---> KEY_0 long press start! <---\r\n");
	   		break;

	    case LONG_PRESS_HOLD:
	        printf("***> KEY_0 long press hold! <***\r\n");
	    	break;
	    default: break;
	}
}

/*-----------------------------------------------------------------------------
 * @name  : KEY_1_Button_Callback
 * @brief : 按键KEY_1回调函数
 * @param : button 结构体指针
 * @retval:
 * @date  : 2021/08/16
 * @note  :
 * ---------------------------------------------------------------------------*/
void KEY_1_Button_Callback(void *button)
{
    uint32_t btn_event_val;

    btn_event_val = get_button_event((struct Button *)button);

    switch(btn_event_val)
    {
	    case PRESS_DOWN:
	        printf("---> KEY_1 press down! <---\r\n");
	    	break;

	    case PRESS_UP:
	        printf("***> KEY_1 press up! <***\r\n");
	    	break;

	    case PRESS_REPEAT:
	        printf("---> KEY_1 press repeat! <---\r\n");
	    	break;

	    case SINGLE_CLICK:
	        printf("---> KEY_1 single click! <---\r\n");
	    	break;

	    case DOUBLE_CLICK:
	        printf("***> KEY_1 double click! <***\r\n");
	    	break;

	    case LONG_PRESS_START:
	        printf("---> KEY_1 long press start! <---\r\n");
	   		break;

	    case LONG_PRESS_HOLD:
	        printf("***> KEY_1 long press hold! <***\r\n");
	    	break;
	    default: break;
	}
}

/*-----------------------------------------------------------------------------
 * @name  : KEY_2_Button_Callback
 * @brief : 按键KEY_2回调函数
 * @param : button 结构体指针
 * @retval:
 * @date  : 2021/08/16
 * @note  :
 * ---------------------------------------------------------------------------*/
void KEY_2_Button_Callback(void *button)
{
    uint32_t btn_event_val;

    btn_event_val = get_button_event((struct Button *)button);

    switch(btn_event_val)
    {
	    case PRESS_DOWN:
	        printf("---> KEY_2 press down! <---\r\n");
	    	break;

	    case PRESS_UP:
	        printf("***> KEY_2 press up! <***\r\n");
	    	break;

	    case PRESS_REPEAT:
	        printf("---> KEY_2 press repeat! <---\r\n");
	    	break;

	    case SINGLE_CLICK:
	        printf("---> KEY_2 single click! <---\r\n");
	    	break;

	    case DOUBLE_CLICK:
	        printf("***> KEY_2 double click! <***\r\n");
	    	break;

	    case LONG_PRESS_START:
	        printf("---> KEY_2 long press start! <---\r\n");
	   		break;

	    case LONG_PRESS_HOLD:
	        printf("***> KEY_2 long press hold! <***\r\n");
	    	break;
	    default: break;
	}
}

/*-----------------------------------------------------------------------------
 * @name  : KEY_UP_Button_Callback
 * @brief : 按键KEY_UP回调函数
 * @param : button 结构体指针
 * @retval:
 * @date  : 2021/08/16
 * @note  :
 * ---------------------------------------------------------------------------*/
void KEY_UP_Button_Callback(void *button)
{
    uint32_t btn_event_val;

    btn_event_val = get_button_event((struct Button *)button);

    switch(btn_event_val)
    {
	    case PRESS_DOWN:
	        printf("---> KEY_UP press down! <---\r\n");
	    	break;

	    case PRESS_UP:
	        printf("***> KEY_UP press up! <***\r\n");
	    	break;

	    case PRESS_REPEAT:
	        printf("---> KEY_UP press repeat! <---\r\n");
	    	break;

	    case SINGLE_CLICK:
	        printf("---> KEY_UP single click! <---\r\n");
	    	break;

	    case DOUBLE_CLICK:
	        printf("***> KEY_UP double click! <***\r\n");
	    	break;

	    case LONG_PRESS_START:
	        printf("---> KEY_UP long press start! <---\r\n");
	   		break;

	    case LONG_PRESS_HOLD:
	        printf("***> KEY_UP long press hold! <***\r\n");
	    	break;
	    default: break;
	}
}

/*-----------------------------------------------------------------------------
 * @name  : KEY_Init
 * @brief : 按键初始化函数
 * @param : button 结构体指针
 * @retval:
 * @date  : 2021/08/16
 * @note  :
 * ---------------------------------------------------------------------------*/
void KEY_Init(void)
{
	KEY_GPIO_Init();

	button_init(&KEY_0, Read_KEY_0_GPIO, GPIO_PIN_RESET);
	button_init(&KEY_1, Read_KEY_1_GPIO, GPIO_PIN_RESET);
	button_init(&KEY_2, Read_KEY_2_GPIO, GPIO_PIN_RESET);
	button_init(&KEY_UP, Read_KEY_UP_GPIO, GPIO_PIN_SET);

	button_attach(&KEY_0, PRESS_DOWN,       KEY_0_Button_Callback);
	button_attach(&KEY_0, PRESS_UP,         KEY_0_Button_Callback);
	button_attach(&KEY_0, PRESS_REPEAT,     KEY_0_Button_Callback);
	button_attach(&KEY_0, SINGLE_CLICK,     KEY_0_Button_Callback);
	button_attach(&KEY_0, DOUBLE_CLICK,     KEY_0_Button_Callback);
	button_attach(&KEY_0, LONG_PRESS_START, KEY_0_Button_Callback);
	button_attach(&KEY_0, LONG_PRESS_HOLD,  KEY_0_Button_Callback);

	button_attach(&KEY_1, PRESS_DOWN,       KEY_1_Button_Callback);
	button_attach(&KEY_1, PRESS_UP,         KEY_1_Button_Callback);
	button_attach(&KEY_1, PRESS_REPEAT,     KEY_1_Button_Callback);
	button_attach(&KEY_1, SINGLE_CLICK,     KEY_1_Button_Callback);
	button_attach(&KEY_1, DOUBLE_CLICK,     KEY_1_Button_Callback);
	button_attach(&KEY_1, LONG_PRESS_START, KEY_1_Button_Callback);
	button_attach(&KEY_1, LONG_PRESS_HOLD,  KEY_1_Button_Callback);

	button_attach(&KEY_2, PRESS_DOWN,       KEY_2_Button_Callback);
	button_attach(&KEY_2, PRESS_UP,         KEY_2_Button_Callback);
	button_attach(&KEY_2, PRESS_REPEAT,     KEY_2_Button_Callback);
	button_attach(&KEY_2, SINGLE_CLICK,     KEY_2_Button_Callback);
	button_attach(&KEY_2, DOUBLE_CLICK,     KEY_2_Button_Callback);
	button_attach(&KEY_2, LONG_PRESS_START, KEY_2_Button_Callback);
	button_attach(&KEY_2, LONG_PRESS_HOLD,  KEY_2_Button_Callback);

	button_attach(&KEY_UP, PRESS_DOWN,       KEY_UP_Button_Callback);
	button_attach(&KEY_UP, PRESS_UP,         KEY_UP_Button_Callback);
	button_attach(&KEY_UP, PRESS_REPEAT,     KEY_UP_Button_Callback);
	button_attach(&KEY_UP, SINGLE_CLICK,     KEY_UP_Button_Callback);
	button_attach(&KEY_UP, DOUBLE_CLICK,     KEY_UP_Button_Callback);
	button_attach(&KEY_UP, LONG_PRESS_START, KEY_UP_Button_Callback);
	button_attach(&KEY_UP, LONG_PRESS_HOLD,  KEY_UP_Button_Callback);

	button_start(&KEY_0);
	button_start(&KEY_1);
	button_start(&KEY_2);
	button_start(&KEY_UP);
}


/*-----------------------------------------------------------------------------
 * @name  : KEY_Ctrl_Task
 * @brief : 按键后台处理函数
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/08/16
 * @note  :
 * ---------------------------------------------------------------------------*/
void KEY_Ctrl_Task(void *parameter)
{
    KEY_Init();

    while(1)
    {
//    	ThreadTime(19);

    	//每隔5ms调用一次后台处理函数
    	button_ticks();
    	vTaskDelay(TICKS_INTERVAL);
    }
}
