
#ifdef USER_CONFIG
	#include "user_config.h"
#endif
#include "user_disk_manage.h"
#include "exfuns.h"

/* 目前文件中已经存储的数据长度 */    //TODO:目前前32bytes作为参数区
#define NUM_IN_FILE(v)			    ((((v)->rp) <= ((v)->wp))?(((v)->wp)-((v)->rp)):(((v)->sizeLimit)-((v)->rp)+((v)->wp))-sizeof(My_File_Parameter_T))
/* 获取文件剩余可存放数据个数 */
#define LFET_NUM_IN_FILE(v)			((((v)->rp) <= ((v)->wp))?((((v)->sizeLimit))-(((v)->wp)-((v)->rp))-sizeof(My_File_Parameter_T)):(((v)->rp)-((v)->wp)-1))
/* 获得两者最小值 */
#define MIN(a,b)                    (a<b ? a : b)


/* NAND FLASH磁盘管理句柄：初始化返回值、磁盘路径、文件系统对象、文件对象 */
Nand_Disk_T nandDiskHandle;


/**
  * @brief	获取文件最大长度
  * @param	f 文件信息句柄指针
  *
  * @return	uint32_t：获取文件最大长度  0xFFFFFFFF 获取错误
  * @remark
  */
uint32_t mf_sizeLimit_get(My_File_Format_T *f)
{
	uint32_t fsize = 0;
	if(f == NULL)
		return 0xFFFFFFFF;
	NAND_DISK_API_LOCK(f->lock);  //上锁
	fsize = f->sizeLimit;
	NAND_DISK_API_UNLOCK(f->lock);  //解锁
	return fsize;
}

/**
  * @brief	获取文件当前长度
  * @param	f 文件信息句柄指针
  *
  * @return	uint32_t：获取文件当前长度  0xFFFFFFFF 获取错误
  * @remark
  */
uint32_t mf_fileSize_get(My_File_Format_T *f)
{
	uint32_t fsize = 0;
	if(f == NULL)
		return 0xFFFFFFFF;
	NAND_DISK_API_LOCK(f->lock);  //上锁
	fsize = f->fileSize;
	NAND_DISK_API_UNLOCK(f->lock);  //解锁
	return fsize;
}

/**
  * @brief	获取文件剩余长度
  * @param	f 文件信息句柄指针
  *
  * @return	uint32_t：获取文件剩余长度  0xFFFFFFFF 获取错误
  * @remark
  */
uint32_t mf_remainSize_get(My_File_Format_T *f)
{
	uint32_t fsize = 0;
	if(f == NULL)
		return 0xFFFFFFFF;
	NAND_DISK_API_LOCK(f->lock);  //上锁
	fsize = f->remainSize;
	NAND_DISK_API_UNLOCK(f->lock);  //解锁
	return fsize;
}


/**
  * @brief	文件参数初始化：用于获取文件上次掉电前的状态，避免读写数据丢失
  * @param	f 文件信息句柄指针
  * @param	path 文件路径
  * @param	sizeLimit 文件最大长度，一般用于第一次初始化文件/更新文件大小时调用
  * @return	FRESULT
  * @remark
  */
FRESULT mf_format_get(My_File_Format_T *file, const TCHAR* path, uint32_t sizeLimit)
{
	FRESULT res = FR_INT_ERR;
	UINT br = 0, bw = 0;
	My_File_Parameter_T parameter;
	memset(&parameter, 0, sizeof(My_File_Parameter_T));  //注意内存初始化，否则可能导致变量值没有更新

	NAND_DISK_API_LOCK(file->lock);  //上锁
	res = f_open(&file->fileName, path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res == FR_OK)
	{
		f_rewind(&file->fileName); //文件读指针复位至参数区
		res = f_read (&file->fileName, &parameter, sizeof(My_File_Parameter_T), &br);
		if(res == FR_OK)
		{
			/* 检查是否已经完成文件参数区的存储 */
			if(parameter.sizeLimit == sizeLimit &&
			   parameter.sizeLimit == (parameter.remainSize + parameter.fileSize + sizeof(My_File_Parameter_T)))
			{
				file->fileSize = parameter.fileSize;
				file->sizeLimit = parameter.sizeLimit;
				file->remainSize = parameter.remainSize;
				file->wp = parameter.wp;
				file->rp = parameter.rp;
			}
			else
			{
				parameter.fileSize = file->fileSize = 0; //文件长度恢复默认
				parameter.sizeLimit = file->sizeLimit = sizeLimit;
				parameter.remainSize = file->remainSize = LFET_NUM_IN_FILE(file);
				parameter.wp = file->wp = 0 + sizeof(My_File_Parameter_T);  //复位:跳过文件参数区
				parameter.rp = file->rp = 0 + sizeof(My_File_Parameter_T);
				/* 将参数写入参数区 */
				f_rewind(&file->fileName); //文件写指针复位至参数区
				res = f_write (&file->fileName, &parameter, sizeof(My_File_Parameter_T), &bw);
			}
		}
		f_close(&file->fileName);
	}
	NAND_DISK_API_UNLOCK(file->lock);  //解锁
	return res;
}

