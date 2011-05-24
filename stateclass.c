
#include <avr/io.h>
#include <stdio.h>
//#include <iostream.h>
//#include <assert.h>

#include "TStatechart.h"
#include "StateClass.h"

TStatechart SJoesTriacStateChart;
TStatechart* PJoesTriacStateChart;

CJoesTriacEvent* currentEvent;

extern const uStInt uStIntHandlingDone;
extern const uStInt uStIntNoMatch;




#include "TriacIntr.h"
#include "triacPID.h"
#include "triacUI.h"
#include "TriacKeyPad.h"





// This defines and names the states the class has.
enum eStates
{
	eStateJoesTriac,
	eStartState = eStateJoesTriac,
	eStateAskForCalibration,
	eStateCalibrating,
	eStateCalibrateLow,
	eStateCalibrateHigh,
	eStateTriacIdle,
	eStateEditIdle,
	eStateEditAmps,
	eStateEditDuration,
	eStateTriacRunning,
	eNumberOfStates
};

uStInt evJoesTriacChecker(void)
{
	return (uStIntNoMatch);
}

void entryAskForCalibrationState(void)
{
//	printf("entry AskForCalibration\n");
	displayCalibrationPrompt();
	startDurationTimer(120);
}

void exitAskForCalibrationState(void)
{
//	printf("exit I\n");
	stopDurationTimer();
}

int8_t bl = 0;

uStInt evAskForCalibrationChecker(void)
{
	int res = uStIntNoMatch;
//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evTimeOutDurationTimer) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	if ((currentEvent->evType == evCharEntered) && (currentEvent->keyCode==kpFunction1)) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrating);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evSecondsTick) 
	{	
		int16_t srem = getSecondsRemaining();
//		bl != bl;
//		if (bl) {
			displayCountDown();
			printf("cnt %i\n",srem);
//		}		
		res =  uStIntHandlingDone;
	}
	return (res); 
}

void entryCalibratingState(void)
{
//	printf("entry I\n");
	displayCalibratingExplanation();
	startDurationTimer(0xFFFF);   // enable secondsTick
	startTriacRun();
}

void exitCalibratingState(void)
{
//	printf("exit I\n");
	stopDurationTimer();
	stopTriacRun();
}

uStInt evCalibratingChecker(void)
{
	int16_t triggerDelay;
	int8_t res;
	
	res = uStIntNoMatch;
	if (currentEvent->evType == evCharEntered) {
		triggerDelay = triacTriggerDelayCms;
		switch (currentEvent->keyCode) {
			case kp1 : 
				triggerDelay++;
				break;
			case kp2 :
				triggerDelay += 2;
				break ;	
			case kp3 :
				triggerDelay += 10;
				break ;		
			case kp7 : 
				triggerDelay--;
				break;
			case kp8 :
				triggerDelay -= 2;
				break ;	
			case kp9 :
				triggerDelay -= 10;
				break ;									
		}
		setTriacTriggerDelay(triggerDelay);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evSecondsTick) 
	{	
		displayCurrentAmps();
		res =  uStIntHandlingDone;
	}
	
	return res;
}


void entryCalibrateLowState(void)
{
//	printf("entry I\n");
	displayCalibrateLow();
}

void exitCalibrateLowState(void)
{
//	printf("exit I\n");
	storeCalibLowCms(triacTriggerDelayCms);
	storeCalibLowAdc(ampsADCValue());
}

uStInt evCalibrateLowChecker(void)
{
//	printf("check for event in State evStateIdle\n");
	int res = uStIntNoMatch;

	if ((currentEvent->evType == evCharEntered) && (currentEvent->keyCode==kpFunction1)) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateHigh);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	return res;
}


void entryCalibrateHighState(void)
{
//	printf("entry I\n");
	displayCalibrateHigh();
}

void exitCalibrateHighState(void)
{
//	printf("exit I\n");
	storeCalibHighCms(triacTriggerDelayCms);
	storeCalibHighAdc(ampsADCValue());
}

