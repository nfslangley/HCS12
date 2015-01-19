//================================================================
//ADC.h
//================================================================
#ifndef _ADC_H
#define _ADC_H
#include "ADC.h"
#include "itct_hcs12.h"
#include "utils.h"

#define CHAN_MASK 0x07
#define ACCEL_X 0
#define ACCEL_Y 1
#define ACCEL_Z 2
#define ADC_RES 0x08

void init_ADC(void);
void init_accel(void);
void ADCchan_select(uint8_t ix);
uint8_t analog(uint8_t iChan);
void read_accel(int8_t *buf);
#endif