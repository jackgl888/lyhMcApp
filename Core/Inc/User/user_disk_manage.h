
#ifndef DISK_MANAGE_H
#define DISK_MANAGE_H

#include "stm32h7xx_hal.h"
#include "sys.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "nand_diskio.h"

/* NAND FLASH 文件系统结构 */
#define NAND_FATBUF_SIZE     512   //NAND FLASH 文件系统缓存
#define NAND_PATH_SIZE       100   //逻辑驱动器路径名称长度(注意路径长度不能大于该值)


/* TODO:以下信息待修改 */
#define FSIZE_EQUIPMENTINFO        (1*1024*1024)     //1M
#define FSIZE_STEPLOADINFO         (1*1024*1024)     //1M
#define FSIZE_LOSEPOWERINFO        (1*1024*1024)     //1M
#define FSIZE_UARTSCREENLOG        (1*1024*1024)     //1M
#define FSIZE_HISTROYERRORINFO     (1*1024*1024)     //1M
#define FSIZE_HISTORYRECORDINFO    (100*1024*1024)   //100M


typedef enum
{
	NAND_EQUIPMENTINFO  = 0,    //设备信息
	NAND_STEPLOADINFO,          //工步加载信息(用于断电接续)
	NAND_LOSEPOWERINFO,         //化成流程掉电接续
	NAND_UARTSCREENLOG,         //串口屏日志信息
	NAND_HISTORYERRORINFO,      //历史错误信息
	NAND_HISTORYRECORDINFO,     //历史记录信息
}BTE_File_Name_E;


/* 文件参数信息,会存在文件头中 */
typedef struct
{
	uint32_t sizeLimit;          //文件最大长度
	uint32_t fileSize;           //当前文件大小
	uint32_t remainSize;         //剩余文件大小
	FSIZE_t wp;                  //写指针
	FSIZE_t rp;                  //读指针
}My_File_Parameter_T;


typedef struct
{
	FIL fileName;
	uint32_t sizeLimit;          //文件最大长度(包含文件参数信息)
	uint32_t fileSize;           //当前文件大小
	uint32_t remainSize;         //剩余文件大小
	FSIZE_t wp;                  //写指针
	FSIZE_t rp;                  //读指针
#ifdef NAND_DISK_USE_OS
	SemaphoreHandle_t lock;      //保护锁
#endif
}My_File_Format_T;


typedef struct
{
	My_File_Format_T equipmentInfo;          //设备信息
	My_File_Format_T stepLoadInfo;           //工步加载信息(用于断电接续)
	My_File_Format_T losePowerInfo;          //化成流程掉电接续
	My_File_Format_T uartScreenLog;          //串口屏日志信息
	My_File_Format_T histroyErrorInfo;       //历史错误信息
	My_File_Format_T historyRecordInfo;      //历史记录信息
}Nand_File_Family_T;


typedef struct
{
	uint8_t retNAND;              //初始化返回值
	FATFS nandFatFS;   	          //逻辑驱动器的文件系统对象
	volatile char nandPath[NAND_PATH_SIZE];    //逻辑驱动器路径
	Nand_File_Family_T nandFile;  //文件对象组
	DIR nandDir;                  //目录对象
	FILINFO fileInfo;	          //用于获取文件信息
	uint8_t *fatbuf;	          //数据缓存区
}Nand_Disk_T;


typedef Nand_Disk_T * Nand_Disk_Handle_T;

extern Nand_Disk_T nandDiskHandle;



uint32_t mf_sizeLimit_get(My_File_Format_T *f);
uint32_t mf_fileSize_get(My_File_Format_T *f);
uint32_t mf_remainSize_get(My_File_Format_T *f);
FRESULT mf_format_get(My_File_Format_T *file, const TCHAR* path, uint32_t sizeLimit);
FRESULT mf_format_save(My_File_Format_T *file,const TCHAR* path);
FRESULT mf_nand_init(Nand_Disk_T *diskHandle);
void mf_nand_deinit(Nand_Disk_T *diskHandle);
/* 以下文件读写接口供应用层使用 */
FRESULT NAND_Write_File(BTE_File_Name_E fileName,const void* buf,uint32_t size,uint32_t *wsize);
FRESULT NAND_Read_File(BTE_File_Name_E fileName,void* buf,uint32_t size,uint32_t *rsize);


#endif //DISK_MANAGE_H
