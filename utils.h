#ifndef _UTILS_H
#define _UTILS_H

//================================================================
//Macros
//================================================================

#define SET_BITS( port, bits )  ((port) |= (bits))
#define CLR_BITS( port, bits )  ((port) &= LOW(~(bits)))
#define FLIP_BITS( port, bits ) ((port) ^= (bits))
#define FORCE_BITS( port, mask, value ) ((port) = ((port) & ~(mask)) | ((value) & (mask)))
#define BIT_IS_SET( port, bits )   ((port) & (bits))  //test if bit set
#define BIT_IS_CLR( port, bits )  ((~(port))&(bits))  //test if bit clear

#define LOW(var)	((var)&(0xFF))
#define LO_NYBBLE(var)	(0x0F&(var))
#define HI_NYBBLE(var)	(((unsigned)(var)>>4)&0x0F)


typedef char int8_t;
typedef short int16_t;
typedef long int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
  


#endif