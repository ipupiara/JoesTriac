
#include <math.h>
#include <avr/eeprom.h>
#include "triacPID.h"
#include "TriacDefines.h"
#include "TriacIntr.h"


int8_t m_started;
real m_kP, m_kI, m_kD, m_stepTime, m_inv_stepTime, m_prev_error, m_error_thresh, m_integral;

float gradAmps; // amperes ....

float carryCorr;     // carry amount if correction in float gives zero correction in int

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
    m_kP = kp;
    m_kI = ki;
    m_kD = kd;
    m_error_thresh = error_thresh;

    // Controller step time and its inverse
    m_stepTime = step_time;
    m_inv_stepTime = 1 / step_time;

    // Initialize integral and derivative calculations
    m_integral = 0;
    m_started = 0;
	
	 updateGradAmps();

	 carryCorr = 0;
}

real Update(real error)
{
    // Set q_fact to 1 if the error magnitude is below
    // the threshold and 0 otherwise
    real q_fact;
	real res;
    if (fabs(error) < m_error_thresh)
        q_fact = 1.0;
    else
        q_fact = 0.0;

    // Update the error integral
    m_integral += m_stepTime*q_fact*error;

    // Compute the error derivative
    real deriv;
    if (!m_started)
    {
        m_started = 1;
        deriv = 0;
    }
    else
        deriv = (error - m_prev_error) * m_inv_stepTime;

    m_prev_error = error;

    // Return the PID controller actuator command
	res = m_kP*(error + m_kI*m_integral + m_kD*deriv);
    return res;
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
	float corr;
	float newDelay;
	int16_t corrInt;
	err = currentAmps()  - desiredAmps ;
	corr = Update(err) + carryCorr;
	corrInt = corr;     
	if (corrInt == 0) {
		carryCorr = corr;
	} else  {
		carryCorr = 0;  // forget rounding errors, if any correction took place, 
						// but dont forget, if no correction at all took place
	}
	newDelay = triacTriggerDelayCms - corr;
	setTriacTriggerDelay(newDelay);
	
}

void InitPID()
{
//	InitializePID(real kp, real ki, real kd, real error_thresh, real step_time);   
	InitializePID(-0.01, 0.3, 0.3, 4, (pidStepDelays/42.18));
}


