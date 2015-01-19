
/******************************************************************************
* File: CAN.c
*
* Purpose: Library file containing CAN functions
* Rev: 1.0.110513
*
* Author:  Langley Hodgson		Date:11/05/13
*
******************************************************************************/
//================================================================
//Libraries
//================================================================
#include "itct_hcs12.h"
#include "CAN.h"
//================================================================
//Global Variables
//================================================================

static volatile uint8_t rxdata[DATA_LENGTH];
static uint8_t engine_flag=1;
static uint16_t last_steps=1024;
volatile int8_t  CANIDAC_reg;
int8_t CAN_error;


//================================================================
//Functions
//================================================================

void Engine_ON(void){
 engine_flag = 0xFF;
 CAN_error = CAN_Send(STD_ID(ID100),0x00,sizeof(int8_t), &engine_flag);
 PTS |=PTS_PTS3_MASK;  //TURN LED ON HERE  
}

void Engine_OFF(void){
  engine_flag = 0x00;
  CAN_error = CAN_Send(STD_ID(ID100),0x00,sizeof(int8_t), &engine_flag);
  CLR_BITS(PTS,PTS_PTS3_MASK);  
}

void CAN_Gas (uint8_t resolution, uint16_t steps){
  uint8_t txbuff[5];
  last_steps=steps;
  txbuff[0]=0xFF;
  txbuff[1]= resolution;
  txbuff[2]= (steps>>8) & 0xFF;
  txbuff[3]= steps & 0xFF;
  CAN_error = CAN_Send(STD_ID(ID101),0x00,sizeof(txbuff), txbuff);
}
  
void CAN_Motor_Report(uint32_t distance, uint32_t speed){
  int8_t distbuff[4]={0};
  int8_t speedbuff[4]={0};
  uint8_t txbuff[8];
  
   distbuff[0] = (distance >> 24);
   distbuff[1] = (distance >> 16) & 0xFF;
   distbuff[2] = (distance >>  8) & 0xFF;
   distbuff[3] =  distance        & 0xFF;
   
   speedbuff[0] = (speed >> 24);
   speedbuff[1] = (speed >> 16) & 0xFF;
   speedbuff[2] = (speed >>  8) & 0xFF;
   speedbuff[3] =  speed        & 0xFF;
  
  txbuff[0]=distbuff[0];
  txbuff[1]=speedbuff[0];
  txbuff[2]=distbuff[1];
  txbuff[3]=speedbuff[1];
  txbuff[4]=distbuff[2];
  txbuff[5]=speedbuff[2];
  txbuff[6]=distbuff[3];
  txbuff[7]=speedbuff[3];
  CAN_error = CAN_Send(STD_ID(ID102),0x00,sizeof(txbuff), txbuff);
}

void Notification_ON(void){
  uint8_t flag = 0xFF;
  CAN_error = CAN_Send(STD_ID(ID104),0x00,sizeof(int8_t), &flag);  
}

int8_t ID_hit(void){
  CANIDAC_reg = (CANIDAC&CANIDAC_ID_FIELD);
  return  CANIDAC_reg;
}




