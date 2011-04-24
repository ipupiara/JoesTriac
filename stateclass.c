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

extern const uStInt u32HandlingDone;
extern const uStInt u32NoMatch;

TStatechart SHumidityStateChart;
TStatechart* PHumidityStateChart;

CGrowBoxEvent* currentEvent;

// This defines and names the states the class has.
enum eStates
{
	eStateGrowBoxKeepingHumidity,
	eStartState = eStateGrowBoxKeepingHumidity,
	eStateHumidifying,
	eStateIdle,
	eStateNonVentilating,
	eStateVentilating,
	eStateDrying,
	eNumberOfStates
};


void entryGBKHState(void)
{
//	printf("entry GHHK\n");

}

void exitGBKHState(void)
{
//	printf("exit GHHK\n");
}

uStInt evGBKHChecker(void)
{
	//	printf("check for event in State evStateGrowBoxKeepingHumidity\n");
	if (currentEvent->evType == eReset)
	{
		resetHistoryReturns(&SHumidityStateChart);
		BEGIN_EVENT_HANDLER( PHumidityStateChart, eStateGrowBoxKeepingHumidity );
			// No event action.
		END_EVENT_HANDLER( PHumidityStateChart);
		
		return (u32HandlingDone);
	}
	return (u32NoMatch);
}



void entryHState(void)
{
//	printf("entry H\n");
}

void exitHState(void)
{
//	printf("exit H\n");
}

uStInt evHChecker(void)
{
//	printf("check for event in State evStateHumidifying\n");
	if ((currentEvent->evType == eValueAssignement) && (HumidifyingUpperLimit < currentEvent->humidity))
	{
		BEGIN_EVENT_HANDLER( PHumidityStateChart, eStateIdle);
			// No event action.
		END_EVENT_HANDLER( PHumidityStateChart);
		return (u32HandlingDone);
	}
	return (u32NoMatch);
}


void entryIState(void)
{
//	printf("entry I\n");
}

void exitIState(void)
{
//	printf("exit I\n");
}

uStInt evIChecker(void)
{
//	printf("check for event in State evStateIdle\n");

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
	return (u32NoMatch);
}

void entryNVState(void)
{
//	printf("entry NV\n");
}

void exitNVState(void)
{
//	printf("exit NV\n");
}

uStInt evNVChecker(void)
{
//	printf("check for event in State evStateNonVentilating\n");

	if (currentEvent->evType == eVentilationStartTimer) 
	{
		BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateVentilating);
			// No event action.
		END_EVENT_HANDLER(PHumidityStateChart);
		return (u32HandlingDone);
	
	}
	return (u32NoMatch);
}


void entryVState(void)
{
 //	printf("entry V\n");
}

void exitVState(void)
{
//	printf("exit V\n");
}

uStInt evVChecker(void)
{
//	printf("check for event in State evStateVentilating\n");

	if (currentEvent->evType == eVentilationStopTimer) 
	{
		BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateNonVentilating);
			// No event action.
		END_EVENT_HANDLER(PHumidityStateChart);
		return (u32HandlingDone);
	
	}
	return (u32NoMatch);
}

void entryDState(void)
{
// 	printf("entry D\n");
}

void exitDState(void)
{
//	printf("exit D\n");
}

uStInt evDChecker(void)
{
//	printf("check for event in State evStateDrying\n");
	if ((currentEvent->evType == eValueAssignement) && (DryingLowerLimit > currentEvent->humidity))
	{
		BEGIN_EVENT_HANDLER(PHumidityStateChart, eStateIdle);
			// No event action.
		END_EVENT_HANDLER(PHumidityStateChart);
		return (u32HandlingDone);
	}
	return (u32NoMatch);
}

#ifndef  sdccNULL 

#define tfNull 0

#else

t_fvoid  tfNull;

#endif 

xStateType xaStates[eNumberOfStates] = {
 	{eStateGrowBoxKeepingHumidity,    // name
 	-1,									//parent
 	eStateIdle,    // default substate
 	0,    // keep history
 	evGBKHChecker,    // event checking fu
   tfNull,       // def state entry function
 	entryGBKHState,     //    entering state
 	exitGBKHState},     // exiting state

 	{eStateHumidifying,
 	eStateGrowBoxKeepingHumidity,
 	-1,
 	0,									
 	evHChecker,
 	tfNull,
 	entryHState,
 	exitHState},

 	{eStateIdle,
 	eStateGrowBoxKeepingHumidity,
 	eStateNonVentilating,
 	0,
 	evIChecker,
 	tfNull,
 	entryIState,
 	exitIState},

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

 	PHumidityStateChart = & SHumidityStateChart; 
	createTStatechart (& SHumidityStateChart, xaStates, eNumberOfStates, eStartState);
	
}


void stopStateCharts()
{
	destructTStatechart(&SHumidityStateChart);
}


bool processGrowBoxEvent(TStatechart* t,CGrowBoxEvent* ev)
{
	currentEvent = ev;
	return ProcessEvent(t);
}



