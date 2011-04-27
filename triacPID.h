
#ifndef triacPIDh
#define triacPIDh

#include<stdint.h>
#include <math.h>

// Select 'double' or 'float' here:
typedef double real;


void InitializePID(real kp, real ki, real kd, real error_thresh, real step_time);

real Update(real error);

void createPID();

int m_started;
real m_kp, m_ki, m_kd, m_h, m_inv_h, m_prev_error, m_error_thresh, m_integral;

uint16_t calibLow, calibHigh;

void storeCalibLow(uint16_t cl);

void storeCalibHigh(uint16_t ch);

void calcNextTriacDelay();

#endif