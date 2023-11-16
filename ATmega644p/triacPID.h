
#ifndef triacPIDh
#define triacPIDh

#include<stdint.h>
#include <math.h>
#include "triacUI.h"

// Select 'double' or 'float' here:
typedef double real;


#define pidStepDelays  30  // unit is (1/42.18) sec, 
							// eg. 20 means approx 2 per second, 80 means 1 all 2 seconds


float zeroAdjustDiffVoltage;

void InitPID();

void resetPID();

void printPIDState();

void onEntryIdlePID();

void onTriacIdleSecondTick_PID();

real Update(real error);

double currentAmpsValue;

int8_t stableZeroAdjReached;

uint16_t  adcValueForAmps (float amps);

void updateGradAmps();

//void onIdleAdcTick();

void checkTWIZeroAdjustMsg();

//int sendZeroAdjustMsg(int8_t jobS);

//int8_t getAndTestZeroAdjustState(int8_t jobS);

void persistentZeroAdjustSecondTickJob();

int8_t setAdjustJob(int8_t jobS);


void calcNextTriacDelay();


#endif
