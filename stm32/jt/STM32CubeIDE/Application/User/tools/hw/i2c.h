#ifndef i2c_header
#define i2c_header

#include  <stm32f7xx_hal.h>

//#define i2cUseDma

/*
 *
 *    ATTENTION: i2c, in the current implementation, may only be called from kernel aware code.
 * 		(as nothing else is required as per today   (PN 7 Jul. 2021)
 *
 */

//uint8_t pollForReady(uint8_t adr, uint8_t delay);
uint8_t sendI2cByteArray(uint8_t adr,uint8_t* pString,uint8_t amtChars);
uint8_t receiveI2cByteArray(uint8_t adr,uint8_t* pResultString,uint8_t amtChars);

void initI2c();
//void reInitI2cAfterError();
//void enableI2c();

#endif