/**
  * @brief	文件参数存储(掉电前需存储，否则文件的读写位置会丢失)
  * @param	f 文件信息句柄指针
  * @param	path 文件路径
  * @return	FRESULT
  * @remark
  */
FRESULT mf_format_save(My_File_Format_T *file,const TCHAR* path)
{
	FRESULT res = FR_INT_ERR;
	UINT bw = 0;
	My_File_Parameter_T parameter;
	/* 获取当前文件的参数 */
	parameter.fileSize = file->fileSize;
	parameter.sizeLimit = file->sizeLimit;
	parameter.remainSize = file->remainSize;
	parameter.wp = file->wp;
	parameter.rp = file->rp;

	NAND_DISK_API_LOCK(file->lock);  //上锁
	res = f_open(&file->fileName, path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res == FR_OK)
	{
		/* 将参数写入参数区 */
		f_rewind(&file->fileName); //文件写指针复位至参数区
		res = f_write (&file->fileName, &parameter, sizeof(My_File_Parameter_T), &bw);
		f_close(&file->fileName);
	}
	NAND_DISK_API_UNLOCK(file->lock);  //解锁
	return res;
}

/**
  * @brief	NAND FLASH文件系统初始化
  * @param	diskHandle NAND FLASH文件系统信息句柄指针
  * @return	FRESULT
  * @remark
  */
