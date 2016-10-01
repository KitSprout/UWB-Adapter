/* #include "algorithms\mathUnit.h" */

#ifndef __MATHUNIT_H
#define __MATHUNIT_H

#include "stm32f4xx.h"
#include "arm_math.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define invSqrtf( iSq )   (1.0f / sqrtf((float)(iSq)))
#define squa( Sq )        (((float)Sq) * ((float)(Sq)))
#define toRad( _mathD )   ((_mathD) * 0.0174532925f)
#define toDeg( _mathR )   ((_mathR) * 57.2957795f)

typedef struct {
  float pitch;
  float roll;
  float yaw;
} eulerAngle_t;
/*====================================================================================================*/
/*====================================================================================================*/
#endif
