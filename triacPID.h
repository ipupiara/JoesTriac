
#ifndef triacPIDh
#define triacPIDh

#include<stdint.h>
#include <math.h>
#include "triacUI.h"

// Select 'double' or 'float' here:
typedef double real;


#define pidStepDelays  30  // unit is 1/42.18 sec, 
							// eg. 20 means approx 2 per second, 80 means 1 all 2 seconds


float zeroAdjustDiffVoltage;

void InitPID();

void resetPID();

//void resetZeroAdj();

void onEntryIdle();

void onIdleSecondTick();

real Update(real error);

// void zeroAdjTest();

double currentAmpsValue;

int8_t stableZeroAdjReached;

/*

uint16_t calibLowCms, calibHighCms;

uint16_t calibLowAdc, calibHighAdc;

void storeCalibLowCms(uint16_t cl);

void storeCalibHighCms(uint16_t ch);

void storeCalibLowAdc(uint16_t cl);

void storeCalibHighAdc(uint16_t ch);

*/

//void onIdleAdcTick();

void checkTWIZeroAdjustMsg();

void sendZeroAdjustMsg(int8_t jobS);

void persistentZeroAdjStep();

void calcNextTriacDelay();


#endif
