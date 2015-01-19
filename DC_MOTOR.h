/******************************************************************************
* File: DC_MOTOR.h 
*
* Purpose: Supporting file for DC motor and encoder operations
* Rev: 1.0.090412
*
* Author:  Langley Hodgson		Date:08/04/12
*
******************************************************************************/
#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H
#include "DC_MOTOR.h"

#define uMETER_PER_SEC 271
#define nMETER_PER_SEC 2741

void motor(uint8_t imotor, uint8_t iDuty, uint8_t cDirection);
void init_motors(void);

void init_encoders(void);
uint16_t get_period(void);
//uint16_t calc_freq(uint16_t iPer);
//uint16_t calc_speed(uint16_t iPer);

#define MOTOR_DDR_PORT DDRB
#define MORTOR_DDR_BITS   (DDRB_BIT0_MASK|DDRB_BIT1_MASK|DDRB_BIT2_MASK|DDRB_BIT3_MASK)
#define MOTOR_PORT PORTB

#define M1A PORTB_BIT0_MASK
#define M1B PORTB_BIT1_MASK
#define M2A PORTB_BIT2_MASK
#define M2B PORTB_BIT3_MASK
#define MOTOR_PORT_BITS (M1A|M2A|M1B|M2B)

#define MOTOR_EN SET_BITS(PWME,(PWME_PWME5_MASK|PWME_PWME4_MASK))
#define MOTOR_DI SET_BITS(PWME,(PWME_PWME5_MASK|PWME_PWME4_MASK))

#define STOP_MOTOR CLR_BITS(MOTOR_PORT,MOTOR_PORT_BITS)
#endif