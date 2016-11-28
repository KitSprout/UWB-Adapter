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
  * @date    27-Nov-2016
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

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/

/* Default antenna delay values for 64 MHz PRF. */
#define TX_ANT_DLY                    16436
#define RX_ANT_DLY                    16436

/* Length of the common part of the message. */
#define ALL_MSG_COMMON_LEN            10

/* Indexes to access some of the fields in the frames defined above. */
#define ALL_MSG_SN_IDX                2
#define FINAL_MSG_TS_LEN              4
#define FINAL_MSG_POLL_TX_TS_IDX      10
#define FINAL_MSG_RESP_RX_TS_IDX      14
#define FINAL_MSG_FINAL_TX_TS_IDX     18

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
 * 1 uus = 512 / 499.2 ms and 1 ms = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME               65536

/* Speed of light in air, in metres per second. */
#define SPEED_OF_LIGHT                299702547

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/

/* Frame sequence number, incremented after each transmission. */
static uint8_t frame_seq_nb = 0;

static uint32_t status;

static DWT_ConfigTypeDef dwtConfig = {
  .LoadCode          = DW_LOAD_UCODE,
  .Channel           = DW_CHANNEL_2,
  .PulseRepFreq      = DW_PRF_64M,
  .TxPreambLen       = DW_TX_PLEN_1024,
  .PreambleAcqChunk  = DW_PAC_32,
  .TxCode            = 9,
  .RxCode            = 9,
  .NonStandardSFD    = ENABLE,
  .DataRate          = DW_DATARATE_110K,
  .PhrMode           = DW_PHR_MODE_STD,
  .SFDTimeout        = (1024 + 1 + 64 - 32) // TxPreambLen + 1 + SFD length - PAC
};


/* Private function prototypes -------------------------------------------------------------*/
void DEMO_SSTWR_INITIATOR( void );
void DEMO_SSTWR_RESPONDER( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_Init();

  BSP_GPIO_Config();
  BSP_UART_Config(NULL, NULL);
  BSP_UWB_Config();

  DWT_SetSpeedLow();
  DWT_Initialise(&dwtConfig);
  DWT_SetSpeedHigh();

  /* Configure DW1000.
   * >> NOTE:
   * In a real application, for optimum performance within regulatory limits, it may be
   * necessary to set TX pulse bandwidth and TX power, (using the dwt_configuretxrf API
   * call) to per device calibrated values saved in the target system or the DW1000
   * OTP memory. */
  DWT_Configure(&dwtConfig);

  /* Apply default antenna delay value.
   * >> NOTE:
   * The sum of the values is the TX to RX antenna delay, experimentally determined by a
   * calibration process. Here we use a hard coded typical value but, in a real application,
   * each device should have its own antenna delay properly calibrated to get the best
   * possible precision when performing range measurements. */
  DWT_SetRxAntennaDelay(RX_ANT_DLY);
  DWT_SetTxAntennaDelay(TX_ANT_DLY);

  if (KEY_Read()) {
    LED_G_Reset();
    delay_ms(500);
    LED_G_Set();
    printf(" ---- INITIATOR ----\r\n");
    DEMO_SSTWR_INITIATOR();
  }
  else {
    LED_B_Reset();
    delay_ms(500);
    LED_B_Set();
    printf(" ---- RESPONDER ----\r\n");
    DEMO_SSTWR_RESPONDER();
  }

  while (1) { ; }
}

int8_t checkPacket( uint8_t *pMsg1, uint8_t *pMsg2 )
{
  for (uint8_t i = 0; i < ALL_MSG_COMMON_LEN; i++) {
    if (pMsg1[i] != pMsg2[i]) {
      return -1;
    }
  }
  return 0;
}

/* ---------------------------------------------------------------------------------------- *
 *                                                                                          *
 *                          Single-Sided Two-Way Ranging Initiator                          *
 *                                                                                          *
 * ---------------------------------------------------------------------------------------- */

/* Delay between frames, in UWB microseconds. See NOTE 4 below. */
/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
#define POLL_TX_TO_RESP_RX_DLY_UUS 150

/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.66 ms with above configuration. */
#define RESP_RX_TO_FINAL_TX_DLY_UUS 3100

/* Receive response timeout. See NOTE 5 below. */
#define RESP_RX_TIMEOUT_UUS 2700

/* Preamble timeout, in multiple of PAC size. See NOTE 6 below. */
#define PRE_TIMEOUT 8

