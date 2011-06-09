
#include <math.h>
#include <avr/eeprom.h>
#include "triacPID.h"
#include "TriacDefines.h"
#include "TriacIntr.h"


int m_started;
real m_kp, m_ki, m_kd, m_h, m_inv_h, m_prev_error, m_error_thresh, m_integral;

float gradAmps; // amperes ....

void updateGradAmps()
{
	float dADC;
	float dAmps;
	dAmps = calibHighAmps - calibLowAmps;
	dADC = calibHighADC - calibLowADC;
	if ( fabs(dADC) > 1) {  
		gradAmps = dAmps / dADC;
	} else gradAmps = 0;
}


void InitializePID(real kp, real ki, real kd, real error_thresh, real step_time)
{
    // Initialize controller parameters
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
    m_error_thresh = error_thresh;

    // Controller step time and its inverse
    m_h = step_time;
    m_inv_h = 1 / step_time;

    // Initialize integral and derivative calculations
    m_integral = 0;
    m_started = 0;
	
	 updateGradAmps();
}

real Update(real error)
{
    // Set q to 1 if the error magnitude is below
    // the threshold and 0 otherwise
    real q;
    if (fabs(error) < m_error_thresh)
        q = 1;
    else
        q = 0;

    // Update the error integral
    m_integral += m_h*q*error;

    // Compute the error derivative
    real deriv;
    if (!m_started)
    {
        m_started = 1;
        deriv = 0;
    }
    else
        deriv = (error - m_prev_error) * m_inv_h;

    m_prev_error = error;

    // Return the PID controller actuator command
    return m_kp*(error + m_ki*m_integral + m_kd*deriv);
}

float currentAmps()
{
	int16_t adcAmps;
	float res;
	adcAmps = ampsADCValue();
	res = calibLowAmps + (gradAmps * (adcAmps - calibLowADC  ));
	currentAmpsValue = res;   // set this for ui
	return res;
}

void calcNextTriacDelay()
{  
	float err;
//	float corr, newDelay;
	err = currentAmps()  - desiredAmps ;
//	corr = Update(err);
//	newDelay = triacTriggerDelayCms - corr;
//	setTriacTriggerDelay(newDelay);
	
}


