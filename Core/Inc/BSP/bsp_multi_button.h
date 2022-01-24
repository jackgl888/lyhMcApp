/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */

#ifndef _MULTI_BUTTON_H_
#define _MULTI_BUTTON_H_

#include "stdint.h"
#include "string.h"

//According to your need to modify the constants.
#define TICKS_INTERVAL    5	//ms
#define DEBOUNCE_TICKS    3	//MAX 8
#define SHORT_TICKS       (300 /TICKS_INTERVAL)
#define LONG_TICKS        (1000 /TICKS_INTERVAL)


#pragma  pack(push)  	//保存对齐状态
#pragma  pack(1)		//改为1字节对齐，防止内存对齐导致的错误


typedef void (*BtnCallback)(void*);

typedef enum {
	PRESS_DOWN = 0,    //按键按下，每次按下都触发
	PRESS_UP,          //按键弹起，每次松开都触发
	PRESS_REPEAT,      //重复按下触发，变量repeat计数连击次数
	SINGLE_CLICK,      //单击按键事件
	DOUBLE_CLICK,      //双击按键事件
	LONG_PRESS_START,  //长按期间一直触发
	LONG_PRESS_HOLD,   //长按期间一直触发
	number_of_event,   //上述事件总数：7个事件
	NONE_PRESS         //无按键事件
}PressEvent;

typedef struct Button {
	uint16_t ticks;
	uint8_t  repeat : 4;
	uint8_t  event : 4;
	uint8_t  state : 3;
	uint8_t  debounce_cnt : 3;
	uint8_t  active_level : 1;
	uint8_t  button_level : 1;
	uint8_t  (*hal_button_Level)(void);
	BtnCallback  cb[number_of_event];
	struct Button* next;
}Btn;

#pragma  pack(pop)		//恢复对齐状态

#ifdef __cplusplus
extern "C" {
#endif

void button_init(struct Button* handle, uint8_t(*pin_level)(), uint8_t active_level);
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
PressEvent get_button_event(struct Button* handle);
int  button_start(struct Button* handle);
void button_stop(struct Button* handle);
void button_ticks(void);

#ifdef __cplusplus
}
#endif

#endif
