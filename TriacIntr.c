
#include <avr/io.h>

//#include "uart.h"


#include "TriacIntr.h"

int16_t triacTriggerDelayus;

void setTriacTriggerDelay(int16_t usecs)
{
	triacTriggerDelayus = usecs;
}

ISR(PCINT0_vect)
{
      
}   


void initInterrupts()
{
	// Ext. Interrupt
	  cli();
	  EICRA = 0x01;   // both, fall/rise edge trigger    
      EIMSK = 0x00;   
      // sei();  
	  
	  // Timer 2 as Duration Timer
	  
	      TCCR1A = 0x00;  // normal mode or CTC dep.on TCCR1B
		//TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 24, timer started
	  
		  TCCR1B = 0b00001000  ;  // CTC, timer stopped
	
		  TCCR1C = 0x00; // no Force output compare
	  
		  OCR1A = 0xAAAA;  // counter top value  , still to be defined
	      TCNT1 = 0x00 ;  
	  
		TIMSK1  = 0x00; // disa  Interrupt 
//		TIMSK1   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
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
	
	TIMSK1   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
	TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 24, timer started
	sei();
}



void stopDurationTimer()
{
	TCCR1B = 0b00001000 ;  // CTC, timer stopped
	TIMSK1 = 0x00;
	
}





