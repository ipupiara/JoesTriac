
#if !defined(CCtateClassHeader)
#define CCtateClassHeader

#include "TStatechart.h"

extern TStatechart SJoesTriacStateChart;
extern TStatechart* PJoesTriacStateChart;



enum eEventTypes
{
	evCharEntered,
	evF1Pressed,
	evF2Pressed,
	evAstPressed,
	evNumPressed,
	evTimeOutDurationTimer,
	evSecondsTick,
	evStartPressed,
	evStopPressed,
	evZeroSignalOK,
	evAdcTick,
	evTWIDataReceived,
	evEditFinished,
	evFatalError
};


typedef struct  {
	int evType;
	union {
		int8_t keyCode;
		struct {			// currently not in use
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


