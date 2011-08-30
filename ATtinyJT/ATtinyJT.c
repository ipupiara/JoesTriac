#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/eeprom.h>
#include "i2c-slave.h"



#define zeroPotiPosEEPROMpos                0   // unit8



int8_t runningSecondsTick;
int8_t adcTick;



int8_t* p_zeroPotiPos;
float* p_voltage;
int8_t* p_jobState;
//int8_t currentJobState;

enum jobStates 
{
	idle,
	persistentZeroAdjust,
	transientZeroAdjust,
	fatalError	
};


ISR(TIM1_COMPA_vect)
{
		runningSecondsTick = 1;
}


void onRunningSecondsTick()
{
}


void setPotiCS(int8_t on)
{
	if (on) {
//		PORTA &= ~0x80;
	} else  {
//		PORTA |= 0x80;
	}
}


void setPotiINC(int8_t on)
{
	if (on) {
//		PORTA &= ~0x40;
	} else  {
//		PORTA |= 0x40;
	}
}

void setPotiUp(int8_t up)
{
	if (up) {
//		PORTA &= ~0x20;
	} else  {
//		PORTA |= 0x20;
	}
}




void storeZeroPotiPos(int8_t val)
{
	*p_zeroPotiPos = val;
	eeprom_write_byte((uint8_t *) zeroPotiPosEEPROMpos, *p_zeroPotiPos);
}


void zeroPotiPosUpPersistent(int8_t up, int8_t persistent)
{
//	if ((zeroPotiPos > 0) && (zeroPotiPos < 100) ) { checked by caller
		setPotiCS(1);
		if (up) {
			setPotiUp(1);
			++ *p_zeroPotiPos;
		} else {
			setPotiUp(0);
			-- *p_zeroPotiPos;
		}
		setPotiINC(1);
		if (persistent) {
			setPotiINC(0);
		}
		setPotiCS(0);
		if (persistent) {
			storeZeroPotiPos(*p_zeroPotiPos);
		} else {
			setPotiINC(0);
		}
		setPotiUp(0);
//	}
}


void errorPotiPosExceeded()
{
//	sprintf((char *) &lastFatalErrorString,"out of 0 Pos");
//	fatalErrorOccurred = 1;
}

void volatileZeroAdjStep()
{
/*
	double volts;
	volts = adcVoltage();
   	if (volts > 3E-3) {		
		if (zeroPotiPos > 0)  {
			zeroPotiPosUpPersistent(0, 0);
		} else {
			errorPotiPosExceeded();
		}
	} else { 
		if (volts < -3E-3) {
			if (zeroPotiPos < 100) {
				zeroPotiPosUpPersistent(1, 0);
			} else {
				errorPotiPosExceeded();
			}
		}
	}
	*/
}



void persistentZeroAdjStep()
{	
/*	double volts;
	volts = adcVoltage();
   	if (volts > 3E-3) {	
		stableStepsCnt = 0;	
		if (zeroPotiPos > 0)  {
			zeroPotiPosUpPersistent(0,1);
		} else {
			errorPotiPosExceeded();
		}
	} else { 
		if (volts < -3E-3) {
			stableStepsCnt = 0;
			if (zeroPotiPos < 100) {
				zeroPotiPosUpPersistent(1,1);
			} else {
				errorPotiPosExceeded();
			}
		} else {
			stableStepsCnt ++;
		}
	}
	if ( stableStepsCnt > 30) {
		stableZeroAdjReached = 1;
	}
	*/
}


void resetZeroAdj()
{
/*	int i1;

	setPotiCS(1);
	setPotiUp(1);
	for (i1 = 0; i1 < 100; ++ i1) 
	{
		setPotiINC(1);
		setPotiINC(0);
	}	
	setPotiCS(0);	
	storeZeroPotiPos(100);   // up on 100  , debug stop

	setPotiCS(1);
	setPotiUp(0);
	for (i1 = 0; i1 < 100; ++ i1) 
	{
		setPotiINC(1);
		setPotiINC(0);
	}	
	setPotiCS(0);	
	storeZeroPotiPos(0x00);    //down on zero, debug stop

*/
}




ISR(ADC_vect)
{
		adcTick = 1; 
}


void onADCTick()
{
}

void initHW()
{

	cli();
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

	TIMSK1   = 0b00000010;  //  Output Compare A Match Interrupt Enable 
	TCCR1B = 0b00001101  ; // CTC on CC1A , set clk / 24, timer started 


		ADCSRA  = 0b00000111;  // disa ADC, ADATE, ADIE	
		adcTick = 0;

		ADMUX = 0b11001101;      // 2.56V as ref,  right adjust, mux to diff adc3, adc2
//		ADCSRA = 0b10101111;  
								// int ena, prescale /128
								// ADC clock will run at 86400 hz, or max 6646. read per sec,what is ok
								// for our settings of 42. read per sec	
//		ADCSRB = 0x03;  // no ACME, trigger ADC on Timer0 compare match

		ADCSRA = 0b10001111;    // adc ena, no auto trigger, prescale 128
		ADCSRB = 0x00;
		DIDR0 = 0x0F;			// disa digital input on a0..a3

		sei();


}

void initPID()
{
	p_zeroPotiPos = (int8_t*) (&i2c_rdbuf[0]);
	p_voltage    =  (float*) (&i2c_rdbuf[1]);
	p_jobState =(int8_t*) (&i2c_rdbuf[5]);

	*p_zeroPotiPos = eeprom_read_byte((uint8_t*)zeroPotiPosEEPROMpos);	
	if ((*p_zeroPotiPos < 0x00) || (*p_zeroPotiPos > 100)) { storeZeroPotiPos(0x00);}   


}




int main(void)
{
	initPID();
	initHW();
	i2c_initialize (0x10);

	while(1) {

		if (runningSecondsTick == 1) {
			runningSecondsTick = 0;
			onRunningSecondsTick();
		}
		if (adcTick == 1)  {
		  onADCTick();
		}
	
	}

}
