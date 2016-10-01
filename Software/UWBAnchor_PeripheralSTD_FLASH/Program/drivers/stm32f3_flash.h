/* #include "stm32f3_flash.h" */

#ifndef __STM32F3_FLASH_H
#define __STM32F3_FLASH_H

#include "stm32f30x.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define FLASH_START_ADDR      ((uint32_t)0x08000000)
#define FLASH_PAGE_SIZE       ((uint32_t)0x00000800)    /* 2 KBytes */
#define FLASH_PAGE_ADDR(PAGE) ((uint32_t)(FLASH_START_ADDR + FLASH_PAGE_SIZE * (PAGE)))
/*=====================================================================================================*/
/*=====================================================================================================*/
int8_t  Flash_WritePageU8( uint32_t writeAddr, const uint8_t *writeData, uint32_t lens );
int8_t  Flash_WritePageU16( uint32_t writeAddr, const uint16_t *writeData, uint32_t lens );
int8_t  Flash_WritePageU32( uint32_t writeAddr, const uint32_t *writeData, uint32_t lens );
//int8_t  Flash_WritePageU64( uint32_t writeAddr, const uint64_t *writeData, uint32_t lens );
void    Flash_ReadPageU8( uint32_t readAddr, uint8_t *readData, uint32_t lens );
void    Flash_ReadPageU16( uint32_t readAddr, uint16_t *readData, uint32_t lens );
void    Flash_ReadPageU32( uint32_t readAddr, uint32_t *readData, uint32_t lens );
//void    Flash_ReadPageU64( uint32_t readAddr, uint64_t *readData, uint32_t lens );
int8_t  Flash_ErasePage( uint32_t erasePage );
int8_t  Flash_ErasePages( uint32_t erasePageStart, uint32_t nbPages );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif	 
