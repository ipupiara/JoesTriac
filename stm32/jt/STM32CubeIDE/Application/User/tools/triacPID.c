
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "triacPID.h"
#include <defines.h>
#include <TriacIntr.h>
#include <adcControl.h>
#include <uart-comms.h>
#include <mainJt.h>
#include <extiCheck.h>
#include <triacControl.h>


#define printfPid
#define printfAmps

void initTwa();

typedef enum
{
	nearScope,
	farScope
} errorScopeEnum;




errorScopeEnum  errorScope;

float currentAmpsValue;
uint16_t currentAmpsADCValue;

int8_t m_started;
real m_kPTot, m_kP, m_kI, m_kD, m_stepTime, m_inv_stepTime, m_prev_error, m_integral_thresh, m_integral;
real m_correctionThresh, q_fact;
real error , correction;
real debPart, debDeriv, debInteg, debZXcnt;


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
//	taskENTER_CRITICAL();
	res = currentAmpsADCValue;
//	taskEXIT_CRITICAL();
	return res;
}

void adcValueReceived(uint16_t adcVal)
{
//	taskENTER_CRITICAL();
	currentAmpsADCValue = adcVal;
//	taskEXIT_CRITICAL();
}

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

void resetPID()
{
	corrCarryOver = 0;
 	m_integral =0;
	m_prev_error = 0;
	m_started = 0;
	updateGradAmps();
}

real nextCorrection(real error)
{
	real res;
    if (fabs(error) < m_integral_thresh) {
		q_fact = 1.0;
		m_kI = 	kIntegral;
		m_integral += m_stepTime*q_fact*error;
    } else  {
		m_kI = 0.0;
		q_fact = 0.0;
		m_integral = 0.0;
	}

    real deriv;
    if (!m_started)
    {
        m_started = 1;
        deriv = 0;
    }
    else
        deriv = (error - m_prev_error) * m_inv_stepTime;

    m_prev_error = error;

	res = m_kPTot*(m_kP*error + m_kI*m_integral + m_kD*deriv);
			//  todo multiply with time factor, so that correction does not so much depend from stepwidth
	if (res > m_correctionThresh) {
		res = m_correctionThresh;
	} else if (res < -1*m_correctionThresh) {
		res = -1* m_correctionThresh;
	}

//	if (getCurrentAmpsADCValue() < 500  )  {   //  todo add global constants  for introduced values
//		res= res/4;
//	}

#ifdef printfPid
	double errD = m_kD * error;
	double parD = m_kP * error;
	double intD = m_kI * m_integral;
	double derivD = deriv;
	pid_printf("err %f par %f int %f deriv %f corr %f\n",errD, parD, intD, derivD, res);
#endif
    return res;
}


void calcNextTriacDelay()
{
	float err;
	int16_t newDelay;
	int16_t corrInt;
	err = getDefinesWeldingAmps() - currentAmps() ;
	correction = nextCorrection(err) + corrCarryOver;
	corrInt = correction;
	corrCarryOver = correction - corrInt;
	newDelay = getTriacTriggerDelay() - corrInt;
	setTriacTriggerDelay(newDelay);

#ifdef printfPid
	double corrD = correction;
	double carryCorrD = corrCarryOver;
	double ampsD  = currentAmps();
	uint32_t adcVal =  getCurrentAmpsADCValue();
	pid_printf(" corr %f corrI %i cry %f delay %x  amps %f adc %i\n",corrD,corrInt, carryCorrD, newDelay, ampsD, adcVal);
	pid_printf("amtMissedTotal %i  maxMissed %i\n",amtMissedZpTotal, maxMissedZp);
#endif
}



void InitPID()
{

	//	initTwa();

	q_fact = 0.0;
	currentAmpsValue = 0.0;

	m_kPTot = kTotal;
	m_kP   = kPartial ;
	m_kI = kIntegral ;
	m_kD = kDerivativ ;
	m_integral_thresh =  integral_thres ;
	m_correctionThresh = correctionThreshold;
	real thousand = 1000.0;

	m_stepTime = (pidStepDelays / thousand)  ;
	m_inv_stepTime = 1 / m_stepTime;

	m_integral = 0;
	m_started = 0;
	corrCarryOver = 0.0;

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
//
//	printf("\nPID State\n");
//	printf("calLowA %i calHighA %i\n",calibLowAmps,calibHighAmps);
//	printf("calLowAdc %i caHiAdc %i \n",calibLowADC, calibHighADC);
//	printf("shows at 0 ADC : %f A  grad %f \n",resD, gradD);
////	checkEEPOROM();
}







/*
////////////////////////////////////////////////   twa   code  ///////////////////

define amtTwaValues   20
#define twaWeight  (amtTwaValues *  (amtTwaValues + 1) / 2)


typedef struct twaValue  {
	float   val;
	float   sub;
} twaValue  ;

twaValue twaValues [amtTwaValues] ;

float  twaAveridgeValue;
uint32_t amtAdded;


void addTwaValue(float newValue)
{
	float newRes = 0.0;
	int16_t cnt;
	int16_t amtTwa = amtTwaValues;
	uint32_t  weight;
	if (amtAdded >20 ) {weight = ((amtAdded * (amtAdded + 1)) / 2);}  else {weight = twaWeight}

	for (cnt = 0; cnt < amtTwaValues - 1; ++ cnt)  {
		newRes += twaValues[cnt + 1].val  = twaValues[cnt].val - twaValues[cnt].sub;
	}
	newRes += twaValues[0].val = newValue;
	twaValues [0].sub = newValue / amtTwaValues;
	twaValues[0].sub = newValue /  twaWeight;
	++ amtAdded
}

float getTwaAveridgeValue()
{
    if  amtAdd < amtTwaElement  return twaVal[0]  else twaAveridgeValue;
	return twaAveridgeValue;
}

void testTwa()
{
	uint16_t cnt;
	float res;
	for (cnt = 0; cnt < amtTwaValues; ++ cnt) {
		addTwaValue(1.2345);
	}
	res = getTwaAveridgeValue();
}


void initTwa()
{
	amtAdded = 0;
	twaAveridgeValue = 0.0;
	memset(twaValues,0, sizeof(twaValues));
	testTwa();
}
*/
