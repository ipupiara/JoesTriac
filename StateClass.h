
#if !defined(CCtateClassHeader)
#define CCtateClassHeader

#include "TStatechart.h"

enum eEventTypes
{
	evCharEntered,
	evFunction1Pressed,
	evFunction2Pressed,
	evRedPressed,
	evWhitePressed,
	evTimeOutDurationTimer,
	evSecondsTick,
	evStartPressed,
	evStopPressed
};


typedef struct  {
	int evType;
	int8_t keyCode;
} CJoesTriacEvent ;




void startStateCharts();


void stopStateCharts();


bool processTriacEvent(TStatechart* t,CJoesTriacEvent* ev);



#endif


