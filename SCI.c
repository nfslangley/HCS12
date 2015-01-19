/******************************************************************************
* File: SCI.c
*
* Purpose: Library file containing Servo functions
* Rev: 1.0.080412
*
* Author:  Langley Hodgson		Date:08/04/12
*
******************************************************************************/
//================================================================
//Libraries
//================================================================
#include "SCI.h"
#include <mc9s12c32.h>
#include "utils.h"
//================================================================
//Global Variables
//================================================================
volatile int8_t clean_status;
volatile uint8_t nextProduced, nextConsumed;
volatile uint8_t SCIbuffer[MAX_BUFF_SIZE];
volatile uint16_t count, in, out;

//================================================================
//Functions
//================================================================

//Function: init_sci()	
//Purpose: To initialize the SCI port and set proper register values
//Inputs: None 
//Outputs: None
void init_SCI(void) {
    char clean_comm;
    SCIBD = BAUD_CLOCK_VALUE; //ECLK/16/baud rate (set in SCI.h)
    SCICR1 = N81;             // no parity 8 bits 1 start/stop       
    SET_BITS(SCICR2,(SCICR2_RE_MASK|SCICR2_TE_MASK|SCICR2_RIE_MASK)); //enable interupts and turn on interrupts
    clean_comm = SCISR1;    //Clear out any comm errors
    clean_comm = SCIDRL;
}

//Function: putcSCI(int8_t cx)	
//Purpose: Put a character onto the SCI line
//Inputs: int8_t cx -- The data going over serial 
//Outputs: None
void putcSCI(int8_t cx) {
    while (BIT_IS_CLR(SCISR1, SCISR1_TDRE_MASK)); //While transmit not ready, wait
        SCIDRL = cx;                              //Send away
}

//Function: putsSCI(int8_t *str)	
//Purpose: To put a string of character through the SCI line
//Inputs: None 
//Outputs: None
void putsSCI(int8_t *str) {
    while(*str !=0){
        putcSCI(*str++);
    }
}

//Function: 	
//Purpose: 
//Inputs: None 
//Outputs: 
char getcSCI(void) {
    while(BIT_IS_CLR(SCISR1,SCISR1_RDRF_MASK));
        return(SCIDRL);
} 
        


//Function: 	
//Purpose: 
//Inputs: None 
//Outputs:  
int8_t SCIdequeue(void){
    if(count == 0){
       return -1;
    } else
    START_CRITICAL;
    nextConsumed = SCIbuffer[out];
    out= (out+1)%MAX_BUFF_SIZE;
    count--;
    END_CRITICAL;
    return nextConsumed;
    
}
   
//Function: 	
//Purpose: 
//Inputs: None 
//Outputs: None        
interrupt VectorNumber_Vsci void SCI_REC(void) {
    clean_status = SCISR1;
    nextProduced=SCIDRL;    
    while(count == MAX_BUFF_SIZE);
    SCIbuffer[in] = nextProduced;
    in = (in+1)%MAX_BUFF_SIZE;
    count++;
    
}