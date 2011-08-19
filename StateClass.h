
#if !defined(CCtateClassHeader)
#define CCtateClassHeader

#include "TStatechart.h"

TStatechart SJoesTriacStateChart;
TStatechart* PJoesTriacStateChart;



enum eEventTypes
{
	evCharEntered,
	evFunction1Pressed,
	evFunction2Pressed,
	evAstPressed,
	evNumPressed,
	evTimeOutDurationTimer,
	evSecondsTick,
	evStartPressed,
	evStopPressed,
	evZeroSignalOK,
	evAdcTick,
	evFatalError
};


typedef struct  {
	int evType;
	int8_t keyCode;
} CJoesTriacEvent ;




void startStateCharts();


void stopStateCharts();


bool processTriacEvent(TStatechart* t,CJoesTriacEvent* ev);




#endif


