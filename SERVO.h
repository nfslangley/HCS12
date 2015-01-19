/******************************************************************************
* File: SERVO.h
*
* Purpose: Header for SERVO.c File
* Rev: 1.0.080412
*
* Author:  Langley Hodgson		Date:08/04/12
*
******************************************************************************/
#ifndef _SERVO_H
#define _SERVO_H
#include "SERVO.h"
#include "itct_hcs12.h"


//================================================================
//General Stepper Info
//================================================================
#define SERVO_PULSE     2500 //20ms/(prescale/eclk)
#define SERVO_ANGLE_MIN -90
#define SERVO_ANGLE_MAX 90
#define US_PER_DEG      10      //uS per degree
#define SERVO_OFFSET    1500    //initial offset to 0 Deg signed or 90 unsigned
#define PRESCALE_DIVSOR 800/64 // 800/64 to avoid floating math and set for prescale of 64
#define SERVO_FLOAT_CORRECTION 100
#define SERVO_FLOAT_ERROR   8

//================================================================
//Stepper Functions and Macro Functions
//================================================================
void init_Servo(void);
void servo_pos(int16_t iDegree);
void servo_sweep(void);
#endif


                                          