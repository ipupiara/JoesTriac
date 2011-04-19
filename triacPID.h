
#ifndef triacPIDh
#define triacPIDh

#include <math.h>

// Select 'double' or 'float' here:
typedef double real;


void Initialize(real kp, real ki, real kd, real error_thresh, real step_time);

real Update(real error);

void createPID();

int m_started;
real m_kp, m_ki, m_kd, m_h, m_inv_h, m_prev_error, m_error_thresh, m_integral;



#endif