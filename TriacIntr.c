
#include <avr/io.h>
#include <stdio.h>

//#include "uart.h"


#include "TriacIntr.h"
#include "TriacDefines.h"
#include "triacPID.h"

int16_t lastAmpsADCVal;

uint16_t  shortCircuitAlarmAmpsADCValue;

int16_t remainingTriacTriggerDelayCounts;

int16_t triacTriggerTimeTcnt2;

int16_t secondsDurationTimerRemaining;

int16_t secondsInDurationTimer;

int8_t adcCnt;

int16_t amtInductiveRepetitions;

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

#define ocra2aValueMax 0XFC  // still to be defined

void setTcnt2AndOcra2a(int16_t newTcnt2Val,int16_t newOcra2a)
{
	// timer must be stopped to set tcnt, because else, on an 
	// unprotected set, the timer itself could interfere with the *non double buffered feature" write access.
	// resulting in a more or less random set value.
	int8_t tccr2bStack;
	tccr2bStack = TCCR2B;
	TCCR2B = 0b00000000  ;  // CTC, timer stopped		
	if (TCNT2 != newTcnt2Val) {  // dont set if not needed , because  .....
		TCNT2 = newTcnt2Val;	
		if (newOcra2a == (TCNT2 + 1)) {++ newOcra2a; }  // .... updating avoids triggering of next clock cycle, but needs overnext.
	}
	OCR2A = newOcra2a;  
	TCCR2B = tccr2bStack  ; // set previous value
}

