#ifndef i2c_header
#define i2c_header

#ifdef  __cplusplus
extern "C"
{
#endif

#include  <stm32f7xx_hal.h>
uint8_t sendI2cByteArray(uint8_t adr,uint8_t* pString,uint8_t amtChars);
uint8_t receiveI2cByteArray(uint8_t adr,uint8_t* pResultString,uint8_t amtChars);

void initI2c();
//void reInitI2cAfterError();
//void enableI2c();

#ifdef  __cplusplus
}
#endif


#endif
