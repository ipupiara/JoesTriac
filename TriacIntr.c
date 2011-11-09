
#include <avr/io.h>
#include <stdio.h>

//#include "uart.h"


#include "TriacIntr.h"
#include "TriacDefines.h"
#include "triacPID.h"


int16_t lastAmpsADCVal;

int16_t remainingTriacTriggerDelayCounts;

int16_t secondsRemaining;

int16_t secondsInDurationTimer;

int8_t adcCnt;

int16_t getSecondsRemaining()
{
	int16_t res;
	cli();
	res = secondsRemaining;
	sei();
	return res;
}

int16_t getSecondsInDurationTimer()
{
	int16_t res;
	cli();
	res = secondsInDurationTimer;
	sei();
	return res;
}

#define ocra2aValue 0XFC  // still to be defined

//int phaseCount;

int8_t t2Running;

void startTriacTriggerDelay()  // must run protected between cli and sei
{
	if (!t2Running) {
		TIFR2 = 0x00;
//		phaseCount = 0;

//		++phaseCount;
//		PORTA &= ~0x3C;
//		PORTA |= (1 << (1 + phaseCount));
		
		if (triacTriggerDelayCms > 0) {   
			remainingTriacTriggerDelayCounts = triggerDelayMax - triacTriggerDelayCms;
			if (remainingTriacTriggerDelayCounts <= 14) {
				remainingTriacTriggerDelayCounts = 15;  // dont set to 0  (means below tcnt2 to ocra2aValue)
														// cause timer will run once more

														// further on values below 11 cant be used by
														// 230 V 50Hz AC because trigger comes too early
														// and cant set on current
			}
			if (remainingTriacTriggerDelayCounts < ocra2aValue) {    
				TCNT2 = ocra2aValue - remainingTriacTriggerDelayCounts; 
				remainingTriacTriggerDelayCounts = 0;
			} else {
				remainingTriacTriggerDelayCounts -= ocra2aValue;
				TCNT2 = 0;
			}
			TIMSK2   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
			TCCR2B = 0b00000101  ; // CTC on CC2A , set clk / 128, timer 2 started
			t2Running = 1;
		}
	}
}

void stopTriacTriggerDelay()   // must run protected between cli and sei
{
/*	if (t2Running) {
		TCCR2B = 0b00000000  ;  // CTC, timer 2 stopped
		TIMSK2  = 0x00;
		t2Running = 0; 
	//	TCNT2 = 0;
	}
				does not work, no idea so far what goes wrong.
				Gives problems when t2 interrupt intersects with 
				external interrupt, trying to stop t2 timer. 
				Maybe not possible to synchronize properly ?????
				Suddenly t2-trigger-interrupt happens ocra2aValue-times earlier 
				(approx 2.8 ms, approx 1 phase in above debbuging code)  
				on oscilloscope if it intersects.....
				Left this problem for later for "time to market" reasons.
				Triac application runs like that at 50 Hz AC without any problems.
				t2-interrupt will stop early enough by itself with delay max value of 810 (TriacDefines.h).
				But would be more state of the art when stopping the trigger from 
				the ext-interrupt would  work
 
				*/  
}	  

void setTriacTriggerDelay(int16_t cmsecs)
{
	cli();
	triacTriggerDelayCms = cmsecs;
	sei();
}


ISR(TIMER2_COMPA_vect)
{
	cli();

	if (remainingTriacTriggerDelayCounts == 0) {

		TCCR2B = 0b00000000  ;  // CTC, timer stopped
		TIMSK2  = 0x00;

		// Trigger Triac
		PORTD |= 0x10;
		delay6pnt2d5us(10);   // approx 30 us of triac trigger
		PORTD &= ~0x10;			// handled synchronous
		t2Running = 0;
	} else {
		if (remainingTriacTriggerDelayCounts < ocra2aValue) { 
			TCNT2 =    ocra2aValue - remainingTriacTriggerDelayCounts;
			remainingTriacTriggerDelayCounts = 0;
		} else {
			remainingTriacTriggerDelayCounts -= ocra2aValue;  // TCNT2 is already zero due to interrupt
		}
	}
	sei();
}

ISR(ADC_vect)
{
	lastAmpsADCVal = ADC;
	++ adcCnt;

	if (adcCnt == pidStepDelays)  {     
		adcCnt = 0;
		adcTick = 1;
	}
}

ISR(INT0_vect)
{
	cli();
	if ((PIND & 0x04) != 0) {
		stopTriacTriggerDelay();
	} else {
		startTriacTriggerDelay();
	}
	sei();		  
}   

ISR(TIMER0_COMPA_vect)
{    // needed for ADC so far..
}

