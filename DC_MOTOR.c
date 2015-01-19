/******************************************************************************
* File: DC_MOTOR.c 
*
* Purpose: Library file for DC motor and encoder operations
* Rev: 1.0.090412
*
* Author:  Langley Hodgson		Date:08/04/12
*
*Lacking in PI and PID Controller
******************************************************************************/
#include "itct_hcs12.h"

uint16_t num_vanes;
uint16_t period;
uint8_t dc_period = 255;

//Function:init_motors();	
//Purpose: To initialize the motor ports and set up proper PWM channels 
//Inputs:  None
//Outputs: None
void init_motors(void){
    CLR_BITS(PWMCTL,(PWMCTL_PSWAI_MASK|PWMCTL_PFRZ_MASK)); //Set 8 bit mode, dont stop in wait and freeze 
    SET_BITS(PWMPRCLK,(PWMPRCLK_PCKA0_MASK));              //Prescale A clk by 2
    SET_BITS(PWMPOL,(PWMPOL_PPOL4_MASK|PWMPOL_PPOL5_MASK));//Set 4&5 high at start of period
    CLR_BITS(PWMCLK,(PWMCLK_PCLK4_MASK|PWMCLK_PCLK4_MASK));//4&5 use Clk A as source
    CLR_BITS(PWMCAE,(PWMCAE_CAE4_MASK|PWMCAE_CAE5_MASK));  //left aligned
    PWMPER5=dc_period;                                           //MAXIMUM RESOLUTION!
    PWMPER4=dc_period;
    MOTOR_EN;
    SET_BITS(MOTOR_DDR_PORT,MORTOR_DDR_BITS);//enable motor to outputs
    STOP_MOTOR;                              //make sure none are on
}

//Function:	
//Purpose: 
//Inputs: 
//Outputs: None
void motor(uint8_t imotor, uint8_t iDuty, uint8_t cDirection){
    uint8_t PWDUTY = LOW((iDuty*dc_period)/100);   //get value that will set to spec'd duty cycle
    MOTOR_DI;         //disable motors
    PWMDTY4=PWDUTY;   //set duty for 4&5
    PWMDTY5=PWDUTY;   
    MOTOR_EN;         //enable motors so they are in phase
    if (imotor==1){                     
        if(cDirection == 'f'){          //action if Motor 1 and forward
            SET_BITS(MOTOR_PORT,M1A);
            CLR_BITS(MOTOR_PORT,M1B);
        }
        if(cDirection == 'r'){          //action if Motor 1 and reverse
            SET_BITS(MOTOR_PORT,M1B);
            CLR_BITS(MOTOR_PORT,M1A);
        }
    }
    if (imotor==2){
        if(cDirection == 'f'){          //action if Motor 2 and forward
            SET_BITS(MOTOR_PORT,M2A);
            CLR_BITS(MOTOR_PORT,M2B);
        }
        if(cDirection == 'r'){          //action if Motor 2 and reverse
            SET_BITS(MOTOR_PORT,M2B);
            CLR_BITS(MOTOR_PORT,M2A);
        }
    }
}

//Function:	
//Purpose: 
//Inputs: 
//Outputs: None
void init_encoders(void){
    MAKE_CHNL_IC(0);  //set encoder to input capture
    MAKE_CHNL_IC(1); 
    SET_BITS(TCTL4, (TCTL4_EDG0A_MASK|TCTL4_EDG1A_MASK)); //trigger on rising edge only
    TFLG1= 0xFF;
    
    
}

//Function:	
//Purpose: 
//Inputs: 
//Outputs: None
uint16_t get_period(void) {
    uint16_t edge1;
    
    while(BIT_IS_CLR(TFLG1,TFLG1_C0F_MASK));
        edge1 = TC0;
        
    while(BIT_IS_CLR(TFLG1,TFLG1_C0F_MASK));
        period = (TC0-edge1)*8; 
        return period; 
}
/*
uint16_t calc_freq(uint16_t iPer){
    return (1000000/iPer);  
} 

uint16_t calc_speed(uint16_t iPer){
    return (nMETER_PER_SEC/iPer);
}  */