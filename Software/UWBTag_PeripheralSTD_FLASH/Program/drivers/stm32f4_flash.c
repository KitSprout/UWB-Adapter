/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_flash.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_WritePageU8
**功能 : Write Flash uint8_t
**輸入 : writeAddr, *writeData, lens
**輸出 : None
**使用 : Flash_WritePageU8(FLASH_PAGE_ADDR(PAGE), WriteData, 1024);
**=====================================================================================================*/
/*=====================================================================================================*/
int8_t Flash_WritePageU8( uint32_t writeAddr, const uint8_t *writeData, uint32_t lens )
{
  int8_t status = ERROR;
  uint32_t count = 0;

  FLASH_Unlock();
  do {
    status = FLASH_ProgramByte(writeAddr + count, writeData[count]);
  } while((++count < lens) && (status == FLASH_COMPLETE));
  FLASH_Lock();

  return status;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_WritePageU16
**功能 : Write Flash uint16_t
**輸入 : writeAddr, *writeData, lens
**輸出 : None
**使用 : Flash_WritePageU16(FLASH_PAGE_ADDR(PAGE), writeData, 512);
**=====================================================================================================*/
/*=====================================================================================================*/
int8_t Flash_WritePageU16( uint32_t writeAddr, const uint16_t *writeData, uint32_t lens )
{
  int8_t status = ERROR;
  uint32_t count = 0;

  FLASH_Unlock();
  do {
    status = FLASH_ProgramHalfWord(writeAddr + (count << 1), writeData[count]);
  } while((++count < lens) && (status == FLASH_COMPLETE));
  FLASH_Lock();

  return status;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_WritePageU32
**功能 : Write Flash uint32_t
**輸入 : writeAddr, *writeData, lens
**輸出 : None
**使用 : Flash_WritePageU32(FLASH_PAGE_ADDR(PAGE), writeData, 256);
**=====================================================================================================*/
/*=====================================================================================================*/
int8_t Flash_WritePageU32( uint32_t writeAddr, const uint32_t *writeData, uint32_t lens )
{
  int8_t status = ERROR;
  uint32_t count = 0;

  FLASH_Unlock();
  do {
    status = FLASH_ProgramWord(writeAddr + (count << 2), writeData[count]);
  } while((++count < lens) && (status == FLASH_COMPLETE));
  FLASH_Lock();

  return status;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_WritePageU64
**功能 : Write Flash uint64_t
**輸入 : writeAddr, *writeData, lens
**輸出 : None
**使用 : Flash_WritePageU64(FLASH_PAGE_ADDR(PAGE), writeData, 128);
**=====================================================================================================*/
/*=====================================================================================================*/
//int8_t Flash_WritePageU64( uint32_t writeAddr, const uint64_t *writeData, uint32_t lens )
//{
//  int8_t status = ERROR;
//  uint32_t count = 0;
//  uint64_t writeBuf = 0;

//  FLASH_Unlock();
//  do {
//    writeBuf = writeData[count]
//    status = FLASH_ProgramWord(writeAddr + (count << 3), writeData[count]);
//  } while((++count < lens) && (status == FLASH_COMPLETE));
//  FLASH_Lock();

//  return status;
//}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ReadPageU8
**功能 : Read Flash uint8_t
**輸入 : readAddr, *readData, lens
**輸出 : None
**使用 : Flash_ReadPageU8(FLASH_PAGE_ADDR(PAGE), readData, 1024);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_ReadPageU8( uint32_t readAddr, uint8_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint8_t)(*(__IO uint32_t*)(readAddr + count));
  } while(++count < lens);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ReadPageU16
**功能 : Read Flash uint16_t
**輸入 : readAddr, *readData, lens
**輸出 : None
**使用 : Flash_ReadPageU16(FLASH_PAGE_ADDR(PAGE), readData, 512);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_ReadPageU16( uint32_t readAddr, uint16_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint16_t)(*(__IO uint32_t*)(readAddr + (count << 1)));
  } while(++count < lens);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ReadPageU32
**功能 : Read Flash uint32_t
**輸入 : readAddr, *readData, lens
**輸出 : None
**使用 : Flash_ReadPageU32(FLASH_PAGE_ADDR(PAGE), readData, 256);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_ReadPageU32( uint32_t readAddr, uint32_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint32_t)(*(__IO uint32_t*)(readAddr + (count << 2)));
  } while(++count < lens);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ReadPageU64
