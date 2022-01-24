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
#ifdef USER_CONFIG
	#include "user_config.h"
#endif
#include "lan8742.h"
/*---------------------------------------------------------------------------
		                               全局变量定义区
 *-------------------------------------------------------------------------*/
static TaskHandle_t TCP_CTRL_TASK_Handle_1 = NULL; //TCP任务句柄
static TaskHandle_t TCP_CTRL_TASK_Handle_2 = NULL; //TCP任务句柄
static TaskHandle_t TCP_CTRL_TASK_Handle_3 = NULL; //TCP任务句柄

struct netconn *TCP_Netconn = NULL; //用于其他线程调用netcoon发送数据

#if __LWIP_ENABLE

/*-----------------------------------------------------------------------------
 * @name  : TCP_QueryIsNull
 * @brief : 通信：查询是否存在
 * @param : pLayer：存在的记录结构体句柄
 *          num：查询的次数
 * @retval: 0 不存在 1 存在
 * @date  : 2021/11/05
 * @note  :
 * ---------------------------------------------------------------------------*/
static uint8_t TCP_QueryIsNull(ExistClient_U* pClient, uint8_t num)
{
	uint8_t sta = 0;

	if(((pClient->Num >> (num-1)) & 0x01) == 0x01)
	{
		sta  = 1;
	}

	return sta;
}

/*-----------------------------------------------------------------------------
 * @name  : TCP_TcpCtrl_Task_1
 * @brief : TCP传输线程
 * @param :
 * @retval:
 * @date  : 2021/11/15
 * @note  :
 * ---------------------------------------------------------------------------*/
