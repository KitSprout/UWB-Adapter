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
  * @date    26-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\dw1000.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* DEMO TX PROGRAM SELECT */
#define EX_SIMPLE_TX
//#define EX_TX_SLEEP
//#define EX_TX_SLEEP_AUTO
//#define EX_TX_TIMED_SLEEP

/* DEMO TX PROGRAM SELECT */
//#define EX_SIMPLE_RX
//#define EX_RX_DIAG
//#define EX_RX_SNIFF
#define EX_RX_DBL_BUFF

#if defined(EX_SIMPLE_TX)
#define DEMO_TX_STRING "SIMPLE_TX"
#elif defined(EX_TX_SLEEP)
#define DEMO_TX_STRING "TX_SLEEP"
#elif defined(EX_TX_SLEEP_AUTO)
#define DEMO_TX_STRING "TX_SLEEP_AUTO"
#elif defined(EX_TX_TIMED_SLEEP)
#define DEMO_TX_STRING "TX_TIMED_SLEEP"
#else
#error "No TX DEMO"
#endif

#if defined(EX_SIMPLE_RX)
#define DEMO_RX_STRING "SIMPLE_RX"
#elif defined(EX_RX_DIAG)
/* This application waits for reception of a frame. After each frame received with a good
 * CRC it reads some data provided by DW1000:
 *  - Diagnostics data (e.g. first path index, first path amplitude, channel impulse
 *    response, etc.). See dwt_rxdiag_t structure for more * details on the data read.
 *  - Accumulator values around the first path. It also reads event counters (e.g. CRC
 *    good, CRC error, PHY header error, etc.) after any event, be it a good frame or an
 *    RX error. See dwt_deviceentcnts_t structure for more details on the counters read. */
#define DEMO_RX_STRING "RX_DIAG"
#elif defined(EX_RX_SNIFF)
#define DEMO_RX_STRING "RX_SNIFF"
#elif defined(EX_RX_DBL_BUFF)
/* This example keeps listening for any incoming frames, storing in a local buffer any 
 * frame received before going back to listening. This examples activates interrupt
 * handling and the double buffering feature of the DW1000 (but automatic RX re-enabling 
 * is not supported). Frame processing and manual RX re-enabling are performed in the RX
 * good frame callback. */
#define DEMO_RX_STRING "RX_DBL_BUFF"
#else
#error "No RX DEMO"
#endif

DWT_ConfigTypeDef dwtConfig = {
  /* In this example, LDE microcode is not loaded upon calling dwt_initialise().
   * This will prevent the IC from generating an RX timestamp. If time-stamping
   * is required, DWT_LOADUCODE parameter should be used. */
  .LoadCode          = DW_LOAD_NONE,
  .Channel           = DW_CHANNEL_2,
  .PulseRepFreq      = DW_PRF_64M,
  .TxPreambLen       = DW_TX_PLEN_1024,
  .PreambleAcqChunk  = DW_PAC_32,
  .TxCode            = 9,
  .RxCode            = 9,
  .NonStandardSFD    = ENABLE,
  .DataRate          = DW_DATARATE_6M8,
  .PhrMode           = DW_PHR_MODE_STD,
  .SFDTimeout        = (1024 + 1 + 64 - 32) // TxPreambLen + 1 + SFD length - PAC
};

void DEMO_TX( void );
void DEMO_RX( void );

int main( void )
{
  HAL_Init();

  BSP_GPIO_Config();
  BSP_UART_Config(NULL, NULL);
  BSP_UWB_Config();

  if (KEY_Read()) {
    LED_G_Reset();
    delay_ms(500);
    LED_G_Set();
    printf(" ---- %s ----\r\n", DEMO_TX_STRING);
    DEMO_TX();
  }
  else {
    LED_B_Reset();
    delay_ms(500);
    LED_B_Set();
    printf(" ---- %s ----\r\n", DEMO_RX_STRING);
    DEMO_RX();
  }

  while (1) { ; }
}

/* Inter-frame delay period, in milliseconds. */
#define TX_DELAY_MS 100

