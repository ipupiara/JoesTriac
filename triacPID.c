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



//  code to adjust zero-offset of current sensor

/*
void setPotiCS(int8_t on)
{
	if (on) {
		PORTA &= ~0x80;
	} else  {
		PORTA |= 0x80;
	}
}


void setPotiINC(int8_t on)
{
	if (on) {
		PORTA &= ~0x40;
	} else  {
		PORTA |= 0x40;
	}
}

void setPotiUp(int8_t up)
{
	if (up) {
		PORTA &= ~0x20;
	} else  {
		PORTA |= 0x20;
	}
}


void zeroPotiPosUpPersistent(int8_t up, int8_t persistent)
{
//	if ((zeroPotiPos > 0) && (zeroPotiPos < 100) ) { checked by caller
		setPotiCS(1);
		if (up) {
			setPotiUp(1);
			++ zeroPotiPos;
		} else {
			setPotiUp(0);
			-- zeroPotiPos;
		}
		setPotiINC(1);
		if (persistent) {
			setPotiINC(0);
		}
		setPotiCS(0);
		if (persistent) {
			storeZeroPotiPos(zeroPotiPos);
		} else {
			setPotiINC(0);
		}
		setPotiUp(0);
//	}
}


void errorPotiPosExceeded()
{
	sprintf((char *) &lastFatalErrorString,"out of 0 Pos");
	fatalErrorOccurred = 1;
}

void volatileZeroAdjStep()
{

	double volts;
	volts = adcVoltage();
   	if (volts > 3E-3) {		
		if (zeroPotiPos > 0)  {
			zeroPotiPosUpPersistent(0, 0);
		} else {
			errorPotiPosExceeded();
		}
	} else { 
		if (volts < -3E-3) {
			if (zeroPotiPos < 100) {
				zeroPotiPosUpPersistent(1, 0);
			} else {
				errorPotiPosExceeded();
			}
		}
	}
	
}

int8_t stableStepsCnt;
*/
/*
void persistentZeroAdjStep()
{	
	double volts;
	volts = adcVoltage();
   	if (volts > 3E-3) {	
		stableStepsCnt = 0;	
		if (zeroPotiPos > 0)  {
			zeroPotiPosUpPersistent(0,1);
		} else {
			errorPotiPosExceeded();
		}
	} else { 
		if (volts < -3E-3) {
			stableStepsCnt = 0;
			if (zeroPotiPos < 100) {
				zeroPotiPosUpPersistent(1,1);
			} else {
				errorPotiPosExceeded();
			}
		} else {
			stableStepsCnt ++;
		}
	}
	if ( stableStepsCnt > 30) {
		stableZeroAdjReached = 1;
	}
*/	
//}



/*
void resetZeroAdj()
{
	int i1;

	setPotiCS(1);
	setPotiUp(1);
	for (i1 = 0; i1 < 100; ++ i1) 
	{
		setPotiINC(1);
		setPotiINC(0);
	}	
	setPotiCS(0);	
	storeZeroPotiPos(100);   // up on 100  , debug stop

	setPotiCS(1);
	setPotiUp(0);
	for (i1 = 0; i1 < 100; ++ i1) 
	{
		setPotiINC(1);
		setPotiINC(0);
	}	
	setPotiCS(0);	
	storeZeroPotiPos(0x00);    //down on zero, debug stop


}
*/

/*
void zeroAdjTest()
{
		DDRA = 0b11100000;    // set pin 7 to 5 of port A as output for digital poti (zero adj)
		PORTA = 0b11100000; 
		DIDR0 = 0x0F;			// disa digital input on a0..a3

	while (1) {
		int i1;

		i1= 0;
		
		for (i1= 0; i1 < 100; ++ i1) {
			zeroPotiPosUpPersistent(1, 1);
		}

		i1 = 1;

		for (i1= 0; i1 < 100; ++ i1) {
			zeroPotiPosUpPersistent(0, 1);
		}

		i1 = 2;

		for (i1= 0; i1 < 50; ++ i1) {
			zeroPotiPosUpPersistent(1, 0);
		}

		i1 = 3;

		for (i1= 0; i1 < 50; ++ i1) {
			zeroPotiPosUpPersistent(0, 0);
		}
	
	}

}
*/

