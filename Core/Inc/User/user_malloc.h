/*
 * user_malloc.h
 *
 *  Created on: 2021年9月1日
 *      Author: Administrator
 */

#ifndef INC_USER_USER_MALLOC_H_
#define INC_USER_USER_MALLOC_H_

#include "main.h"

//定义内存池
#define SDRAM_POOL	0      //外部内存池(SDRAM)，SDRAM共32MB

//上面定义的内存池数量
#define SDRAMBANK 	1      //定义支持的RAM块数.

//mem1内存参数设定.mem1的内存池处于外部SRAM里面:32x+4y=32*1024*1024 & x=y；则 x=932067
#define MEM1_BLOCK_SIZE			32          //内存块大小为32字节
#define MEM1_MAX_SIZE			29826144    //最大管理内存32M
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//内存表大小

//内存管理控制器
struct _m_mallco_dev
{
	void     (* init)    (uint8_t);	    //初始化
	uint8_t  (* perused) (uint8_t);	    //内存使用率
	uint8_t   * membase  [SDRAMBANK];   //内存池 管理SDRAMBANK个区域的内存
	uint16_t  * memmap   [SDRAMBANK];   //内存管理状态表
	uint8_t     memrdy   [SDRAMBANK];   //内存管理是否就绪
};
extern struct _m_mallco_dev mallco_dev;  //在mallco.c里面定义


//用户调用函数
void mymemset(void *s,uint8_t c,uint32_t count);        //设置内存
void mymemcpy(void *des,void *src,uint32_t n);          //复制内存
void my_mem_init(uint8_t memx);                         //内存管理初始化函数(外/内部调用)
uint8_t my_mem_perused(uint8_t memx);                   //获得内存使用率(外/内部调用)
void myfree(uint8_t memx,void *ptr);                    //内存释放(外部调用)
void *mymalloc(uint8_t memx,uint32_t size);             //内存分配(外部调用)
void *myrealloc(uint8_t memx,void *ptr,uint32_t size);  //重新分配内存(外部调用)



#endif /* INC_USER_USER_MALLOC_H_ */