FRESULT mf_nand_init(Nand_Disk_T *diskHandle)
{
	FRESULT res = FR_INT_ERR;
	TCHAR* path = NULL;
	FIL *fp = NULL;
	DIR *dir = NULL;
	FILINFO *fileInfo = NULL;

	/* 为文件系统申请内存 */
#ifdef NAND_DISK_USE_OS
	diskHandle->fatbuf = (uint8_t*)pvPortMalloc(NAND_FATBUF_SIZE);
	NAND_DISK_API_CREATELOCK(diskHandle->nandFile.equipmentInfo.lock);
	NAND_DISK_API_CREATELOCK(diskHandle->nandFile.stepLoadInfo.lock);
	NAND_DISK_API_CREATELOCK(diskHandle->nandFile.losePowerInfo.lock);
	NAND_DISK_API_CREATELOCK(diskHandle->nandFile.uartScreenLog.lock);
	NAND_DISK_API_CREATELOCK(diskHandle->nandFile.histroyErrorInfo.lock);
	NAND_DISK_API_CREATELOCK(diskHandle->nandFile.historyRecordInfo.lock);
	/* 内存申请检查 */
	if(diskHandle->fatbuf == NULL ||
	   diskHandle->nandFile.equipmentInfo.lock == NULL ||
	   diskHandle->nandFile.stepLoadInfo.lock == NULL ||
	   diskHandle->nandFile.losePowerInfo.lock == NULL ||
	   diskHandle->nandFile.uartScreenLog.lock == NULL ||
	   diskHandle->nandFile.histroyErrorInfo.lock == NULL ||
	   diskHandle->nandFile.historyRecordInfo.lock == NULL)
		return FR_NOT_ENOUGH_CORE;
#else
	diskHandle = malloc(sizeof(Nand_Disk_T));
	diskHandle->fatbuf = (uint8_t*)malloc(NAND_FATBUF_SIZE);
	if(diskHandle->fatbuf == NULL || diskHandle == NULL)
		return FR_NOT_ENOUGH_CORE;
#endif

	/* 配置底层函数和驱动路径 */
	path = (TCHAR*)diskHandle->nandPath;
	diskHandle->retNAND = FATFS_LinkDriver(&NAND_Driver, path);  //设置各类底层接口

	/* 注册一个工作区域 */
	res = f_mount(&diskHandle->nandFatFS, path, 1);  //立即挂载 NAND FLASH
	if(res == FR_NO_FILESYSTEM)  //如果文件系统还没有初始化
	{
		/* 创建一个文件系统 */
		res = f_mkfs(path, FM_ANY, 0, diskHandle->fatbuf, _MAX_SS);	//格式化FLASH,2,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:NAND_DISK");		//设置Flash磁盘的名字为：NAND_DISK
			DEBUG_PRINTF("NAND Disk Format Finish \r\n");	//格式化完成
		}
		else
		{
			DEBUG_PRINTF("NAND Disk Format Error \r\n");	//格式化完成
		}
	}
	else if(res != FR_OK)
	{
		DEBUG_PRINTF("NAND Disk Work Area Init Error \r\n");
	}

	/* 创建文件夹 */
	res = f_mkdir("1:bte");  	   //创建主文件夹
	if(res != FR_OK && res != FR_EXIST) DEBUG_PRINTF("NAND Disk Work Area Init Error \r\n");
	res = f_mkdir("1:bte/mid");    //创建中位机文件夹
	if(res != FR_OK && res != FR_EXIST) DEBUG_PRINTF("NAND Disk Work Area Init Error \r\n");

	/* 创建/打开文件族 */
	fp = &diskHandle->nandFile.equipmentInfo.fileName;  	  // 创建设备信息文件
	res = f_open(fp, "1:bte/mid/EquipmentInfo.text", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res != FR_OK)
		DEBUG_PRINTF("EquipmentInfo.text Open Error \r\n");
	else
	{
		res = f_close(fp);  //创建完成后要关闭,每次读写文件再打开,FATFS仅支持同时打开6个
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	}

	fp = &diskHandle->nandFile.stepLoadInfo.fileName;       //创建工步加载信息文件
	res = f_open(fp, "1:bte/mid/StepLoadInfo.text", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res != FR_OK)
		DEBUG_PRINTF("StepLoadInfo.text Open Error \r\n");
	else
	{
		res = f_close(fp);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	}

	fp = &diskHandle->nandFile.losePowerInfo.fileName;      //创建掉电接续文件
	res = f_open(fp, "1:bte/mid/LosePowerInfo.text", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res != FR_OK)
		DEBUG_PRINTF("LosePowerInfo.text Open Error \r\n");
	else
	{
		res = f_close(fp);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	}

	fp = &diskHandle->nandFile.uartScreenLog.fileName;      //创建串口屏日志文件
	res = f_open(fp, "1:bte/mid/UartScreenLog.text", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res != FR_OK)
		DEBUG_PRINTF("UartScreenLog.text Open Error \r\n");
	else
	{
		res = f_close(fp);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	}

	fp = &diskHandle->nandFile.histroyErrorInfo.fileName;   //创建历史错误日志文件
	res = f_open(fp, "1:bte/mid/HistroyErrorInfo.text", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res != FR_OK)
		DEBUG_PRINTF("HistroyErrorInfo.text Open Error \r\n");
	else
	{
		res = f_close(fp);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	}

	fp = &diskHandle->nandFile.historyRecordInfo.fileName;  //创建历史记录信息文件
	res = f_open(fp, "1:bte/mid/HistoryRecordInfo.text", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res != FR_OK)
		DEBUG_PRINTF("HistoryRecordInfo.text Open Error \r\n");
	else
	{
		res = f_close(fp);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	}

	/* 打开文件夹并创建一个目录对象 */
	dir = &diskHandle->nandDir;
	res = f_opendir (dir,"1:bte/mid");
	if(res != FR_OK)
		DEBUG_PRINTF("Opendir bte/mid Error \r\n");

	/* 测试:打印文件夹中的文件名字 */
#if 1
	fileInfo = &diskHandle->fileInfo;
    for (;;)
    {
    	/* Read a directory item */
        res = f_readdir(dir, fileInfo);
        /* Break on error or end of dir */
        if (res != FR_OK || fileInfo->fname[0] == 0)
        	break;
        /* It is a directory */
        if (fileInfo->fattrib & AM_DIR)
        {
            DEBUG_PRINTF("/%s", fileInfo->fname);
            if (res != FR_OK) break;
        }
        else
        {
        	/* It is a file. */
        	DEBUG_PRINTF("%s/%s\n", path, fileInfo->fname);
        }
    }