uStInt evCalibrateHighChecker(void)
{
	int res = uStIntNoMatch;

	if ((currentEvent->evType == evCharEntered) && (currentEvent->keyCode==kpFunction1)) 
	{	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	return res;
}

int8_t keyInd;


void entryTriacIdleState(void)
{
//	printf("entry I\n");
	displayEditAmpsDuration();
}

void exitTriacIdleState(void)
{
//	printf("exit I\n");
}

uStInt evTriacIdleChecker(void)
{
//	printf("check for event in State evStateIdle\n");
	int res = uStIntNoMatch;

	if ((currentEvent->evType == evCharEntered) && (currentEvent->keyCode==kpStart)) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacRunning);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	return res;
}


void entryEditIdleState(void)
{
//	printf("entry I\n");
}

void exitEditIdleState(void)
{
//	printf("exit I\n");
}

uStInt evEditIdleChecker(void)
{
	int res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evCharEntered) {
		if (currentEvent->keyCode==kpAst) {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditAmps);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
		if (currentEvent->keyCode==kpNum) {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditDuration);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
	}			
	return res;
}

void entryEditAmpsState(void)
{
//	printf("entry I\n");
	startEditAmps();
	keyInd = 0;
}

void exitEditAmpsState(void)
{
//	printf("exit I\n");
}

uStInt evEditAmpsChecker(void)
{
	int res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evCharEntered) {
		if (currentEvent->keyCode==kpNum)  {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditDuration);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
				if (currentEvent->keyCode==kpAst)  {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
		if ((currentEvent->keyCode <= kp9) && (currentEvent->keyCode >= kp0)) {
			switch (keyInd)
			{
				case 0: 
					setAmps100(currentEvent->keyCode);
					break;
				case 1:
					setAmps10(currentEvent->keyCode);
					break;
				case 2:
					setAmps(currentEvent->keyCode);
					BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
				
					END_EVENT_HANDLER(PJoesTriacStateChart);				
			}
			keyInd ++;
			res =  uStIntHandlingDone;
		}
	}		
	return res;
}


void entryEditDurationState(void)
{
//	printf("entry I\n");
	startEditDuration();
	keyInd = 0;
}

void exitEditDurationState(void)
{
//	printf("exit I\n");
}

uStInt evEditDurationChecker(void)
{
	int res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evCharEntered) {
		if (currentEvent->keyCode==kpAst)  {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditAmps);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
		if (currentEvent->keyCode==kpNum)  {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
		if ((currentEvent->keyCode <= kp9) && (currentEvent->keyCode >= kp0)) {
			switch (keyInd)
			{
				case 0: 
					setMin10(currentEvent->keyCode);
					break;
				case 1:
					setMin(currentEvent->keyCode);
					break;
				case 2:	
					if (currentEvent->keyCode <= kp5) setSec10(currentEvent->keyCode);
					break;
				case 3:
					setSec(currentEvent->keyCode);
					BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
				
					END_EVENT_HANDLER(PJoesTriacStateChart);				
			}
			keyInd ++;
			res =  uStIntHandlingDone;
		}
	}		
	return res;
}

void entryTriacRunningState(void)
{
//	printf("entry I\n");
	displayTriacRunning();
	startTriacRun();
}

void exitTriacRunningState(void)
{
//	printf("exit I\n");
	stopTriacRun();
}

uStInt evTriacRunningChecker(void)
{
	int res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evCharEntered) {
		if (currentEvent->keyCode==kpStop)  {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
	}		
	if (currentEvent->evType == evTimeOutDurationTimer) {
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}			
	if (currentEvent->evType == evSecondsTick) {
		displayCurrentAmps();
		displayCountDown();
		res =  uStIntHandlingDone;
	}			
	return res;
}


#ifndef  sdccNULL 

#define tfNull 0

#else

t_fvoid  tfNull;

#endif 

