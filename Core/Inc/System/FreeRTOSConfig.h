/* USER CODE BEGIN Header */
/*
 * FreeRTOS Kernel V10.3.1
 * Portion Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Portion Copyright (C) 2019 StMicroelectronics, Inc.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */
/* USER CODE END Header */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * These parameters and more are described within the 'configuration' section of the
 * FreeRTOS API documentation available on the FreeRTOS.org web site.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* USER CODE BEGIN Includes */
/* Section where include file can be added */
#include "stm32h7xx.h"

/* USER CODE END Includes */

/* Ensure definitions are only used by the compiler, and not by the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
  #include <stdint.h>
  extern uint32_t SystemCoreClock;
#endif
#define configENABLE_FPU                         0
#define configENABLE_MPU                         0

#define configUSE_PREEMPTION                     1
//支持静态内存
#define configSUPPORT_STATIC_ALLOCATION          0
//支持动态内存申请
#define configSUPPORT_DYNAMIC_ALLOCATION         1


/***************************************************************
FreeRTOS与钩子函数有关的配置选项
**************************************************************/
/* 置1：使用空闲钩子（Idle Hook类似于回调函数）；置0：忽略空闲钩子
*
* 空闲任务钩子是一个函数，这个函数由用户来实现，
* FreeRTOS规定了函数的名字和参数：void vApplicationIdleHook(void )，
* 这个函数在每个空闲任务周期都会被调用
* 对于已经删除的RTOS任务，空闲任务可以释放分配给它们的堆栈内存。
* 因此必须保证空闲任务可以被CPU执行
* 使用空闲钩子函数设置CPU进入省电模式是很常见的
* 不可以调用会引起空闲任务阻塞的API函数
*/
#define configUSE_IDLE_HOOK                      0

/* 置1：使用时间片钩子（Tick Hook）；置0：忽略时间片钩子
*
*
* 时间片钩子是一个函数，这个函数由用户来实现，
* FreeRTOS规定了函数的名字和参数：void vApplicationTickHook(void )
* 时间片中断可以周期性的调用
* 函数必须非常短小，不能大量使用堆栈，
* 不能调用以”FromISR" 或 "FROM_ISR”结尾的API函数
*/
/*xTaskIncrementTick函数是在xPortSysTickHandler中断函数中被调用的。因此，vApplicationTickHook()函数执行的时间必须很短才行*/
#define configUSE_TICK_HOOK                      0

/*
* 写入实际的CPU内核时钟频率，也就是CPU指令执行频率，通常称为Fclk
* Fclk为供给CPU内核的时钟信号，我们所说的cpu主频为 XX MHz，
* 就是指的这个时钟信号，相应的，1/Fclk即为cpu时钟周期；
*/
#define configCPU_CLOCK_HZ                       ( SystemCoreClock )

//RTOS系统节拍中断的频率。即一秒中断的次数，每次中断RTOS都会进行任务调度
#define configTICK_RATE_HZ                       ((TickType_t)1000)

//系统最大优先级
#define configMAX_PRIORITIES                     ( 32 )

#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)
//系统所有总的堆大小
#define configTOTAL_HEAP_SIZE                    ((size_t)128*1024)

#define configMAX_TASK_NAME_LEN                  ( 16 )
#define configUSE_16_BIT_TICKS                   0
#define configQUEUE_REGISTRY_SIZE                8
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  1
//空闲任务放弃CPU使用权给其他同优先级的用户任务
#define configIDLE_SHOULD_YIELD                  1

//启用队列
#define configUSE_QUEUE_SETS                     1

//开启任务通知功能，默认开启
#define configUSE_TASK_NOTIFICATIONS             1

//使用互斥信号量
#define configUSE_MUTEXES                        1

//使用递归互斥信号量
#define configUSE_RECURSIVE_MUTEXES	             1

//为1时使用计数信号量
#define configUSE_COUNTING_SEMAPHORES            1

/* USER CODE BEGIN MESSAGE_BUFFER_LENGTH_TYPE */
/* Defaults to size_t for backward compatibility, but can be changed
   if lengths will always be less than the number of bytes in a size_t. */
#define configMESSAGE_BUFFER_LENGTH_TYPE         size_t
/* USER CODE END MESSAGE_BUFFER_LENGTH_TYPE */

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                    0
#define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )

/* The following flag must be enabled only when using newlib */
#define configUSE_NEWLIB_REENTRANT               1

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

/***********************************************************************
FreeRTOS与软件定时器有关的配置选项
**********************************************************************/
//启用软件定时器
#define configUSE_TIMERS				      1
//软件定时器优先级
#define configTIMER_TASK_PRIORITY		      (configMAX_PRIORITIES-1)
//软件定时器队列长度
#define configTIMER_QUEUE_LENGTH		      100
//软件定时器任务堆栈大小
#define configTIMER_TASK_STACK_DEPTH	      (configMINIMAL_STACK_SIZE*2)

/************************************************************
                  FreeRTOS可选函数配置选项
************************************************************/
#define INCLUDE_xTaskGetSchedulerState       		1
#define INCLUDE_vTaskPrioritySet		     		1
#define INCLUDE_uxTaskPriorityGet		     		1
#define INCLUDE_vTaskDelete				     		1
#define INCLUDE_vTaskCleanUpResources	     		1
#define INCLUDE_vTaskSuspend			     		1
#define INCLUDE_vTaskDelayUntil			    	 	1
#define INCLUDE_vTaskDelay				           	1
#define INCLUDE_eTaskGetState			           	1
#define INCLUDE_xTimerPendFunctionCall	     		1
#define INCLUDE_xTaskGetCurrentTaskHandle       	1
#define INCLUDE_uxTaskGetStackHighWaterMark     	0
#define INCLUDE_xTaskGetIdleTaskHandle          	0

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
 /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
 #define configPRIO_BITS         __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS         4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
/* USER CODE BEGIN 1 */
#define configASSERT( x ) if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );}
/* USER CODE END 1 */

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler

/* IMPORTANT: This define is commented when used with STM32Cube firmware, when the timebase source is SysTick,
              to prevent overwriting SysTick_Handler defined within STM32Cube HAL */

#define xPortSysTickHandler SysTick_Handler

/* USER CODE BEGIN Defines */
/* Section where parameter definitions can be added (for instance, to override default ones in FreeRTOS.h) */
/* USER CODE END Defines */

#endif /* FREERTOS_CONFIG_H */
