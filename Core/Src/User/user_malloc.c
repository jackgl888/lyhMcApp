/*
 * user_malloc.c
 *
 *  Created on: 2021年9月1日
 *      Author: Administrator
 */

#include "user_malloc.h"

//内存池(32字节对齐)
__attribute__ ((aligned(32))) uint8_t mem1base[MEM1_MAX_SIZE] __attribute__((section(".sdram_membase"))); //外部SDRAM内存池

//内存管理表
uint16_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE] __attribute__((section(".sdram_memmap"))); //外部SDRAM内存池MAP

//内存管理参数
const uint32_t memtblsize[SDRAMBANK] = { MEM1_ALLOC_TABLE_SIZE}; //内存表大小
const uint32_t memblksize[SDRAMBANK] = { MEM1_BLOCK_SIZE };      //内存分块大小
const uint32_t memsize[SDRAMBANK]    = { MEM1_MAX_SIZE};         //内存总大小


//内存管理控制器
struct _m_mallco_dev mallco_dev =
{
	my_mem_init,        //内存初始化
	my_mem_perused,     //内存使用率
	{mem1base},         //内存池
	{mem1mapbase},      //内存管理状态表
	{0},                 //内存管理未就绪
};


/*-----------------------------------------------------------------------------
 * @name  : mymemcpy
 * @brief : 复制内存
 * @param : *des:目的地址
 *          *src:源地址
 *          n:需要复制的内存长度(字节为单位)
 * @retval:
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
void mymemcpy(void *des, void *src, uint32_t n)
{
	uint8_t *xdes = des;
	uint8_t *xsrc = src;
	while(n--) *xdes++ = *xsrc++;
}

/*-----------------------------------------------------------------------------
 * @name  : mymemset
 * @brief : 设置内存
 * @param : *s:内存首地址
 *          c :要设置的值
 *          count:需要设置的内存大小(字节为单位)
 * @retval:
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
void mymemset(void *s, uint8_t c, uint32_t count)
{
	uint8_t *xs = s;
	while(count--) *xs++ = c;
//	for(uint32_t i = 0; i < count; i++)
//		xs[i] = c;
}

/*-----------------------------------------------------------------------------
 * @name  : my_mem_init
 * @brief : 内存管理初始化
 * @param : memx:所属内存块
 * @retval:
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
void my_mem_init(uint8_t memx)
{
	mymemset(mallco_dev.memmap[memx], 0, memtblsize[memx]*2); //内存状态表数据清零 每个成员16位，故需乘以2
	mymemset(mallco_dev.membase[memx], 0, memsize[memx]*1);	  //内存池所有数据清零
	mallco_dev.memrdy[memx] = 1;                              //内存管理初始化OK
}

/*-----------------------------------------------------------------------------
 * @name  : my_mem_perused
 * @brief : 获取内存使用率
 * @param : memx:所属内存块
 * @retval:
 * @date  : 2021/09/01
 * @note  : 返回值:使用率(0~100)
 * ---------------------------------------------------------------------------*/
uint8_t my_mem_perused(uint8_t memx)
{
	uint32_t used = 0;
	uint32_t i;

	for(i=0;i<memtblsize[memx];i++)  //根据对应的内存管理状态表的大小遍历状态表，不为0则说明状态表对应的内存块被占用
	{
		if(mallco_dev.memmap[memx][i])    //内存管理状态表不为0则表示内存块被使用
			used++;
	}

	return (used*100)/(memtblsize[memx]);
}

/*-----------------------------------------------------------------------------
 * @name  : my_mem_malloc
 * @brief : 内存分配(内部调用)
 * @param : memx:所属内存块
 *          size:要分配的内存大小(字节)
 * @retval: 0XFFFFFFFF,代表错误;其他,内存偏移地址
 * @date  : 2021/10/08
 * @note  :
 * ---------------------------------------------------------------------------*/
