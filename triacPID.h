
#ifndef triacPIDh
#define triacPIDh

#include<stdint.h>
#include <math.h>
#include "triacUI.h"

// Select 'double' or 'float' here:
typedef double real;


void InitializePID(real kp, real ki, real kd, real error_thresh, real step_time);

real Update(real error);

/*

uint16_t calibLowCms, calibHighCms;

uint16_t calibLowAdc, calibHighAdc;

void storeCalibLowCms(uint16_t cl);

void storeCalibHighCms(uint16_t ch);

void storeCalibLowAdc(uint16_t cl);

void storeCalibHighAdc(uint16_t ch);

*/

void calcNextTriacDelay();

#endif
