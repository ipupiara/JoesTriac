
#ifndef triacPIDh
#define triacPIDh

#include<stdint.h>
#include <math.h>

#ifdef  __cplusplus
extern "C"
{
#endif
// Select 'double' or 'float' here:
typedef double real;

#define pidStepDelays  499  // unit is 1/1000 sec = 1 mSec,
							// eg. 20 means approx 2 per second, 80 means 1 all 2 seconds
//#define pidStepDelays  787  // unit is 1/1000 sec = 1 mSec,
//#define pidStepDelays pidStepDelays337
//#define pidStepDelays 251



#define kTotal (float) 1.0 * ((float)1000.0 / (float)499.0)  //  used to compensate a changes in cycle time pidStepDelays,
								// the higher cycle frequency the lower kTotal
#define kPartial   1.0     //  0.15
#define kIntegral  0.2  // 1.0       //  0.2
#define kDerivativ    0.0      //  0.2
#define integral_thres 4.0    //  5.0
#define correctionThreshold  80.0   //10 .0   // good to use at least for inductive loads where too high changes can lead to short-circuit


float getCurrentAmpsValue();
void startAmpsADC();
void stopAmpsADC();
void adcValueReceived(uint16_t adcVal);
uint32_t getCurrentAmpsADCValue();
void setCurrentAmpsADCValueNonIsr(uint32_t adcV );
float adcVoltage();

void InitPID();
void resetPID();
void startTriacPidRun();
void stopTriacPidRun();

void calcNextTriacDelay(uint8_t pidOn);

#ifdef  __cplusplus
}
#endif


#endif
