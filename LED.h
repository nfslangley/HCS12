//================================================================
//LED.h
//================================================================
#ifndef _LED_H
#define _LED_H
#include "LED.h"

#include <mc9s12c32.h>

#define LED_BITS_MASK (PTS_PTS2_MASK | PTS_PTS3_MASK)
#define LED_DDR_MASK (DDRS_DDRS2_MASK | DDRS_DDRS3_MASK)
#define LED_BITS_INIT PTS_PTS2_MASK
#define RTI_PERIOD 0x7F
#define LED_DOWNCNT_VAL 8


void init_LED(void);

#endif