#if defined(EX_TX_TIMED_SLEEP)
/* Sleep time, in milliseconds. ( use in "EX_TX_TIMED_SLEEP" )
 * Take a small margin compared to TX_DELAY_MS so that the DW1000 has the time to wake up.
 * >> NOTE:
 * The sleep counter is 16 bits wide but represents the upper 16 bits of a 28 bits counter.
 * Thus the granularity of this counter is 4096 counts. Combined with the frequency of the
 * internal RING oscillator being typically between 7 and 13 kHz, this means that the time
 * granularity that we get when using the timed sleep feature is typically between 315 and
 * 585 ms. As the sleep time calculated is rounded down to the closest integer number of
 * sleep counts, this means that the actual sleep time can be significantly less than the
 * one defined here. */
#define SLP_TIME  (TX_DELAY_MS - 10)
#endif

#if defined(EX_TX_SLEEP) | defined(EX_TX_SLEEP_AUTO) | defined(EX_TX_TIMED_SLEEP)
/* Dummy buffer for DW1000 wake-up SPI read.
 * >> NOTE:
 * The chosen method for waking the DW1000 up in this example is by using the internal sleep
 * timer to wake-up automatically but we also need to activate SPI chip select wake-up for
 * the initial wake-up (in case of reset). In this particular case we need to drive SPI chip
 * select line low for at least 500 us. This means that we need a buffer to collect the data
 * that DW1000 outputs during this dummy SPI transaction. The length of the transaction, and
 * then the time for which the SPI chip select is held low, is determined by the buffer
 * length given to DWT_SpiCsWakeup() so this length must be chosen high enough so that the
 * DW1000 has enough time to wake up. */
#define DUMMY_BUFFER_LEN 600
static uint8_t dummy_buffer[DUMMY_BUFFER_LEN];
#endif

/* ---------------------------------------------------------------------------------------- *
 *                                                                                          *
 *                                  Ultra-Wideband TX                                       *
 *                                                                                          *
 * ---------------------------------------------------------------------------------------- */

/* The frame sent in this example is an 802.15.4e standard blink.
 * It is a 12-byte frame composed of the following fields:
 *  - byte 0     : frame type (0xC5 for a blink).
 *  - byte 1-9   : "KitSprout"
 *  - byte 10/11 : sequence number, incremented for each new frame.
 *  - byte 12    : LED state.
 *  - byte 13/14 : frame check-sum, automatically set by DW1000. */
static uint8_t tx_msg[] = { 0xC5, 'K', 'i', 't', 'S', 'p', 'r', 'o', 'u', 't', 0, 0, 0, 0, 0 };
static uint8_t ledState = 0;
static uint16_t sequenceNum = 0;

