
#include <avr/io.h>

//#include "uart.h"


#include "TriacIntr.h"

int16_t triacTriggerDelaydms;   // deci-millis-secs, not exactly but approximate, PID will handle the rest

void startTriacTriggerDelay()
{
	OCR0A = triacTriggerDelaydms;
	TIMSK0   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
	TCCR0B = 0b00000101  ; // CTC on CC0A , set clk / 24, timer started
}

void stopTriacTriggerDelay()
{
	  TCCR0B = 0b00000000  ;  // CTC, timer stopped
	  TIMSK0  = 0x00;
}	  

void setTriacTriggerDelay(int16_t dmsecs)
{
	cli();
	triacTriggerDelaydms = dmsecs;
	sei();
}

ISR(TIMER0_COMPA_vect)
{
	
}

ISR(PCINT0_vect)
{
      
}   

ISR(TIMER1_COMPA_vect)
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
		//TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 1024, timer started
	  
		  TCCR1B = 0b00001000  ;  // CTC, timer stopped
	
		  TCCR1C = 0x00; // no Force output compare
	  
		  OCR1A = 0x2A30;  // counter top value  , this value at clk/1024 will cause a delay of exact 1 sec
	      TCNT1 = 0x00 ;  
	  
		TIMSK1  = 0x00; // disa  Interrupt 
//		TIMSK1   = 0b00000010;  //  Output Compare A Match Interrupt Enable 

	  // Timer 0 as Triac Trigger Delay Timer
	  
	      TCCR0A = 0b00000010;  //  CTC 
		  
		//TCCR0B = 0b00000101  ; // CTC on CC0A , set clk / 24, timer started
	  
		  TCCR0B = 0b00000000  ;  // CTC, timer stopped
	  
		  OCR0A = 0xAA;  // counter top value  , just anything for start, will later be set by PID
	      TCNT0 = 0x00 ;  
	  
		TIMSK0  = 0x00; // disa  Interrupt 
//		TIMSK0   = 0b00000010;  //  Output Compare A Match Interrupt Enable 

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