void setTriacTriggerDelayValues()
{
	if (remainingTriacTriggerDelayCounts < ocra2aValueMax) {		
		setTcnt2AndOcra2a (0, remainingTriacTriggerDelayCounts);
		triacTriggerTimeTcnt2 += remainingTriacTriggerDelayCounts;
		remainingTriacTriggerDelayCounts = 0;
	} else {
		remainingTriacTriggerDelayCounts -= ocra2aValueMax;
		setTcnt2AndOcra2a(0, ocra2aValueMax);
		triacTriggerTimeTcnt2 +=  ocra2aValueMax;
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
	TIFR2 = (1<< OCF2A);    // cleared by writing a "logic" one to the flag
}


void startTriacTriggerDelay( int16_t delayDuration)  // must run protected between cli and sei
{
	if (delayDuration <= 0) { 
		delayDuration = 1;   // just a very short duration, but one that will happen in future
	}
	remainingTriacTriggerDelayCounts = delayDuration;
	setTriacTriggerDelayValues();
	startTimer2();		
}

void setTriacFireDuration(int16_t durationTcnt2)
{
	cli();
	if (durationTcnt2 < triggerDelayMaxTcnt2) {
		if (durationTcnt2 > 0) {
			triacFireDurationTcnt2 = durationTcnt2;}
		else {
			triacFireDurationTcnt2 = 0;
		}
	} else {
		triacFireDurationTcnt2 = triggerDelayMaxTcnt2;
	}
	sei();
}

void calcAmtInductiveRepetitions(int16_t triacFireDurationTcnt2)
{
	if ( inductiveLoad)  {
		float amtInductiveRepetitionsF = 0.0;
		float triacFireDurationTcnt2F = triacFireDurationTcnt2;
//		amtInductiveRepetitions = ((triacFireDurationTcnt2 * ( 1  /(11.0592e+6  /128) )) * 1.0e+6  ) /  measuredRepetitionIntervalus; 
		amtInductiveRepetitionsF = (triacFireDurationTcnt2F * 11.63  )  /  measuredRepetitionIntervalus; 
		// always cut off modulo part when converting to int
		amtInductiveRepetitions = amtInductiveRepetitionsF;   // tobe  debugged
	} else {
		amtInductiveRepetitions = 1;
	}
	
}

ISR(TIMER2_COMPA_vect)
{
	cli();
	if (remainingTriacTriggerDelayCounts <= 0) {
		PORTD |= 0x10;
		delay6pnt2d5us(triacTriggerLength);   // approx 5 us of triac trigger , try later half or even less, measured 7 with oscilloscope
		PORTD &= ~0x10;			// handled synchronous
		if ((triacTriggerTimeTcnt2 >= triggerDelayMaxTcnt2) ) {   //  || (amtInductiveRepetitions <= 0)  ) {
			stopTimer2();
		} else {
			startTriacTriggerDelay(delayBetweenTriacTriggers);
			// --amtInductiveRepetitions;
		}
	} else {
		setTriacTriggerDelayValues();
	}
	sei();	
}

ISR(ADC_vect)
{
	cli();
	lastAmpsADCVal = ADC;
	sei();
	++ adcCnt;
#ifdef shortCircuitAlarmSupported	
	checkShortCircuitCondition();
#endif	
	if (adcCnt == pidStepDelays)  {     
		adcCnt = 0;
		adcTick = 1;
	}
}

ISR(INT0_vect)
{
	cli();
	if ((PIND & 0x04) != 0) {
		stopTimer2();		
	} else {
		triacTriggerTimeTcnt2 = 0;
		if (triacFireDurationTcnt2 > 0)  {
			startTriacTriggerDelay(  triggerDelayMaxTcnt2 - triacFireDurationTcnt2);
//			calcAmtInductiveRepetitions(triacFireDurationTcnt2);
		}
	}
	sei();		  
}   

ISR(TIMER0_COMPA_vect)
{    // needed for ADC so far..
}

int8_t sec10Counter;
int8_t shortCircuitSec10Counter;
uint8_t  shortCircuitAlarmOn;
int8_t  dValueSec10Counter;
uint8_t  dValueAlarmOn;

ISR(TIMER1_COMPA_vect)
{
#ifdef shortCircuitAlarmSupported
	cli();
	if (shortCircuitSec10Counter > 0)  {
		-- shortCircuitSec10Counter;
		if (shortCircuitSec10Counter == 0)  {
			shortCircuitSec10Counter = -1;
			shortCircuitAlarmOn = 1;
			sprintf((char *) &lastFatalErrorString,"short circuit");
			fatalErrorOccurred = 1;
		}
	}
	if (dValueSec10Counter > 0)  {
		-- dValueSec10Counter;
		if (dValueSec10Counter == 0)  {
			dValueSec10Counter = -1;
			if (dValueAlarmFatal > 0) {
				sprintf((char *) &lastFatalErrorString,"DValue low/high");
				fatalErrorOccurred = 1;
			} else {
				dValueAlarmOn = 1;
			}
		}
	}
	sei();
	if ((shortCircuitAlarmOn > 0) || (dValueAlarmOn > 0)) {
		if ((sec10Counter == 5) || (sec10Counter ==  10 )) {
			toggleCompletionAlarm();
		}
	}  else {
		setCompletionAlarmOff();
	}
#endif	
	if ( sec10Counter >= 10)  {
		sec10Counter = 0;
		secondsDurationTimerRemaining --;
		secondsInDurationTimer ++;
		if (secondsDurationTimerRemaining <= 0) {
			stopDurationTimer();
			durationTimerReachead = 1;
		} else {
			runningSecondsTick = 1;
		}
	}  else {
		++ sec10Counter;
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
			sec10Counter = 0;
			resetAlarms();
	  
		TCCR1A = 0x00;  // normal mode or CTC dep.on TCCR1B
		//TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 1024, timer started

		TCCR1B = 0b00001000  ;  // CTC, timer stopped

		TCCR1C = 0x00; // no Force output compare

//		OCR1A = 0x2A30;  // counter top value  , this value at clk/1024 will cause a delay of exact 1 sec
		OCR1A = 0x0438;  // counter top value  , this value at clk/1024 will cause a delay of exact 1/10 sec
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
	  
		TCCR2A = 0b00000010;  //  CTC 

		//TCCR2B = 0b00000101  ; // CTC on CC0A , set clk / 128, timer started

		TCCR2B = 0b00000000  ;  // CTC, timer stopped
		ASSR = 0x00;

		OCR2A = ocra2aValueMax;  // counter top value  , just anything for start, will later be set by PID
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

void startAmpsADC()
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

void stopAmpsADC()
{
	ADCSRA  = 0b00000111;  // disa ADC, ADATE, ADIE	

	TCCR0B = 0b00000000  ; // stop timer 0	  
	TIMSK0  = 0b00000000;  // 
}

void startTriacRun()
{
	resetPID();
	shortCircuitAlarmAmpsADCValue = adcValueForAmps(shortCircuitAlarmAmps);
	shortCircuitAlarmOn = 0;
	shortCircuitSec10Counter = 0;
	dValueAlarmOn = 0;
	dValueSec10Counter = 0;
	startAmpsADC();
	EIFR = 0x00;
	EIMSK = 0x01;  				// start external interrupt (zero pass detection)
}

void stopTriacRun()
{
	resetAlarms();    // stops also circuit alarms (shortCircuit, DValue)
	EIMSK = 0x00;				// stop external interrupt
	cli();
	stopTimer2();
	sei();
	stopAmpsADC();
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

void resetAlarms()
{
	shortCircuitAlarmOn = 0;
	dValueAlarmOn = 0;
	shortCircuitSec10Counter = 0;
	dValueSec10Counter = 0;
	setCompletionAlarmOff();
	
}

//  checkShortCircuitCondition, pn 27oct2016
//  due to recent events in jo's production, we created
//  this method which should be able to detect a behaviour that indicates a "short circuit" abnormality 
//  on the load side of the triac. compared to a not regulated power source it is much more difficult
//  to find such an abnormality on a regulated one, because our triac application immediately starts regulating the 
//  current down as soon as a short circuit takes place, ie. a drastically reduction of the 
//  load resistance. Anyhow some indicator might show such a situation, but there will be always
//  some uncertainty in the determination of such situation and wrong alerts have to be minimized
//  as far as anyhow possible, because each interrupt of the welding process due to wrong alert 
//  means a high risk of material loss.
//  indicators might be:
//  - low triac delay value  - rapid increase of current to high level for short time before regulation down
//  - hardware problems on triac regulation resulting in regulation loss

void checkShortCircuitCondition()
{
	cli();
	if (lastAmpsADCVal > shortCircuitAlarmAmpsADCValue) {
		if (shortCircuitSec10Counter == 0)  {
			shortCircuitSec10Counter = shortCircuitAlarmSecond10Barrier;
		} 
	
	} else {
		shortCircuitSec10Counter = 0;
		shortCircuitAlarmOn = 0;
	}
	if ((triacFireDurationTcnt2 > dValueAlarmHigh) || (triacFireDurationTcnt2 < dValueAlarmLow)) {
		if (dValueSec10Counter == 0)  {
			dValueSec10Counter = dValueAlarmSec10;
	}
			
	} else {
		dValueSec10Counter = 0;
		dValueAlarmOn = 0;
	}
	sei();
}


