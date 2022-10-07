
#if !defined(CCtateClassHeader)
#define CCtateClassHeader

#include "TStatechart.h"

extern TStatechart SJoesTriacStateChart;
extern TStatechart* PJoesTriacStateChart;



typedef enum eEventTypes
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
}fsmTriacEventType;


typedef struct  {
	fsmTriacEventType evType;
	union {
		int8_t keyCode;
		struct {			// currently not in use
			float   voltage;  
			int8_t  potiPos;
			int8_t  jobType;
		} zeroAdjustingState;
	}  evData;
} fsmTriacEvent ;




void startStateCharts();


void stopStateCharts();


bool processTriacFsmEvent(TStatechart* t,fsmTriacEvent* ev);




#endif


