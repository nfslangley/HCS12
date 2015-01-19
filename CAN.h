/******************************************************************************
* File: CAN.h
*
* Purpose: Header for CAN.c File
* Rev: 1.0.080412
*
* Author:  Langley Hodgson		Date:08/04/12
*
******************************************************************************/
#ifndef _CAN_H
#define _CAN_H
#include "itct_hcs12.h"


#define DATA_LENGTH 8

#define VCI 1
#define EMS 2
#define AMS 3
#define CAN_TEST 0

#define CANIDAC_ID_FIELD 0x07

#define CAN_MODE_EXT 1
#define CAN_MODE_STD 0

/* Acceptance Code Definitions */
#define ACC_CODE_ID100 0x2000
#define ACC_CODE_ID100_HIGH ((ACC_CODE_ID100&0xFF00)>>8)
#define ACC_CODE_ID100_LOW (ACC_CODE_ID100&0x00FF)

#define ACC_CODE_ID101 0x2020
#define ACC_CODE_ID101_HIGH ((ACC_CODE_ID101&0xFF00)>>8)
#define ACC_CODE_ID101_LOW (ACC_CODE_ID101&0x00FF)

#define ACC_CODE_ID102 0x2040
#define ACC_CODE_ID102_HIGH ((ACC_CODE_ID102&0xFF00)>>8)
#define ACC_CODE_ID102_LOW (ACC_CODE_ID102&0x00FF)

#define ACC_CODE_ID103 0x2060
#define ACC_CODE_ID103_HIGH ((ACC_CODE_ID103&0xFF00)>>8)
#define ACC_CODE_ID103_LOW (ACC_CODE_ID103&0x00FF)

#define ACC_CODE_ID104 0x2080
#define ACC_CODE_ID104_HIGH ((ACC_CODE_ID104&0xFF00)>>8)
#define ACC_CODE_ID104_LOW (ACC_CODE_ID104&0x00FF)

/* Mask Code Definitions */
#define MASK_CODE_ST_ID 0x0017
#define MASK_CODE_ST_ID_HIGH ((MASK_CODE_ST_ID&0xFF00)>>8)
#define MASK_CODE_ST_ID_LOW (MASK_CODE_ST_ID&0xFF)

/* ID Definition */
#define STD_ID(id) (id<<21)
#define ID100 0x100L
#define ID101 0x101L
#define ID102 0x102L
#define ID103 0x103L
#define ID104 0x104L
//#define ST_ID_100 0x20000000 // Standard ID 0x100 formatted to be loaded
//* in IDRx Registers in Tx Buffer
//*/

void Engine_ON(void);
void Engine_OFF(void);
void CAN_Gas (uint8_t resolution, uint16_t steps);
void CAN_Motor_Report(uint32_t distance, uint32_t speed);
void Notification_ON(void);
int8_t ID_hit(void);
void init_CAN(int8_t ID);
uint8_t CAN_Send(unsigned long id, unsigned char priority, unsigned char length, unsigned char *txdata );
#endif