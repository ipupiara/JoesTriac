#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <avr/eeprom.h>
#include "triacPID.h"
#include "TriacDefines.h"
#include "TriacIntr.h"
#include "TWI_master.h"

//#define printfPid
//#define printfAmps

enum adcScopeEnum
{
	nearScope,
	farScope
};


int8_t m_started;
real m_kPTot, m_kP, m_kI, m_kD, m_stepTime, m_inv_stepTime, m_prev_error, m_error_thresh, m_integral;

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

uint8_t  sendMessageBuffer [4];
uint8_t  receiveMessageBuffer[8];

int sendZeroAdjustMsg(int8_t jobS)   // local use in this file only
{
	int res;
//	printf("send synch zero Adj Msg\n");

	memset(sendMessageBuffer,0,sizeof(sendMessageBuffer));
	sendMessageBuffer[0] = jobS;
	res = twi_synch_tx(zeroAdjustATtinyID, (uint8_t *) &sendMessageBuffer, 1);
//	printf("synch tx returned %x\n",res);
	return res;
}

int8_t getAndTestZeroAdjustState(int8_t jobS)    // called so far only in setAdjustJob
{  
	int8_t res;
	int8_t js;
	res = 0;
	
	printf("start getAndTestZeroAdjustState\n");
	memset(receiveMessageBuffer,0,sizeof(receiveMessageBuffer));
	res  = twi_synch_rx(zeroAdjustATtinyID, (uint8_t *) &receiveMessageBuffer, 6);
//	printf("synch rx returned %x\n",res);
	js = receiveMessageBuffer[5];
	res = (jobS == js);
	printf("getAndTestZeroAdjustState to test was %i received was %i with result %i\n",jobS,js,res);

	return res;
}

int8_t setAdjustJob(int8_t jobS)    // interface stateclass.c  
{  int8_t res;
	int8_t cnt1;
	res = 0;
	cnt1= 0;
	while ((!res) && (cnt1 < 2)) {
		sendZeroAdjustMsg(jobS);
		res = getAndTestZeroAdjustState(jobS);
		++cnt1;
	}
	return res;
}

void persistentZeroAdjustSecondTickJob()
{	int8_t  jobS;
	int16_t adcVal;
	int16_t* adcVP;
	int8_t adcScope;
	uint8_t res;
//	double debugV;

	memset(receiveMessageBuffer,0,sizeof(receiveMessageBuffer));
	res  = twi_synch_rx(zeroAdjustATtinyID, (uint8_t *) &receiveMessageBuffer, 6);
//	printf("synch rx returned %x\n",res);
	if (res == TWI_STATUS_DONE) {
		jobS =  receiveMessageBuffer[5];
		zeroPotiPos = (int8_t) receiveMessageBuffer[0];
		adcVP = (int16_t*) (&receiveMessageBuffer[1]);	
		adcVal = *adcVP;
		adcScope = (int8_t) receiveMessageBuffer[3];
//		printf("  zPP %i adcV %i adcSc %i\n",zeroPotiPos,adcVal,adcScope);

		if (adcScope == nearScope) {
			zeroAdjustDiffVoltage =  (adcVal * 1.1) / (20.0 * 0x200);
		} else {
			zeroAdjustDiffVoltage =  (adcVal * 1.1) / ( 0x200);
		}
//		debugV = zeroAdjustDiffVoltage;
//		printf("jb %i, V %f\n\n",jobS,debugV);

		if (jobS == jobIdle) {
			stableZeroAdjReached = 1;
		}
		if (jobS == fatalError) {
			sprintf((char *) &lastFatalErrorString,"ammeter out of poti");
			fatalErrorOccurred = 1;		
		}
	} else {
		sprintf((char *) &lastFatalErrorString,"i2c com/Err");
		fatalErrorOccurred = 1;		
	}
}