static uint64_t get_tx_timestamp_u64( void )
{
  uint8_t ts_tab[5];
  uint64_t ts = 0;
  DWT_ReadTxTimestamp(ts_tab);
  for (int i = 4; i >= 0; i--) {
    ts <<= 8;
    ts |= ts_tab[i];
  }
  return ts;
}

static uint64_t get_rx_timestamp_u64( void )
{
  uint8_t ts_tab[5];
  uint64_t ts = 0;
  DWT_ReadRxTimestamp(ts_tab);
  for (int i = 4; i >= 0; i--) {
    ts <<= 8;
    ts |= ts_tab[i];
  }
  return ts;
}

static void final_msg_set_ts( uint8_t *ts_field, uint64_t ts )
{
  for (int i = 0; i < FINAL_MSG_TS_LEN; i++) {
    ts_field[i] = (uint8_t) ts;
    ts >>= 8;
  }
}

void DEMO_SSTWR_INITIATOR( void )
{
  /* Frames used in the ranging process. See NOTE 2 below. */
  static uint8_t tx_poll_msg[]  = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x21, 0, 0};
  static uint8_t rx_resp_msg[]  = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0x10, 0x02, 0, 0, 0, 0};
  static uint8_t tx_final_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  /* Buffer to store received response message.
   * Its size is adjusted to longest frame that this example code is supposed to handle. */
  #define RX_BUF_LEN  20
  static uint8_t rx_buffer[RX_BUF_LEN];

  uint32_t frameLen, final_tx_time;
  uint64_t poll_tx_ts, resp_rx_ts, final_tx_ts;

  /* Set expected response's delay and timeout.
   * As this example only handles one incoming frame with always the same delay and timeout,
   * those values can be set here once for all. */
  DWT_SetRxAfterTxDelay(POLL_TX_TO_RESP_RX_DLY_UUS);
  DWT_SetRxTimeout(RESP_RX_TIMEOUT_UUS);

  while (1) {

    /* Write frame data to DW1000 and prepare transmission. See NOTE 8 below. */
    tx_poll_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
    DWT_WriteTxData(sizeof(tx_poll_msg), tx_poll_msg, 0); /* Zero offset in TX buffer. */
    DWT_WriteTxFCtrl(sizeof(tx_poll_msg), 0, 1); /* Zero offset in TX buffer, ranging. */

    /* Start transmission, indicating that a response is expected so that reception is enabled
     * automatically after the frame is sent and the delay set by dwt_setrxaftertxdelay()
     * has elapsed. */
    DWT_StartTx(DWT_TX_IMMEDIATE | DWT_RESPONSE);

    /* We assume that the transmission is achieved correctly, poll for reception of a frame or error/timeout. See NOTE 9 below. */
    do {
      status = DWT_GetSysStatus();
    } while (!(status & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)));

    /* Increment frame sequence number after transmission of the poll message (modulo 256). */
    frame_seq_nb++;

    if (status & SYS_STATUS_RXFCG) {
      /* Clear good RX frame event and TX frame sent in the DW1000 status register. */
      DWT_SetSysStatus(SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);

      /* A frame has been received, read it into the local buffer. */
      frameLen = DWT_ReadData32(DW1000_RX_FINFO, 0x00) & 0x0000007FUL;
      if (frameLen <= RX_BUF_LEN) {
        DWT_ReadRxData(rx_buffer, frameLen, 0x00);
      }

      /* Check that the frame is the expected response from the companion "DS TWR responder" example.
       * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */

      rx_buffer[ALL_MSG_SN_IDX] = 0;
      if (checkPacket(rx_buffer, rx_resp_msg) == 0) {

        /* Retrieve poll transmission and response reception timestamp. */
        poll_tx_ts = get_tx_timestamp_u64();
        resp_rx_ts = get_rx_timestamp_u64();

        /* Compute final message transmission time. See NOTE 10 below. */
        final_tx_time = (resp_rx_ts + (RESP_RX_TO_FINAL_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
        DWT_SetDelayedTxRxTime(final_tx_time);

        /* Final TX timestamp is the transmission time we programmed plus the TX antenna delay. */
        final_tx_ts = (((uint64_t)(final_tx_time & 0xFFFFFFFEUL)) << 8) + TX_ANT_DLY;

        /* Write all timestamps in the final message. See NOTE 11 below. */
        final_msg_set_ts(&tx_final_msg[FINAL_MSG_POLL_TX_TS_IDX], poll_tx_ts);
        final_msg_set_ts(&tx_final_msg[FINAL_MSG_RESP_RX_TS_IDX], resp_rx_ts);
        final_msg_set_ts(&tx_final_msg[FINAL_MSG_FINAL_TX_TS_IDX], final_tx_ts);

        /* Write and send final message. See NOTE 8 below. */
        tx_final_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
        DWT_WriteTxData(sizeof(tx_final_msg), tx_final_msg, 0); /* Zero offset in TX buffer. */
        DWT_WriteTxFCtrl(sizeof(tx_final_msg), 0, 1); /* Zero offset in TX buffer, ranging. */
        status = DWT_StartTx(DWT_TX_DELAYED);

        /* If dwt_starttx() returns an error, abandon this ranging exchange and proceed to the next one. See NOTE 12 below. */
        if (status == HAL_OK) {
          /* Poll DW1000 until TX frame sent event set. See NOTE 9 below. */
          while (!(DWT_GetSysStatus() & SYS_STATUS_TXFRS)) { };

          /* Clear TXFRS event. */
          DWT_SetSysStatus(SYS_STATUS_TXFRS);

          /* Increment frame sequence number after transmission of the final message (modulo 256). */
          frame_seq_nb++;

          LED_G_Toggle();
        }
      }
    }
    else {
      /* Clear RX error/timeout events in the DW1000 status register. */
      DWT_SetSysStatus(SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

      /* Reset RX to properly reinitialise LDE operation. */
      DWT_RxReset();
    }
    LED_R_Toggle();
    delay_ms(100);
  }
}




/* ---------------------------------------------------------------------------------------- *
 *                                                                                          *
 *                          Single-Sided Two-Way Ranging Responder                          *
 *                                                                                          *
 * ---------------------------------------------------------------------------------------- */

/* Delay between frames, in UWB microseconds. See NOTE 4 below. */
/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.46 ms with above configuration. */
#define POLL_RX_TO_RESP_TX_DLY_UUS 2600

/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
#define RESP_TX_TO_FINAL_RX_DLY_UUS 500

/* Receive final timeout. See NOTE 5 below. */
#define FINAL_RX_TIMEOUT_UUS 3300

/* Preamble timeout, in multiple of PAC size. See NOTE 6 below. */
#define PRE_TIMEOUT 8

static void final_msg_get_ts( const uint8_t *ts_field, uint32_t *ts )
{
  *ts = 0;
  for (int i = 0; i < FINAL_MSG_TS_LEN; i++) {
    *ts += ts_field[i] << (i * 8);
  }
}

void DEMO_SSTWR_RESPONDER( void )
{
  /* Frames used in the ranging process. See NOTE 2 below. */
  static uint8_t rx_poll_msg[]  = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x21, 0, 0};
  static uint8_t tx_resp_msg[]  = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0x10, 0x02, 0, 0, 0, 0};
  static uint8_t rx_final_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  /* Buffer to store received response message.
   * Its size is adjusted to longest frame that this example code is supposed to handle. */
  #define RX_RESP_BUF_LEN  24
  static uint8_t rx_resp_buffer[RX_RESP_BUF_LEN];

  uint32_t frameLen, resp_tx_time;
  uint64_t poll_rx_ts, resp_tx_ts, final_rx_ts;

  float64_t tof;
  float64_t distance;

  /* Set preamble timeout for expected frames. See NOTE 6 below. */
  DWT_SetPreambleDetectTimeout(PRE_TIMEOUT);

  while (1) {

    /* Clear reception timeout to start next ranging process. */
    DWT_SetRxTimeout(0);

    /* Activate reception immediately. */
    DWT_RxEnable(DWT_RX_IMMEDIATE);

    /* Poll for reception of a frame or error/timeout. See NOTE 8 below. */
    do {
      status = DWT_GetSysStatus();
    } while (!(status & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)));

    if (status & SYS_STATUS_RXFCG) {

      /* Clear good RX frame event in the DW1000 status register. */
      DWT_SetSysStatus(SYS_STATUS_RXFCG);

      /* A frame has been received, read it into the local buffer. */
      frameLen = DWT_ReadData32(DW1000_RX_FINFO, 0x00) & 0x000003FF;
      if (frameLen <= 1024) {
        DWT_ReadRxData(rx_resp_buffer, frameLen, 0);
      }

      /* Check that the frame is the expected response from the companion "DS TWR responder" example.
       * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
      rx_resp_buffer[ALL_MSG_SN_IDX] = 0;
      if (memcmp(rx_resp_buffer, rx_poll_msg, ALL_MSG_COMMON_LEN) == 0) {

        /* Retrieve poll reception timestamp. */
        poll_rx_ts = get_rx_timestamp_u64();

        /* Set send time for response. See NOTE 9 below. */
        resp_tx_time = (poll_rx_ts + (POLL_RX_TO_RESP_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
        DWT_SetDelayedTxRxTime(resp_tx_time);

        /* Set expected delay and timeout for final message reception. See NOTE 4 and 5 below. */
        DWT_SetRxAfterTxDelay(RESP_TX_TO_FINAL_RX_DLY_UUS);
        DWT_SetRxTimeout(FINAL_RX_TIMEOUT_UUS);

        /* Write and send the response message. See NOTE 10 below.*/
        tx_resp_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
        DWT_WriteTxData(sizeof(tx_resp_msg), tx_resp_msg, 0); /* Zero offset in TX buffer. */
        DWT_WriteTxFCtrl(sizeof(tx_resp_msg), 0, 1); /* Zero offset in TX buffer, ranging. */
        status = DWT_StartTx(DWT_TX_DELAYED | DWT_RESPONSE);

        /* If dwt_starttx() returns an error, abandon this ranging exchange and proceed to the next one. See NOTE 11 below. */
        if (status == HAL_ERROR) {
          continue;
        }

        /* Poll for reception of expected "final" frame or error/timeout. See NOTE 8 below. */
        do {
          status = DWT_GetSysStatus();
        } while (!(status & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)));

        /* Increment frame sequence number after transmission of the response message (modulo 256). */
        frame_seq_nb++;

        if (status & SYS_STATUS_RXFCG) {
          LED_R_Toggle();
          /* Clear good RX frame event and TX frame sent in the DW1000 status register. */
          DWT_SetSysStatus(SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);

          /* A frame has been received, read it into the local buffer. */
          frameLen = DWT_ReadData32(DW1000_RX_FINFO, 0x00) & 0x0000007FUL;
          if (frameLen <= RX_RESP_BUF_LEN) {
            DWT_ReadRxData(rx_resp_buffer, frameLen, 0);
          }

          /* Check that the frame is a final message sent by "DS TWR initiator" example.
           * As the sequence number field of the frame is not used in this example, it can be zeroed to ease the validation of the frame. */
          rx_resp_buffer[ALL_MSG_SN_IDX] = 0;
          
          if (checkPacket(rx_resp_buffer, rx_final_msg) == 0) {
            uint32_t poll_tx_ts, resp_rx_ts, final_tx_ts;
            uint32_t poll_rx_ts_32, resp_tx_ts_32, final_rx_ts_32;
            float64_t Ra, Rb, Da, Db;
            int64_t tof_dtu;

            /* Retrieve response transmission and final reception timestamps. */
            resp_tx_ts  = get_tx_timestamp_u64();
            final_rx_ts = get_rx_timestamp_u64();

            /* Get timestamps embedded in the final message. */
            final_msg_get_ts(&rx_resp_buffer[FINAL_MSG_POLL_TX_TS_IDX], &poll_tx_ts);
            final_msg_get_ts(&rx_resp_buffer[FINAL_MSG_RESP_RX_TS_IDX], &resp_rx_ts);
            final_msg_get_ts(&rx_resp_buffer[FINAL_MSG_FINAL_TX_TS_IDX], &final_tx_ts);

            /* Compute time of flight. 32-bit subtractions give correct answers even if clock has wrapped. See NOTE 12 below. */
            poll_rx_ts_32 = (uint32_t)poll_rx_ts;
            resp_tx_ts_32 = (uint32_t)resp_tx_ts;
            final_rx_ts_32 = (uint32_t)final_rx_ts;
            Ra = (float64_t)(resp_rx_ts - poll_tx_ts);
            Rb = (float64_t)(final_rx_ts_32 - resp_tx_ts_32);
            Da = (float64_t)(final_tx_ts - resp_rx_ts);
            Db = (float64_t)(resp_tx_ts_32 - poll_rx_ts_32);
            tof_dtu = (int64_t)((Ra * Rb - Da * Db) / (Ra + Rb + Da + Db));

            tof = tof_dtu * DWT_TIME_UNITS;
            distance = tof * SPEED_OF_LIGHT;

            /* Display computed distance on LCD. */
            printf("DIST: %3.2f m\r\n", distance);
          }
        }
        else {
          /* Clear RX error/timeout events in the DW1000 status register. */
          DWT_SetSysStatus(SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

          /* Reset RX to properly reinitialise LDE operation. */
          DWT_RxReset();
        }
      }
    }
    else {
      /* Clear RX error/timeout events in the DW1000 status register. */
      DWT_SetSysStatus(SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

      /* Reset RX to properly reinitialise LDE operation. */
      DWT_RxReset();
    }
  }
}

/*************************************** END OF FILE ****************************************/
