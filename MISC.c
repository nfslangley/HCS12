//MISC
#include "itct_hcs12.h"
#define delay_count 125
//Usage:	ms_delay(10);
//Purpose:	General Delay
//Inputs:	uint8_t x->amount of time to delay
//Outputs:	None
void ms_delay(uint16_t x) {
    
    uint16_t ik;            //count for number of ms to delay
    
    TC7 = TCNT + delay_count;      //preset TC7 for first event
    MAKE_CHNL_OC(7); //Set TC7 for OC and go
    
    for (ik=0;ik<x;ik++){     //delay for x times
        while(BIT_IS_CLR(TFLG1,TFLG1_C7F_MASK));  //wait for event
            TC7+=delay_count;         //set again for 1 ms
        }
    MAKE_CHNL_IC(7);   //done, turn off TC7
}
    
    
void init_timer_module(void){
    SET_BITS(TSCR1,TSCR1_INIT);  //enable timer module, fast clear and freeze
    SET_BITS(TSCR2,TSCR2_INIT);  //prescale by predefined value
    TFLG1 = 0xFF;        //clear pending     
}