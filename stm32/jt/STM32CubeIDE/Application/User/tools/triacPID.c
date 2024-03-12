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


//#define printfPid
#define printfAmps

// void initTwa();


extern void startTriacRun();
extern void stopTriacRun();
void printTriacData(doPidAndPrint pidNPrint);
void initPidData();

int8_t m_started;
real m_kPTot, m_kP, m_kI, m_kD, m_stepTime, m_inv_stepTime, m_prev_error, m_integral_thresh, m_integral;
real m_correctionThresh, q_fact;
real error , correction;
real  Vp, Vi, Vd;
int16_t newDelay;
int16_t corrInt;

//__attribute__((section("TableSection")))  graphDataRec  triacPidGraphData;
graphDataRec  triacPidGraphData;


real corrCarryOver;     // carry amount if correction in float gives zero correction in int
uint32_t pidStepCnt;

uint8_t signum(real re)
{
	int8_t res;
	if (re > 0.0) {res =  1;} else {res = 0;}
	return res;
}

uint8_t idleTickCnt;

//uint8_t  sendMessageBuffer [4];
//uint8_t  receiveMessageBuffer[8];



void startTriacPidRun()
{
	pidStepCnt = 0;
	startADC();
	resetPID();
	startTriacRun();
}

void stopTriacPidRun()
{
	stopADC();
	stopTriacRun();
}

void resetPID()
{
	corrCarryOver = 0;
 	m_integral =0;
	m_prev_error = 0;
	m_started = 0;
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

	res =  m_kPTot * ((Vp = (m_kP*error)) + (Vi=(m_kI*m_integral)) + ( Vd= (m_kD*deriv)));

	if (res > m_correctionThresh) {
		res = m_correctionThresh;
	} else if (res < -1*m_correctionThresh) {
		res = -1* m_correctionThresh;
	}
    return res;
}


void calcNextTriacDelay(doPidAndPrint pidNPrint)
{

	if (pidNPrint > printOnly ) {
		float err;
		err = getDefinesWeldingAmps() - currentAmps() ;
		correction = (real) nextCorrection(err) + (real) corrCarryOver;
		corrInt = correction;
		corrCarryOver = correction - corrInt;
		newDelay = getTriacTriggerDelay() - corrInt;
		setTriacTriggerDelay(newDelay);
	}  else   {
		corrInt = 0;
		newDelay = getTriacTriggerDelay();
	}


//	printTriacData(pidAndPrint);

#ifdef printfPid
	double ampsD  = currentAmps();
	uint32_t adcVal =  getCurrentAmpsADCValue();

	CJoesModelEventT  msg;
	msg.messageType = pidPrint;
	msg.evData.pidPrintData.triAdc = adcVal;
	msg.evData.pidPrintData.triCorrInt = corrInt;
	msg.evData.pidPrintData.triDelay = newDelay;
	msg.evData.pidPrintData.ampsV = ampsD;
	msg.evData.pidPrintData.Vde = Vd ;
	msg.evData.pidPrintData.Vin = Vi ;
	msg.evData.pidPrintData.Vpa = Vp ;
	sendModelMessage(&msg);
#endif

//	if (pidNPrint > printOnly) {
//		if ((pidStepCnt & ((uint32_t) 0x01)) == 0  ) {
//
//			if (triacPidGraphData.amtValidDataPoints < pidGraphSize) {
//				triacPidGraphData.dataValues[triacPidGraphData.amtValidDataPoints] = ampsD;  // zero based
//				++triacPidGraphData.amtValidDataPoints;
//
//				CJoesPresenterEventT  msg;
//				msg.messageType = paintPidGraph;
//				msg.evData.pidGraphData.ampsF = ampsD;
//				msg.evData.pidGraphData.goalF = getDefinesWeldingAmps();
//				sendPresenterMessage(&msg);
//			}
//		}
//	}
	++ pidStepCnt;
}

void printTriacData(doPidAndPrint pidNPrint)
{
//	double ampsD  = currentAmps();
//	uint32_t adcVal =  getCurrentAmpsADCValue();
//
//	CJoesModelEventT  msg;
//	msg.messageType = pidPrint;
//	msg.evData.pidPrintData.triAdc = adcVal;
//	msg.evData.pidPrintData.triCorrInt = corrInt;
//	msg.evData.pidPrintData.ampsV = ampsD;
//	msg.evData.pidPrintData.triCorrInt = corrInt;
//	msg.evData.pidPrintData.Vde = Vd ;
//	msg.evData.pidPrintData.Vin = Vi ;
//	msg.evData.pidPrintData.Vpa = Vp ;
//	msg.evData.pidPrintData.pidAndPrintBool = pidNPrint;
//	sendModelMessage(&msg);
}

void printExistingGraph()
{
//	CJoesPresenterEventT  msg;          // todo consider implementing this in a method in triacpid or so
//	msg.messageType = pidGraphFromData;
//	msg.evData.pidDataArrayPtr = &triacPidGraphData;
//	sendPresenterMessage(&msg);
}

void printNextGraphDataPoint(float value)
{
	if (triacPidGraphData.amtValidDataPoints < pidGraphSize) {
		triacPidGraphData.dataValues[triacPidGraphData.amtValidDataPoints] = currentAmps();  // zero based
		triacPidGraphData.triacValues[triacPidGraphData.amtValidDataPoints] = getTriacTriggerDelay();
		++triacPidGraphData.amtValidDataPoints;

		CJoesPresenterEventT  msg;
		msg.messageType = paintPidGraph;
		msg.evData.pidDataArrayPtr=&triacPidGraphData;
		sendPresenterMessage(&msg);
	}
}

//typedef struct {
//	float  goalValue;
//	uint32_t weldingDuration;
//	uint32_t amtValidDataPoints;
//	uint32_t amtValidGoalPoints;
//	float  dataValues [pidGraphSize];
//} graphDataRec;


void initPidGraphData(float goalVal, uint32_t secsDuration)
{
	memset(&triacPidGraphData,0,sizeof(graphDataRec));
	triacPidGraphData.goalValue = goalVal;
	triacPidGraphData.weldingDuration = secsDuration;
	triacPidGraphData.amtValidGoalPoints = secsDuration; // with current graphs !!!
}


void InitPID()
{
	//	initTwa();

	q_fact = 0.0;

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
	Vp = Vi = Vd = 0;

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
