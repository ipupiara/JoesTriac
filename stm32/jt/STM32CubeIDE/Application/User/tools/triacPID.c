
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "triacPID.h"
#include <defines.h>
#include <TriacIntr.h>
#include <adcControl.h>
//#include <uart-comms.h>
#include <mainJt.h>


//#define printfPid
//#define printfAmps

enum adcScopeEnum
{
	nearScope,
	farScope
};

float currentAmpsValue;
uint16_t currentAmpsADCValue;
//int8_t stableZeroAdjReached;

int8_t m_started;
real m_kPTot, m_kP, m_kI, m_kD, m_stepTime, m_inv_stepTime, m_prev_error, m_integral_thresh, m_integral;
real deriv, error, partial;

float gradAmps; //   (delta amperes) / (delta adc)   ....
float gradAdc;  
uint32_t calibHighADC;
uint32_t calibLowADC;
real corrCarryOver;     // carry amount if correction in float gives zero correction in int

uint8_t signum(real re)
{
	int8_t res;
	if (re > 0.0) {res =  1;} else {res = 0;}
	return res;
}

uint32_t getCurrentAmpsADCValue()
{
	uint32_t res;
	taskENTER_CRITICAL();
	res = currentAmpsADCValue;
	taskEXIT_CRITICAL();
	return res;
}

void setCurrentAmpsADCValueNonIsr(uint32_t adcV )
{
	taskENTER_CRITICAL();
	currentAmpsADCValue = adcV;
	taskEXIT_CRITICAL();
}

void adcValueReceived(uint16_t adcVal)
{
	taskENTER_CRITICAL();
	setCurrentAmpsADCValueNonIsr(adcVal);
	taskEXIT_CRITICAL();
}

float adcVoltage()
{
	int16_t ampsAdcHex;
	float   ampsAdcF;
	float   adcMaxF = 0x0FFF;

	float    Vf;

	ampsAdcHex = getCurrentAmpsADCValue();
	ampsAdcF  = ampsAdcHex;
	Vf = (ampsAdcF * 3.3) / adcMaxF;  //  todo set final ref voltage here

	return Vf;
}

void InitializePID(real kpTot,real kpP, real ki, real kd, real integral_thresh, real step_time);

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




float currentAmps()
{
	uint32_t adcVal;
	float res = 0.0;

	adcVal = getCurrentAmpsADCValue();

	int32_t diffAdc = ((uint32_t) (adcVal)) - ((uint32_t) ( getDefinesCalibLowAdc()))  ;

	float diffI = (gradAmps * diffAdc);

	res = calibLowAmps +  diffI;
	return res;
}


float getCurrentAmpsValue()
{
	float  res;
	res = currentAmps();
	return res;
}

void startAmpsADC()
{
	setCurrentAmpsADCValueNonIsr( 0);
	startADC();
}

void stopAmpsADC()
{
	stopADC();
	setCurrentAmpsADCValueNonIsr(0);
}

void startTriacPidRun()
{
	startADC();
	resetPID();
	startTriacRun();
}

void stopTriacPidRun()
{
	stopADC();
	stopTriacRun();
}
//uint16_t  adcValueForAmps (float amps)
//{
//	uint16_t res = 0;
////	uint16_t dAdc = gradAdc * (amps - calibLowAmps);
////	res = calibLowADC + dAdc;
//	return res;
//}


#define correctionThreshold  20 * kStepUnitsFactor

real nextCorrection(real err)
{
    real iFact = 1.0;
	real pFact = 1.0;
	real dFact = 1.0;
	real res;
    if (fabs(err) < m_integral_thresh) {
        iFact = 1.0;
        pFact = 0.0;
        dFact = 0.0;
    } else  {
        iFact = 0.0;
		m_integral = 0.0;
	}
    if (signum(err) != signum(m_prev_error))  {
    	m_integral = 0.0;
    }


    m_integral += m_stepTime*iFact*err;

    if (!m_started)
    {
        m_started = 1;
        deriv = 0;
    }
    else  {
        deriv = dFact* ((err - m_prev_error) * m_inv_stepTime);
    }
    partial  = pFact* err;
    m_prev_error = err;
#ifdef printfPid
    real mk, mi, md;
	res = m_kPTot*((mk=(m_kP*partial)) + (mi=(m_kI*m_integral)) - (md=(m_kD*deriv)));
#else
	res = m_kPTot*((m_kP*err) + (m_kI*m_integral) + (m_kD*deriv));
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

int16_t delayCorrectionI;

void calcNextTriacDelay()
{  
	float err;
	float delayCorrectionF;
	int16_t newDelay;


	float amps;
	error = (amps = currentAmps()) - getDefinesWeldingAmps();
	taskENTER_CRITICAL();
	currentAmpsValue = amps;
	taskEXIT_CRITICAL();
	delayCorrectionF = nextCorrection(error);
	delayCorrectionF += corrCarryOver;
	delayCorrectionI = delayCorrectionF;
	corrCarryOver = delayCorrectionF - delayCorrectionI;

	if (((error > 0)  && (delayCorrectionI < 0)) || ((error < 0)  && (delayCorrectionI > 0)))   {
		delayCorrectionI = 0;  //  do not go into the wrong direction
	}

	newDelay = getTriacTriggerDelay() + delayCorrectionI;
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
	real step = pidStepDelays;
	real maxV = 1000.0;
	real stepf = step / maxV;
	InitializePID( 0.8, 2.0, 6.0, 0.01, 4, stepf);

	//	InitializePID(real kpTot, real kpP, real ki, real kd, real error_thresh, real step_time);
	currentAmpsValue = 0.0;
}

void InitializePID(real kpTot,real kpP, real ki, real kd, real integral_thresh, real step_time)
{
	m_kPTot = kpTot  ;
    m_kI = ki *  kStepUnitsFactor ;
    m_kP   = kpP *  kStepUnitsFactor ;
    m_kD = kd  *  kStepUnitsFactor ;
    m_integral_thresh = integral_thresh;

    m_stepTime = step_time;
    m_inv_stepTime = 1 / step_time;

    m_integral = 0;
    m_started = 0;
    deriv = 0;
    corrCarryOver = 0;

	updateGradAmps();
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

