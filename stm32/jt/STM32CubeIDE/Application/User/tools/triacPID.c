
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
real error;
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

void resetPID()
{
	corrCarryOver = 0;
 	m_integral =0;
	m_prev_error = 0;
	m_started = 0;
	updateGradAmps();
}

#ifdef  newPid


#define correctionThreshold  10 * kStepUnitsFactor

real nextCorrection(real err)
{
	//  todo check if fp and art, icache and dchace are running (fp =float processor) ?
//    real iFact = 1.0;
//	real pFact = 1.0;
//	real dFact = 1.0;
	real res;
	real deriv;

    if (fabs(err) < m_integral_thresh) {
    	if (errorScope != nearScope )  {
//    		m_kPTot = kpTot / 2.0  ;
    		m_kI = kIntegral *  kStepUnitsFactor ;
    		m_kP   =  ( kPartial *  kStepUnitsFactor) / 100.0;
//    		m_kP  = 0;
//    		m_kD = (kDerivativ  *  kStepUnitsFactor) / 2.0;
			m_integral = 0.0;
			errorScope = nearScope;
    	}
        if (signum(err) != signum(m_prev_error))  {
        	++debZXcnt;
        	m_integral = 0.0;
        }

        m_integral += m_stepTime * err;
    } else  {
    	if (errorScope != farScope) {
//    		m_kPTot = kpTot  ;
    		m_kI = 0.0;
    	    m_kP   = kPartial *  kStepUnitsFactor ;
//    	    m_kD = kDerivativ  *  kStepUnitsFactor ;
			m_integral = 0.0;
			errorScope = farScope;
    	}
	}
//
    if (!m_started)
    {
        m_started = 1;
        deriv = 0;
    }
    else  {
        deriv =  ((err - m_prev_error) * m_inv_stepTime);
    }
    m_prev_error = err;
#ifdef printfPid
    real mk, mi, md;
	res = m_kPTot*((mk=(m_kP*err)) + (mi=(m_kI*m_integral)) - (md=(m_kD*deriv)));
#else
	res = m_kPTot*((debPart =(m_kP*err)) + (debInteg = (m_kI*m_integral)) + ( debDeriv = (m_kD*deriv)));
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

int32_t delayCorrectionI;

void calcNextTriacDelay()
{  
	float delayCorrectionF;
	int32_t newDelay;


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

}


//todo  introduce maxDelay  and maxTriger  to prevent current to Load when it should switch off compltely

void InitPID()
{
//	initTwa();
	debZXcnt = 0;

	currentAmpsValue = 0.0;
	errorScope = farScope;

	m_kPTot = kTotal  ;
    m_kI = kIntegral *  kStepUnitsFactor ;
    m_kP   = kPartial *  kStepUnitsFactor ;
    m_kD = kDerivativ  *  kStepUnitsFactor ;
    m_integral_thresh = error_thresh ;

	real thousand = 1000.0;

	m_stepTime = (step_time / thousand)  ;
    m_inv_stepTime = 1 / m_stepTime;

    m_integral = 0;
    m_started = 0;
    corrCarryOver = 0;

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

#else





void InitPID()
{

	//	initTwa();
		debZXcnt = 0;

		currentAmpsValue = 0.0;
		errorScope = farScope;
//		m_kPTot = -0.45
//		m_kPTot = -0.45 *(5000.0 /810.0 );
		m_kPTot = - 2.777;



		 m_kP   = 1.1 ;
	    m_kI = 0.2 ;
	    m_kD = 0.2 ;
	    m_integral_thresh = 5 ;

		real thousand = 1000.0;

		m_stepTime = (step_time / thousand)  ;
	    m_inv_stepTime = 1 / m_stepTime;

	    m_integral = 0;
	    m_started = 0;
	    corrCarryOver = 0.0;

		updateGradAmps();
}

#define correctionThreshold  100

real nextCorrection(real error)
{
    // Set q_fact to 1 if the error magnitude is below
    // the threshold and 0 otherwise
    real q_fact;
	real res;
    if (fabs(error) < m_integral_thresh)
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
	res = m_kPTot*(m_kP*error + m_kI*m_integral + m_kD*deriv);  //  todo multiply with time factor, so that correction does not mucn depend from stepwidth
	if (res > correctionThreshold) {
		res = correctionThreshold;
	} else if (res < -1*correctionThreshold) {
		res = -1* correctionThreshold;
	}

#ifdef printfPID
	double errD = error;
	double intD = m_integral;
	double derivD = deriv;
	printf("err %f int %f deriv %f \n",errD, intD, derivD);
#endif
    return res;
}


void calcNextTriacDelay()
{
	float err;
	float corr;
	int16_t newDelay;
	int16_t corrInt;
	err = currentAmps()  - getDefinesWeldingAmps();
	corr = nextCorrection(err) + corrCarryOver;
//	float fact = (5000.0 /810.0 );
//	corr = corr *fact;
	corrInt = corr;
	corrCarryOver = corr - corrInt;
	newDelay = getTriacTriggerDelay() + corrInt;
	setTriacTriggerDelay(newDelay);

#ifdef printfPID
	double corrD = corr;
	double carryCorrD = corrCarryOver;
	double ampsD  = currentAmps();
	printf(" corr %f corrI %i cry %f delay %x  amps %f\n",corrD,corrInt, carryCorrD, newDelay, ampsD);
#endif
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
//	printf("calLowA %i calHighA %i caLowDelay %i caHiDelay %i\n",calibLowAmps,calibHighAmps, calibLowTriacFireDuration, calibHighTriacFireDuration);
//	printf("calLowAdc %i caHiAdc %i \n",calibLowADC, calibHighADC);
//	printf("shows at 0 ADC : %f A  grad %f zeroPotiPos %i\n",resD, gradD,zeroPotiPos);
////	checkEEPOROM();
}





#endif

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
