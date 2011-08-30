
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

enum jobStates 
{
	idle,
	persistentZeroAdjust,
	transientZeroAdjust,
	fatalError	
};


typedef struct  {
	int evType;
	union {
		int8_t keyCode;
		struct {
			float   voltage;
			int8_t  potiPos;
			int8_t  jobType;
		} zeroAdjustingState;
	}  evData;
} CJoesTriacEvent ;




void startStateCharts();


void stopStateCharts();


bool processTriacEvent(TStatechart* t,CJoesTriacEvent* ev);




#endif


