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
  * @date    29-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "uwbAdapter_bsp.h"
#include "sampleRate.h"

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
void TIM4_EvenCallBack( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  float32_t sendBuf[PACKET_DATALENS] = {0};

  HAL_InitTick();

  UWBA_GPIO_Config();
  UWBA_UART_Config(NULL);
  UWBA_TIM4_Config(TIM4_EvenCallBack);

  while (1) {
    sendBuf[0] = time.min;
    sendBuf[1] = time.sec;
    sendBuf[2] = time.msec;
    kSerial_sendData(sendBuf, PACKET_DATALENS, KS_FLOAT32);
  }
}

void TIM4_EvenCallBack( void )
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
