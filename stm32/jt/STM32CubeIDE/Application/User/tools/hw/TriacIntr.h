

#ifndef TriacIntr_H
	#define TriacIntr_H

#ifdef  __cplusplus
extern "C"
{
#endif
	
#include <stdio.h>

#define maxSecsPossible  0xFFFFFFFF
	
//int8_t durationTimerReachead;
//int8_t runningSecondsTick;

//extern int64_t  secondCount;


//	#define stmTriggerDelayMax  5000
	#define stmTriggerDelayMax  4950
	#define kStepUnitsFactor  5
	#define triacDelayPsc  197   // (defaultTriacDelayPsc / kStepUnitsFactor)




extern uint32_t secondsDurationTimerRemaining;  // todo check this memory direct access

extern uint32_t amtExtiMissedTotal;
extern uint32_t   maxMissedExti;
extern uint32_t  amtIllegalExti;
extern uint32_t amtSyncMissed;


void startDurationTimer(uint32_t secs);
uint32_t getSecondsDurationTimerRemaining();
uint32_t getSecondsInDurationTimer();
void resumeDurationTimer();
void haltDurationTimer();
void stopDurationTimer();
void durationTimerTick();

void setBuzzerOn();
void setBuzzerOff();
void toggleBuzzer();

extern int32_t triacTriggerDelay;
void setTriacTriggerDelay(int32_t cmsecs);
int32_t getTriacTriggerDelay();

void startTriacRun();
void stopTriacRun();

void setCompletionAlarmOn();
void setCompletionAlarmOff();
void toggleCompletionAlarm();

void initTriacIntr();
//
//void printDValueVars();
//
///*
//opto coupler based 0x (zerocross) handling. optocoupler based diode opens at
//*/

#ifdef  __cplusplus
}
#endif


#endif
