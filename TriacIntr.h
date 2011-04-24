

#ifndef TriacIntr_H
	#define TriacIntr_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
	
	
	int calibTimerReachead;



void startCalibrationTimer();
void stopCalibrationTimer();

void startTriacDurationTimer();
void stopTriacDurationTimer();

void initExtInterrupts();
void startExtInt();
void stopExtInt();


#endif
