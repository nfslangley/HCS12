/******************************************************************************
* File: LCD.c 
*
* Purpose: LCD Functions for the hitachi LCD module
* Rev: 1.0.080412
*
* Author:  Langley Hodgson		Date:08/04/12
*
******************************************************************************/


#include "itct_hcs12.h"

uint8_t flip[16] = {0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};   // trances on HW are reveresed, this is a look up table to fix

//Usage:LCDprintf("hello world, %d is awesome", 42);	
//Purpose: To print to an LCD screen 
//			
//			
//Inputs: *string followed by arguments
//
//Outputs: None
void LCDprintf(int8_t *fmt, ...){
    int16_t error;
    va_list args;

  // need to guarantee that buffer big enough for any string
  char buffer[LCD_MAX_BUFSIZ];    

  va_start( args, fmt );  //variables start after fmt
  error = vsprintf( buffer, fmt, args );
  va_end( args );
 
  LCDputs( buffer );     //print buffer 
}

//Function: LCDputc(int8_t cx)	
//Purpose: put one character onto the LCD screen. If a 
//          new line feed or carrige return is found special
//          actions are taken
//						
//Inputs: int8_t cx the character going to the screen
//
//Outputs: None
void LCDputc(int8_t cx){
    if(cx == CR) {   //Deal with carrige return
        LCDhome();
    } else
    if(cx == LF) {   //Deal with new line
        LCDcmd(LCD_LINE_TWO);
        ms_delay(16);
    } else
                     //Send data to LCD Module
    {
        LCDdata(cx);
    }
    
}

//Function: LCDputs(int8_t *str)	
//Purpose: To print a strig to the LCD module
//						
//Inputs:  int8_t *str the string to be printed
//Outputs: None
void LCDputs(int8_t *str) {
    while(*str !=0){     //As long as the string is not NULL
        LCDputc(*str++);   //Put a character and increment the *pointer posistion
    }
}

//Function:	init_LCD(void)
//Purpose: To turn on the LCD and set it for 4 data bits and various other options
//						
//Inputs:  None
//Outputs: None
void init_LCD(void){
    LCD_PORT_DDR = LCD_DDR_BITS;        //Set all lines execpt PortA7 to output.
    LCD_E_HI;
    LCD_BUS( FIX_BITS(0x03));      // wake up display & sync
 
    LCD_E_LO;
    
    ms_delay( 5 );

    LCD_E_HI;
    LCD_BUS( FIX_BITS (0x03) );      // wake up display & sync
  
    LCD_E_LO;

    ms_delay( 1 );   
    
    LCD_E_HI;
    LCD_BUS( FIX_BITS(0x03));      // wake up display & sync
   
    LCD_E_LO;
    
    LCD_E_HI;
    LCD_BUS( FIX_BITS(0x02));      // wake up display & sync - go to 4-bit mode
  
    LCD_E_LO;

	ms_delay( 2 );
    
    LCDcmd( LCD_CMD_FUNCTION | LCD_FUNCTION_4BIT | LCD_FUNCTION_2LINES | LCD_FUNCTION_5X8FONT );
    LCDcmd( LCD_CMD_DISPLAY | LCD_DISPLAY_OFF );
    LCDclear();
    LCDcmd( LCD_CMD_ENTRY | LCD_ENTRY_INC | LCD_ENTRY_NOSHIFT );
    LCDcmd( LCD_CMD_DISPLAY | LCD_DISPLAY_ON | LCD_DISPLAY_NOCURSOR | LCD_DISPLAY_NOBLINK );
    
}

//Function:LCDcmd(uint8_t x) 	
//Purpose: To send a command to the LCD rather than data, only difference between 
//          the two is that the RS line is pulled to instruction
//						
//Inputs:  uint8_t x -- The CMD being sent to LCD module
//Outputs: None
void LCDcmd(uint8_t x){

    LCD_E_LO;
    LCD_RS_IR;
    LCD_RW_W;
    LCD_E_HI;
    LCD_BUS( FIX_BITS(HI_NYBBLE(x)));
    LCD_E_LO;
    LCD_E_HI;
    LCD_BUS( FIX_BITS(LO_NYBBLE(x)));
    LCD_E_LO;
    ms_delay(2);
}

//Function: LCDdata(uint8_t x)	
//Purpose: To send data to the LCD module to write to screen
//						
//Inputs:  uint8_t x -- The data to be written
//Outputs: None
void LCDdata(uint8_t x){

    LCD_E_LO;
    LCD_RS_DR;
    LCD_RW_W;
    LCD_E_HI;
    LCD_BUS(FIX_BITS(HI_NYBBLE(x)));
    LCD_E_LO;
    LCD_E_HI;
    LCD_BUS(FIX_BITS(LO_NYBBLE(x)));
    LCD_E_LO;
    ms_delay(2);

}

//Function:LCDclear(void)	
//Purpose: Clears the LCD screen and sets home position
//						
//Inputs:  None
//Outputs: None
void LCDclear(void){
    LCDcmd(LCD_CLEAR_DISP);
    ms_delay(16);
}

//Function:LCDhome(void)	
//Purpose: Homes the LCD cursor to start posistion
//						
//Inputs:  None
//Outputs: None
void LCDhome(void){
    LCDcmd(LCD_CURSOR_HOME);
    ms_delay(16);
}


//Function:	FIX_BITS(uint8_t var)
//Purpose: To flip the bits that are going to be sent to the data bus
//			this fixes the hardware issue
//Inputs: uint8_t var -- The original Data
//
//Outputs: uint8_t flip[var] -- the data with rearranged bits.	
uint8_t FIX_BITS(uint8_t var) {
 return flip[var];   
}

    