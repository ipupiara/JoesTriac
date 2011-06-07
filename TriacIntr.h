

#ifndef TriacIntr_H
	#define TriacIntr_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
	
	
int8_t durationTimerReachead;
int8_t runningSecondsTick;

int8_t adcTick;
	
int16_t ampsADCValue();
int16_t triacTriggerDelayCms;   // centi-millis-secs, not exactly but approximate, PID will handle the rest


void startDurationTimer(int16_t secs);
int16_t getSecondsRemaining();

void stopDurationTimer();

void initInterrupts();
void setTriacTriggerDelay(int16_t cmsecs);
void startTriacRun();
void stopTriacRun();

#endif