**功能 : Read Flash uint64_t
**輸入 : readAddr, *readData, lens
**輸出 : None
**使用 : Flash_ReadPageU64(FLASH_PAGE_ADDR(PAGE), readData, 128);
**=====================================================================================================*/
/*=====================================================================================================*/
//void Flash_ReadPageU64( uint32_t readAddr, uint64_t *readData, uint32_t lens )
//{
//  uint32_t count = 0;

//  do {
//    readData[count] = (uint64_t)(*(__IO uint32_t*)(readAddr + (count << 3)));
//  } while(++count < lens);
//}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_GetSector
**功能 : Get Sector
**輸入 : address
**輸出 : sector
**使用 : sector = Flash_GetSector(FLASH_ADDR_SECTOR_3);
**=====================================================================================================*/
/*=====================================================================================================*/
uint32_t Flash_GetSector( uint32_t addr )
{
  if     ((addr >= FLASH_ADDR_SECTOR_0)  && (addr < FLASH_ADDR_SECTOR_1))     return FLASH_Sector_0;
  else if((addr >= FLASH_ADDR_SECTOR_1)  && (addr < FLASH_ADDR_SECTOR_2))     return FLASH_Sector_1;
  else if((addr >= FLASH_ADDR_SECTOR_2)  && (addr < FLASH_ADDR_SECTOR_3))     return FLASH_Sector_2;
  else if((addr >= FLASH_ADDR_SECTOR_3)  && (addr < FLASH_ADDR_SECTOR_4))     return FLASH_Sector_3;
  else if((addr >= FLASH_ADDR_SECTOR_4)  && (addr < FLASH_ADDR_SECTOR_5))     return FLASH_Sector_4;
  else if((addr >= FLASH_ADDR_SECTOR_5)  && (addr < FLASH_ADDR_SECTOR_6))     return FLASH_Sector_5;
  else if((addr >= FLASH_ADDR_SECTOR_6)  && (addr < FLASH_ADDR_SECTOR_7))     return FLASH_Sector_6;
  else if((addr >= FLASH_ADDR_SECTOR_7)  && (addr < FLASH_ADDR_SECTOR_8))     return FLASH_Sector_7;
  else if((addr >= FLASH_ADDR_SECTOR_8)  && (addr < FLASH_ADDR_SECTOR_9))     return FLASH_Sector_8;
  else if((addr >= FLASH_ADDR_SECTOR_9)  && (addr < FLASH_ADDR_SECTOR_10))    return FLASH_Sector_9;
  else if((addr >= FLASH_ADDR_SECTOR_10) && (addr < FLASH_ADDR_SECTOR_11))    return FLASH_Sector_10;
  else /*((addr >= FLASH_ADDR_SECTOR_7) && (addr < FLASH_ADDR_SECTOR_END)) */ return FLASH_Sector_11;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_EraseSector
**功能 : Erase Flash Memory
**輸入 : eraseSector
**輸出 : status
**使用 : Flash_EraseSector(FLASH_PAGE_ADDR(PAGE), 8);
**=====================================================================================================*/
/*=====================================================================================================*/
int8_t Flash_EraseSector( uint32_t eraseSector )
{
  int8_t status = ERROR;
  uint32_t sector = Flash_GetSector(eraseSector);

  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  status = FLASH_EraseSector(sector, VoltageRange_3);
  FLASH_Lock();

  return status;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_EraseSectors
**功能 : Erase Flash Memory
**輸入 : eraseSectorStart, nbSectors
**輸出 : status
**使用 : Flash_EraseSectors(FLASH_PAGE_ADDR(PAGE), 8);
**=====================================================================================================*/
/*=====================================================================================================*/
//int8_t Flash_EraseSectors( uint32_t eraseSectorStart, uint32_t nbSectors )
//{
//  int8_t status = ERROR;

//  FLASH_Unlock();
//  FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
//                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
//  while(nbSectors--) {
//    status = FLASH_EraseSector(eraseSectorStart, VoltageRange_3);
//    if(status!= FLASH_COMPLETE)
//      return status;
//    eraseSectorStart = eraseSectorStart + FLASH_PAGE_SIZE;
//  }
//  FLASH_Lock();

//  return status;
//}
/*=====================================================================================================*/
/*=====================================================================================================*/
