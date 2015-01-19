//================================================================
//SCI.h
//================================================================
#ifndef _SCI_H
#define _SCI_H
#include "SCI.h"

#include "utils.h"
#include "itct_hcs12.h"
#define N81 0

#define MAX_BUFF_SIZE 1024
//================================================================
//Clocking
//================================================================


#define BAUD_RATE		9600
#define BAUD_CLOCK_VALUE	(((E_CLOCK)/16.0)/((BAUD_RATE)+0.5))
#define MAX_BAUD_CLOCK	8191        //(2^13 - 1)

#if (BAUD_CLOCK_VALUE > MAX_BAUD_CLOCK)
#error Baud Clock too big
#endif
  
 
//================================================================
//Functions
//================================================================
void init_SCI(void);
void putcSCI(int8_t cx);
void putsSCI(int8_t *str);
char getcSCI(void);
int8_t SCIdequeue(void);


#endif