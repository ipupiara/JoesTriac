
#include <avr/io.h>
#include <stdio.h>

//#include "uart.h"


#include "TriacIntr.h"
#include "TriacDefines.h"


int16_t lastAmpsADCVal;

int16_t remainingTriacTriggerDelayCounts;

int16_t secondsRemaining;

int8_t adcCnt;

int16_t getSecondsRemaining()
{
	int16_t res;
	cli();
	res = secondsRemaining;
	sei();
	return res;
}



#define ocra2aValue 0XAA  // still to be defined

void startTriacTriggerDelay()
{
	remainingTriacTriggerDelayCounts = triacTriggerDelayCms;
	TIMSK2   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
	TCCR2B = 0b00000111  ; // CTC on CC2A , set clk / 1024, timer 2 started
}

void stopTriacTriggerDelay()
{
	  TCCR2B = 0b00000000  ;  // CTC, timer 2 stopped
	  TIMSK2  = 0x00;
}	  

void setTriacTriggerDelay(int16_t cmsecs)
{
	cli();
	triacTriggerDelayCms = cmsecs;
	sei();
}

ISR(TIMER2_COMPA_vect)
{
	if (remainingTriacTriggerDelayCounts == 0) {
				
		TCCR2B = 0b00000000  ;  // CTC, timer stopped
		TIMSK2  = 0x00;

		// Trigger Triac

	}
	if (remainingTriacTriggerDelayCounts < ocra2aValue) {    
		TCNT2 = ocra2aValue - remainingTriacTriggerDelayCounts; // 8-bit access already atomic 
		remainingTriacTriggerDelayCounts = 0;
	} else {
		remainingTriacTriggerDelayCounts -= ocra2aValue;
	}
}

ISR(ADC_vect)
{
	lastAmpsADCVal = ADC;
	++ adcCnt;
	if (adcCnt == 30)  {
		adcCnt = 0;
		adcTick = 1;
	}
}

ISR(INT0_vect)
{
	printf(" int 0 \n");
	if (PD2 == 0) {
		PORTA |= 0x80;
//		stopTriacTriggerDelay();
	} else {
		PORTA &= ~0x80;
//		startTriacTriggerDelay();
	}			  
}   

ISR(TIMER0_COMPA_vect)
{    // needed for ADC so far..
}

ISR(TIMER1_COMPA_vect)
{
	secondsRemaining --;
	if (secondsRemaining <= 0) {
		stopDurationTimer();
		durationTimerReachead = 1;
	} else {
		runningSecondsTick = 1;
	}
	
}



void initInterrupts()
{
// Ext. Interrupt

		DDRA |= 0x80;    // set pin 7 of port A as output for debugging
	
	  EICRA = 0x01;   // both, fall/rise edge trigger    
      EIMSK = 0x00;   
	  
// Timer 1 as Duration Timer
	  
			runningSecondsTick = 0;
	  
	      TCCR1A = 0x00;  // normal mode or CTC dep.on TCCR1B
		//TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 1024, timer started
	  
		  TCCR1B = 0b00001000  ;  // CTC, timer stopped
	
		  TCCR1C = 0x00; // no Force output compare
	  
		  OCR1A = 0x2A30;  // counter top value  , this value at clk/1024 will cause a delay of exact 1 sec
	      TCNT1 = 0x00 ;  
	  
		TIMSK1  = 0x00; // disa  Interrupt 
	//		TIMSK1   = 0b00000010;  //  Output Compare A Match Interrupt Enable 

// Timer 0    used for ADC triggering
	  
	      TCCR0A = 0b00000010;  //  CTC 

		  OCR0A = 0x4F;  // counter top value, 0xFF means 42.18 ADC measures and write to mem per sec
						// far too much for our needs, but runs parallel except 
						//  the very short ADC-complete interrrupt
	      TCNT0 = 0x00 ;  
	  	  
//		TCCR0B = 0b00000101  ; // CTC on CC0A , set clk / 1024, timer started	  
//		TIMSK0  = 0b00000010;  // ena  interrupts, and let run ADC
// 	not yet start Timer0 and ADC, to be tested
		TCCR0B = 0b00000000  ; // CTC on CC0A , not yet started	  
		TIMSK0  = 0b00000000;
	

// Timer 2 as Triac Trigger Delay Timer
	  
	      TCCR2A = 0b00000010;  //  CTC 
		  
		//TCCR2B = 0b00000111  ; // CTC on CC0A , set clk / 1024, timer started
	  
		  TCCR2B = 0b00000000  ;  // CTC, timer stopped
		  ASSR = 0x00;
	  
		  OCR2A = 0xAA;  // counter top value  , just anything for start, will later be set by PID
	      TCNT2 = 0x00 ;  
	  
		TIMSK2  = 0x00; // disa  Interrupt 
//		TIMSK2   = 0b00000010;  //  Output Compare A Match Interrupt Enable 

//  init ADC
		
		ADMUX = 0b01000000;      // AVCC as ref,  right adjust, mux to adc0
		ADCSRA = 0b10101111;  // ADC ena, not yet start (single start mode),  Autotrigger, iflag = 0
								// int ena, prescale /128
								// ADC clock will run at 86400 hz, or max 6646. read per sec,what is ok
								// for our settings of 42. read per sec	

		ADCSRB = 0x03;  // no ACME, trigger ADC on Timer0 compare match

		adcTick = 0;
		adcCnt = 0;

		sei();  // start interrupts if not yet started
		
}

void startTriacRun()
{
	EIFR = 0x00;
	EIMSK = 0x01;  				// start external interrupt (zero pass detection)

	TCCR0B = 0b00000101  ; // CTC on CC0A , set clk / 1024, timer 0 started	  
	TIMSK0  = 0b00000010;  // ena  interrupts, and let run ADC
}

void stopTriacRun()
{
	EIMSK = 0x00;				// stop external interrupt
	stopTriacTriggerDelay();

	TCCR0B = 0b00000000  ; // stop timer 0	  
	TIMSK0  = 0b00000000;  // to stop ADC
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
	secondsRemaining = secs;
	
	TIMSK1   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
	TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 24, timer started 
}



void stopDurationTimer()
{
	TCCR1B = 0b00001000 ;  // CTC, timer stopped
	TIMSK1 = 0x00;
	
}


