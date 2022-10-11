
#if !defined(CCtateClassHeader)
#define CCtateClassHeader

#include "TStatechart.h"

#ifdef  __cplusplus
extern "C"
{
#endif


extern TStatechart SJoesTriacStateChart;
extern TStatechart* PJoesTriacStateChart;



typedef enum eEventTypes
{
	evTimeOutDurationTimer,
	evSecondsTick,
	evStartPressed,
	evStopPressed,
	evZeroSignalOK,
	evAdcTick,
	evConfigBackPressed,
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


#ifdef  __cplusplus
}
#endif


#endif


