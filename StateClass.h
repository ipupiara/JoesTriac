//
// Module Name:	StateClass.hpp
//
// Description:	class for statechart engine.  This class represents
//				any class that would actually contain and use the engine.
//


#if !defined(CCtateClassHeader)
#define CCtateClassHeader


//#include "stdtype.h"
#include "TStatechart.h"

#define HumidifyingUpperLimit  92.0
#define HumidifyingLowerLimit   98.5
#define DryingUpperLimit   93.0 
#define DryingLowerLimit  94.5


enum eEventTypes
{
	eReset,
	eValueAssignement,
	eVentilationStartTimer,
	eVentilationStopTimer
};




typedef struct  {
	int evType;
	float temperature;
	float humidity;
	
//	const CGrowBoxEvent& operator=(const CGrowBoxEvent& src);
} CGrowBoxEvent ;




void startStateCharts();


void stopStateCharts();


bool processGrowBoxEvent(TStatechart* t,CGrowBoxEvent* ev);



#endif