void DEMO_TX( void )
{
#if defined(EX_TX_SLEEP) | defined(EX_TX_SLEEP_AUTO) | defined(EX_TX_TIMED_SLEEP)
  /* Issue a wake-up in case DW1000 is asleep.
   * Since DW1000 is not woken by the reset line, we could get here with it asleep.
   * Note that this may be true in other examples but we pay special
   * attention here because this example is precisely about sleeping. */
  DWT_SpiCsWakeup(dummy_buffer, DUMMY_BUFFER_LEN);
#endif

  DWT_SetSpeedLow();
  DWT_Initialise(&dwtConfig);

#if defined(EX_TX_TIMED_SLEEP)
  uint16_t lp_oscFreq, sleepCnt;
  /* Calibrate and configure sleep count.
   * This has to be done with DW1000 clocks set to crystal speed. */
  lp_oscFreq = (DWT_XTAL_FREQ_HZ / 2) / DWT_CalibRateSleepCnt();
  sleepCnt = ((SLP_TIME * lp_oscFreq) / 1000) >> 12;
  DWT_ConfigureSleepCnt(sleepCnt);
#endif

  DWT_SetSpeedHigh();

  /* In a real application, for optimum performance within regulatory limits, it may be necessary
   * to set TX pulse bandwidth and TX power, (using the dwt_configuretxrf API call) to per device
   * calibrated values saved in the target system or the DW1000 OTP memory. */
  DWT_Configure(&dwtConfig);

#if defined(EX_TX_SLEEP) | defined(EX_TX_SLEEP_AUTO)
  /* Configure sleep and wake-up parameters. */
  DWT_ConfigureSleep(DWT_PRESRV_SLP | DWT_CONFIG, DWT_WAKE_CS | DWT_SLP_EN);
#elif defined(EX_TX_TIMED_SLEEP)
  DWT_ConfigureSleep(DWT_PRESRV_SLP | DWT_CONFIG, DWT_WAKE_SLPCNT | DWT_WAKE_CS | DWT_SLP_EN);
#endif

#if defined(EX_TX_SLEEP_AUTO) | defined(EX_TX_TIMED_SLEEP)
  /* Configure DW1000 to automatically enter sleep mode after transmission of a frame. */
  DWT_EnterSleepAfterTx(ENABLE);
#endif

  while (1) {
    /* Write frame data to DW1000 and prepare transmission.
     * >> NOTE:
     * DWT_WriteTxData() takes the full size of tx_msg as a parameter but only copies (size - 2)
     * bytes as the check-sum at the end of the frame is automatically appended by the DW1000.
     * This means that our tx_msg could be two bytes shorter without losing any data (but the
     * sizeof would not work anymore then as we would still have to indicate the full length of
     * the frame to DWT_WriteTxData()). */
    DWT_WriteTxData(sizeof(tx_msg), tx_msg, 0); /* Zero offset in TX buffer. */
    DWT_WriteTxFCtrl(sizeof(tx_msg), 0, 0);     /* Zero offset in TX buffer, no ranging. */

    /* Start transmission. */
    DWT_StartTx(DWT_TX_IMMEDIATE);

#if defined(EX_TX_SLEEP)
    /* Poll DW1000 until TX frame sent event set. STATUS register is 5 bytes long but, as the event
     * we are looking at is in the first byte of the register, we can use this simplest API function
     * to access it.
     * >> NOTE:
     * We use polled mode of operation here to keep the example as simple as possible but the TXFRS
     * status event can be used to generate an interrupt. Please refer to DW1000 User Manual for
     * more details on "interrupts". */
    while (!(DWT_GetSysStatus() & SYS_STATUS_TXFRS)) { };

    /* Clear TX frame sent event. */
    DWT_SetSysStatus(SYS_STATUS_RXFCG);

    /* Put DW1000 to sleep. */
    DWT_EnterSleep();

#elif defined(EX_TX_SLEEP_AUTO) | defined(EX_TX_TIMED_SLEEP)
    /* It is not possible to access DW1000 registers once it has sent the frame and gone to sleep,
     * and therefore we do not try to poll for TX frame sent, but instead simply wait sufficient
     * time for the DW1000 to wake up again before we loop back to send another frame. If interrupts
     * are enabled, (e.g. if MTXFRS bit is set in the SYS_MASK register) then the TXFRS event will
     * cause an active interrupt and prevent the DW1000 from sleeping. */

#endif

    /* Execute a delay between transmissions.
     * >> NOTE: ("EX_TX_TIMED_SLEEP")
     * Here we just wait for the DW1000 to wake up but, in a practical implementation, this
     * microprocessor could be put to sleep too and waken up using an interrupt generated by
     * the DW1000 waking. */
    delay_ms(TX_DELAY_MS);

#if defined(EX_TX_SLEEP) | defined(EX_TX_SLEEP_AUTO)
    /* Wake DW1000 up.
     * >> NOTE:
     * The chosen method for waking the DW1000 up here is by maintaining SPI chip select line
     * low for at least 500 us. This means that we need a buffer to collect the data that DW1000
     * outputs during this dummy SPI transaction. The length of the transaction, and then the
     * time for which the SPI chip select is held low, is determined by the buffer length given
     * to dwt_spicswakeup() so this length must be chosen high enough so that the DW1000 has
     * enough time to wake up. */
    DWT_SpiCsWakeup(dummy_buffer, DUMMY_BUFFER_LEN);

#elif defined(EX_TX_TIMED_SLEEP)
    /* there is nothing to do to wake the DW1000 up as it is handled by the sleep timer. */

#endif

    /* Increment the blink frame sequence number. */
    sequenceNum++;
    ledState = !ledState;
    tx_msg[10] = sequenceNum;
    tx_msg[11] = sequenceNum >> 8;
    tx_msg[12] = ledState;
  }
}

/* ---------------------------------------------------------------------------------------- *
 *                                                                                          *
 *                                  Ultra-Wideband RX                                       *
 *                                                                                          *
 * ---------------------------------------------------------------------------------------- */

/* Buffer to store received frame.
 * >> NOTE:
 * maximum frame length is set to 127 bytes which is 802.15.4 UWB standard maximum frame
 * length. DW1000 supports an extended frame length (up to 1023 bytes long) mode. */
