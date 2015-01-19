//================================================================
//ADC.c
//================================================================
 #include "ADC.h"

 void init_ADC(void){
    SET_BITS(ATDCTL2,(ATDCTL2_ADPU_MASK|ATDCTL2_AFFC_MASK));//Turn on and enable fast clear
    ms_delay(1);
    SET_BITS(ATDCTL3,(ATDCTL3_S1C_MASK|ATDCTL3_FRZ1_MASK));//1 conversion, stored in same reg, finish then freeze
    SET_BITS(ATDCTL4,(ATDCTL4_SRES8_MASK|ATDCTL4_SMP0_MASK|ATDCTL4_PRS0_MASK));//8 bit operation, 4 sample clocks, prescale by 4 to 2MHz
    }
    
void init_accel(void){
    SET_BITS(DDRP,DDRP_DDRP6);
    SET_BITS(PTP,PTP_PTP6_MASK);//Turn accelerometer on ~ wake from sleep
    ms_delay(1);
    SET_BITS(DDRA,DDRA_BIT7_MASK);//Turn on g select switch
    CLR_BITS(PORTA,PORTA_BIT7_MASK);//set to zero for 1.5g   
}
uint8_t analog(uint8_t iChan){
     
    SET_BITS(ATDCTL5,(/*ATDCTL5_DSGN_MASK|*/(iChan&CHAN_MASK)));//signed value will be in ATDxDRHy
    while(BIT_IS_CLR(ATDSTAT0,ATDSTAT0_SCF_MASK));
        return ATDDR0H;
    
}
 
void read_accel(int8_t *buf){
    
    SET_BITS(ATDCTL3,(ATDCTL3_S1C_MASK|ATDCTL3_S2C_MASK|ATDCTL3_FRZ1_MASK));  //set to three conversions
    SET_BITS(ATDCTL5,(ATDCTL5_DSGN_MASK|ATDCTL5_MULT_MASK|(ACCEL_X&CHAN_MASK)));//signed value will be in ATDxDRHy
    while(BIT_IS_CLR(ATDSTAT0,ATDSTAT0_SCF_MASK));
        buf[0]= ATDDR0H;
        buf[1]= ATDDR1H;
        buf[2]= ATDDR2H;
        
    SET_BITS(ATDCTL3,(ATDCTL3_S1C_MASK|ATDCTL3_FRZ1_MASK));//Set back to 1 conversion
    
} 
    