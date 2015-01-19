/******************************************************************************
* File: Servo.c
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
#include "SERVO.h"
//================================================================
//Global Variables
//================================================================
volatile int16_t active_time;

//================================================================
//Functions
//================================================================

//Function: init_servo(); 	
//Purpose: To initialize the RC servo by setting up proper registers
//          on the timer channel 
//Inputs: None 
//Outputs: None
void init_Servo(void)
{
   MAKE_CHNL_OC(2);                 //set channels to proper state
   SET_BITS(TIE,TIE_C2I_MASK);      //set interrupts on channels
   SET_BITS(TCTL2,TCTL2_OL2_MASK);  //set up timer control
   TFLG1 = 0xFF;                    //clear pending
   TC2=TCNT + SERVO_PULSE;          //initial interrupt setup 
    
}

//Function: servo_pos(int16_t iDegree)	
//Purpose: To set the active the will be read by the servo. User enters degree
//          and function converts to active time 
//Inputs: int16_t iDegree -- Degree that user want to set from -90 to +90 
//Outputs: None
void servo_pos(int16_t iDegree){    
        START_CRITICAL;
        active_time=((((iDegree*US_PER_DEG)+SERVO_OFFSET)*(PRESCALE_DIVSOR))/SERVO_FLOAT_CORRECTION)+SERVO_FLOAT_ERROR;
        END_CRITICAL;     
}

//Function: servo_sweep() 	
//Purpose: Move the servo to a position within a specified time 
//Inputs:  
//Outputs:
void servo_sweep(void){       
}

//Function: Send out a pulse train 	
//Purpose: To hold the stepper at a specified angle based on active time 
//Inputs: None 
//Outputs: None
interrupt VectorNumber_Vtimch2 void tChan2(void)
{
    static uint8_t duty_flag=0;
    //On time
    if (duty_flag==1){
        
        TC2 += active_time;             //reset timer to active time length
        duty_flag=0; 
    }
    //Off time 
    else {
        TC2 += SERVO_PULSE-active_time; //run for rest of 20ms period
        duty_flag=1;
    }
    
    
}


