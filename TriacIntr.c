
#include <avr/io.h>
#include "TriacIntr.h"




int16_t triacTriggerDelayus;


void setTriacTriggerDelay(int16_t usecs)
{
	triacTriggerDelayus = usecs;
}


ISR(PCINT0_vect)
{
      
}   


void initExtInterrupts()
{
	  cli();
	  EICRA = 0x01;   // both, fall/rise edge trigger    
      EIMSK = 0x00;   
      // sei();  
	  
}

void startExtInt()
{
	EIFR = 0x00;
	EIMSK = 0x01;
}

void stopExtInt()
{
	EIMSK = 0x00;
}



void startDurationTimer(int16_t secs)
{
	durationTimerReachead = 0;
}



void stopDurationTimer()
{
	
}