#endif

    /* 获取文件参数区的数据并进行参数句柄初始化 */
    res = mf_format_get(&diskHandle->nandFile.equipmentInfo,
    		            "1:bte/mid/EquipmentInfo.text",FSIZE_EQUIPMENTINFO);
	if(res != FR_OK) DEBUG_PRINTF("equipmentInfo format get Error \r\n");

    res = mf_format_get(&diskHandle->nandFile.stepLoadInfo,
    		            "1:bte/mid/StepLoadInfo.text",FSIZE_STEPLOADINFO);
	if(res != FR_OK) DEBUG_PRINTF("stepLoadInfo format get Error \r\n");

    res = mf_format_get(&diskHandle->nandFile.losePowerInfo,
    		            "1:bte/mid/LosePowerInfo.text",FSIZE_LOSEPOWERINFO);
	if(res != FR_OK) DEBUG_PRINTF("losePowerInfo format get Error \r\n");

    res = mf_format_get(&diskHandle->nandFile.uartScreenLog,
    		            "1:bte/mid/UartScreenLog.text",FSIZE_UARTSCREENLOG);
	if(res != FR_OK) DEBUG_PRINTF("uartScreenLog format get Error \r\n");

    res = mf_format_get(&diskHandle->nandFile.histroyErrorInfo,
    		            "1:bte/mid/HistroyErrorInfo.text",FSIZE_HISTROYERRORINFO);
	if(res != FR_OK) DEBUG_PRINTF("histroyErrorInfo format get Error \r\n");

    res = mf_format_get(&diskHandle->nandFile.historyRecordInfo,
    		            "1:bte/mid/HistoryRecordInfo.text",FSIZE_HISTORYRECORDINFO);
	if(res != FR_OK) DEBUG_PRINTF("historyRecordInfo format get Error \r\n");


    /* 关闭文件夹 */
#if 0
	res = f_closedir(dir);
	if(res != FR_OK)
		DEBUG_PRINTF("NAND Disk Dir Close Error \r\n");
#endif

	return res;
}

/**
  * @brief	NAND FLASH文件系统卸载
  * @param	diskHandle NAND FLASH文件系统信息句柄指针
  * @return	none
  * @remark
  */