#define FRAME_LEN_MAX 127
static uint8_t rx_buffer[FRAME_LEN_MAX];

#if defined(EX_RX_SNIFF)
/* SNIFF mode on/off times.
 * ON time is expressed in multiples of PAC size (with the IC adding 1 PAC automatically). So the ON time of 1 here gives 2 PAC times and, since the
 * configuration (above) specifies DWT_PAC8, we get an ON time of 2x8 symbols, or around 16 盜.
 * OFF time is expressed in multiples of 128/125 盜 (~1 盜).
 * These values will lead to a roughly 50% duty-cycle, each ON and OFF phase lasting for about 16 盜. */
#define SNIFF_ON_TIME   1
#define SNIFF_OFF_TIME  16
#endif

#if defined(EX_RX_DBL_BUFF)
static void rx_ok_cb( const dwt_cb_data_t *cb_data );
static void rx_err_cb( const dwt_cb_data_t *cb_data );
#endif

__IO uint8_t flag = ERROR;

void PrintRawData( uint8_t *buffer, uint8_t lens )
{
  for(uint8_t i = 0; i < lens; i++ ) {
    printf("%02X ", buffer[i]);
  }
}

void GetRecvData( uint8_t disp, uint8_t *buffer )
{
  if (disp == SUCCESS) {
    uint16_t sequenceNum = (buffer[11] << 8) | buffer[10];
    printf("[%5d] ", sequenceNum);
    for(uint8_t i = 1; i < 10; i++ ) {
      printf("%c", buffer[i]);
    }
    if(buffer[12]) {
      LED_B_Set();
    }
    else {
      LED_B_Reset();
    }
  }
  else {
    
  }
}

void DEMO_RX( void )
{

#if defined(EX_SIMPLE_RX) | defined(EX_RX_DIAG) | defined(EX_RX_SNIFF)
  uint16_t frameLen = 0;
  uint32_t status = 0;
#endif

//#if defined(EX_RX_DBL_BUFF)
//  DWT_EnableExtiIRQ();
//#endif

  DWT_SetSpeedLow();
  DWT_Initialise(&dwtConfig);
  DWT_SetSpeedHigh();

#if defined(EX_RX_SNIFF)
  /* This is put here for testing, so that we can see the receiver ON/OFF pattern using an oscilloscope. */
  DWT_SetLnaPaMode(ENABLE, ENABLE);
#endif

  DWT_Configure(&dwtConfig);

#if defined(EX_RX_DIAG)
  /* Activate event counters. */
  DWT_ConfigEventCounters(ENABLE);

#elif defined(EX_RX_SNIFF)
  /* Configure SNIFF mode. */
  DWT_SetSniffMode(ENABLE, SNIFF_ON_TIME, SNIFF_OFF_TIME);

#elif defined(EX_RX_DBL_BUFF)
  /* Activate double buffering. */
  DWT_SetDblRxBufMode(ENABLE);

  /* Register RX call-back. */
  DWT_SetCallbacks(NULL, &rx_ok_cb, NULL, &rx_err_cb);

  /* Enable wanted interrupts (RX good frames and RX errors). */
  DWT_SetInterrupt(DWT_INT_RFCG | DWT_INT_RPHE | DWT_INT_RFCE | DWT_INT_RFSL | DWT_INT_SFDT, ENABLE);

  /* Activate reception immediately. */
  DWT_RxEnable(DWT_RX_IMMEDIATE);
#endif

#if defined(EX_SIMPLE_RX)
  while (1) {
    /* Clear local RX buffer to avoid having leftovers from previous receptions
     * This is not necessary but is included here to aid reading the RX buffer.
     * This is a good place to put a breakpoint. Here (after first time through the loop)
     * the local status register will be set for last event and if a good receive has
     * happened the data buffer will have the data in it, and frame_len will be set to
     * the length of the RX frame. */
    for (uint8_t i = 0 ; i < FRAME_LEN_MAX; i++ ) {
      rx_buffer[i] = 0;
    }

    /* Activate reception immediately.
     * >> NOTE:
     * Manual reception activation is performed here but DW1000 offers several features
     * that can be used to handle more complex scenarios or to optimise system's overall
     * performance (e.g. timeout after a given time, automatic re-enabling of reception
     * in case of errors, etc.). */
    DWT_RxEnable(DWT_RX_IMMEDIATE);

    /* Poll until a frame is properly received or an error/timeout occurs.
     * STATUS register is 5 bytes long but, as the event we are looking at is in the
     * first byte of the register, we can use this simplest API function to access it.
     * >> NOTE:
     * We use polled mode of operation here to keep the example as simple as possible
     * but RXFCG and error/timeout status events can be used to generate interrupts.
     * Please refer to DW1000 User Manual for more details on "interrupts". */
    do {
      status = DWT_GetSysStatus();
    } while (!(status & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR)));

    if (status & SYS_STATUS_RXFCG) {
      /* A frame has been received, copy it to our local buffer. */
      frameLen = DWT_ReadData32(DW1000_RX_FINFO, 0x00) & 0x00003FFF;
      LED_G_Toggle();
      if (frameLen <= FRAME_LEN_MAX) {
        /* - byte 0    : frame type (0xC5 for a blink).
         * - byte 1-9  : "KitSprout"
         * - byte 10/11: sequence number, incremented for each new frame.
         * - byte 12   : LED state.
         * - byte 13/14: frame check-sum, automatically set by DW1000. */
        DWT_ReadRxData(rx_buffer, frameLen, 0);
        flag = SUCCESS;
      }
      /* Clear good RX frame event in the DW1000 status register. */
      DWT_SetSysStatus(SYS_STATUS_RXFCG);
    }
    else {
      /* Clear RX error events in the DW1000 status register. */
      DWT_SetSysStatus(SYS_STATUS_ALL_RX_ERR);
    }

    PrintRawData(rx_buffer, 16);
    printf("    ");
    GetRecvData(flag, rx_buffer);
    printf("\r\n");
    flag = ERROR;
  }

