

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



extern uint32_t secondsDurationTimerRemaining;  // todo check this memory direct access

//int8_t adcTick;
void startAmpsADC();
void stopAmpsADC();
void adcValueReceived(uint16_t adcVal);
uint32_t getCurrentAmpsADCValue();
float adcVoltage();

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

extern uint16_t triacTriggerDelay;
void setTriacTriggerDelay(int32_t cmsecs);
uint16_t getTriacTriggerDelay();

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
