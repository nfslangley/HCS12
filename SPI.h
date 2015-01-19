/******************************************************************************
* File: Spi.h
*
* Purpose: Header for SPI.c File
* Rev: 1.0.080412
*
* Author:  Langley Hodgson		Date:08/04/12
*
******************************************************************************/
#ifndef _SPI_H
#define _SPI_H
#include "itct_hcs12.h"


//================================================================
//SPI PORT INFO -HCS12
//================================================================
#define initSPICR1 (SPICR1_SPE_MASK|SPICR1_MSTR_MASK)  //Set as master,enable, phase select
#define baudSPI 0x02
#define SPI_CS_DDR1_BITS (DDRB_BIT7_MASK|DDRB_BIT6_MASK|DDRB_BIT5_MASK)
#define SPI_CS_DDR2_BITS (DDRJ_DDRJ7_MASK)
#define SPI_DDR1 DDRB
#define SPI_DDR2 DDRJ
//================================================================
//CS Macros & Defines
//================================================================ 
#define CS_HI CLR_BITS(PTJ,PTJ_PTJ7_MASK)

#define CS_A PORTB_BIT7_MASK
#define CS_B PORTB_BIT6_MASK
#define CS_C PORTB_BIT5_MASK 
#define CS_MASK 0xE0

//================================================================
//SPI Functions and MACRO Functions
//================================================================
void init_SPI(void);
void putcSPI(uint8_t cx);
char getcSPI(void);
uint8_t FIX_CS(uint8_t var);
void CS_LO(uint8_t cs);
void write_DAC(uint8_t iCommand, uint8_t iData);
void init_DAC(void);
void sawtooth_wave(uint8_t iFreq);
#define CS_PORT(cs) FORCE_BITS(PORTB,CS_MASK,((FIX_CS(cs)<<5)&CS_MASK))


void SPI_DISP(void);
void putchar_frame(char *message, int position, char color);  

//================================================================
//DAC Macros & Defines
//================================================================
#define LOAD_DACA_INPUT 0b00010000 //followed by 8 bit data and S3-S0 = 0000
#define LOAD_DACB_INPUT 0b00100000 //followed by 8 bit data and S3-S0 = 0000
#define LOAD_DACAB_REG  0b10000000 //followed by 8 bit data and S3-S0 = 0000
#define LOAD_DACA_REG   0b10010000 //followed by 8 bit data and S3-S0 = 0000
#define LOAD_DACB_REG   0b10100000 //followed by 8 bit data and S3-S0 = 0000
#define STANDBY_DAC     0b11000000 //followed by D7(1) & D6(1) for internal volage reference S3-0 = 0000
#define NORM_OP_DAC     0b11010000 //followed by D7(1) & D6(1) for internal volage reference S3-0 = 0000
#define SHUTDOWN_DAC    0b11100000 //followed by D7(1) & D6(1) for internal volage reference S3-0 = 0000
//RUN_DAC loads input and shift regs
#define RUN_DAC         0b11110000 //followed by 8 bit data and S3-S0 = 0000
#define S_MASK          0b00000000

#define DAC_VREF        0b11000000

//================================================================
//LED Matrix Colours
//================================================================
#define BLACK	0x00
#define RED		0xE0
#define GREEN	0x1C
#define BLUE	0x07
#define YELLOW	0xFC
#define MAGENTA 0XE3
#define TEAL	0x1F
#define WHITE	0xFF

#endif