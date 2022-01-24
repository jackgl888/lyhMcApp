/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#ifndef INC_USER_USER_TCPECHO_H_
#define INC_USER_USER_TCPECHO_H_

#include "user_config.h"

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0                    192
#define IP_ADDR1                    168
#define IP_ADDR2                      0
#define IP_ADDR3                    100

/*NETMASK*/
#define NETMASK_ADDR0               255
#define NETMASK_ADDR1               255
#define NETMASK_ADDR2               255
#define NETMASK_ADDR3                 0

/*Gateway Address*/
#define GW_ADDR0                    192
#define GW_ADDR1                    168
#define GW_ADDR2                      0
#define GW_ADDR3                      1

//本地端口号
#define LOCAL_PORT                10001



#pragma  pack(push)  	//保存对齐状态
#pragma  pack(1)		//改为1字节对齐，防止内存对齐导致的错误

//-----------------用户数据结构------------------------

/* 存在的客户端数记录 */
typedef union
{
	struct
	{
		uint8_t exist1  :1; // bit0 传输第1个
		uint8_t exist2  :1; // bit1 传输第2个
		uint8_t exist3  :1; // bit2 传输第3个
		uint8_t exist4  :1; // bit3 传输第4个
		uint8_t exist5  :1; // bit4 传输第5个
		uint8_t exist6  :1; // bit5 传输第6个
		uint8_t exist7  :1; // bit6 传输第7个
		uint8_t exist8  :1; // bit7 传输第8个
	}Bit;
	uint8_t Num;
}ExistClient_U;

ExistClient_U uExistClient;


/* 多客户端记录 */
typedef struct
{
	struct netconn *newconn;
	TaskHandle_t tempHandle;
}MoreClient_T;

MoreClient_T tMoreClient[3];


#pragma pack(pop)



extern struct netconn *TCP_Netconn; //用于其他线程调用netcoon发送数据


void TCP_CtrlTask(void *parameter);
void TCP_Init(void);


#endif /* INC_USER_USER_TCPECHO_H_ */
