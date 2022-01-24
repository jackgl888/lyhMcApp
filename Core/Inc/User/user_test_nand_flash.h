/*
 * user_nand_flash.h
 *
 *  Created on: 2021年9月3日
 *      Author: Administrator
 */

#ifndef INC_USER_USER_TEST_NAND_FLASH_H_
#define INC_USER_USER_TEST_NAND_FLASH_H_

#include "main.h"

uint8_t test_writepage(uint32_t pagenum,uint16_t colnum,uint16_t writebytes, uint8_t CNT);
uint8_t test_readpage(uint32_t pagenum,uint16_t colnum,uint16_t readbytes);
uint8_t test_copypageandwrite(uint32_t spnum,uint32_t dpnum,uint16_t colnum,uint16_t writebytes);
uint8_t test_readspare(uint32_t pagenum,uint16_t colnum,uint16_t readbytes);
void test_readallblockinfo(uint32_t sblock);
uint8_t test_ftlwritesectors(uint32_t secx,uint16_t secsize,uint16_t seccnt);
uint8_t test_ftlreadsectors(uint32_t secx,uint16_t secsize,uint16_t seccnt);


#endif /* INC_USER_USER_TEST_NAND_FLASH_H_ */
