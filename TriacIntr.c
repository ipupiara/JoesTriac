
#include <avr/io.h>

//#include "uart.h"


#include "TriacIntr.h"

int16_t triacTriggerDelaydms;   // deci-millis-secs, not exactly but approximate, PID will handle the rest

int16_t lastAmpsADCVal;

void startTriacTriggerDelay()
{
	OCR2A = triacTriggerDelaydms;
	TIMSK2   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
	TCCR2B = 0b00000111  ; // CTC on CC2A , set clk / 1024, timer started
}

void stopTriacTriggerDelay()
{
	  TCCR2B = 0b00000000  ;  // CTC, timer stopped
	  TIMSK2  = 0x00;
}	  

void setTriacTriggerDelay(int16_t dmsecs)
{
	cli();
	triacTriggerDelaydms = dmsecs;
	sei();
}

ISR(TIMER2_COMPA_vect)
{
	
}

ISR(ADC_vect)
{
	lastAmpsADCVal = ADC;
}

ISR(PCINT0_vect)
{
	if (PD2 == 0) {
		ADCSRA |= 0b01000000 ;
        //   ADCSRA = 0b11001111;  // start one single AD conversion
	} else {
	}			  
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
	  
	  // Timer 1 as Duration Timer
	  
			runningSecondsTimer = 0;
	  
	      TCCR1A = 0x00;  // normal mode or CTC dep.on TCCR1B
		//TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 1024, timer started
	  
		  TCCR1B = 0b00001000  ;  // CTC, timer stopped
	
		  TCCR1C = 0x00; // no Force output compare
	  
		  OCR1A = 0x2A30;  // counter top value  , this value at clk/1024 will cause a delay of exact 1 sec
	      TCNT1 = 0x00 ;  
	  
		TIMSK1  = 0x00; // disa  Interrupt 
//		TIMSK1   = 0b00000010;  //  Output Compare A Match Interrupt Enable 

	  // Timer 0 as ADC clock 
	  
	      TCCR0A = 0b00000010;  //  CTC 
		  
		TCCR0B = 0b00000101  ; // CTC on CC0A , set clk / 1024, timer started
	  
	  
		  OCR0A = 0xAA;  // counter top value  , just anything for start, will later be set by PID
	      TCNT0 = 0x00 ;  
	  
		TIMSK0  = 0x00; // disa  Interrupt 


	  // Timer 2 as Triac Trigger Delay Timer
	  
	      TCCR2A = 0b00000010;  //  CTC 
		  
		//TCCR2B = 0b00000111  ; // CTC on CC0A , set clk / 1024, timer started
	  
		  TCCR2B = 0b00000000  ;  // CTC, timer stopped
		  ASSR = 0x00;
	  
		  OCR2A = 0xAA;  // counter top value  , just anything for start, will later be set by PID
	      TCNT2 = 0x00 ;  
	  
		TIMSK2  = 0x00; // disa  Interrupt 
//		TIMSK2   = 0b00000010;  //  Output Compare A Match Interrupt Enable 

//     init ADC
		
		ADMUX = 0b01000000;      // AVCC as ref,  right adjust, mux to adc0
		ADCSRA = 0b10001111;  // ADC ena, not yet start (single start mode), no Autotrigger, iflag = 0, inz ena, prescale /128
		ADCSRB = 0x00;  // no ACME, no free running mode

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

int16_t ampsADCValue()
{
	int16_t res;
	cli();
	res = lastAmpsADCVal;
	sei();
	return res;
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





