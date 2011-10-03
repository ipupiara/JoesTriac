#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/eeprom.h>
#include "ATtinyjt.h"
#include "USI_TWI_Slave.h"


int16_t lastAmpsADCVal;

int8_t runningSecondsTick;
int8_t adcTick;
int32_t adcSum;
int8_t adcCnt;
int8_t outside;
int8_t tcnt;


#define zeroPotiPosEEPROMpos                0   // int8
#define nearScopeOffsetCorrectionEEPROMpos       1    // int8 processor dependent offset corrections
#define farScopeOffsetCorrectionEEPROMpos      2      // int8 nasty, but tests showed necessity 

#define nearScopeOffsetCorrectionDefault    5  
#define farScopeOffsetCorrectionDefault     10			

int8_t nearScopeOffsetCorrection;
int8_t farScopeOffsetCorrection;

int8_t stableStepsCnt;

int8_t firstPersistentStepDone;


int8_t  extraJob;

int8_t* p_zeroPotiPos;
int16_t* p_ADCvoltage;
int8_t* p_adcScope;
int8_t* p_dummy1;
int8_t* p_jobState;

int8_t  prevJobState;
//int8_t currentJobState;

void debugLightOn()
{
	PORTA |= 0x80;
}

void debugLightOff()
{
	PORTA &= ~0x80;
}

void debugLightToggle()
{
	if (PORTA & 0x80) {
		PORTA &= ~0x80;
	} else {
		PORTA |= 0x80;
	}
}



#define adcThreshold 12


enum zeroAdjustJobStates 
{
	jobIdle = 1,
	persistentZeroAdjust,
	volatileZeroAdjust,
	up10,
	up1,
	down10,
	down1,
	fatalError	
};

enum adcScopeEnum
{
	nearScope,
	farScope
};

int16_t ampsADCValue()
{
	int16_t res;
	cli();
	res = lastAmpsADCVal;
	sei();
//	printf("ampsADC %i ",lastAmpsADCVal);
	return res;
}

int16_t  valueFrom6Bit2Complement(int16_t adcV)
{
	if (adcV & 0x0200) {
		adcV--;
		adcV = ~(adcV | 0xFC00);
		adcV = - adcV;
	}
	return adcV;
}
/*
int16_t diffADCValue()
{  
	int16_t res;
	res = ampsADCValue();
//	res = valueFrom6Bit2Complement(res);      done during interrupt
	return res;
}
*/

int16_t adcVoltage()
{
	int16_t VHex;
//	float   VFl;

//	VHex = 0x0000;

	VHex = ampsADCValue();
//	VHex = diffADCValue();
//	VFl =  (VHex * 1.1) / (1.0 * 0x200);

	while (messageOnGoing) {}		// wait until no more message will be processed, not to change values
									// during message transaction

	cli();  // dont change voltage while interrupt happens (4 byte variable), 
			// with above while () should be rather threadsafe, worst case message initalization might
			// happen between while() and cli, still no risk for correct value transmission
			
	*p_ADCvoltage = VHex;

	sei();
	
	return VHex;
}


void setPotiCS(int8_t on)
{
	if (on) {
		PORTB &= ~0x02;
	} else  {
		PORTB |= 0x02;
	}
}


void setPotiINC(int8_t on)
{
	if (on) {
		PORTB &= ~0x04;
	} else  {
		PORTB |= 0x04;
	}
}

void setPotiUp(int8_t up)
{
	if (up) {
		PORTB &= ~0x01;
	} else  {
		PORTB |= 0x01;
	}
}


void storeZeroPotiPos(int8_t val)
{
	*p_zeroPotiPos = val;
	eeprom_write_byte((uint8_t *) zeroPotiPosEEPROMpos, *p_zeroPotiPos);
}

void storeNearOffsetCorrection(int8_t val)
{
	nearScopeOffsetCorrection = val;
	eeprom_write_byte((uint8_t *) nearScopeOffsetCorrectionEEPROMpos, nearScopeOffsetCorrection);
}

void storeFarOffsetCorrection(int8_t val)
{
	farScopeOffsetCorrection = val;
	eeprom_write_byte((uint8_t *) farScopeOffsetCorrectionEEPROMpos, farScopeOffsetCorrection);
}


