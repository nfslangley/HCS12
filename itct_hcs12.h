#ifndef ITCT_HCS12_H
#define _ITCT_HCS12_H

//================================================================
//HCS12 Header
//================================================================
#include <stdio.h>
#include <mc9s12c32.h>
#include <stdarg.h>
#include "utils.h"
#include "STEPPER.h"
#include "SPI.h"
#include "SERVO.h"
#include "ADC.h"
#include "DC_MOTOR.h"
#include "LCD.h"
#include "LED.h"
#include "SCI.h"
#include "CAN.h"

//================================================================
//General Functions
//================================================================
void ms_delay(uint16_t x);
void init_timer_module(void);

#define START_CRITICAL DisableInterrupts
#define END_CRITICAL EnableInterrupts 

//================================================================
//General Defines
//================================================================
 #define CR 0x0D
 #define LF 0x0A
 
 #define MSDELAY 125
//================================================================
//HCS12 Macros
//================================================================


//================================================================
//Timer Channel
//================================================================
#define MAKE_CHNL_OC( chnl )    SET_BITS( TIOS, 1 << (chnl) )
#define MAKE_CHNL_IC( chnl )    CLR_BITS( TIOS, 1 << (chnl) )
#define TSCR1_INIT (TSCR1_TSFRZ_MASK|TSCR1_TFFCA_MASK|TSCR1_TEN_MASK)	//enable timer module, fast clear and freeze
#define TSCR2_INIT (TSCR2_PR2_MASK|TSCR2_PR1_MASK)						//prescale by predefined value
                                    


//================================================================
//SCI
//================================================================
#define SCI_RCV_ERROR_MASK	((SCISR1_PF_MASK | SCISR1_FE_MASK | SCISR1_NF_MASK | SCISR1_OR_MASK))
#define SCI_HAS_RCV_ERRORS 	BIT_IS_SET( SCISR1,SCI_RCV_ERROR_MASK )

 

//================================================================
//Clocking
//================================================================

#define OSCCLK		16000000L
#define E_CLOCK		(OSCCLK/2)

#endif