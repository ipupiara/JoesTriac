
#include <avr/io.h>
#include <stdio.h>

//#include "uart.h"


#include "TriacIntr.h"
#include "TriacDefines.h"
#include "triacPID.h"

enum triacTriggerStates
{
	triacTriggerIdle,
	triacTriggerDelay,
	triacTriggerFireOn,
	triacTriggerFireOff
};

int8_t triacTriggerState;

int16_t lastAmpsADCVal;

int16_t remainingTriacTriggerDelayCounts;

int16_t triacTriggerDelayTime;

int16_t secondsDurationTimerRemaining;

int16_t secondsInDurationTimer;

int8_t adcCnt;

int16_t getSecondsDurationTimerRemaining()
{
	int16_t res;
	cli();
	res = secondsDurationTimerRemaining;
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

void setTriacTriggerDelayValues()
{
	if (remainingTriacTriggerDelayCounts < ocra2aValue) {
		TCNT2 = ocra2aValue - remainingTriacTriggerDelayCounts;
		triacTriggerDelayTime = triacTriggerDelayTime + remainingTriacTriggerDelayCounts;
		remainingTriacTriggerDelayCounts = 0;
	} else {
		remainingTriacTriggerDelayCounts -= ocra2aValue;
		TCNT2 = 0;
		triacTriggerDelayTime = triacTriggerDelayTime + ocra2aValue;
	}
}

void startTimer2()
{
	TIFR2 = 0x00;
	TIMSK2   = 0b00000010;  //  Output Compare A Match Interrupt Enable
	TCCR2B = 0b00000101  ; // CTC on CC2A , set clk / 128, timer 2 started
}

void stopTimer2()
{
	TCCR2B = 0b00000000  ;  // CTC, timer stopped
	TIMSK2  = 0x00;
}


void startTriacTriggerDelay(int8_t newState, int16_t delayDuration)  // must run protected between cli and sei
{
	if (delayDuration <= 0) { 
		delayDuration = 1;   // just a very short duration, but one that will happen in future
	}
	remainingTriacTriggerDelayCounts = delayDuration;
	setTriacTriggerDelayValues();
	triacTriggerState = newState;
	startTimer2();		
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

void setTriacFireDuration(int16_t cmsecs)
{
	cli();
	if (cmsecs < triggerDelayMax) {
		if (cmsecs > 0) {
			triacFireDurationCms = cmsecs;}
		else {
			triacFireDurationCms = 0;
		}
	} else {
		triacFireDurationCms = triggerDelayMax;
	}
	sei();
}


ISR(TIMER2_COMPA_vect)
{
	cli();
	if (remainingTriacTriggerDelayCounts <= 0) {
		if ((triacTriggerState == triacTriggerDelay) || (triacTriggerState == triacTriggerFireOff) ) {
			// Trigger Triac
			startTriacTriggerDelay(triacTriggerFireOn,1);
			PORTD |= 0x10;	
		} else if (triacTriggerState == triacTriggerFireOn) {
			PORTD &= ~0x10;	
			if (triacTriggerDelayTime >= triggerDelayMax) {
				stopTimer2();
				triacTriggerState = triacTriggerIdle;
			} else {
				startTriacTriggerDelay(triacTriggerFireOff,1);
			}
		} 
	} else {
		setTriacTriggerDelayValues();
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
		triacTriggerDelayTime = 0;
		startTriacTriggerDelay(triacTriggerDelay,  triggerDelayMax - triacFireDurationCms);
	}
	sei();		  
}   

ISR(TIMER0_COMPA_vect)
{    // needed for ADC so far..
}

ISR(TIMER1_COMPA_vect)
{
	secondsDurationTimerRemaining --;
	secondsInDurationTimer ++;
	if (secondsDurationTimerRemaining <= 0) {
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

		PORTD &= ~0x08; 		
		DDRD |= 0x08;			// set Portd pin 03 to be completionAlarm
		PORTD &= ~0x08; 		// and initialize with 0-value



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

		triacTriggerState = triacTriggerIdle;

		
	  
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
	secondsDurationTimerRemaining = secs;
	secondsInDurationTimer = 0;
	
	TIMSK1   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
	TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 24, timer started 
}

void stopDurationTimer()
{
	TCCR1B = 0b00001000 ;  // CTC, timer stopped
	TIMSK1 = 0x00;
	
}

void setCompletionAlarmOff()
{
	PORTD &= ~0x08; 		
}

void setCompletionAlarmOn()
{
	PORTD |= 0x08; 	
}

void toggleCompletionAlarm()
{  
	if (PORTD & 0x08) {
		setCompletionAlarmOff();
	} else {
		setCompletionAlarmOn();
	}
}



