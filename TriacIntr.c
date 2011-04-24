
#include <avr/io.h>
#include "TriacIntr.h"








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

void startCalibrationTimer()
{
	calibTimerReachead = 0;
	
	
}

void stopCalibrationTimer()
{
	
}

void startTriacDurationTimer()
{
	
}

void stopTriacDurationTimer()
{
	
}
