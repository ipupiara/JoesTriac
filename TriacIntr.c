
#include <avr/io.h>

//#include "uart.h"


#include "TriacIntr.h"


int16_t lastAmpsADCVal;

int16_t remainingTriacTriggerDelayCounts;

int16_t secondsRemaining;

#define triacAtmega128

#ifdef triacAtmega32

//Atmega324P

#define ocra2aValue 0XAA  // still to be defined

void startTriacTriggerDelay()
{
	remainingTriacTriggerDelayCounts = triacTriggerDelayCms;
	TIMSK2   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
	TCCR2B = 0b00000111  ; // CTC on CC2A , set clk / 1024, timer started
}

void stopTriacTriggerDelay()
{
	  TCCR2B = 0b00000000  ;  // CTC, timer stopped
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
		
		// todo  Trigger Triac			// T O D O   try to cast types so that only 8 bit single cycle operations are happen
		
		TCCR2B = 0b00000000  ;  // CTC, timer stopped
		TIMSK2  = 0x00;
	}
	if (remainingTriacTriggerDelayCounts < ocra2aValue) {    
		TCNT2 = remainingTriacTriggerDelayCounts; // 8-bit access already atomic 
		remainingTriacTriggerDelayCounts = 0;
	} else {
		remainingTriacTriggerDelayCounts -= ocra2aValue;
	}
}

ISR(ADC_vect)
{
	lastAmpsADCVal = ADC;
}

ISR(PCINT0_vect)
{
	if (PD2 == 0) {
		ADCSRA |= 0b01000000 ;
		stopTriacTriggerDelay();
        //   ADCSRA = 0b11001111;  // start one single AD conversion
	} else {
		startTriacTriggerDelay();
	}			  
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

// Timer 0 as ADC clock 
	  
	      TCCR0A = 0b00000010;  //  CTC 
		  
		TCCR0B = 0b00000101  ; // CTC on CC0A , set clk / 1024, timer started
	  
	  
		  OCR0A = 0xAA;  // counter top value  , just anything for start, will later be set by PID
	      TCNT0 = 0x00 ;  
	  
		TIMSK0  = 0b00000010;  // disa  ena, just let run ADC

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
		ADCSRA = 0b10001111;  // ADC ena, not yet start (single start mode), no Autotrigger, iflag = 0, inz ena, prescale /128
		ADCSRB = 0x00;  // no ACME, no free running mode

		sei();  // start interrupts if not yet started
}

void startTriacRun()
{
	EIFR = 0x00;
	EIMSK = 0x01;
}

void stopTriacRun()
{
	EIMSK = 0x00;
	stopTriacTriggerDelay();
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


#endif


#ifdef triacAtmega128


#define ocra2aValue 0XAA  // still to be defined

void startTriacTriggerDelay()
{
	remainingTriacTriggerDelayCounts = triacTriggerDelayCms;
	TIMSK2   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
	TCCR2B = 0b00000111  ; // CTC on CC2A , set clk / 1024, timer started
}

void stopTriacTriggerDelay()
{
	  TCCR2B = 0b00000000  ;  // CTC, timer stopped
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
		
		// todo  Trigger Triac			// T O D O   try to cast types so that only 8 bit single cycle operations are happen
		
		TCCR2B = 0b00000000  ;  // CTC, timer stopped
		TIMSK2  = 0x00;
	}
	if (remainingTriacTriggerDelayCounts < ocra2aValue) {    
		TCNT2 = remainingTriacTriggerDelayCounts; // 8-bit access already atomic 
		remainingTriacTriggerDelayCounts = 0;
	} else {
		remainingTriacTriggerDelayCounts -= ocra2aValue;
	}
}

ISR(ADC_vect)
{
	lastAmpsADCVal = ADC;
}

ISR(PCINT0_vect)
{
	if (PD2 == 0) {
		ADCSRA |= 0b01000000 ;
		stopTriacTriggerDelay();
        //   ADCSRA = 0b11001111;  // start one single AD conversion
	} else {
		startTriacTriggerDelay();
	}			  
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
	  
		TIMSK  = 0x00; // disa  Interrupt 
		ETIFR = 0x00;

// Timer 0 as ADC clock 
	  
	      TCCR0 = 0b00001111;  //  No Force, CTC , clk / 1024, no OC0 connected
		  OCR0 = 0xAA;  // counter top value  
	      TCNT0 = 0x00 ;  
		  
		  TIMSK &= 0b11111100  ;   // disa interrupts, just let run on ADC

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
		ADCSRA = 0b10001111;  // ADC ena, not yet start (single start mode), no Autotrigger, iflag = 0, int ena, prescale /128

		sei();  // start interrupts if not yet started
}



void startTriacRun()
{
	EIFR = 0x00;
	EIMSK = 0x01;
}

void stopTriacRun()
{
	EIMSK = 0x00;
	stopTriacTriggerDelay();
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
	
//	TIMSK   = 0b00010000;  //  Output Compare A Match Interrupt Enable 
	TIMSK |=  0b00010000;
	TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 24, timer started
}



void stopDurationTimer()
{
	TCCR1B = 0b00001000 ;  // CTC, timer stopped
	TIMSK &= 0b11000011 ;
//	TIMSK = 0x00;
	
}


#endif