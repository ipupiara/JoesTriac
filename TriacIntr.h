

#ifndef TriacIntr_H
	#define TriacIntr_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
	
	
	int8_t durationTimerReachead;
	int8_t runningSecondsTimer;
	
	int16_t ampsADCValue();


void startDurationTimer(int16_t secs);
void stopDurationTimer();


void initInterrupts();
void setTriacTriggerDelay(int16_t usecs);
void startExtInt();
void stopExtInt();


#endif
