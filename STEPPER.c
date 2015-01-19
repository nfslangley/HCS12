/******************************************************************************
* File: Stepper.c
*
* Purpose: Library containing functions for stepper movement
* Rev: 1.0.080412
*
* Author:  Langley Hodgson		Date:08/04/12
*
******************************************************************************/
//================================================================
//Libraries
//================================================================
#include "STEPPER.h"

//================================================================
//Global Variables
//================================================================
uint8_t const step_table[8]={PATTERN0,PATTERN1,PATTERN2,PATTERN3,PATTERN4,PATTERN5,PATTERN6,PATTERN7};
volatile uint8_t step_speed=5;  // variable on how many ms to delay per step
volatile int8_t step_type;    // full step half step clockwise ccw? should only ever be (-2,-1,0,1,2)
volatile int16_t step_count; //a global var to keep count of what step in on
volatile uint16_t max_steps; //maximum steps that can be taken
volatile int16_t current_step; //the current step position
volatile uint8_t homed_flag=0; //flag for knowing if step count is to add/subtract or just add

//================================================================
//Functions
//================================================================

//Function: init_stepper()
//Purpose: To initialize the stepper module including timer channels and IO lines
//          this sets the stepper port bits to output and sets a timer channel for 
//          step counts 	
//Inputs: None 
//Outputs:None
void init_stepper(void){
    SET_BITS(STEPPERPORT_DDR,STEPPERPORT_DDRBITS);   //set ddr to output
    STOP_STEPPER;                                    //clear so no movement
    MAKE_CHNL_OC(6);                                 //make timer channel 6 output
    SET_BITS(TIE,TIE_C6I_MASK);                      //enable interrupt  on TC6
    CLR_BITS(TCTL1,(TCTL1_OL6_MASK|TCTL1_OL6_MASK)); //no action on interrupt
    SET_BITS(ATDDIEN,(LIMIT_SW_RGHT_DIG|LIMIT_SW_LEFT_DIG)); //set atd 6 & 7 to digital input for limit switchs
    step_count = 0;                                  //initialize step count 
    TFLG1=0xFF;                                      //clear any flags
    TC6=TCNT+(step_speed*MSDELAY);                   //initial time to 1 ms .. **needed??**
}

//Function: stepper_movement(uint8_t iDelay, int8_t iStep)
//Purpose: To adjust stepper movement settings. a speed in ms to delay per step
//          and what direction and step type  	
//Inputs: uint8_t iDelay -- how man ms to delay between steps
//        uint8_t iStep  -- a value between -2 -> +2 to determine direction and 
//                          step type                           
//Outputs:None
void stepper_movement(uint8_t iDelay, int8_t iStep){
    if(iStep<=RGHT_FULL && iStep>=LEFT_FULL)
      step_type = iStep;        //set half step or full step and direction (+,-,1,2);
    if(iDelay>=STEP_DELAY_MIN)  
      step_speed = iDelay;      //set ms delay per step 
      else
        step_speed = STEP_DELAY_MIN;   
}

//Function: stepper_pos()
//Purpose: To return the current step position on the motor	
//Inputs: None 
//Outputs:uint16_t step_count -- the current step count 
uint16_t stepper_pos(void){
    return step_count;
}


//Function: stepper_home()
//Purpose:	To home the stepper to the middle of the robot 
//          this moves the stepper full one way, clears the step count,
//          goes full the other way and returns to half way of counted steps
//
//Inputs:   None
//Outputs:  None
void stepper_home(void){
    
    uint16_t home_count;                            //get a middle of robot var
    homed_flag=0;
    stepper_movement(LIMIT_SPEED,LEFT_HALF);        //move in one direction
    while(BIT_IS_SET(LIMIT_SW_PORT,LIMIT_SW_LEFT)); //sit tight until limit switch found //***CHECK FOR BOTH???****                                                   
    step_count=0;                                   //clear value, at one limit
    stepper_movement(LIMIT_SPEED,RGHT_HALF);        //move in opposite direction
    while(BIT_IS_SET(LIMIT_SW_PORT,LIMIT_SW_RGHT)); //sit tight until other limit hit
    home_count=step_count/2;                        //now know where middle sits
    step_count=0;                                   //clear count
    stepper_movement(LIMIT_SPEED,LEFT_HALF);        //move in opposite direction
    while(step_count<=home_count);                  //until in the middle
    stepper_movement(0,0);                          //stop motors
    STOP_STEPPER;                                   //clear bits to kill current draw
                                                    //***MIGHT NEED TO LEAVE ON FOR DETENT TORQUE***
    homed_flag=1;               
}

//Function: stepper_xsteps(uint8_t iNumSteps, int8_t iStep)
//Purpose:	To move the stepper x amount of steps in specified direction 
//
//Inputs:   uint8_t iNumSteps -- How many steps you would like to move
//          int8_t iStep -- in which direction would you like to move
//Outputs:  None 
void stepper_xsteps(uint8_t iNumSteps, int8_t iStep){
    uint16_t final_count;
    step_speed=25;
    if(iStep == LEFT_FULL | iStep == LEFT_HALF){
        final_count =(step_count-iNumSteps);
        stepper_movement(step_speed,iStep);
        while(step_count>final_count);
        stepper_movement(0,0);
        STOP_STEPPER;
    } else{
        final_count =(step_count+iNumSteps);
        stepper_movement(step_speed,iStep);
        while(step_count<final_count);
        stepper_movement(0,0);
        STOP_STEPPER;
    }
        
}

//Function: Take a step
//Purpose:	To move one step, increment the fact a step has been taken 
//           and reset time till next step
//
//Inputs:   None
//Outputs:  None  
interrupt VectorNumber_Vtimch6 void move_stepper(void){
    static uint8_t next_pos;
      
    CLR_BITS(PTT,STEPPERPORT_BITS);         //clear current stepper contents
    SET_BITS(PTT,(step_table[next_pos]));   //set next positions 
    next_pos= (next_pos+step_type)&0x07;    //fetch the next position and mod to stay inside table
    if(homed_flag == 0) 
        step_count++;                       //if not homed just count
    else{                                   //otherwise, homed then one direction in position minus 
                                            //and other is position ++
        if(step_type == LEFT_FULL | step_type == LEFT_HALF)
            step_count--;
        else
            step_count++;
    }
    TC6+=(step_speed*125);                  //set next delay flag
        
}