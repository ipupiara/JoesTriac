
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "triacPID.h"
#include <defines.h>
#include <TriacIntr.h>
#include <uart-comms.h>
#include <mainJt.h>


#define printfPid
//#define printfAmps

enum adcScopeEnum
{
	nearScope,
	farScope
};

float currentAmpsValue;
//int8_t stableZeroAdjReached;

int8_t m_started;
real m_kPTot, m_kP, m_kI, m_kD, m_stepTime, m_inv_stepTime, m_prev_error, m_error_thresh, m_integral;

float gradAmps; //   (delta amperes) / (delta adc)   ....
float gradAdc;  
uint32_t calibHighADC;
uint32_t calibLowADC;

real corrCarryOver;     // carry amount if correction in float gives zero correction in int



void updateGradAmps()
{
	float dADC;
	float dAmps;
	dAmps = calibHighAmps - calibLowAmps;
	dADC = getDefinesCalibHighAdc() - getDefinesCalibLowAdc();
	if ( fabs(dADC) > 1) {
		gradAmps = dAmps / dADC;
	} else gradAmps = 0;
	if (fabs (dAmps) > 1)  {
		gradAdc = dADC / dAmps;
	} else gradAdc = 0;
}


uint8_t idleTickCnt;

uint8_t  sendMessageBuffer [4];
uint8_t  receiveMessageBuffer[8];


void InitializePID(real kpTot,real kpP, real ki, real kd, real error_thresh, real step_time)
{
    // Initialize controller parameters
	// PN 3.Oct 2011, added m_kP for better setting of proportional factor only
	// though these 4 factors will be linearly dependent
	real avrMax = avrTriggerDelayMaxTcnt;
	real stmMax = stmTriggerDelayMax;
	real delta = (stmMax / avrMax);

	m_kP   = kpP;
    m_kPTot = kpTot;
    kpTot = kpTot * delta;
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


float currentAmps()
{
	uint32_t adcVal;
	float res = 0.0;

	adcVal = getCurrentAmpsADCValue();

	int32_t diffAdc = ((uint32_t) (adcVal)) - ((uint32_t) ( getDefinesCalibLowAdc()))  ;

	float diff = (gradAmps * diffAdc);

	res = calibLowAmps +  diff;
	return res;
}


float getCurrentAmpsValue()
{
	float  res;
	res = currentAmps();
	return res;
}

//uint16_t  adcValueForAmps (float amps)
//{
//	uint16_t res = 0;
////	uint16_t dAdc = gradAdc * (amps - calibLowAmps);
////	res = calibLowADC + dAdc;
//	return res;
//}


#define correctionThreshold  30

real nextCorrection(real error)
{
    // Set q_fact to 1 if the error magnitude is below
    // the threshold and 0 otherwise
    real q_fact;
	real res;
    if (fabs(error) < m_error_thresh) {
        q_fact = 1.0;
    } else  {
        q_fact = 0.0;
		m_integral = 0.0;
	}

    m_integral += m_stepTime*q_fact*error;

    real deriv;
    if (!m_started)
    {
        m_started = 1;
        deriv = 0;
    }
    else  {
        deriv = (error - m_prev_error) * m_inv_stepTime;
    }
    m_prev_error = error;
#ifdef printfPid
    real mk, mi, md;
	res = m_kPTot*((mk=(m_kP*error)) + (mi=(m_kI*m_integral)) + (md=(m_kD*deriv)));
#else
	res = m_kPTot*((m_kP*error) + (m_kI*m_integral) + (m_kD*deriv));
#endif
	if (res > correctionThreshold) {
		res = correctionThreshold;
	} else if (res < -1*correctionThreshold) {
		res = -1* correctionThreshold;
	}
#ifdef printfPid
	info_printf("m_kPTot*tot %10.3f, m_kP*e %10.3f,m_kI*i %10.3f, m_kD*d %10.3f\n",\
					res, mk, mi, md);
#endif
    return res;
}


void calcNextTriacDelay()
{  
	float err;
	float corr;
	int16_t newDelay;
	int16_t corrInt;
	float amps;
	err = (amps = currentAmps()) - getDefinesWeldingAmps() ;
	taskENTER_CRITICAL();
	currentAmpsValue = amps;
	taskEXIT_CRITICAL();
	corr = nextCorrection(err) + corrCarryOver;
	corrInt = corr;
	corrCarryOver = corr - corrInt;
	newDelay = getTriacTriggerDelay() + corrInt;
	setTriacTriggerDelay(newDelay);
//#ifdef printfPID
//	double corrD = corr;
//	double carryCorrD = corrCarryOver;
//	double ampsD  = currentAmps();
//	printf(" corr %f corrI %i cry %f delay %x  amps %f\n",corrD,corrInt, carryCorrD, newDelay, ampsD);
//#endif
}

void InitPID()
{
//	InitializePID(real kpTot, real kpP, real ki, real kd, real error_thresh, real step_time);   
	InitializePID( -0.45, 1.1, 0.2, 0.2, 8, (pidStepDelays/1000));
	currentAmpsValue = 0.0;
//	stableZeroAdjReached = 0;
}


void resetPID()
{
	corrCarryOver = 0;
 	m_integral =0;
	m_prev_error = 0;
	m_started = 0;
	updateGradAmps();
}

void printPIDState()
{
//	int16_t adcAmps;
//	float res;
//	double resD;
//	double gradD = gradAmps;
//
//	adcAmps = 0;
//
//	res = calibLowAmps +  (gradAmps * ((int16_t) adcAmps - (int16_t) calibLowADC  ));
//	resD = res;

//	printf("\nPID State\n");
//	printf("calLowA %i calHighA %i caLowDelay %i caHiDelay %i\n",calibLowAmps,calibHighAmps, calibLowTriacFireDuration, calibHighTriacFireDuration);
//	printf("calLowAdc %i caHiAdc %i \n",calibLowADC, calibHighADC);
//	printf("shows at 0 ADC : %f A  grad %f zeroPotiPos %i\n",resD, gradD,zeroPotiPos);
//	checkEEPOROM();
}

