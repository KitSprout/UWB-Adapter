/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\w25q.h"

#include "uwbTag.h"
/*====================================================================================================*/
/*====================================================================================================*/
void UWBT_Init( void )
{
  UWBT_GPIO_Config();
  UWBT_UART_Config(NULL);
  UWBT_W25Q_Config();
}
/*====================================================================================================*/
/*====================================================================================================*/
uint8_t readBuffer[8] = {0};
//#define WRITE_FLASH
void UWBT_Loop( void )
{
  printf("[1] READ\r\n");
  W25Q_ReadData(readBuffer, 0x00000000, 8);
  for(uint8_t i = 0; i < 8; i++)
    printf("\t[%2X]", i);
  printf("\r\n");
  for(uint8_t i = 0; i < 8; i++)
    printf("\t %2X ", readBuffer[i]);
  printf("\r\n");

#if defined(WRITE_FLASH)
  printf("[2] WRITE\r\n");
  readBuffer[0] = 0x00;
  readBuffer[1] = 0x01;
  readBuffer[2] = 0x02;
  readBuffer[3] = 0x03;
  readBuffer[4] = 0x64;
  readBuffer[5] = 0x75;
  readBuffer[6] = 0x28;
  readBuffer[7] = 0x30;
  W25Q_WritePage(readBuffer, 0x00000000, 8);

  printf("[3] READ\r\n");
  W25Q_ReadData(readBuffer, 0x00000000, 8);
  for(uint8_t i = 0; i < 8; i++)
    printf("\t[%2X]", i);
  printf("\r\n");
  for(uint8_t i = 0; i < 8; i++)
    printf("\t %2X ", readBuffer[i]);
  printf("\r\n");
#endif

  while(1) {
    LED_G_Toggle();
    delay_ms(100);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
