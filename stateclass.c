//
// Module Name:	StateClass.cpp
//
// Description:	Test class for statechart engine.  This class exemplifies
//				any class that would actually contain and use the engine.
//
// Copyright (c) 1994-2005 Grant D. Schultz.	All rights reserved.
//

#include <avr/io.h>
//#include <iostream.h>
//#include <assert.h>

#include "TStatechart.h"
#include "StateClass.h"
#include "TriacIntr.h"

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
	//	printf("check for event in State evStateGrowBoxKeepingHumidity\n");
/*	if (currentEvent->evType == eReset)
	{
		resetHistoryReturns(&SHumidityStateChart);
		BEGIN_EVENT_HANDLER( PHumidityStateChart, eStateGrowBoxKeepingHumidity );
			// No event action.
		END_EVENT_HANDLER( PHumidityStateChart);
		
		return (u32HandlingDone);
	} */
	return (u32NoMatch);
}

void entryAskForCalibrationState(void)
{
//	printf("entry AskForCalibration\n");
}

void exitAskForCalibrationState(void)
{
//	printf("exit I\n");
}

uStInt evAskForCalibrationChecker(void)
{
//	printf("check for event in State evStateIdle\n");
/*
	if (currentEvent->evType == eValueAssignement) 
	{	if (HumidifyingLowerLimit > currentEvent->humidity)
		{
			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateHumidifying);
				// No event action.
			END_EVENT_HANDLER(PHumidityStateChart);
			return (u32HandlingDone);
		}
		if (DryingUpperLimit < currentEvent->humidity)
		{
			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateDrying);
				// No event action.
			END_EVENT_HANDLER(PHumidityStateChart);
			return (u32HandlingDone);
		}
	}*/
	return (u32NoMatch); 
}


uStInt evCalibratingChecker(void)
{
//	printf("check for event in State evStateIdle\n");

//	if (currentEvent->evType == eValueAssignement) 
//	{	if (HumidifyingLowerLimit > currentEvent->humidity)
//		{
//			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PHumidityStateChart);
//			return (u32HandlingDone);
//		}
//	}
	return (u32NoMatch);
}


void entryCalibrateLowState(void)
{
//	printf("entry I\n");
}

void exitCalibrateLowState(void)
{
//	printf("exit I\n");
}

uStInt evCalibrateLowChecker(void)
{
//	printf("check for event in State evStateIdle\n");

//	if (currentEvent->evType == eValueAssignement) 
//	{	if (HumidifyingLowerLimit > currentEvent->humidity)
//		{
//			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PHumidityStateChart);
//			return (u32HandlingDone);
//		}
//	}
	return (u32NoMatch);
}


void entryCalibrateHighState(void)
{
//	printf("entry I\n");
}

void exitCalibrateHighState(void)
{
//	printf("exit I\n");
}

uStInt evCalibrateHighChecker(void)
{
//	printf("check for event in State evStateIdle\n");

//	if (currentEvent->evType == eValueAssignement) 
//	{	if (HumidifyingLowerLimit > currentEvent->humidity)
//		{
//			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PHumidityStateChart);
//			return (u32HandlingDone);
//		}
//	}
	return (u32NoMatch);
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
//			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PHumidityStateChart);
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
//			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PHumidityStateChart);
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
//			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PHumidityStateChart);
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
//			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PHumidityStateChart);
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

//	if (currentEvent->evType == eValueAssignement) 
//	{	if (HumidifyingLowerLimit > currentEvent->humidity)
//		{
//			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PHumidityStateChart);
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
//			BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PHumidityStateChart);
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
 	tfNull,
 	tfNull},
	 
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



