/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    main.c
  * @author  KitSprout
  * @date    28-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define PACKET_DATALENS 3

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
struct {
  float32_t msec;
  float32_t sec;
  float32_t min;
} time;

/* Private function prototypes -------------------------------------------------------------*/
void TIMER_CallBack( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  static float32_t sendBuf[PACKET_DATALENS] = {0};

  HAL_Init();
  BSP_GPIO_Config();
  BSP_UART_Config(NULL, NULL);
  BSP_TIMER2_Config(TIMER_CallBack, SAMPLE_RATE_FREQ);

  while (1) {
    sendBuf[0] = time.min;
    sendBuf[1] = time.sec;
    sendBuf[2] = time.msec;
    kSerial_sendData(sendBuf, PACKET_DATALENS, KS_FLOAT32);
  }
}

void TIMER_CallBack( void )
{
  LED_R_Set();

  time.msec += SAMPLE_RATE;
  if (time.msec >= 1.0f) {
    time.msec = 0.0f;
    time.sec += 1.0f;
    if (time.sec >= 60.0f) {
      time.sec = 0.0f;
      time.min += 1.0f;
    }
  }

  LED_R_Reset();
}

/*************************************** END OF FILE ****************************************/
