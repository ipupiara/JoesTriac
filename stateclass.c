
#include <avr/io.h>
//#include <iostream.h>
//#include <assert.h>

#include "TStatechart.h"
#include "StateClass.h"
#include "TriacIntr.h"
#include "triacPID.h"
#include "triacUI.h"
#include "TriacKeyPad.h"


extern const uStInt u32HandlingDone;
extern const uStInt u32NoMatch;

TStatechart SJoesTriacStateChart;
TStatechart* PJoesTriacStateChart;

CJoesTriacEvent* currentEvent;

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
	eStateIdle,
	eStateEditAmps,
	eStateEditDuration,
	eStateTriacRunning,
	eNumberOfStates
};

uStInt evJoesTriacChecker(void)
{
	return (u32NoMatch);
}

void entryAskForCalibrationState(void)
{
//	printf("entry AskForCalibration\n");
	displayCalibrationPrompt();
	startDurationTimer(3);
}

void exitAskForCalibrationState(void)
{
//	printf("exit I\n");
	stopDurationTimer();
}

uStInt evAskForCalibrationChecker(void)
{
	int res = u32NoMatch;
//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evTimeOutDurationTimer) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  u32HandlingDone;
	}
	if ((currentEvent->evType == evCharEntered) && (currentEvent->keyCode==kpFunction1)) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrating);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  u32HandlingDone;
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
	}
	if (currentEvent->evType == evSecondsTick) 
	{	
		displayCurrentAmps();
	}
	
	return (u32NoMatch);
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
	int res = u32NoMatch;

	if ((currentEvent->evType == evCharEntered) && (currentEvent->keyCode==kpFunction1)) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateHigh);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  u32HandlingDone;
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
	int res = u32NoMatch;

	if ((currentEvent->evType == evCharEntered) && (currentEvent->keyCode==kpFunction1)) 
	{	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  u32HandlingDone;
	}
	return res;
}


void entryTriacIdleState(void)
{
//	printf("entry I\n");
}

void exitTriacIdleState(void)
{
//	printf("exit I\n");
}

uStInt evTriacIdleChecker(void)
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


void entryIdleState(void)
{
//	printf("entry I\n");
}

void exitIdleState(void)
{
//	printf("exit I\n");
}

uStInt evIdleChecker(void)
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

void entryEditAmpsState(void)
{
//	printf("entry I\n");
}

void exitEditAmpsState(void)
{
//	printf("exit I\n");
}

uStInt evEditAmpsChecker(void)
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


void entryEditDurationState(void)
{
//	printf("entry I\n");
}

void exitEditDurationState(void)
{
//	printf("exit I\n");
}

uStInt evEditDurationChecker(void)
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

void entryTriacRunningState(void)
{
//	printf("entry I\n");
}

void exitTriacRunningState(void)
{
//	printf("exit I\n");
}

uStInt evTriacRunningChecker(void)
{
//	printf("check for event in State evStateIdle\n");

    // secondsTick
	//calcNextTriacDelay();
	//displayRunningValues();



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
 	eStateIdle,
 	0,
 	evTriacIdleChecker,
 	tfNull,
 	entryTriacIdleState,
 	exitTriacIdleState},

 	{eStateIdle,
 	eStateTriacIdle,
 	-1,
 	0,
 	evIdleChecker,
 	tfNull,
 	entryIdleState,
 	exitIdleState},

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