void zeroPotiPosUpPersistent(int8_t up, int8_t persistent)
{
	if ((*p_zeroPotiPos >= 0) && (*p_zeroPotiPos < 100) ) { 
		setPotiCS(1);
		if (up) {
			if (*p_zeroPotiPos >= 0)  {
				setPotiUp(1);
				++ *p_zeroPotiPos;
			}
		} else {
			if (*p_zeroPotiPos < 100)   {
				setPotiUp(0);
				-- *p_zeroPotiPos;
			}
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
	}
}

void storePotiPos()  // one should not make too much (only 50000 possible) EEPROM storages
{
	int i1;
//		setPotiINC(1);		  // to be tested which oone

		setPotiCS(1);

		setPotiINC(1);   // to be tested which oone
		setPotiINC(0);
		for (i1 = 0; i1 < 20;++ i1) {}  // respect Tcyc min., and..
		setPotiUp(1);

		setPotiINC(1); 
		setPotiINC(0);
		for (i1 = 0; i1 <10;++ i1) {}  // respect Tic min. and.. hope persistency will work now, tobe tested
		setPotiCS(0);
		setPotiUp(0);
		storeZeroPotiPos(*p_zeroPotiPos);
}



// use this method only during initial program test. Otherwise it might kill the digital poti (ds1804)

void volatilePotiUpAmt(int8_t  up, int8_t amt)
{
	int8_t  cnt;
	for (cnt = 0; cnt < amt; ++cnt)
	{
		zeroPotiPosUpPersistent(up, 0);	
	}
}




void errorPotiPosExceeded()
{
	*p_jobState = fatalError ;
}

void volatileZeroAdjStep()
{
	int16_t volts;

	debugLightToggle();

	volts = adcVoltage();
   	if (volts > adcThreshold) {		
		if (*p_zeroPotiPos > 0)  {
			zeroPotiPosUpPersistent(0, 0);
			outside = 1;
		} else {
			errorPotiPosExceeded();
		}
	} else { 
		if (volts < - adcThreshold) {
			if (*p_zeroPotiPos < 100) {
				zeroPotiPosUpPersistent(1, 0);
				outside = 1;
			} else {
				errorPotiPosExceeded();
			}
		}else {
			outside = 0;
		}
	} 
	
}

void persistentZeroAdjStep()
{	
	int16_t volts;

	debugLightToggle();

	volts = adcVoltage();
   	if (volts > adcThreshold) {	
		stableStepsCnt = 0;	
		if (*p_zeroPotiPos >= 0)  { 
			if (*p_jobState == persistentZeroAdjust) // job might have changed meanwhile
//			zeroPotiPosUpPersistent(0,1);
			zeroPotiPosUpPersistent(0,0);
		} else {
			errorPotiPosExceeded();
		}
	} else { 
		if (volts < - adcThreshold) {
			stableStepsCnt = 0;
			if (*p_zeroPotiPos < 100 ) {
				if (*p_jobState == persistentZeroAdjust) {// job might have changed meanwhile
//					zeroPotiPosUpPersistent(1,1);
					zeroPotiPosUpPersistent(1,0);
				}
			} else {
				errorPotiPosExceeded();
			}
		} else {
			stableStepsCnt ++;
		}
	}
	if ( stableStepsCnt > 30) {
		storePotiPos();
		prevJobState = *p_jobState;
		*p_jobState = jobIdle;
	}	
}


void resetZeroAdj()
{
	int i1;

	setPotiCS(1);
	setPotiUp(0);
//	setPotiUp(1);
	for (i1 = 0; i1 < 110; ++ i1) 
	{
		setPotiINC(1);
		setPotiINC(0);
	}
	setPotiINC(1);	// do not store
	setPotiCS(0);
	setPotiINC(0);	
	setPotiUp(0);
	*p_zeroPotiPos = 0;

/*
	setPotiCS(1);
//	setPotiUp(0);
	setPotiUp(1);
	for (i1 = 0; i1 < 110; ++ i1) 
	{
		setPotiINC(1);
		setPotiINC(0);
	}
	setPotiINC(1);	// do not store
	setPotiCS(0);
	setPotiINC(0);	
	setPotiUp(0);
*/
//	storeZeroPotiPos(0x00);    //down on zero, debug stop
}



ISR(ADC_vect)
{
	adcSum += valueFrom6Bit2Complement(ADC);
	++ adcCnt;
	if (adcCnt < 100){
		ADCSRA |= (1<< ADSC);
	} else {
		adcTick = 1;
	}	 
}

ISR(TIM1_COMPA_vect)
{
		runningSecondsTick = 1;
}

void switchToFarScope()
{
	ADCSRA &= ~(1<< ADEN);
	ADMUX = (0x00 |  (1<<REFS1) | (1<< MUX5) | (1<< MUX4)); // int ref 1.1V, ADC2 neg, ADC3 pos 
	*p_adcScope = farScope;
	ADCSRA |= (1<< ADEN);
}

void switchToNearScope()
{
	ADCSRA &= ~(1<< ADEN);
	ADMUX = (0x00 |  (1<<REFS1) | (1<< MUX5) | (1<< MUX4)| (1>>MUX0)); // int ref 1.1V, ADC2 neg, ADC3 pos, 20x
	*p_adcScope = nearScope;
	ADCSRA |= (1<< ADEN);
}


void initHW()
{
	runningSecondsTick = 0;

	cli();
// Timer 1 as Duration Timer

	
	TCCR1A = 0x00;  // normal mode , CTC dep.on TCCR1B
	TCCR1B = (0x00 | (1<<WGM12));  //  CTC, timer still stopped
	TCCR1C = 0x00; // no Force output compare
	OCR1A = 0x7A12;  // counter top value  , this value at clk/256 will cause a delay of exact 1 sec
	TCNT1 = 0x00 ;
	 
//	TIMSK1 = 0x00; 
	TIMSK1   =  (0x00 | (1<<OCIE1A));  //  Output Compare A Match Interrupt Enable 

	TCCR1B |= (1<<CS12); // prescaler clk / 256, timer started

// adc settings
	
	lastAmpsADCVal = 0;
	ADCSRA = (0x0 |(1<<ADPS2) | (1<< ADPS1) );  // prescaler / 64, gives approx. 125 kHz ADC clock freq.
	ADMUX = (0x00 |  (1<<REFS1) | (1<< MUX5) | (1<< MUX4)); // int ref 1.1V, ADC2 neg, ADC3 pos
	ADCSRA |= ((1<< ADEN) | (1<< ADIE));
	ADCSRB = 0x00 | (1<<BIN);
				
	*p_adcScope = farScope;													
	adcTick = 0;	

	sei();

	DDRA |= 0x80; // debuglight enable
	PORTA &= ~0x80; // debuglight off
}



void onSecondTick()
{	

/*	if (PORTA & 0x80) {
		PORTA &= ~0x80;
	}  else {
		PORTA |= 0x80;
	}
	*/
//	*p_jobState = 0x02;

//	debugLightToggle();

	if (*p_jobState == persistentZeroAdjust   ) {
		if (adcCnt == 0) {				// avoid trigger during run, anyhow should not happen, since 
										// collecting 100 values will need 100 * 13 * 64 = 83200 cpu cycles
										// + some few interrupt time, means approx 1 ms + interrupt time
			ADCSRA |= (1<< ADSC);
		}
	} else if (*p_jobState == volatileZeroAdjust) {
		if ((outside) || (tcnt > 10 ) ){
			if (adcCnt == 0) {				// avoid trigger during run, anyhow should not happen, since 
											// collecting 100 values will need 100 * 13 * 64 = 83200 cpu cycles
											// + some few interrupt time, means approx 1 ms + interrupt time
				ADCSRA |= (1<< ADSC);
				tcnt = 0;
			}
		}
		++ tcnt;
	} else if (*p_jobState == jobIdle) {  // send s (morse)
		++tcnt;
		if ((tcnt == 5) || (tcnt == 7) || (tcnt == 9) ) { debugLightOn(); } 
		else { 
			debugLightOff(); 
			if (tcnt == 10) tcnt = 0;
		}
	} else if (*p_jobState == jobIdle) {  // send f
		++tcnt;
		if ((tcnt == 5) || (tcnt == 7) || (tcnt == 9) || (tcnt == 10)|| (tcnt == 12)) { debugLightOn(); } 
		else { 
			debugLightOff(); 
			if (tcnt == 14) tcnt = 0;
		}
	}
}

void onADCTick()
{
	lastAmpsADCVal = adcSum / adcCnt;
	if (*p_adcScope == farScope) {
		lastAmpsADCVal = lastAmpsADCVal + farScopeOffsetCorrection;
	} else {
		lastAmpsADCVal = lastAmpsADCVal + nearScopeOffsetCorrection;
	}
	
	if (*p_jobState == volatileZeroAdjust) {
		volatileZeroAdjStep();
	}
	if (*p_jobState == persistentZeroAdjust) {
		if (!firstPersistentStepDone) {
			resetZeroAdj();
			firstPersistentStepDone = 1;
		} else {
			persistentZeroAdjStep();
		}		
	}  
	if ((*p_adcScope == farScope) && (  abs(lastAmpsADCVal) < 15 )) {
		switchToNearScope();
	}
	if ((*p_adcScope == nearScope) && (abs(lastAmpsADCVal) > 490)) {
		switchToFarScope();
	}

	adcSum  = 0;
	adcCnt = 0;	
}



void initPID()
{
	p_zeroPotiPos = (int8_t*) (&i2c_rdbuf[0]);
	p_ADCvoltage    =  (int16_t*) (&i2c_rdbuf[1]);
	p_adcScope = (int8_t *) (&i2c_rdbuf[3]);
	p_jobState =(int8_t*) (&i2c_rdbuf[5]);

	*p_zeroPotiPos = eeprom_read_byte((uint8_t*)zeroPotiPosEEPROMpos);	
	if ((*p_zeroPotiPos < 0x00) || (*p_zeroPotiPos > 100)) { storeZeroPotiPos(0x00);}   
	nearScopeOffsetCorrection = eeprom_read_byte((uint8_t*)nearScopeOffsetCorrectionEEPROMpos);
	if (nearScopeOffsetCorrection == -1) {storeNearOffsetCorrection(nearScopeOffsetCorrectionDefault);}
	farScopeOffsetCorrection = eeprom_read_byte((uint8_t*)farScopeOffsetCorrectionEEPROMpos);
	if (farScopeOffsetCorrection == -1) {storeFarOffsetCorrection(farScopeOffsetCorrectionDefault);}

	*p_ADCvoltage = 0x0000;
	*p_jobState = jobIdle;
	prevJobState = jobIdle;
	extraJob = jobIdle;
	jobBuffer = 0;
	outside = 0;
	tcnt = 0;
	stableStepsCnt = 0;
	firstPersistentStepDone = 0;
	DDRB |= 0x07;
	PORTB |= 0x07;
	setPotiCS(0);
	setPotiINC(0);
	setPotiUp(0);  
}



void jobReceived(int8_t jS)
{
	if ((jS == up1)  ||  (jS == up10)||  (jS == down1)||  (jS == down10)) {
		extraJob = jS;
	} else {	
		if ((jS !=  *p_jobState) && (* p_jobState != fatalError)) {
			if (jS == persistentZeroAdjust) {
				firstPersistentStepDone = 0;
				stableStepsCnt = 0;
			}
			prevJobState = *p_jobState;
			* p_jobState = jS;
		}
	}	
}


int8_t jobB;


int main(void)
{
//	initPID();

/*	DDRB &= ~(1<< DDB0);

	DDRA |= 0x08;
	PORTA &= ~0x08;

	while (PINB & (1<< PINB0)) {}

	PORTA |= 0x08; 
*/
	initPID();   // needs to be called before initHW();

	initHW();
	USI_TWI_Slave_Initialise(0x10);

	while(1) {
		cli();
		if (jobBuffer) {
			jobB = jobBuffer;
			jobBuffer = 0;
		}
		sei();
		if (jobB) {
			jobReceived(jobB);
			jobB = 0;
		}

//		asm volatile ( "wdr"  );

		if (runningSecondsTick == 1) {
			runningSecondsTick = 0;
				onSecondTick();	
		}

		if (extraJob == up1)  {
			volatilePotiUpAmt(1,1);
		}
		if (extraJob == up10)  {
			volatilePotiUpAmt(1,10);
		}
		if (extraJob == down1)  {
			volatilePotiUpAmt(0,1);
		}
		if (extraJob == down10)  {
			volatilePotiUpAmt(0,10);
		}
		extraJob = jobIdle;
		
		if (adcTick == 1)  {
			adcTick = 0;
		  	onADCTick();
		}
	}
}
