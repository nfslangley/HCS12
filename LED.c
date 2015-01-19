//================================================================
//LED.c
//================================================================
#include "itct_hcs12.h"


void init_LED(void){
    CRGFLG = CRGFLG_RTIF_MASK;  //Clear any pending interrupts
    CRGINT |=CRGINT_RTIE_MASK;  // enable RTI interrupt
    RTICTL = RTI_PERIOD;        // set RTI period
    COPCTL |= CRGFLG_RTIF_MASK; // freeze RTI during BDM active
    
    PTS=LED_BITS_INIT;               //Set one led ON
    DDRS=LED_DDR_MASK;             //Set bits 2&3 to output
    
}


interrupt VectorNumber_Vrti void LEDHEART(void) {
  static char count= LED_DOWNCNT_VAL;;
  CRGFLG = CRGFLG_RTIF_MASK; 
  count--;
  if(count==0) {
  PTS ^=LED_BITS_MASK;
  count = LED_DOWNCNT_VAL;
  
  }
}