static void TCP_TcpCtrl_Task_1(MoreClient_T* pMC)
{
	/* 以太网数据接收处理相关变量 */
	struct netbuf *buf;
	void * databuf = NULL; //指针，用于保存接收数据包的pbuf起始地址
	u16_t len = 0;	       //用于记录TCP控制包的长度
	err_t err = 0;
	//--------------------------------------------------------------------------------------
	/* 用于获取IP地址 */
#if 0
	u32_t addr = 0;
	u16_t port = 0;
	u8_t remot_addr[4];
	err = netconn_getaddr(pMC->newconn,(ip_addr_t*)(&addr),&port,0);
	remot_addr[3] = (uint8_t)(addr >> 24);
	remot_addr[2] = (uint8_t)(addr >> 16);
	remot_addr[1] = (uint8_t)(addr >> 8);
	remot_addr[0] = (uint8_t)(addr);
	DEBUG_PRINTF("Client: %d.%d.%d.%d Connect to the Server,The port number:%d! \r\n",
			remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
#endif
	//--------------------------------------------------------------------------------------

//	tcp_nagle_disable(pMC->newconn->pcb.tcp); //禁用nagle算法

#if 1

	uint8_t CtrlNum = 0;
	tCommPro.Tcp_Comm_Flag[CtrlNum] = 1;
	tCommPro.Tcp_Timeout_MS[CtrlNum] = 0; //清零
	tCommPro.Tcp_Timeout_S[CtrlNum] = 0; //清零

	pMC->newconn->recv_timeout = 100; //以太网接收不永久阻塞，只阻塞100ms

	while(1)
	{
		if((err = netconn_recv(pMC->newconn, &buf)) == ERR_OK)
		{
			/* 拷贝以太网接收缓冲区的内容 */
			do{
				netbuf_data(buf, &databuf, &len);
			}while (netbuf_next(buf) >= 0);

			uQueueWrite(&tUpperPcRxQueue, (uint8_t *)databuf, len); //写入环形队列

			tCommPro.Tcp_Timeout_MS[CtrlNum] = 0; //清零
			tCommPro.Tcp_Timeout_S[CtrlNum] = 0; //清零

			/* 清理netbuf空间，等待下一次数据连接*/
			netbuf_delete(buf);
		}

		/* 未连接 */
		if(err == ERR_CONN)
		{
			tcp_shutdown(pMC->newconn->pcb.tcp, 1, 1); //断开TCP连接
			break;
		}

		/* 5秒保活时间 */
		if(tCommPro.Tcp_Timeout_S[CtrlNum] > 5)
		{
			tcp_shutdown(pMC->newconn->pcb.tcp, 1, 1); //断开TCP连接
			break;
		}
	}

	tCommPro.Tcp_Comm_Flag[CtrlNum] = 0;
	tCommPro.Tcp_Timeout_MS[CtrlNum] = 0; //清零
	tCommPro.Tcp_Timeout_S[CtrlNum] = 0; //清零

#else

	pMC->newconn->recv_timeout = 10000; //以太网接收不永久阻塞，只阻塞10s

	/* 等待数据接收完成 */
	while (netconn_recv(pMC->newconn, &buf) == ERR_OK)
	{
		/* 拷贝以太网接收缓冲区的内容 */
		do{
			netbuf_data(buf, &databuf, &len);
//			netconn_write(pMC->newconn, databuf, len, NETCONN_NOCOPY); //发送接收到的数据
		}while (netbuf_next(buf) >= 0);

//		DEBUG_PRINTF("收到以太网控制包长度：%d\n字节",len);

		uQueueWrite(&tUpperPcRxQueue, (uint8_t *)databuf, len); //写入环形队列

		/* 清理netbuf空间，等待下一次数据连接*/
		netbuf_delete(buf);
	}

#endif

	/* 断开TCP连接，删除netconn结构体，等待下一次握手*/
	netconn_close(pMC->newconn);
	netconn_delete(pMC->newconn);
	pMC->newconn =  NULL;
	pMC->tempHandle = NULL;
	uExistClient.Bit.exist1 = 0; //不存在
	vTaskDelete(pMC->tempHandle);
}

/*-----------------------------------------------------------------------------
 * @name  : TCP_TcpCtrl_Task_2
 * @brief : TCP传输线程
 * @param :
 * @retval:
 * @date  : 2021/11/15
 * @note  :
 * ---------------------------------------------------------------------------*/
static void TCP_TcpCtrl_Task_2(MoreClient_T* pMC)
{
	/* 以太网数据接收处理相关变量 */
	struct netbuf *buf;
	void * databuf = NULL; //指针，用于保存接收数据包的pbuf起始地址
	u16_t len = 0;	       //用于记录TCP控制包的长度
	err_t err = 0;
	//--------------------------------------------------------------------------------------
	/* 用于获取IP地址 */
#if 0
	u32_t addr = 0;
	u16_t port = 0;
	u8_t remot_addr[4];
	err = netconn_getaddr(pMC->newconn,(ip_addr_t*)(&addr),&port,0);
	remot_addr[3] = (uint8_t)(addr >> 24);
	remot_addr[2] = (uint8_t)(addr >> 16);
	remot_addr[1] = (uint8_t)(addr >> 8);
	remot_addr[0] = (uint8_t)(addr);
	DEBUG_PRINTF("Client: %d.%d.%d.%d Connect to the Server,The port number:%d! \r\n",
			remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
#endif
	//--------------------------------------------------------------------------------------

//	tcp_nagle_disable(pMC->newconn->pcb.tcp); //禁用nagle算法

#if 1

	uint8_t CtrlNum = 1;
	tCommPro.Tcp_Comm_Flag[CtrlNum] = 1;
	tCommPro.Tcp_Timeout_MS[CtrlNum] = 0; //清零
	tCommPro.Tcp_Timeout_S[CtrlNum] = 0; //清零

	pMC->newconn->recv_timeout = 100; //以太网接收不永久阻塞，只阻塞100ms

	while(1)
	{
		if((err = netconn_recv(pMC->newconn, &buf)) == ERR_OK)
		{
			/* 拷贝以太网接收缓冲区的内容 */
			do{
				netbuf_data(buf, &databuf, &len);
			}while (netbuf_next(buf) >= 0);

			uQueueWrite(&tUpperPcRxQueue, (uint8_t *)databuf, len); //写入环形队列

			tCommPro.Tcp_Timeout_MS[CtrlNum] = 0; //清零
			tCommPro.Tcp_Timeout_S[CtrlNum] = 0; //清零

			/* 清理netbuf空间，等待下一次数据连接*/
			netbuf_delete(buf);
		}

		/* 未连接 */
		if(err == ERR_CONN)
		{
			tcp_shutdown(pMC->newconn->pcb.tcp, 1, 1); //断开TCP连接
			break;
		}

		/* 5秒保活时间 */
		if(tCommPro.Tcp_Timeout_S[CtrlNum] > 5)
		{
			tcp_shutdown(pMC->newconn->pcb.tcp, 1, 1); //断开TCP连接
			break;
		}
	}

	tCommPro.Tcp_Comm_Flag[CtrlNum] = 0;
	tCommPro.Tcp_Timeout_MS[CtrlNum] = 0; //清零
	tCommPro.Tcp_Timeout_S[CtrlNum] = 0; //清零

#else

	pMC->newconn->recv_timeout = 10000; //以太网接收不永久阻塞，只阻塞10s

	/* 等待数据接收完成 */
	while (netconn_recv(pMC->newconn, &buf) == ERR_OK)
	{
		/* 拷贝以太网接收缓冲区的内容 */
		do{
			netbuf_data(buf, &databuf, &len);
//			netconn_write(pMC->newconn, databuf, len, NETCONN_NOCOPY); //发送接收到的数据
		}while (netbuf_next(buf) >= 0);

//		DEBUG_PRINTF("收到以太网控制包长度：%d\n字节",len);

		uQueueWrite(&tUpperPcRxQueue, (uint8_t *)databuf, len); //写入环形队列

		/* 清理netbuf空间，等待下一次数据连接*/
		netbuf_delete(buf);
	}

#endif

	/* 断开TCP连接，删除netconn结构体，等待下一次握手*/
	netconn_close(pMC->newconn);
	netconn_delete(pMC->newconn);
	pMC->newconn =  NULL;
	pMC->tempHandle = NULL;
	uExistClient.Bit.exist2 = 0; //不存在
	vTaskDelete(pMC->tempHandle);
}

/*-----------------------------------------------------------------------------
 * @name  : TCP_TcpCtrl_Task_3
 * @brief : TCP传输线程
 * @param :
 * @retval:
 * @date  : 2021/11/15
 * @note  :
 * ---------------------------------------------------------------------------*/
static void TCP_TcpCtrl_Task_3(MoreClient_T* pMC)
{
	/* 以太网数据接收处理相关变量 */
	struct netbuf *buf;
	void * databuf = NULL; //指针，用于保存接收数据包的pbuf起始地址
	u16_t len = 0;	       //用于记录TCP控制包的长度
	err_t err = 0;
	//--------------------------------------------------------------------------------------
	/* 用于获取IP地址 */
#if 0
	u32_t addr = 0;
	u16_t port = 0;
	u8_t remot_addr[4];
	err = netconn_getaddr(pMC->newconn,(ip_addr_t*)(&addr),&port,0);
	remot_addr[3] = (uint8_t)(addr >> 24);
	remot_addr[2] = (uint8_t)(addr >> 16);
	remot_addr[1] = (uint8_t)(addr >> 8);
	remot_addr[0] = (uint8_t)(addr);
	DEBUG_PRINTF("Client: %d.%d.%d.%d Connect to the Server,The port number:%d! \r\n",
			remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
#endif
	//--------------------------------------------------------------------------------------

	//	tcp_nagle_disable(pMC->newconn->pcb.tcp); //禁用nagle算法

#if 1

	uint8_t CtrlNum = 2;
	tCommPro.Tcp_Comm_Flag[CtrlNum] = 1;
	tCommPro.Tcp_Timeout_MS[CtrlNum] = 0; //清零
	tCommPro.Tcp_Timeout_S[CtrlNum] = 0; //清零

	pMC->newconn->recv_timeout = 100; //以太网接收不永久阻塞，只阻塞100ms

	while(1)
	{
		if((err = netconn_recv(pMC->newconn, &buf)) == ERR_OK)
		{
			/* 拷贝以太网接收缓冲区的内容 */
			do{
				netbuf_data(buf, &databuf, &len);
			}while (netbuf_next(buf) >= 0);

			uQueueWrite(&tUpperPcRxQueue, (uint8_t *)databuf, len); //写入环形队列

			tCommPro.Tcp_Timeout_MS[CtrlNum] = 0; //清零
			tCommPro.Tcp_Timeout_S[CtrlNum] = 0; //清零

			/* 清理netbuf空间，等待下一次数据连接*/
			netbuf_delete(buf);
		}

		/* 未连接 */
		if(err == ERR_CONN)
		{
			tcp_shutdown(pMC->newconn->pcb.tcp, 1, 1); //断开TCP连接
			break;
		}

		/* 5秒保活时间 */
		if(tCommPro.Tcp_Timeout_S[CtrlNum] > 5)
		{
			tcp_shutdown(pMC->newconn->pcb.tcp, 1, 1); //断开TCP连接
			break;
		}
	}

	tCommPro.Tcp_Comm_Flag[CtrlNum] = 0;
	tCommPro.Tcp_Timeout_MS[CtrlNum] = 0; //清零
	tCommPro.Tcp_Timeout_S[CtrlNum] = 0; //清零

#else

	pMC->newconn->recv_timeout = 10000; //以太网接收不永久阻塞，只阻塞10s

	/* 等待数据接收完成 */
	while (netconn_recv(pMC->newconn, &buf) == ERR_OK)
	{
		/* 拷贝以太网接收缓冲区的内容 */
		do{
			netbuf_data(buf, &databuf, &len);
//			netconn_write(pMC->newconn, databuf, len, NETCONN_NOCOPY); //发送接收到的数据
		}while (netbuf_next(buf) >= 0);

//		DEBUG_PRINTF("收到以太网控制包长度：%d\n字节",len);

		uQueueWrite(&tUpperPcRxQueue, (uint8_t *)databuf, len); //写入环形队列

		/* 清理netbuf空间，等待下一次数据连接*/
		netbuf_delete(buf);
	}

#endif

	/* 断开TCP连接，删除netconn结构体，等待下一次握手*/
	netconn_close(pMC->newconn);
	netconn_delete(pMC->newconn);
	pMC->newconn =  NULL;
	pMC->tempHandle = NULL;
	uExistClient.Bit.exist3 = 0; //不存在
	vTaskDelete(pMC->tempHandle);
}

/*-----------------------------------------------------------------------------
 * @name  : TCP_CtrlThread
 * @brief : TCP线程
 * @param :
 * @retval:
 * @date  : 2021/10/18
 * @note  :
 * ---------------------------------------------------------------------------*/
static void TCP_CtrlThread(void *arg)
{
	struct netconn *conn, *newconn;
	err_t err = 0;

	uint8_t cnt = 0; //连接客户端计数

	LWIP_UNUSED_ARG(arg); //消除关于未使用参数的编译器警告

	conn = netconn_new(NETCONN_TCP);
	netconn_bind(conn, IP_ADDR_ANY, LOCAL_PORT);
	LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);

	DEBUG_PRINTF("TCP1: Local binding complete! \r\n");

	/* Tell connection to go into listening mode. */
	netconn_listen(conn);

	while (1)
	{
		/* 等待一个新的客户端前来连接，包括完成三次握手，若无连接则一直阻塞 */
		err = netconn_accept(conn, &newconn);

		if(TCP_QueryIsNull(&uExistClient, 1) == 0)
		{
			cnt = 1;
		}
		else if(TCP_QueryIsNull(&uExistClient, 2) == 0)
		{
			cnt = 2;
		}
		else if(TCP_QueryIsNull(&uExistClient, 3) == 0)
		{
			cnt = 3;
		}
		else
		{
			if(newconn != NULL)
			{
				/* 超过限制客户端次数则清除 */
				/* 断开TCP连接，删除netconn结构体，等待下一次握手*/
				netconn_close(newconn);
				netconn_delete(newconn);
				cnt = 0;
			}
		}

		/* 新的客户端连接成功 */
		if((err == ERR_OK) && (newconn != NULL))
		{
			taskENTER_CRITICAL(); //进入临界区

			if(cnt == 1)
			{
				if(tMoreClient[0].newconn == NULL)
				{
					uExistClient.Bit.exist1 = 1; //存在

					tMoreClient[0].newconn = TCP_Netconn = newconn;
					tMoreClient[0].tempHandle = TCP_CTRL_TASK_Handle_1;

					xTaskCreate((TaskFunction_t) TCP_TcpCtrl_Task_1,
								(const char*   ) "TCP_Task1",
								(uint16_t      ) 256,
								(void*         ) &tMoreClient[0].newconn,
								(UBaseType_t   ) 11,
								(TaskHandle_t* ) &tMoreClient[0].tempHandle);
				}
			}
			else if(cnt == 2)
			{
				if(tMoreClient[1].newconn == NULL)
				{
					uExistClient.Bit.exist2 = 1; //存在

					tMoreClient[1].newconn = TCP_Netconn = newconn;
					tMoreClient[1].tempHandle = TCP_CTRL_TASK_Handle_2;

					xTaskCreate((TaskFunction_t) TCP_TcpCtrl_Task_2,
								(const char*   ) "TCP_Task2",
								(uint16_t      ) 256,
								(void*         ) &tMoreClient[1].newconn,
								(UBaseType_t   ) 11,
								(TaskHandle_t* ) &tMoreClient[1].tempHandle);
				}
			}
			else if(cnt == 3)
			{
				if(tMoreClient[2].newconn == NULL)
				{
					uExistClient.Bit.exist3 = 1; //存在

					tMoreClient[2].newconn = TCP_Netconn = newconn;
					tMoreClient[2].tempHandle = TCP_CTRL_TASK_Handle_3;

					xTaskCreate((TaskFunction_t) TCP_TcpCtrl_Task_3,
								(const char*   ) "TCP_Task3",
								(uint16_t      ) 256,
								(void*         ) &tMoreClient[2].newconn,
								(UBaseType_t   ) 11,
								(TaskHandle_t* ) &tMoreClient[2].tempHandle);
				}
			}

		    taskEXIT_CRITICAL();	//退出临界区
		}
	}
}

/*-----------------------------------------------------------------------------
 * @name  : TCP_CtrlTask
 * @brief : 与上位机通信处理任务
 * @param : parameter 任意指针参数
 * @retval:
 * @date  : 2021/10/19
 * @note  :
 * ---------------------------------------------------------------------------*/
void TCP_CtrlTask(void *parameter)
{
	uint8_t cnt = 0;

	while(1)
	{
		if((tMoreClient[0].newconn == NULL) && (tMoreClient[1].newconn == NULL))
		{
			TCP_Netconn = NULL;
			tCommPro.Upper_PC_Comm_Flag = 0; //清零
		}

		/* 获取当前以太网接口连接状态 */
		HAL_ETH_ReadPHYRegister(&heth, LAN8742_PHY_ADDRESS, LAN8742_BSR, &tCommPro.tempReg);

		/* 连接断开 */
		if(((tCommPro.tempReg & LAN8742_BSR_AUTONEGO_CPLT) == 0) ||
		   ((tCommPro.tempReg & LAN8742_BSR_LINK_STATUS) == 0))     //网线被拔
		{
			if(cnt == 0)
			{
				cnt = 1;
				//TODO:掉线处理
			}
		}
		else
		{
			if(cnt != 0)
			{
				cnt = 0;
			}
		}

		vTaskDelay(50);
	}
}

/*-----------------------------------------------------------------------------
 * @name  : TCP_init
 * @brief : TCP线程创建
 * @param :
 * @retval: 0：成功  	其他：失败
 * @date  : 2021/10/18
 * @note  :
 * ---------------------------------------------------------------------------*/
void TCP_Init(void)
{
	TCP_TASK_Handle = sys_thread_new( "TCP_Ctrl_thread", TCP_CtrlThread, NULL, 1024, 10 );
}

/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
