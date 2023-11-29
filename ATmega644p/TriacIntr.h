

#ifndef TriacIntr_H
	#define TriacIntr_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
	
	
int8_t durationTimerReachead;
int8_t runningSecondsTick;

int64_t  secondCount;

int8_t adcTick;
void startAmpsADC();
void stopAmpsADC();
//void setDiffADC();
//void closeDiffADC();
void startSingleADC();
int16_t ampsADCValue();
int16_t diffADCValue();
double adcVoltage();

int16_t triacFireDurationTcnt2;   // centi-millis-secs, not exactly but approximate, PID will handle the rest

void startDurationTimer(int16_t secs);
int16_t getSecondsDurationTimerRemaining();
int16_t getSecondsInDurationTimer();

void stopDurationTimer();

void initInterrupts();
void setTriacFireDuration(int16_t cmsecs);
void startTriacRun();
void stopTriacRun();

void setCompletionAlarmOn();
void setCompletionAlarmOff();
void toggleCompletionAlarm();
void checkShortCircuitCondition();  // see comments at implementation in .c file
void resetCircuitAlarms();

//void printDValueVars();

/*
opto coupler based 0x (zerocross) handling. optocoupler based diode opens at 
*/

#endif