ISR(TIMER1_COMPA_vect)
{
	secondsRemaining --;
	secondsInDurationTimer ++;
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

		DDRA = 0b11110000;    // set pin 7 to 4 of port A as output for digital poti (zero adj)
		PORTA = 0b11100000;
		DIDR0 = 0x0F;			// disa digital input on a0..a3

		DDRD &= ~0x04;		// set PortD pin 2 as input for trigger Ext Int 0
		PORTD &=  ~0x04;   // without pullup 

		PORTD &= ~0x10; 		// done also before setting DDR to avoid eventual accidental triac trigger
		DDRD |= 0x10;			// set Portd pin 04 be Triac output
		PORTD &= ~0x10; 		// and initialize with 0-value

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

// Timer 0    used for ADC triggering  in TriaRunning mode
	  
		TCCR0A = 0b00000010;  //  CTC 

		OCR0A = 0xFF;  // counter top value, 0xFF means approx 42.18 ADC measures and write to mem per sec
					// far too much for our needs, but runs parallel except 
					//  the very short ADC-complete interrrupt
		TCNT0 = 0x00 ;  
  
//		TCCR0B = 0b00000101  ; // CTC on CC0A , set clk / 1024, timer started	  
//		TIMSK0  = 0b00000010;  // ena  interrupts, and let run ADC
// 		not yet start Timer0 and ADC, to be tested
		TCCR0B = 0b00000000  ; // CTC on CC0A , not yet started	  
		TIMSK0  = 0b00000000;


// Timer 2 as Triac Trigger Delay Timer

		t2Running = 0;
	  
		TCCR2A = 0b00000010;  //  CTC 

		//TCCR2B = 0b00000101  ; // CTC on CC0A , set clk / 128, timer started

		TCCR2B = 0b00000000  ;  // CTC, timer stopped
		ASSR = 0x00;

		OCR2A = ocra2aValue;  // counter top value  , just anything for start, will later be set by PID
		TCNT2 = 0x00 ;  

		TIMSK2  = 0x00; // disa  Interrupt 
		//		TIMSK2   = 0b00000010;  //  Output Compare A Match Interrupt Enable 

//  init ADC

		ADCSRA  = 0b00000111;  // disa ADC, ADATE, ADIE	
		adcTick = 0;
		adcCnt = 0;
		lastAmpsADCVal = 0;

		sei();  // start interrupts if not yet started
		
}

void setAmpsADC()
{
	ADCSRA  = 0b00000111;  // disa ADC, ADATE, ADIE	
	adcTick = 0;
	adcCnt = 0;

	if (ampsInputPin == avg) {
		ADMUX = 0b01000001;
	} else {
		ADMUX = 0b01000010;      // AVCC as ref,  right adjust, mux to adc2/adc1
	}
	ADCSRA = 0b10101111;  
							// int ena, prescale /128
							// ADC clock will run at 86400 hz, or max 6646. 
							//  read per sec,what is ok
							// for our settings of 42. read per sec	
							// or manuals start 

	ADCSRB = 0x03;  // no ACME, trigger ADC on Timer0 compare match

	TCCR0B = 0b00000101  ; // CTC on CC0A , set clk / 1024, timer 0 started	  
	TIMSK0  = 0b00000010;  // ena  interrupts, and let run ADC	
}

void closeAmpsADC()
{
	ADCSRA  = 0b00000111;  // disa ADC, ADATE, ADIE	

	TCCR0B = 0b00000000  ; // stop timer 0	  
	TIMSK0  = 0b00000000;  // 
}

void startTriacRun()
{
	resetPID();
	EIFR = 0x00;
	EIMSK = 0x01;  				// start external interrupt (zero pass detection)
}

void stopTriacRun()
{
	EIMSK = 0x00;				// stop external interrupt
	cli();
	stopTriacTriggerDelay();
	sei();
}

int16_t ampsADCValue()
{
	int16_t res;
	cli();
	res = lastAmpsADCVal;
	sei();
//	printf("ampsADC %i ",lastAmpsADCVal);
	return res;
}

/*
int16_t  valueFrom6Bit2Complement(int16_t adcV)
{
	if (adcV & 0x0200) {
		adcV--;
		adcV = ~(adcV | 0xFC00);
		adcV = - adcV;
	}
	return adcV;
}

int16_t diffADCValue()
{  
	int16_t res;
	res = ampsADCValue();
	res = valueFrom6Bit2Complement(res);
	return res;
}
*/

double adcVoltage()
{
	int16_t VHex;
	double   VFl;

	VFl = 0.0;

	VHex = ampsADCValue();
	VFl = (VHex * 5.0) / 0x03FF;
	
	return VFl;
}

void startDurationTimer(int16_t secs)
{
	durationTimerReachead = 0;
	secondsRemaining = secs;
	secondsInDurationTimer = 0;
	
	TIMSK1   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
	TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 24, timer started 
}

void stopDurationTimer()
{
	TCCR1B = 0b00001000 ;  // CTC, timer stopped
	TIMSK1 = 0x00;
	
}