xStateType xaStates[eNumberOfStates] = {
 	{eStateJoesTriac,    // name
 	-1,									//parent
 	eStateAskForCalibration,    // default substate
 	0,    // keep history
 	evJoesTriacChecker,    // event checking fu
	tfNull,       // def state entry function
 	tfNull,     //    entering state
 	tfNull},     // exiting state

 	{eStateAskForCalibration,
 	eStateJoesTriac,
 	-1,
 	0,									
 	evAskForCalibrationChecker,
 	tfNull,
 	entryAskForCalibrationState,
 	exitAskForCalibrationState},

 	{eStateCalibrating,
 	eStateJoesTriac,
 	-1,
 	0,
 	evCalibratingChecker,
 	tfNull,
 	entryCalibratingState,
 	exitCalibratingState},
	 
	 {eStateCalibrateLow,
 	eStateCalibrating,
 	-1,
 	0,
 	evCalibrateLowChecker,
 	tfNull,
 	entryCalibrateLowState,
 	exitCalibrateLowState},
	 
 	{eStateCalibrateHigh,
 	eStateCalibrating,
 	-1,
 	0,
 	evCalibrateHighChecker,
 	tfNull,
 	entryCalibrateHighState,
 	exitCalibrateHighState},

 	{eStateTriacIdle,
 	eStateJoesTriac,
 	eStateEditIdle,
 	0,
 	evTriacIdleChecker,
 	tfNull,
 	entryTriacIdleState,
 	exitTriacIdleState},

 	{eStateEditIdle,
 	eStateTriacIdle,
 	-1,
 	0,
 	evEditIdleChecker,
 	tfNull,
 	entryEditIdleState,
 	exitEditIdleState},

 	{eStateEditAmps,
 	eStateTriacIdle,
 	-1,
 	0,
 	evEditAmpsChecker,
 	tfNull,
 	entryEditAmpsState,
 	exitEditAmpsState},
	 
	{eStateEditDuration,
 	eStateTriacIdle,
 	-1,
 	0,
 	evEditDurationChecker,
 	tfNull,
 	entryEditDurationState,
 	exitEditDurationState},
	 
	{eStateTriacRunning,
 	eStateJoesTriac,
 	-1,
 	0,
 	evTriacRunningChecker,
 	tfNull,
 	entryTriacRunningState,
 	exitTriacRunningState}	 
};








/*
void entry...State(void)
{
//	printf("entry I\n");
}

void exit...State(void)
{
//	printf("exit I\n");
}

uStInt ev...Checker(void)
{
//	printf("check for event in State evStateIdle\n");

//	if (currentEvent->evType == eValueAssignement) 
//	{	if (HumidifyingLowerLimit > currentEvent->humidity)
//		{
//			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PJoesTriacStateChart);
//			return (u32HandlingDone);
//		}
//	}
	return (u32NoMatch);
}

*/

/*

enum eStates
{
	eStateJoesTriac,
	eStartState = eStateJoesTriac,
	eStateAskForCalibration,
	eStateCalibrating,
	eNumberOfStates
};


uStInt evJoesTriacChecker(void)
{
	return (uStIntNoMatch);
}

uStInt evAskForCalibrationChecker(void)
{
	return (uStIntNoMatch);
}

uStInt evCalibratingChecker(void)
{
	return (uStIntNoMatch);
}


#ifndef  sdccNULL 

#define tfNull 0

#else

t_fvoid  tfNull;

#endif 

xStateType xaStates[eNumberOfStates] = {
 	{eStateJoesTriac,    // name
 	-1,									//parent
 	eStateAskForCalibration,    // default substate
 	0,    // keep history
 	evJoesTriacChecker,    // event checking fu
	tfNull,       // def state entry function
 	tfNull,     //    entering state
 	tfNull},     // exiting state

 	{eStateAskForCalibration,
 	eStateJoesTriac,
 	-1,
 	0,									
 	evAskForCalibrationChecker,
 	tfNull,
 	tfNull,
 	tfNull},

 	{eStateCalibrating,
 	eStateJoesTriac,
 	-1,
 	0,
 	evCalibratingChecker,
 	tfNull,
 	tfNull,
 	tfNull},
	 

};

*/


void startStateCharts()
{

#ifdef  sdccNULL 

	tfNull = (t_fvoid ) NULL;

#endif 

 	PJoesTriacStateChart = & SJoesTriacStateChart; 
	createTStatechart (& SJoesTriacStateChart, xaStates, eNumberOfStates, eStartState);
	
}


void stopStateCharts()
{
	destructTStatechart(&SJoesTriacStateChart);
}


bool processTriacEvent(TStatechart* t,CJoesTriacEvent* ev)
{
	currentEvent = ev;
	return ProcessEvent(t);
}

