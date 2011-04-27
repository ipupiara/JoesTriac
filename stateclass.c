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

TStatechart SHumidityStateChart;
TStatechart* PHumidityStateChart;

CGrowBoxEvent* currentEvent;

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
	}
	return (u32NoMatch); */
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

 	{eStateCalibrate,
 	eStateJoesTriac,
 	-1,
 	0,
 	evCalibrateChecker,
 	tfNull,
 	tfNull,
 	tfNull},
	 
	 {eStateCalibrateLow,
 	eStateCalibrate,
 	-1,
 	0,
 	evCalibrateLowChecker,
 	tfNull,
 	entryCalibrateLowState,
 	exitCalibrateLowState},
	 
 	{eStateCalibrateHigh,
 	eStateAskForCalibration,
 	-1,
 	0,
 	evCalibrateHighChecker,
 	tfNull,
 	entryCalibrateHighState,
 	exitCalibrateHighState},

 
 	{eStateNonVentilating,
 	eStateIdle,
 	-1,
 	0,
 	evNVChecker,
 	tfNull,
 	entryNVState,
 	exitNVState},

 	{eStateVentilating,
 	eStateIdle,
 	-1,
 	0,
 	evVChecker,
 	tfNull,
 	entryVState,
 	exitVState},

 	{eStateDrying,
 	eStateGrowBoxKeepingHumidity,
 	-1,
 	0,
 	evDChecker,
 	tfNull,
 	entryDState,
 	exitDState}
};


void startStateCharts()
{

#ifdef  sdccNULL 

	tfNull = (t_fvoid ) NULL;

#endif 

 	PHumidityStateChart = & SJoesTriacStateChart; 
	createTStatechart (& SJoesTriacStateChart, xaStates, eNumberOfStates, eStartState);
	
}


void stopStateCharts()
{
	destructTStatechart(&SJoesTriacStateChart);
}


bool processJoesTriacEvent(TStatechart* t,CJoesTriacEvent* ev)
{
	currentEvent = ev;
	return ProcessEvent(t);
}



