/* #include "modules\imu.h" */

#ifndef __IMU_H
#define __IMU_H

#include "stm32f4xx.h"

#include "algorithms\mathUnit.h"
#include "modules\mpu9250.h"
//#include "modules\lps25hb.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
/*
|     |            ACCELEROMETER           |
| LPF |  BandW  |  Delay  | Noise | Sample |
+-----+---------+---------+-------+--------+
|  x  |  1130Hz |  0.75ms | 250Hz |  4kHz  |
|  0  |   460Hz |  1.94ms | 250Hz |  1kHz  |
|  1  |   184Hz |  5.80ms | 250Hz |  1kHz  |
|  2  |    92Hz |  7.80ms | 250Hz |  1kHz  |
|  3  |    41Hz | 11.80ms | 250Hz |  1kHz  |
|  4  |    20Hz | 19.80ms | 250Hz |  1kHz  |
|  5  |    10Hz | 35.70ms | 250Hz |  1kHz  |
|  6  |     5Hz | 66.96ms | 250Hz |  1kHz  |
|  7  |   460Hz |  1.94ms | 250Hz |  1kHz  |
*/
/*
|     |          GYROSCOPE         |    TEMPERATURE    |
| LPF |  BandW  |  Delay  | Sample |  BandW  |  Delay  |
+-----+---------+---------+--------+---------+---------+
|  x  |  8800Hz | 0.064ms |  32kHz |  4000Hz |  0.04ms |
|  x  |  3600Hz |  0.11ms |  32kHz |  4000Hz |  0.04ms |
|  0  |   250Hz |  0.97ms |   8kHz |  4000Hz |  0.04ms |
|  1  |   184Hz |   2.9ms |   1kHz |   188Hz |   1.9ms |
|  2  |    92Hz |   3.9ms |   1kHz |    98Hz |   2.8ms |
|  3  |    41Hz |   5.9ms |   1kHz |    42Hz |   4.8ms |
|  4  |    20Hz |   9.9ms |   1kHz |    20Hz |   8.3ms |
|  5  |    10Hz | 17.85ms |   1kHz |    10Hz |  13.4ms |
|  6  |     5Hz | 33.48ms |   1kHz |     5Hz |  18.6ms |
|  7  |  3600Hz |  0.17ms |   8kHz |  4000Hz |  0.04ms |
*/
/*
| ODR |  Pres  |  Temp  |
+-----+--------+--------+
|  0  | One-Shot Enable |
|  1  |    1Hz |    1Hz |
|  2  |    7Hz |    7Hz |
|  3  | 12.5Hz | 12.5Hz |
|  4  |   25Hz |   25Hz |
|  5  |     Reserved    |
*/


typedef struct {
  float32_t gyrRaw[3];      /* x = raw[0], y = raw[1], z = raw[2] */
  float32_t accRaw[3];
  float32_t magRaw[3];
  float32_t ictempRaw;
  float32_t baroRaw[2];     /* t = raw[0], p = raw[1] */

  float32_t gyrData[3];
  float32_t accData[3];
  float32_t magData[3];
  float32_t ictempData;
  float32_t baroData[2];

  float32_t accMotion[3];

  float32_t gyrScale[3];
  float32_t accScale[3];
  float32_t magScale[3];
  float32_t ictempScale;
  float32_t baroScale[2];

  float32_t gyrCalib[3];
  float32_t accCalib[9];
  float32_t magCalib[9];    /* c11 = calib[0], c12 = calib[1], c13 = calib[2],
                               c21 = calib[3], c22 = calib[4], c23 = calib[5],
                               c31 = calib[6], c32 = calib[7], c33 = calib[8] */

  float32_t gyrOffset[3];
  float32_t accOffset[3];
  float32_t magOffset[3];
  float32_t ictempOffset;

  float32_t accStrength;
  float32_t magStrength;

} __attribute__((aligned(4))) IMU_DataTypeDef;

typedef struct {
  MPU_ConfigTypeDef Init;
//  LPS_ConfigTypeDef InitLPS;
  IMU_DataTypeDef *Data;
} IMU_InitTypeDef;
/*=====================================================================================================*/
/*=====================================================================================================*/
void    IMU_Config( void );
int8_t  IMU_Init( IMU_InitTypeDef *IMUx );
int8_t  IMU_DeviceCheck( IMU_DataTypeDef *pIMU );

void    IMU_InitData( IMU_DataTypeDef *pIMU );
void    IMU_SetSensitivity( IMU_InitTypeDef *IMUx );
void    IMU_GetRawData( IMU_DataTypeDef *pIMU );
void    IMU_GetCalibData( IMU_DataTypeDef *pIMU );
void    IMU_GetScaleData( IMU_DataTypeDef *pIMU );
void    IMU_PrintData( IMU_DataTypeDef *pIMU );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