uint8_t  sendMessageBuffer [4];
uint8_t  receiveMessageBuffer[8];

int sendZeroAdjustMsg(int8_t jobS)
{
	int res;
//	printf("send synch zero Adj Msg\n");

	memset(sendMessageBuffer,0,sizeof(sendMessageBuffer));
	sendMessageBuffer[0] = jobS;
	res = twi_synch_tx(zeroAdjustATtinyID, (uint8_t *) &sendMessageBuffer, 1);
//	printf("synch tx returned %x\n",res);
	return res;
}

int8_t getAndTestZeroAdjustState(int8_t jobS)
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

int8_t setAdjustJob(int8_t jobS)
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

void persistentZeroAdjStep()
{	int8_t  jobS;
	int16_t adcVal;
	int16_t* adcVP;
	int8_t adcScope;
	uint8_t res;
	double debugV;

	memset(receiveMessageBuffer,0,sizeof(receiveMessageBuffer));
	res  = twi_synch_rx(zeroAdjustATtinyID, (uint8_t *) &receiveMessageBuffer, 6);
//	printf("synch rx returned %x\n",res);
	if (res == TWI_STATUS_DONE) {
		jobS =  receiveMessageBuffer[5];
		zeroPotiPos = (int8_t) receiveMessageBuffer[0];
		adcVP = (int16_t*) (&receiveMessageBuffer[1]);	
		adcVal = *adcVP;
//		adcVal  = (int16_t) receiveMessageBuffer[1];
		adcScope = (int8_t) receiveMessageBuffer[3];
//		printf("  zPP %i adcV %i adcSc %i\n",zeroPotiPos,adcVal,adcScope);

		if (adcScope == nearScope) {
			zeroAdjustDiffVoltage =  (adcVal * 1.1) / (20.0 * 0x200);
		} else {
			zeroAdjustDiffVoltage =  (adcVal * 1.1) / ( 0x200);
		}
		debugV = zeroAdjustDiffVoltage;
//		printf("jb %i, V %f\n\n",jobS,debugV);


		if (jobS == jobIdle) {
			stableZeroAdjReached = 1;
		}
		if (jobS == fatalError) {
			sprintf((char *) &lastFatalErrorString,"tiny fatErr");
			fatalErrorOccurred = 1;		
		}
	} else {
		sprintf((char *) &lastFatalErrorString,"i2c com/Err");
		fatalErrorOccurred = 1;		
	}
}

/*
void checkTWIZeroAdjustMsg()
{   
	int8_t  jobS;

	zeroPotiPos = receiveMessageBuffer[0];
	zeroAdjustDiffVoltage = (float) receiveMessageBuffer[1];
	jobS =  receiveMessageBuffer[5];
	if (jobS == jobIdle) {
		stableZeroAdjReached = 1;
	}
	if (jobS == fatalError) {
		sprintf((char *) &lastFatalErrorString,"tiny fatErr");
		fatalErrorOccurred = 1;		
	}
}
*/


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


void onIdleSecondTickPID()
{
	if (idleTickCnt < maxIdleTickCnt) {
		++ idleTickCnt;
	} else {
	 	sendZeroAdjustMsg(volatileZeroAdjust);  // if ever a reset should happen on AtTiny (eg. brown out), send job again
		idleTickCnt = 1;
	}
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

	res = calibLowAmps +  (gradAmps * ((int16_t) adcAmps - (int16_t) calibLowADC  ));

#ifdef printfAmps
	double grdA = gradAmps ;
	double resD = res;

	printf("adcA %i grad %f calow %i cahi %i res %f  ",adcAmps,grdA,calibLowADC, calibHighADC, resD);
	printf("calLowA %i calHighA %i\n",calibLowAmps,calibHighAmps);
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
//	stableStepsCnt = 0;
	stableZeroAdjReached = 0;
//	setPotiCS(0);
//	setPotiINC(0);
//	setPotiUp(0);
}


void resetPID()
{
	corrCarryOver = 0;
 	m_integral =0;
	m_prev_error = 0;
}