void mf_nand_deinit(Nand_Disk_T *diskHandle)
{
	FRESULT res = FR_INT_ERR;
	/* 关闭并删除所有文件 */
	//关闭
	res = f_close(&diskHandle->nandFile.equipmentInfo.fileName);
	if(res != FR_OK && res != FR_INVALID_OBJECT) DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	res = f_close(&diskHandle->nandFile.stepLoadInfo.fileName);
	if(res != FR_OK && res != FR_INVALID_OBJECT) DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	res = f_close(&diskHandle->nandFile.losePowerInfo.fileName);
	if(res != FR_OK && res != FR_INVALID_OBJECT) DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	res = f_close(&diskHandle->nandFile.uartScreenLog.fileName);
	if(res != FR_OK && res != FR_INVALID_OBJECT) DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	res = f_close(&diskHandle->nandFile.histroyErrorInfo.fileName);
	if(res != FR_OK && res != FR_INVALID_OBJECT) DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	res = f_close(&diskHandle->nandFile.historyRecordInfo.fileName);
	if(res != FR_OK && res != FR_INVALID_OBJECT) DEBUG_PRINTF("NAND Disk File Close Error \r\n");
	//删除文件
	res = f_unlink("1:bte/mid/EquipmentInfo.text");
	if(res != FR_OK) DEBUG_PRINTF("NAND Disk File Remove Error \r\n");
	res = f_unlink("1:bte/mid/StepLoadInfo.text");
	if(res != FR_OK) DEBUG_PRINTF("NAND Disk File Remove Error \r\n");
	res = f_unlink("1:bte/mid/LosePowerInfo.text");
	if(res != FR_OK) DEBUG_PRINTF("NAND Disk File Remove Error \r\n");
	res = f_unlink("1:bte/mid/UartScreenLog.text");
	if(res != FR_OK) DEBUG_PRINTF("NAND Disk File Remove Error \r\n");
	res = f_unlink("1:bte/mid/HistroyErrorInfo.text");
	if(res != FR_OK) DEBUG_PRINTF("NAND Disk File Remove Error \r\n");
	res = f_unlink("1:bte/mid/HistoryRecordInfo.text");
	if(res != FR_OK) DEBUG_PRINTF("NAND Disk File Remove Error \r\n");

	/* 关闭并删除所有目录 */
	res = f_closedir(&diskHandle->nandDir);
	if(res != FR_OK) DEBUG_PRINTF("NAND Disk Dir Close Error \r\n");
	res = f_unlink("1:bte/mid");
	if(res != FR_OK) DEBUG_PRINTF("NAND Disk Dir Remove Error \r\n");
	res = f_unlink("1:bte");
	if(res != FR_OK) DEBUG_PRINTF("NAND Disk Dir Remove Error \r\n");

	/* 注销工作区 */
	res = f_mount(0, (TCHAR*)diskHandle->nandPath,1);
	if(res != FR_OK)
	{
		DEBUG_PRINTF("NAND Disk Work Area Unmount Error \r\n");
	}

	/* 释放内存 */
#ifdef NAND_DISK_USE_OS
	NAND_DISK_API_DELETELOCK(diskHandle->nandFile.equipmentInfo.lock);
	NAND_DISK_API_DELETELOCK(diskHandle->nandFile.stepLoadInfo.lock);
	NAND_DISK_API_DELETELOCK(diskHandle->nandFile.losePowerInfo.lock);
	NAND_DISK_API_DELETELOCK(diskHandle->nandFile.uartScreenLog.lock);
	NAND_DISK_API_DELETELOCK(diskHandle->nandFile.histroyErrorInfo.lock);
	NAND_DISK_API_DELETELOCK(diskHandle->nandFile.historyRecordInfo.lock);
	vPortFree(diskHandle->fatbuf);
#else
	vPortFree(diskHandle->fatbuf);
#endif

}

/**
  * @brief	NAND FLASH 写文件
  * @param	file 目标文件信息句柄指针
  * @param	path 文件路径
  * @param	buf 写入数据缓冲区
  * @param	btw 写入字节数
  * @param	bw 写入成功的字节数存储指针
  * @return	FRESULT
  * @remark
  */
FRESULT mf_write (My_File_Format_T* file,
		          const TCHAR* path,
				  const void* buf,
		          UINT btw,UINT* bw)
{
	FRESULT res = FR_OK;
	FIL *fp = NULL;
	/* 参数检查 */
	if(file == NULL || buf == NULL || btw == 0 || bw == NULL)
		return FR_INVALID_PARAMETER;
	fp = &file->fileName;

	/* 打开文件 */
	NAND_DISK_API_LOCK(file->lock);  //上锁
	res = f_open(fp, path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res == FR_OK || res == FR_LOCKED)  //防止文件已经打开
	{
		/* 如果写入文件的长度合法 */
		if(file->sizeLimit >= (btw + sizeof(My_File_Parameter_T)))
		{
			f_lseek(fp, 0 + sizeof(My_File_Parameter_T));  //设置文件写指针
			res = f_write(fp,buf,btw,bw);
			if(*bw != btw) goto ret;
			file->fileSize = f_size(fp)-sizeof(My_File_Parameter_T);  //更新文件长度
			file->remainSize = file->sizeLimit - f_size(fp);  //更新文件剩余长度
		}
		else
		{
			DEBUG_PRINTF("mf_write：write file failed\r\n");
			res = FR_DENIED; //文件存储空间不足
		}
	}
ret:
	res = f_close(fp);  //写入完成关闭文件
	NAND_DISK_API_UNLOCK(file->lock);  //解锁
	return res;
}


/**
  * @brief	NAND FLASH 读文件
  * @param	file 目标文件信息句柄指针
  * @param	path 文件路径
  * @param	buf 读出数据缓冲区
  * @param	btr 读出字节数
  * @param	br 读出成功的字节数存储指针
  * @return	FRESULT
  * @remark
  */
