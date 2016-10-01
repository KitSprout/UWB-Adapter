/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f3_system.h"
#include "stm32f3_flash.h"
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
  uint16_t writeBuf = 0;
  uint32_t count = 0;

  lens = lens >> 1;
  FLASH_Unlock();
  do {
    writeBuf = Byte16(writeData[(count << 1) + 1], writeData[count << 1]);
    status = FLASH_ProgramHalfWord(writeAddr + (count << 1), writeBuf);
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
**函數 : Flash_ErasePage
**功能 : Erase Flash Memory
**輸入 : erasePage
**輸出 : status
**使用 : Flash_ErasePage(FLASH_PAGE_ADDR(PAGE), 8);
**=====================================================================================================*/
/*=====================================================================================================*/
int8_t Flash_ErasePage( uint32_t erasePage )
{
  int8_t status = ERROR;

  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
  status = FLASH_ErasePage(erasePage);
  FLASH_Lock();

  return status;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ErasePages
**功能 : Erase Flash Memory
**輸入 : erasePageStart, nbPages
**輸出 : status
**使用 : Flash_ErasePages(FLASH_PAGE_ADDR(PAGE), 8);
**=====================================================================================================*/
/*=====================================================================================================*/
int8_t Flash_ErasePages( uint32_t erasePageStart, uint32_t nbPages )
{
  int8_t status = ERROR;

  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
  while(nbPages--) {
    status = FLASH_ErasePage(erasePageStart);
    if(status!= FLASH_COMPLETE)
      return status;
    erasePageStart = erasePageStart + FLASH_PAGE_SIZE;
  }
  FLASH_Lock();

  return status;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
