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


enum eEventTypes
{
	eKeyPressed,
	eKeyReleased,
	eTimeOutDurationTimer,
	eStartPressed,
	eStopPressed
};




typedef struct  {
	int evType;
	int8_t keyCode;
} CJoesTriacEvent ;




void startStateCharts();


void stopStateCharts();


bool processJoesTriacEvent(TStatechart* t,CGrowBoxEvent* ev);



#endif