void checkTWIZeroAdjustMsg()     // interface , called in stateclass.c -> efEditIdleChecker
{   
	int8_t  jobS;

	zeroPotiPos = receiveMessageBuffer[0];
	jobS =  receiveMessageBuffer[5];
	if (jobS == fatalError) {
		printf("checkTWI... fatal Error occurred\n");
		sprintf((char *) &lastFatalErrorString," out of ammeter poti");
		fatalErrorOccurred = 1;		
	} else if (jobS != volatileZeroAdjust) {
	 	sendZeroAdjustMsg(volatileZeroAdjust);  
		// if ever a reset should happen on AtTiny (eg. brown out), send job again		
	}
}



#define maxIdleTickCnt  5

/*

it might be needed to add a delay of about 5 seconds after entering idle 
before a first offsetadjust can be done due to ammeter fall - down time

else start whenever count reaches max, even though the count is not set on reentering
idle state and will then have just any value
*/
void onEntryIdlePID()
{
	idleTickCnt = 1;
}


void onTriacIdleSecondTick_PID()
{
	int16_t secs;
	if (idleTickCnt < maxIdleTickCnt) {
		++ idleTickCnt;
	} else {
		memset(receiveMessageBuffer,0,sizeof(receiveMessageBuffer));
		twi_start_rx(zeroAdjustATtinyID, (uint8_t *) &receiveMessageBuffer, 6);
		
		idleTickCnt = 1;
	}
	secs = getSecondsDurationTimerRemaining();
	if ((secs & 0x001f) == 0) {
		printPIDState();
	} 
}



void InitializePID(real kpTot,real kpP, real ki, real kd, real error_thresh, real step_time)
{
    // Initialize controller parameters
	// PN 3.Oct 2011, added m_kP for better setting of proportional factor only
	// though these 4 factors will be linearly dependent
	m_kP   = kpP;
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

#define correctionThreshold  30

real nextCorrection(real error)
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
	res = m_kPTot*(m_kP*error + m_kI*m_integral + m_kD*deriv);
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

float currentAmps()
{
	int16_t adcAmps;
	float res;

	adcAmps = ampsADCValue();

	res = calibLowAmps +  (gradAmps * ((int16_t) adcAmps - (int16_t) calibLowADC  ));

#ifdef printfAmps
	double grdA = gradAmps ;
	double resD = res;

	printf("adcA %i grad %f res %f A, triacDelay %i \n",adcAmps,grdA, resD, triacFireDurationTcnt2);
	printf(" calowAdc %i cahiAdc %i calLowA %i calHighA %i\n",calibLowADC, calibHighADC, calibLowAmps,calibHighAmps);
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
	corr = nextCorrection(err) + corrCarryOver;
	corrInt = corr;     
	corrCarryOver = corr - corrInt;
	newDelay = triacFireDurationTcnt2 + corrInt;
	setTriacFireDuration(newDelay);
#ifdef printfPID
	double corrD = corr;
	double carryCorrD = corrCarryOver;
	double ampsD  = currentAmps();
	printf(" corr %f corrI %i cry %f delay %x  amps %f\n",corrD,corrInt, carryCorrD, newDelay, ampsD); 
#endif
}

void InitPID()
{
//	InitializePID(real kpTot, real kpP, real ki, real kd, real error_thresh, real step_time);   
	InitializePID( -0.45, 1.1, 0.2, 0.2, 5, (pidStepDelays/42.18));

	stableZeroAdjReached = 0;
}


void resetPID()
{
	corrCarryOver = 0;
 	m_integral =0;
	m_prev_error = 0;
}

void printPIDState()
{
	int16_t adcAmps;
	float res;
	double resD;
	double gradD = gradAmps;

	adcAmps = 0;

	res = calibLowAmps +  (gradAmps * ((int16_t) adcAmps - (int16_t) calibLowADC  ));
	resD = res;

	printf("\nPID State\n");
	printf("calLowA %i calHighA %i caLowDelay %i caHiDelay %i\n",calibLowAmps,calibHighAmps, calibLowTriacFireDuration, calibHighTriacFireDuration);
	printf("calowAdc %i cahiAdc %i \n",calibLowADC, calibHighADC);
	printf("shows at 0 ADC : %f A  grad %f zeroPotiPos %i\n",resD, gradD,zeroPotiPos);
//	checkEEPOROM();
}
