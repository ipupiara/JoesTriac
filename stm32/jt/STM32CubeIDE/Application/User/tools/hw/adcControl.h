#ifndef  adcControlDotH
#define adcControlDotH

#ifdef  __cplusplus
extern "C"
{
#endif


#include<stdint.h>
#include <math.h>



void initAdc();
void startADC();
void stopADC();

float currentAmps();
uint32_t getCurrentAmpsADCValue();

void setCurrentAmpsADCValue(uint32_t adcV );

#ifdef  __cplusplus
}
#endif


#endif