#elif defined(EX_RX_DIAG)
  /* Hold copy of accumulator data so that it can be examined at a debug breakpoint.
   * >> NOTE:
   * Accumulator values are complex numbers: one 16-bit integer for real part and one 16-bit
   * value for imaginary part, for each sample. In this example, we chose to read 3 values
   * below the first path index and 3 values above. It must be noted that the first byte read
   * when accessing the accumulator memory is always garbage and must be discarded, that is
   * why the data length to read is increased by one byte here. */
  #define ACCUM_DATA_LEN (2 * 2 * (3 + 3) + 1)
  static uint8_t accum_data[ACCUM_DATA_LEN];

  static dwt_deviceentcnts_t event_cnt;
  static dwt_rxdiag_t rx_diag;

  while (1) {
    for (uint8_t i = 0 ; i < FRAME_LEN_MAX; i++ ) {
      rx_buffer[i] = 0;
      accum_data[i] = 0;
    }
    rx_diag.firstPath     = 0;
    rx_diag.firstPathAmp1 = 0;
    rx_diag.firstPathAmp2 = 0;
    rx_diag.firstPathAmp3 = 0;
    rx_diag.maxGrowthCIR  = 0;
    rx_diag.rxPreamCount  = 0;
    rx_diag.maxNoise      = 0;
    rx_diag.stdNoise      = 0;

    /* Activate reception immediately. */
    DWT_RxEnable(DWT_RX_IMMEDIATE);

    do {
      status = DWT_GetSysStatus();
    } while (!(status & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR)));

    if (status & SYS_STATUS_RXFCG) {
      /* A frame has been received, copy it to our local buffer. */
      frameLen = DWT_ReadData32(DW1000_RX_FINFO, 0x00) & 0x00003FFF;
      LED_G_Toggle();
      if (frameLen <= FRAME_LEN_MAX) {
        DWT_ReadRxData(rx_buffer, frameLen, 0);
        flag = SUCCESS;

        /* Read diagnostics data. */
        DWT_ReadDiagnostics(&rx_diag);

        /* Read accumulator.
         * >> NOTES:
         * Accumulator values are complex numbers: one 16-bit integer for real part
         * and one 16-bit value for imaginary part, for each sample. In this example,
         * we chose to read 3 values below the first path index and 3 values above.
         * It must be noted that the first byte read when accessing the accumulator
         * memory is always garbage and must be discarded, that is why the data length
         * to read is increased by one byte here.
         * >> NOTES:
         * Here we chose to read only a few values around the first path index but it
         * is possible and can be useful to get all accumulator values, using the
         * relevant offset and length parameters. Reading the whole accumulator will
         * require 4064 bytes of memory. First path value gotten from dwt_readdiagnostics
         * is a 10.6 bits fixed point value calculated by the DW1000. By dividing this
         * value by 64, we end up with the integer part of it. This value can be used
         * to access the accumulator samples around the calculated first path index as
         * it is done here. */
        uint16_t fp_int = rx_diag.firstPath >> 6;
        DWT_ReadAccData(accum_data, ACCUM_DATA_LEN, (fp_int - 2) << 2);
      }
    }
    else {
      /* Clear RX error events in the DW1000 status register. */
      DWT_SetSysStatus(SYS_STATUS_ALL_RX_ERR);

      /* Reset RX to properly reinitialise LDE operation. */
      DWT_RxReset();
    }

    /* Read event counters.
     * >> NOTE:
     * Event counters are never reset in this example but this can be done by re-enabling
     * them (i.e. calling again dwt_configeventcounters with "enable" parameter set). */
    DWT_ReadEventCounters(&event_cnt);

    PrintRawData(accum_data, 16);
    printf("    ");
    GetRecvData(flag, accum_data);
    printf("\r\n");
    flag = ERROR;
  }

