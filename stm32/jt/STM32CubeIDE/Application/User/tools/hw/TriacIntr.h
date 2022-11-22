

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


uint32_t getCurrentAmpsADCValue();
void setAmpsADCValue(uint32_t val);

extern uint32_t secondsDurationTimerRemaining;

//int8_t adcTick;
void startAmpsADC();
void stopAmpsADC();
void adcTickHandler(uint16_t adcVal);
////void setDiffADC();
////void closeDiffADC();
//void startSingleADC();
//int16_t ampsADCValue();
//int16_t diffADCValue();
//double adcVoltage();


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


void setTriacFireDuration(int32_t cmsecs);
uint32_t getTriacFireDuration();

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