//Function:init_CAN
//Purpose:Initialize can module 
//Inputs:  
//Outputs: 
void init_CAN(int8_t ID){
  CANCTL0 =  CANCTL0_INITRQ_MASK; //Enter initialization mode
  while (BIT_IS_CLR(CANCTL1,CANCTL1_INITAK_MASK)) {}; //wait for initialization mode, 
                                                      //ack initrq bit = 1
  //now in init mode, set things up 
  CANCTL1 =  (CANCTL1_CANE_MASK /*| CANCTL1_LOOPB_MASK*/);  //enable can module and loopback mode
  
  //Set baudrate prescaler, and time quanta jumps.
  CANBTR0 = (CANBTR0_SJW1_MASK | CANBTR0_SJW0_MASK | CANBTR0_BRP1_MASK | CANBTR0_BRP0_MASK); //0xC3 Set 4 TQ jump widths, Baudrate prescaler by 4
  CANBTR1 = (CANBTR1_TSEG21_MASK | CANBTR1_TSEG20_MASK | CANBTR1_TSEG13_MASK | CANBTR1_TSEG11_MASK); //0x3A; //tseg2 4TQ and tseg1 11TQ  
  
  //set up filters
  //CANIDARx is the id acceptance register,
  //CANIDMRx is the identifier mask register, 
  CANIDAC = (CANIDAC_IDAM0_MASK); // Set four 16-bit Filters
  
  if (ID==VCI){
    CANIDAR0 = ACC_CODE_ID100_HIGH;            //|\    16-bit Filter 0
    CANIDMR0 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR1 = ACC_CODE_ID100_LOW;             //| /   with ID 0x100
    CANIDMR1 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR2 = ACC_CODE_ID102_HIGH;            //|\    16-bit Filter 1
    CANIDMR2 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR3 = ACC_CODE_ID102_LOW;             //| /   with ID 0x100
    CANIDMR3 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR4 = 0x00;                           //|\    16-bit Filter 2
    CANIDMR4 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR5 = 0x00;                           //| /   with ID 0x100
    CANIDMR5 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR6 = 0x00;                           //|\    16-bit Filter 3
    CANIDMR6 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR7 = 0x00;                           //| /   with ID 0x100
    CANIDMR7 = MASK_CODE_ST_ID_LOW;            //|/
  } 
  
  else if (ID==EMS){
    CANIDAR0 = ACC_CODE_ID100_HIGH;            //|\    16-bit Filter 0
    CANIDMR0 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR1 = ACC_CODE_ID100_LOW;             //| /   with ID 0x100
    CANIDMR1 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR2 = ACC_CODE_ID101_HIGH;            //|\    16-bit Filter 1
    CANIDMR2 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR3 = ACC_CODE_ID101_LOW;            //| /   with ID 0x100
    CANIDMR3 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR4 = 0x00;                           //|\    16-bit Filter 2
    CANIDMR4 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR5 = 0x00;                           //| /   with ID 0x100
    CANIDMR5 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR6 = 0x00;                           //|\    16-bit Filter 3
    CANIDMR6 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR7 = 0x00;                           //| /   with ID 0x100
    CANIDMR7 = MASK_CODE_ST_ID_LOW;            //|/
  }
  
    else if (ID==AMS){
    CANIDAR0 = ACC_CODE_ID100_HIGH;            //|\    16-bit Filter 0
    CANIDMR0 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR1 = ACC_CODE_ID100_LOW;             //| /   with ID 0x100
    CANIDMR1 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR2 = ACC_CODE_ID104_LOW;             //|\    16-bit Filter 1
    CANIDMR2 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR3 = ACC_CODE_ID104_LOW;             //| /   with ID 0x100
    CANIDMR3 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR4 = 0x00;                           //|\    16-bit Filter 2
    CANIDMR4 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR5 = 0x00;                           //| /   with ID 0x100
    CANIDMR5 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR6 = 0x00;                           //|\    16-bit Filter 3
    CANIDMR6 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR7 = 0x00;                           //| /   with ID 0x100
    CANIDMR7 = MASK_CODE_ST_ID_LOW;            //|/
  }
  
  else {
    CANIDAR0 = ACC_CODE_ID100_HIGH;            //|\    16-bit Filter 0
    CANIDMR0 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR1 = ACC_CODE_ID100_LOW;             //| /   with ID 0x100
    CANIDMR1 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR2 = ACC_CODE_ID101_HIGH;                           //|\    16-bit Filter 1
    CANIDMR2 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR3 = ACC_CODE_ID102_LOW;                           //| /   with ID 0x100
    CANIDMR3 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR4 = ACC_CODE_ID102_HIGH;                           //|\    16-bit Filter 2
    CANIDMR4 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR5 = ACC_CODE_ID101_LOW;                           //| /   with ID 0x100
    CANIDMR5 = MASK_CODE_ST_ID_LOW;            //|/

    CANIDAR6 = ACC_CODE_ID104_HIGH;                           //|\    16-bit Filter 3
    CANIDMR6 = MASK_CODE_ST_ID_HIGH;           //| \__ Accepts Standard Data Frame Msg
    CANIDAR7 = ACC_CODE_ID101_LOW;                           //| /   with ID 0x100
    CANIDMR7 = MASK_CODE_ST_ID_LOW;            //|/
  }
  
  
  CANCTL0 = 0x00; // Exit Initialization Mode Request
  while ((CANCTL1&0x00) != 0) {}; // Wait for Normal Mode  
}

//Function:CAN_Send
//Purpose: Send a can message
//Inputs:  
//Outputs:
uint8_t CAN_Send (unsigned long id, unsigned char priority, unsigned char length, unsigned char *txdata) {

uint8_t txbuffer,index;

  if (!CANTFLG)                               //Check to see tx buffer full?
    return -1;
  
  CANTBSEL = CANTFLG;                         // Select the lowest empy buffer
  txbuffer = CANTBSEL;                        //make a copy of selected buffer
  
  //load id into the id register
  *((unsigned long *) ((unsigned long)(&CANTXIDR0))) = id;
  
  for (index=0;index<length;index++){
    *(&CANTXDSR0 + index) = txdata[index];    //load data into buffer register
  }
  
  CANTXDLR = length;                          //set data length
  CANTXTBPR = priority;                       //set priority
  
  CANTFLG = txbuffer;                         // start transmission;
  
  while ( (CANTFLG & txbuffer) != txbuffer);  //wait for EOT
}                                             



//Function:CAN_rx
//Purpose: Reviever interrupt for can message 
//Inputs:  
//Outputs:    
interrupt VectorNumber_Vcanrx void CAN_Rec (void){
 uint8_t length, index, id_hit, rtr;
 
 id_hit = ID_hit(); //find which ID was hit 
 
 
 if(BIT_IS_SET(CANRXIDR1,CANRXIDR1_IDE_MASK)){ //Check if STD or EXT MSG
    //check SRR
    if(BIT_IS_SET(CANRXIDR1,CANRXIDR1_SRR_MASK)){
        //check RTR
        if(BIT_IS_SET(CANRXIDR3,CANRXIDR3_RTR_MASK)){
          //RTR SET
          rtr = TRUE; 
          //Check ID_hit, send message
        } else rtr = FALSE;
    }
 } 
 
 
 else //message is STD, check RTR
 if(BIT_IS_SET(CANRXIDR1,CANRXIDR1_SRR_MASK)) {
    //RTR set
    rtr = TRUE;
    if (id_hit == 0){
      if (engine_flag == 0xFF){
        Engine_OFF();
      } else 
        if (engine_flag == 0){
          Engine_ON();
        }
    }
    if (id_hit == 1){
      CAN_Gas(ADC_RES,last_steps);
      //CAN_Motor_Report(0xba5eba11, 0xba5eba11);  
    }
     //PTS |=PTS_PTS3_MASK;  //TURN LED ON HERE
    //check ID_hit, send message
   }
   
  else rtr = FALSE;
                
  
 
      //RTR not set, get data
 if (rtr == FALSE){ 
        
        length = (CANRXDLR & 0x0F); //get length of data
 
        for (index=0;index<length; index++) {
            rxdata[index] = *(&CANRXDSR0 + index); //fill buffer
        }
 }


 
 CANRFLG = CANRFLG_RXF_MASK; // clear rx flag
}
      
      