static uint32_t my_mem_malloc(uint8_t memx, uint32_t size)
{
    signed long offset=0;
    uint32_t nmemb;	//需要的内存块数
    uint32_t cmemb=0;//连续空内存块数
    uint32_t i;

    /* 如果对应的内存池未初始化,先执行初始化 */
    if(!mallco_dev.memrdy[memx])
        mallco_dev.init(memx);

    /* 如果申请的长度为0说明不需要分配 */
    if(size==0)
        return 0XFFFFFFFF;

    /* 计算即将申请的内存需要占用对应内存池中的多少个连续内存块 */
    nmemb=size/memblksize[memx];

    /* 内存块个数向上取整数 */
    if(size%memblksize[memx])
        nmemb++;

    /* 内存池从顶向下（从后到前检索连续可用的内存块个数：由内存管理状态表的值（不为0则被占用）决定） */
    for(offset=memtblsize[memx]-1;offset>=0;offset--) //offset表示内存块在内存池中的编号
    {
		if(!mallco_dev.memmap[memx][offset])
            cmemb++;  //内存管理状态表的值为0则说明连续空内存块数增加
		else
            cmemb=0;  //连续内存块清零（连续长度低于申请的长度则跳过开始下一个连续区域检索 TODO：会产生内存碎片！！）

        /* 找到了连续nmemb个空内存块 */
		if(cmemb==nmemb)
		{
            for(i=0;i<nmemb;i++)  	//将内存块对应的内存管理状态表的值赋值为申请的内存长度，即标注内存块非空
            {
                mallco_dev.memmap[memx][offset+i]=nmemb;
            }
            return (offset*memblksize[memx]);//内存块编号乘以内存块的大小则为真正返回内存的偏移地址
		}
    }
    return 0XFFFFFFFF;//未找到符合分配条件的内存块

}

/*-----------------------------------------------------------------------------
 * @name  : my_mem_free
 * @brief : 释放内存(内部调用)
 * @param : memx:所属内存块
 *          offset:内存地址偏移
 * @retval: 0,释放成功;1,释放失败
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
static uint8_t my_mem_free(uint8_t memx, uint32_t offset)
{
	int i = 0;

	if(!mallco_dev.memrdy[memx]) //未初始化,先执行初始化
	{
	mallco_dev.init(memx);
			return 1; //未初始化
	}

	if(offset < memsize[memx]) //偏移在内存池内.
	{
		int index = offset/memblksize[memx];        //偏移所在内存块号码
		int nmemb = mallco_dev.memmap[memx][index]; //内存块数量
		for(i=0; i<nmemb; i++)                      //内存块清零
		{
			mallco_dev.memmap[memx][index+i]=0;     //将内存块对应的内存管理状态表的值清空，即为空块
		}
		return 0;
	}
	else
		return 2; //偏移超区了.
}

/*-----------------------------------------------------------------------------
 * @name  : myfree
 * @brief : 释放内存(外部调用)
 * @param : memx:所属内存块
 *          ptr:内存首地址
 * @retval:
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
void myfree(uint8_t memx, void *ptr)
{
	uint32_t offset = 0;

	if(ptr == NULL) return; //地址为0.
	offset = (uint32_t)ptr-(uint32_t)mallco_dev.membase[memx];
	my_mem_free(memx, offset); //释放内存
}

/*-----------------------------------------------------------------------------
 * @name  : *mymalloc
 * @brief : 分配内存(外部调用)
 * @param : memx:所属内存块
 *          size:内存大小(字节)
 * @retval: 分配到的内存首地址
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
void *mymalloc(uint8_t memx, uint32_t size)
{
	uint32_t offset = 0;

	offset = my_mem_malloc(memx, size);

	if(offset==0XFFFFFFFF) return NULL;
	else return (void*)((uint32_t)mallco_dev.membase[memx]+offset);
}

/*-----------------------------------------------------------------------------
 * @name  : *myrealloc
 * @brief : 重新分配内存(外部调用)
 * @param : memx:所属内存块
 *          *ptr:旧内存首地址
 *          size:要分配的内存大小(字节)
 * @retval: 新分配到的内存首地址.
 * @date  : 2021/09/01
 * @note  :
 * ---------------------------------------------------------------------------*/
void *myrealloc(uint8_t memx, void *ptr, uint32_t size)
{
	uint32_t offset = 0;

	offset = my_mem_malloc(memx,size);
	if(offset==0XFFFFFFFF) return NULL;
	else
	{
		mymemcpy((void*)((uint32_t)mallco_dev.membase[memx]+offset),ptr,size);  //拷贝旧内存内容到新内存
		myfree(memx,ptr);                                                       //释放旧内存
		return (void*)((uint32_t)mallco_dev.membase[memx]+offset);              //返回新内存首地址
	}
}