FRESULT mf_read (My_File_Format_T* file,
		         const TCHAR* path,
				 void* buf,
		         UINT btr,UINT* br)
{
	FRESULT res = FR_OK;
	FIL *fp = NULL;

	/* 参数检查 */
	if(file == NULL || buf == NULL || btr == 0 || br == NULL)
		return FR_INVALID_PARAMETER;
	fp = &file->fileName;

	/* 打开文件 */
	NAND_DISK_API_LOCK(file->lock);  //上锁
//	taskENTER_CRITICAL(); //进入临界区
	res = f_open(fp, path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res == FR_OK || res == FR_LOCKED)
	{
		/* 如果读出文件合法 */
		if(file->fileSize >= btr)
		{
			f_lseek(fp, 0 + sizeof(My_File_Parameter_T));  //设置文件读指针
			res = f_read (fp,buf,btr,br);
			if(btr != *br)
				res = FR_DISK_ERR;
		}
		else
		{
			DEBUG_PRINTF("mf_read：Read file failed\r\n");
			res = FR_DENIED; //文件存储数据不足以读出数量
		}
	}
	f_close(fp);  //读出完成关闭文件
//	taskEXIT_CRITICAL(); //退出临界区
	NAND_DISK_API_UNLOCK(file->lock);  //解锁
	return res;
}



/**
  * @brief	NAND FLASH 写文件(循环队列)
  * @param	file 目标文件信息句柄指针
  * @param	path 文件路径
  * @param	buf 写入数据缓冲区
  * @param	btw 写入字节数
  * @param	bw 写入成功的字节数存储指针
  * @return	FRESULT
  * @remark
  */
FRESULT mf_circular_write (My_File_Format_T* file,
		                   const TCHAR* path,
					       const void* buf,
		                   UINT btw,UINT* bw)
{
	FRESULT res = FR_OK;
	FIL *fp = NULL;
	UINT bwTemp;
	/* 参数检查 */
	if(file == NULL || buf == NULL || btw == 0 || bw == NULL)
		return FR_INVALID_PARAMETER;
	fp = &file->fileName;
	*bw = 0;  //避免传入的变量非空

	/* 打开文件 */
	NAND_DISK_API_LOCK(file->lock);  //上锁
	res = f_open(fp, path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res == FR_OK || res == FR_LOCKED)  //防止文件已经打开
	{
		size_t left_num_in_file = file->remainSize;  //获取文件剩余可存放数据个数
		if (btw <= left_num_in_file)
		{
			f_lseek(fp, file->wp);  //设置文件写指针
			size_t templen = MIN((file->sizeLimit - file->wp), btw);  //文件剩余存储长度与本次写入长度中的最小值
			/* 由于最小值队尾到队列结束的长度 */
			if (templen > 0)
			{
				res = f_write(fp,buf,templen,&bwTemp);
				if(templen != bwTemp) goto ret;
				*bw += bwTemp;
			}
			/* 如果本次入队长度大于队尾到队列结束的长度，说明需要回环入队 */
			if (btw > templen)
			{
				f_lseek(fp, 0 + sizeof(My_File_Parameter_T));  //TODO：文件写指针复位，跳过文件参数区
				res = f_write(fp, (buf + templen), (btw - templen), &bwTemp);
				if((btw - templen) != bwTemp) goto ret;
				*bw += bwTemp;
			}
			file->wp = f_tell(fp);  //更新文件写指针
			file->remainSize = LFET_NUM_IN_FILE(file);  //更新文件剩余长度
			file->fileSize = NUM_IN_FILE(file);  //更新文件长度
		}
		else
		{
			//TODO：后续需要考虑在此处增加压缩编码存储
			DEBUG_PRINTF("write file failed, num=%d,left=%d,rp=%d,wp=%d,fileSize=%d",
					     btw, left_num_in_file, file->rp, file->wp, file->fileSize);
			res = FR_DENIED; //文件存储空间不足
		}
	}
ret:
	res = f_close(fp);  //写入完成关闭文件
	NAND_DISK_API_UNLOCK(file->lock);  //解锁
	return res;
}


/**
  * @brief	NAND FLASH 读文件(循环队列)
  * @param	file 目标文件信息句柄指针
  * @param	path 文件路径
  * @param	buf 读出数据缓冲区
  * @param	btr 读出字节数
  * @param	br 读出成功的字节数存储指针
  * @return	FRESULT
  * @remark
  */
