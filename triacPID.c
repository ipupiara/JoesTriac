#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <avr/eeprom.h>
#include "triacPID.h"
#include "TriacDefines.h"
#include "TriacIntr.h"

#define usePrintfPid


int8_t m_started;
real m_kPTot, m_kI, m_kD, m_stepTime, m_inv_stepTime, m_prev_error, m_error_thresh, m_integral;

float gradAmps; // amperes ....

real corrCarryOver;     // carry amount if correction in float gives zero correction in int

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


void InitializePID(real kpTot, real ki, real kd, real error_thresh, real step_time)
{
    // Initialize controller parameters
    m_kPTot = kpTot;
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

	 corrCarryOver = 0;
}

real nextAdjust(real error)
{
    // Set q_fact to 1 if the error magnitude is below
    // the threshold and 0 otherwise
    real q_fact;
	real res;
    if (fabs(error) < m_error_thresh)
        q_fact = 1.0;
    else  {
        q_fact = 0.0;
		m_integral = 0.0;
	}

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
	res = m_kPTot*(error + m_kI*m_integral + m_kD*deriv);

#ifdef usePrintfPID
	double errD = error;
	double intD = m_integral;
	double derivD = deriv;
	printf("err %f int %f deriv %f ",errD, intD, derivD);
#endif
    return res;
}

float currentAmps()
{
	int16_t adcAmps;
	float res;
	adcAmps = ampsADCValue();
	res = calibLowAmps + (gradAmps * (adcAmps - calibLowADC  ));

/*	double grdA = gradAmps ;
	double resD = res;
	printf("adcA %i grad %f calow %i cahi %i res %f\n",adcAmps,grdA,calibLowADC, calibHighADC, resD);
*/
	currentAmpsValue = res;   // set this for ui
	return res;
}



void calcNextTriacDelay()
{  
	float err;
	float corr;
	int16_t newDelay;
	int16_t corrInt;
	err = currentAmps()  - desiredAmps ;
	corr = nextAdjust(err) + corrCarryOver;
	corrInt = corr;     
/*	if (corrInt == 0) {
		corrCarryOver += corr;
	} else  {
		corrCarryOver = 0;  // forget rounding errors, if any correction took place, 
						// but dont forget, if no correction at all took place
	}
	*/
	corrCarryOver = corr - corrInt;
	newDelay = triacTriggerDelayCms + corrInt;
	setTriacTriggerDelay(newDelay);
#ifdef usePrintfPID
	double corrD = corr;
	double carryCorrD = corrCarryOver;
	double ampsD  = currentAmps();
	printf(" corr %f corrI %i cry %f delay %x  amps %f\n",corrD,corrInt, carryCorrD, newDelay, ampsD); 
#endif
}

void InitPID()
{
//	InitializePID(real kpTot, real ki, real kd, real error_thresh, real step_time);   
	InitializePID(-0.5, 0.2, 0.13, 5, (pidStepDelays/42.18));
}


void resetPID()
{
	corrCarryOver = 0;
 	m_integral =0;
	m_prev_error = 0;
}
