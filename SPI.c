/******************************************************************************
* File: Spi.c
*
* Purpose: Library file containing SPI and DAC functions
* Rev: 1.0.080412
*
* Author:  Langley Hodgson		Date:08/04/12
*
******************************************************************************/
//================================================================
//Libraries
//================================================================
#include "itct_hcs12.h"

//================================================================
//Global Variables
//================================================================
uint8_t CS_SELECT[8]={0,4,2,6,1,5,3,7};

//================================================================
//Functions
//================================================================

//Function: FIX_CS();	
//Purpose:  ABC selecters on hardware are reversed, this funtion fixes the 
//           order  
//Inputs:  uint8_t var -- what CS to pull low
//Outputs: uint8_t -- the reveresed bit value
uint8_t FIX_CS(uint8_t var) {
 return CS_SELECT[var];   
}
    
//Function: init_SPI();	
//Purpose:  to initialize the SPI port  
//Inputs:  None
//Outputs: None
void init_SPI(){
    SPIBR=baudSPI;      //set baud rate
    //SET_BITS(SPICR1,initSPICR1);
    FORCE_BITS(SPICR1,0xFF,initSPICR1);  //set as master enable and phase
    SPI_DDR1 = SPI_CS_DDR1_BITS;  //set decoder bits to output
    SPI_DDR2 = SPI_CS_DDR2_BITS;  //set decoder enable to output
    CS_HI; //Pull CS High (disable G1 on decoder)
    
}

//Function:	 CS_LO(uint8_t cs)
//Purpose: To pull a CS port LOW, done through a multiplexor allowing for 8 
//          CS's to be chosen
//Inputs:  uint8_t cs -- which CS should output a LOW
//Outputs: None
void CS_LO(uint8_t cs){
    SET_BITS(PTJ,PTJ_PTJ7_MASK);   //set enable on the decoder
    CS_PORT(cs);                   //set appropriate bits on decoder to choose
                                   //proper CS 
}

//Function: putcSPI(uint8_t cx) 	
//Purpose: To send a byte data out through the SPI port
//Inputs:  uint8_t cx -- the byte of data that will be transfered out of SPI
//Outputs: None
void putcSPI(uint8_t cx) 
{
    char temp;  //garbage collection
    
    while(!(SPISR & SPISR_SPTEF_MASK)); //wait until SPI can send
        SPIDR = cx;                     //...then send
        
    while(!(SPISR & SPISR_SPIF_MASK));  //wait until read full
        temp =SPIDR;                    //...then collect garbage
}



//Function: getcSPI(void)	
//Purpose: To fetch a byte of data from the SPI line
//Inputs:  None
//Outputs: the byte of data recieved   
char getcSPI(void) 
{
    while(!(SPISR & SPISR_SPTEF_MASK)); //wait until SPI can send
        SPIDR=0x00;                     //...send garbage
        
     while(!(SPISR & SPISR_SPIF_MASK)); //wait until buffer full
        return(SPIDR);                  //...then return the contents
}

//Function:	 init_DAC(void)
//Purpose:  To set the DAC to normal operation
//Inputs:  None
//Outputs: None
void init_DAC(void){
    write_DAC(NORM_OP_DAC,DAC_VREF);    //send the command to run the DAC in
                                        //normal operation and specified voltage
    
}
//Function:write_DAC(uint8_t iCommand, uint8_t iData)
//Purpose: To write 16 bits of data on the SPI line to the data.
//          proper break up of the data is done in this command to
//          send the high nybble with cmd and low nybble with second 8 bits
//Inputs:  uint8_t iCommand -- how the DAC will funtion, use RUN_DAC for normal 
//                              operation
//         uint8_t iData -- the data to be send to the DAC                                           
//Outputs: None
void write_DAC(uint8_t iCommand, uint8_t iData) 
{
    uint8_t temp; //garbage collection
    uint8_t DAC_HI = (iCommand|HI_NYBBLE(iData));  //get the high nybble of data and merge with iCommand
    uint8_t DAC_LO = (LO_NYBBLE(iData)<<4)|S_MASK; //set low nybble with S_mask for correct DAC operation
    
    
    CS_LO(7);  //DAC always on CS 7 so pull low
       
    while(!(SPISR & SPISR_SPTEF_MASK)); //write first byte
        SPIDR = DAC_HI;
        
    while(!(SPISR & SPISR_SPIF_MASK));
        temp =SPIDR;
        
     while(!(SPISR & SPISR_SPTEF_MASK)); //write second byte
        SPIDR = DAC_LO;
        
    while(!(SPISR & SPISR_SPIF_MASK));
        temp =SPIDR; 
    CS_HI;   
}
//Function:	sawtooth_wave(uint8_t iFreq)
//Purpose: output a square wave on the DAC
//Inputs:  iFreq the resolution of the square wave... (***NEEDS WORK***)
//Outputs: None
void sawtooth_wave(uint8_t iFreq){
    uint8_t wave; 
    for(wave=0;wave<255;wave+=iFreq){
        write_DAC(RUN_DAC,wave);
    
    }
}