FRESULT mf_circular_read (My_File_Format_T* file,
		                  const TCHAR* path,
						  void* buf,
		                  UINT btr,UINT* br)
{
	FRESULT res = FR_OK;
	FIL *fp = NULL;
	UINT brTemp;

	/* 参数检查 */
	if(file == NULL || buf == NULL || btr == 0 || br == NULL)
		return FR_INVALID_PARAMETER;
	fp = &file->fileName;
	*br = 0;  //避免传入的变量非空

	/* 打开文件 */
	NAND_DISK_API_LOCK(file->lock);  //上锁
//	taskENTER_CRITICAL(); //进入临界区
	res = f_open(fp, path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if(res == FR_OK || res == FR_LOCKED)
	{
		size_t num_in_file = file->fileSize;    //目前文件中已缓存的数据长度
		if (btr <= num_in_file)
		{
			f_lseek(fp, file->rp);  //设置文件读指针
			/* 以下是按照队列是否循环分情况 */
			size_t templen = MIN((file->sizeLimit) - (file->rp), btr);
			if (templen > 0)
			{
				res = f_read (fp,buf,templen,&brTemp);
				if(templen != brTemp) goto ret;
				*br += brTemp;
			}

			if (btr > templen)
			{
				f_lseek(fp, 0 + sizeof(My_File_Parameter_T));  //TODO：文件读指针复位，跳过文件参数区
				res = f_read (fp, (buf + templen),(btr - templen), &brTemp);
				if((btr - templen) != brTemp) goto ret;
				*br += brTemp;
			}
			file->rp = f_tell(fp);  //更新文件读指针
			file->remainSize = LFET_NUM_IN_FILE(file);  //更新文件剩余长度
			file->fileSize = NUM_IN_FILE(file);  //更新文件长度
		}
		else
		{
			DEBUG_PRINTF("Read file failed, num=%d,num_in_file=%d,rp=%d,wp=%d,fileSize=%d",
					     btr, num_in_file, file->rp, file->wp, file->fileSize);
			res = FR_DENIED; //文件存储数据不足以读出
		}
	}
ret:
	f_close(fp);  //读出完成关闭文件
//	taskEXIT_CRITICAL(); //退出临界区
	NAND_DISK_API_UNLOCK(file->lock);  //解锁
	return res;
}



/**
  * @brief	NAND FLASH 写文件（应用层接口）
  * @param	fileName 目标文件名称（枚举）
  * @param	buf 写入数据缓冲区
  * @param	size 写入字节数
  * @param	wsize 写入成功的字节数存储指针
  * @return	FRESULT
  * @remark
  */
FRESULT NAND_Write_File(BTE_File_Name_E fileName,
		                const void* buf,
						uint32_t size,
						uint32_t *wsize)
{
	My_File_Format_T* fp = NULL;
	FRESULT res = FR_OK;
	if(nandDiskHandle.fatbuf == NULL)  return FR_NOT_READY;

	/* 根据文件名称不同写入不同的文件 */
	switch(fileName)
	{
	case NAND_EQUIPMENTINFO:
		/* 将文件写入最新的写指针处 */
		fp = &nandDiskHandle.nandFile.equipmentInfo;
		res = mf_write(fp,"1:bte/mid/EquipmentInfo.text",buf,(UINT)size,(UINT*)wsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Write EquipmentInfo Error \r\n");
		mf_format_save(fp, "1:bte/mid/EquipmentInfo.text");  //每次写入信息需要更新参数区
		break;

	case NAND_STEPLOADINFO:
		fp = &nandDiskHandle.nandFile.stepLoadInfo;
		res = mf_write(fp,"1:bte/mid/StepLoadInfo.text", buf, (UINT)size,(UINT*)wsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Write StepLoadInfo Error \r\n");
		mf_format_save(fp, "1:bte/mid/StepLoadInfo.text");  //每次写入信息需要更新参数区
		break;

	case NAND_LOSEPOWERINFO:
		fp = &nandDiskHandle.nandFile.losePowerInfo;
		res = mf_write(fp,"1:bte/mid/LosePowerInfo.text", buf, (UINT)size,(UINT*)wsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Write LosePowerInfo Error \r\n");
		mf_format_save(fp, "1:bte/mid/LosePowerInfo.text");  //每次写入信息需要更新参数区
		break;

	case NAND_UARTSCREENLOG:
		fp = &nandDiskHandle.nandFile.uartScreenLog;
		res = mf_write(fp,"1:bte/mid/UartScreenLog.text", buf, (UINT)size,(UINT*)wsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Write UartScreenLog Error \r\n");
		mf_format_save(fp, "1:bte/mid/UartScreenLog.text");  //每次写入信息需要更新参数区
		break;

	case NAND_HISTORYERRORINFO:
		fp = &nandDiskHandle.nandFile.histroyErrorInfo;
		res = mf_write(fp,"1:bte/mid/HistroyErrorInfo.text", buf, (UINT)size,(UINT*)wsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Write HistroyErrorInfo Error \r\n");
		mf_format_save(fp, "1:bte/mid/HistroyErrorInfo.text");  //每次写入信息需要更新参数区
		break;

	case NAND_HISTORYRECORDINFO:
		fp = &nandDiskHandle.nandFile.historyRecordInfo;
		/* 记录数据量较大，需要使用循环队列式读写 */
		res = mf_circular_write(fp,"1:bte/mid/HistoryRecordInfo.text", buf, (UINT)size,(UINT*)wsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Write HistoryRecordInfo Error \r\n");
		mf_format_save(fp, "1:bte/mid/HistoryRecordInfo.text");  //每次写入信息需要更新参数区
		break;

	default:
		DEBUG_PRINTF("NAND Disk Write No File \r\n");
		return FR_NO_FILE;
	}
	return res;
}



/**
  * @brief	NAND FLASH 读文件（应用层接口）
  * @param	fileName 目标文件名称（枚举）
  * @param	buf 读出数据缓冲区
  * @param	size 读出字节数
  * @param	rsize 读出成功的字节数存储指针
  * @return	FRESULT
  * @remark
  */
FRESULT NAND_Read_File(BTE_File_Name_E fileName,
		               void* buf,
					   uint32_t size,
					   uint32_t *rsize)
{
	My_File_Format_T* fp = NULL;
	FRESULT res = FR_OK;
	if(nandDiskHandle.fatbuf == NULL)  return FR_NOT_READY;

	/* 根据文件名称不同读出不同的文件 */
	switch(fileName)
	{
	case NAND_EQUIPMENTINFO:
		fp = &nandDiskHandle.nandFile.equipmentInfo;
		res = mf_read(fp,"1:bte/mid/EquipmentInfo.text",buf,(UINT)size,(UINT*)rsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Read EquipmentInfo Error \r\n");
		break;

	case NAND_STEPLOADINFO:
		fp = &nandDiskHandle.nandFile.stepLoadInfo;
		res = mf_read(fp,"1:bte/mid/StepLoadInfo.text", buf, (UINT)size,(UINT*)rsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Read StepLoadInfo Error \r\n");
		break;

	case NAND_LOSEPOWERINFO:
		fp = &nandDiskHandle.nandFile.losePowerInfo;
		res = mf_read(fp,"1:bte/mid/LosePowerInfo.text", buf, (UINT)size,(UINT*)rsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Read LosePowerInfo Error \r\n");
		break;

	case NAND_UARTSCREENLOG:
		fp = &nandDiskHandle.nandFile.uartScreenLog;
		res = mf_read(fp,"1:bte/mid/UartScreenLog.text", buf, (UINT)size,(UINT*)rsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Read UartScreenLog Error \r\n");
		break;

	case NAND_HISTORYERRORINFO:
		fp = &nandDiskHandle.nandFile.histroyErrorInfo;
		res = mf_read(fp,"1:bte/mid/HistroyErrorInfo.text", buf, (UINT)size,(UINT*)rsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Read HistroyErrorInfo Error \r\n");
		break;

	case NAND_HISTORYRECORDINFO:
		fp = &nandDiskHandle.nandFile.historyRecordInfo;
		/* 记录数据量较大，需要使用循环队列式读写 */
		res = mf_circular_read(fp,"1:bte/mid/HistoryRecordInfo.text", buf, (UINT)size,(UINT*)rsize);
		if(res != FR_OK)
			DEBUG_PRINTF("NAND Disk Read HistoryRecordInfo Error \r\n");
		break;

	default:
		DEBUG_PRINTF("NAND Disk Read No File \r\n");
		return FR_NO_FILE;
	}
	return res;
}