#elif defined(EX_RX_SNIFF)
  while (1) {
    for (uint8_t i = 0 ; i < FRAME_LEN_MAX; i++ ) {
      rx_buffer[i] = 0;
    }

    /* Activate reception immediately. */
    DWT_RxEnable(DWT_RX_IMMEDIATE);

    do {
      status = DWT_GetSysStatus();
    } while (!(status & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR)));

    if (status & SYS_STATUS_RXFCG) {
      /* Clear good RX frame event in the DW1000 status register. */
      DWT_SetSysStatus(SYS_STATUS_RXFCG);

      /* A frame has been received, copy it to our local buffer. */
      frameLen = DWT_ReadData32(DW1000_RX_FINFO, 0x00) & 0x00003FFF;
      LED_G_Toggle();
      if (frameLen <= FRAME_LEN_MAX) {
        DWT_ReadRxData(rx_buffer, frameLen, 0);
        flag = SUCCESS;
      }
    }
    else {
      /* Clear RX error events in the DW1000 status register. */
      DWT_SetSysStatus(SYS_STATUS_ALL_RX_ERR);
    }

    PrintRawData(rx_buffer, 16);
    printf("    ");
    GetRecvData(flag, rx_buffer);
    printf("\r\n");
    flag = ERROR;
  }

#elif defined(EX_RX_DBL_BUFF)
  while (1) {
    if (flag == SUCCESS) {
      flag = ERROR;
      PrintRawData(rx_buffer, 16);
      printf("    ");
      GetRecvData(SUCCESS, rx_buffer);
      printf("\r\n");
    }
  };

#endif

}

static void rx_ok_cb( const dwt_cb_data_t *cb_data )
{
  /* Perform manual RX re-enabling.
   * >> NOTE:
   * When using double buffering, RX can be re-enabled before reading all the frame
   * data as this is precisely the purpose of having two buffers. All the registers
   * needed to process the received frame are also double buffered with the exception
   * of the Accumulator CIR memory and the LDE threshold (accessed when calling
   * dwt_readdiagnostics). In an actual application where these values might be
   * needed for any processing or diagnostics purpose, they would have to be read
   * before RX re-enabling is performed so that they are not corrupted by a frame
   * being received while they are being read. Typically, in this example, any such
   * diagnostic data access would be done at the very beginning of the rx_ok_cb
   * function.*/
  DWT_RxEnable(DWT_RX_IMMEDIATE | DWT_NO_SYNC_PTRS);

  /* A frame has been received, copy it to our local buffer.
   * >> NOTE:
   * A real application might get an operating system (OS) buffer for this data
   * reading and then pass the buffer onto a queue into the next layer of processing
   * task via an appropriate OS call. */
  if (cb_data->datalength <= FRAME_LEN_MAX) {
    DWT_ReadRxData(rx_buffer, cb_data->datalength, 0);
    flag = SUCCESS;
  }
}

static void rx_err_cb( const dwt_cb_data_t *cb_data )
{
  /* Re-activate reception immediately. */
  DWT_RxEnable(DWT_RX_IMMEDIATE);
}

/*************************************** END OF FILE ****************************************/
