/******************************************************************************
* File: LCD.h 
*
* Purpose: LCD header file for the LCD.c file
* Rev: 1.0.080412
*
* Author:  Langley Hodgson		Date:08/04/12
*
******************************************************************************/
#ifndef _LCD_H
#define _LCD_H
#include "utils.h"

//================================================================
//LCD Port Info - HCS12
//================================================================
#define LCD_PORT	PORTA	
#define LCD_PORT_DDR	DDRA
#define LCD_DDR_BITS 0x7F

#define E_BIT	PORTA_BIT4_MASK
#define RS_BIT	PORTA_BIT6_MASK
#define RW_BIT	PORTA_BIT5_MASK
#define LCD_BUS_BITS	0x0F


//================================================================
//LCD General Info
//================================================================
#define LCD_CLEAR_DISP          0b00000001
#define LCD_CURSOR_HOME         0b00000010
#define LCD_CMD_FUNCTION        0b00100000
#define LCD_FUNCTION_4BIT       0b00000000
#define LCD_FUNCTION_2LINES     0b00001000
#define LCD_FUNCTION_5X8FONT    0b00000000
#define LCD_CMD_DISPLAY         0b00001000
#define LCD_DISPLAY_OFF         0b00000000
#define LCD_DISPLAY_ON          0b00000100
#define LCD_DISPLAY_CURSOR      0b00000010
#define LCD_DISPLAY_NOCURSOR    0b00000000
#define LCD_DISPLAY_BLINK       0b00000001
#define LCD_DISPLAY_NOBLINK     0b00000000
#define LCD_CMD_ENTRY           0b00000100
#define LCD_ENTRY_INC           0b00000010
#define LCD_ENTRY_NOSHIFT       0b00000000
#define LCD_LINE_TWO            0b11000000
#define LCD_MAX_BUFSIZ         0x80


//================================================================
//LCD Functions & Macro Functions
//================================================================
uint8_t FIX_BITS(uint8_t var);
void init_LCD(void);
void LCDcmd(uint8_t x);
void LCDdata(uint8_t x);
void LCDclear(void);
void LCDhome(void);
void LCDputc(int8_t cx);
void LCDputs(int8_t *str);
void LCDprintf(int8_t *fmt, ...);
char reverseBits(char bits);

#define LCD_E_HI	SET_BITS(LCD_PORT,E_BIT)
#define LCD_E_LO	CLR_BITS(LCD_PORT,E_BIT)
#define LCD_RS_DR   SET_BITS(LCD_PORT,RS_BIT)
#define LCD_RS_IR   CLR_BITS(LCD_PORT,RS_BIT)
#define LCD_RW_R    SET_BITS(LCD_PORT,RW_BIT)
#define LCD_RW_W    CLR_BITS(LCD_PORT,RW_BIT)

#define LCD_BUS(var)	FORCE_BITS(LCD_PORT,LCD_BUS_BITS,(var)) //use for correctly wired LCD

//#define LCD_BUS(var)	FORCE_BITS(LCD_PORT,LCD_BUS_BITS,(FIX_BITS(var))).

    

#endif