#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <avr/eeprom.h>
#include "triacPID.h"
#include "TriacDefines.h"
#include "TriacIntr.h"

//#define printfPid
#define printfAmps

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


uint8_t idleTickCnt;

/*

//  draft code to adjust zero-offset of current sensor
//  but now LEM HTFS 200 is used that offers ref voltage as input (and also as output!)
//  so this code was left for eventual future use
//  free portA pins can be used for poti control




void zeroPotiPosDown()
{
	// set cs/UpDn Pin and pulse inc pin
	if (zeroPotiPos > 0) {
	  storeZeroPotiPos(--zeroPotiPos);
	}
}


void zeroPotiPosUp()
{
	// set cs/UpDn Pin and pulse inc pin
	if (zeroPotiPos > 0) {
	  storeZeroPotiPos(++zeroPotiPos);
	}
}


void potiDn()
{
	if (potiPos > 0) {
		if (potiPos >= 100 ) {
			poti2Dn();
		} else {
			poti1Dn();
		}
		--potiPos;
	}
}

void potiUp()
{
	if (potiPos < 199) {
		if (potiPos >= 100 ) {
			poti2Up();
		} else {
			poti1Up();
		}
		++potiPos;
	}
}

void initSensorOffsetAdjust()
{
	//  init for auto-adjust of the sensor's zero-level
	//  if needed
	int8_t cnt;

	idleTickCnt = 0;
	potiPos = 0;
	for (cnt =0; cnt < 99; ++ cnt) {
		poti1Dn();
		poti2Dn();
	}
}

void adjustSensorOffset()
{
	// during idle time, the sensor's zero offset can be adjustet
	// this is done during the timer of the idle state
	if (ampsADCValue() > 0)  {
		while((ampsADCValue() > 0) && ( potiPos > 0 )) {
			potiDn();
			// evtl. delay 
		}
	}  else {
		while((ampsADCValue() < 0) && ( potiPos < 199 )) {
			potiUp();
			// evtl. delay
		}		
	}
}

*/

#define maxIdleTickCnt  59

/*

it might be needed to add a delay of about 5 seconds after entering idle 
before a first offsetadjust can be done due to ammeter fall - down time

else start whenever count reaches max, even though the count is not set on reentering
idle state and will then have just any value

void onEntryIdle()
{
	idleTickCnt = maxIdleTickCnt - 5;
}
*/


void onIdleSecondTick()
{
	if (idleTickCnt == 0) {
//		adjustSensorOffset();
	}
	++ idleTickCnt;
	if (idleTickCnt > maxIdleTickCnt) idleTickCnt = 0;
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

//	 initSensorOffsetAdjust();
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

#ifdef printfPID
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

#ifdef printfAmps
	double grdA = gradAmps ;
	double resD = res;
	printf("adcA %i grad %f calow %i cahi %i res %f\n",adcAmps,grdA,calibLowADC, calibHighADC, resD);
#endif

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
#ifdef printfPID
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
