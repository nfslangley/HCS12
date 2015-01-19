/******************************************************************************
* File: Stepper.h
*
* Purpose: Header for Stepper.c File
* Rev: 1.0.080412
*
* Author:  Langley Hodgson		Date:08/04/12
*
******************************************************************************/
#ifndef _STEPPER_H
#define _STEPPER_H
#include "itct_hcs12.h"


//================================================================
//Stepper Port Info - HCS12
//================================================================ 
#define STEPPERPORT         PTT
#define STEPPERPORT_DDR     DDRT
#define STEPPERPORT_BITS    (PTT_PTT4_MASK|PTT_PTT5_MASK|PTT_PTT6_MASK|PTT_PTT7_MASK)
#define STEPPERPORT_DDRBITS (DDRT_DDRT4_MASK|DDRT_DDRT5_MASK|DDRT_DDRT6_MASK|DDRT_DDRT7_MASK)

#define LIMIT_SW_RGHT_DIG   ATDDIEN_IEN7_MASK
#define LIMIT_SW_LEFT_DIG   ATDDIEN_IEN6_MASK
#define LIMIT_SW_PORT   PORTAD0
#define LIMIT_SW_RGHT   PORTAD0_PTAD7_MASK
#define LIMIT_SW_LEFT   PORTAD0_PTAD6_MASK  

//================================================================
//General Stepper Info
//================================================================ 
#define A_BAR 0b00100000
#define A_REG 0b00010000
#define B_BAR 0b10000000
#define B_REG 0b01000000

#define PATTERN0            (A_REG)
#define PATTERN1            (A_REG|B_REG)
#define PATTERN2            (B_REG)
#define PATTERN3            (A_BAR|B_REG)
#define PATTERN4            (A_BAR)
#define PATTERN5            (A_BAR|B_BAR)
#define PATTERN6            (B_BAR)
#define PATTERN7            (A_REG|B_BAR)

#define LIMIT_SPEED 8
#define RGHT_FULL   2
#define LEFT_FULL   -2
#define RGHT_HALF   1
#define LEFT_HALF   -1

#define STEP_DELAY_MIN 5

//================================================================
//Stepper Functions and Macro Functions
//================================================================ 
void stepper_movement(uint8_t iDelay, int8_t iStep);
void stepper_xsteps(uint8_t iNumSteps, int8_t iStep);
void init_stepper(void);
void stepper_home(void);
uint16_t stepper_pos(void);

#define STOP_STEPPER CLR_BITS(STEPPERPORT,STEPPERPORT_BITS)

#endif