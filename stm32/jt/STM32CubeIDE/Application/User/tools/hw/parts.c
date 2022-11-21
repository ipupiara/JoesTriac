/*
 * parts.c
 *
 *  Created on: Oct 18, 2022
 *      Author: diego
 */

#include <defines.h>

/*


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
	if (durationTcnt2 < avrTriggerDelayMaxTcnt2) {
		if (durationTcnt2 > 0) {
			triacFireDurationTcnt2 = durationTcnt2;}
		else {
			triacFireDurationTcnt2 = 0;
		}
	} else {
		triacFireDurationTcnt2 = avrTriggerDelayMaxTcnt;
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
		if ((triacTriggerTimeTcnt2 >= avrTriggerDelayMaxTcnt2) ) {   //  || (amtInductiveRepetitions <= 0)  ) {
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

double adcVoltage()
{
	int16_t VHex;
	double   VFl;

	VFl = 0.0;

	VHex = ampsADCValue();
	VFl = (VHex * 5.0) / 0x03FF;

	return VFl;
}


void startTriacRun()
{
	resetPID();
	resetCircuitAlarms();
	startAmpsADC();
	EIFR = 0x00;
	EIMSK = 0x01;  				// start external interrupt (zero pass detection)
}

void stopTriacRun()
{
	resetCircuitAlarms();    // stops also circuit alarms (shortCircuit, DValue)
	EIMSK = 0x00;				// stop external interrupt
	cli();
	stopTimer2();
	sei();
	stopAmpsADC();
}



ISR(INT0_vect)
{
	cli();
	if ((PIND & 0x04) != 0) {
		stopTimer2();
	} else {
		triacTriggerTimeTcnt2 = 0;
		if (triacFireDurationTcnt2 > 0)  {
			startTriacTriggerDelay(  avrTriggerDelayMaxTcnt2 - triacFireDurationTcnt2);
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
int16_t  dValueSec10Counter;
uint8_t  dValueAlarmOn;


ISR(TIMER1_COMPA_vect)
{


void displayVoltage()
{
	double   VFl;
	char buffer [13];

	VFl = adcVoltage();

//	sprintf((char*)&buffer,"%5.2fV/%4iD",VFl,triacFireDurationTcnt2);

	lcd_goto(3, 6);
	lcd_write_str((char*)&buffer);
}

void displayDebugVoltageNTriggerDelay()
{
	double   VFl;
	char buffer [16];
	int16_t adc =  ampsADCValue();

	VFl = adcVoltage();

//	sprintf((char*)&buffer,"%5.2fV %4iA %3iD",VFl,adc, triacFireDurationTcnt2);

	lcd_Line1();
	lcd_write_str((char*)&buffer);

}


void displayPotiPersistent()
{
	double   VFl;
	char buffer [16];

	VFl = zeroAdjustDiffVoltage;

//	sprintf((char*)&buffer,"%3i P %6.3f V",zeroPotiPos,VFl);

	lcd_Line1();
	lcd_write_str((char*)&buffer);

}

void displayPotiVolatile()
{
	char buffer[10];
	sprintf((char*) &buffer,"%3iP",zeroPotiPos);
	lcd_goto(3,16);
	lcd_write_str((char*)&buffer);
}

void displayCalibrateZeroPotiPos()
{
	lcd_clrscr();
	lcd_write_str("calib Zero  ,# skip");
	lcd_goto(3,0);
	lcd_write_str("wait to stable + 30s");
}


*/


