
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

//#define newPid

#ifdef newPid
#define pidStepDelays 101  //53

#define kTotal  0.2
#define kPartial   0.3     //  0.15
#define kIntegral    0.05
#define kDerivativ    0.0
#define error_thresh 8.0

#else
#define pidStepDelays  711  // unit is 1/1000 sec = 1 mSec,
							// eg. 20 means approx 2 per second, 80 means 1 all 2 seconds
//#define pidStepDelays  787  // unit is 1/1000 sec = 1 mSec,
//#define pidStepDelays pidStepDelays337
//#define pidStepDelays 251

#define kTotal  0.2
#define kPartial   0.3     //  0.15
#define kIntegral    0.05
#define kDerivativ    0.0
#define error_thresh 8.0

#endif


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
