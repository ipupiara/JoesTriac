
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

//#define pidStepDelays  787  // unit is 1/1000 sec = 1 mSec,
//#define pidStepDelays pidStepDelays337
//#define pidStepDelays 251

#define pidStepDelays 101

//extern float zeroAdjustDiffVoltage;

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
void printPIDState();
void updateGradAmps();
extern void calcNextTriacDelay();


//void onEntryIdlePID();

//void onTriacIdleSecondTick_PID();

//real Update(real error);


//extern int8_t stableZeroAdjReached;

//uint16_t  adcValueForAmps (float amps);



//void onIdleAdcTick();

//void checkTWIZeroAdjustMsg();

//int sendZeroAdjustMsg(int8_t jobS);

//int8_t getAndTestZeroAdjustState(int8_t jobS);

//void persistentZeroAdjustSecondTickJob();

//int8_t setAdjustJob(int8_t jobS);




#ifdef  __cplusplus
}
#endif


#endif
