
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TriacDefines.h"
#define F_CPU JT_F_CPU
#include <util/delay.h>
#include "TriacKeyPad.h"



int8_t lastCharPressed;

#define keyPort PORTB
#define keyPin  PINB
#define keyDDR  DDRB
#define IntrMsk  PCMSK1
#define PCICRPos  1
#define PCINTVECT  PCINT1_vect

//#define keyPort PORTC
//#define keyPin PINC
//#define keyDDR  DDRC
//#define IntrMsk  PCMSK2
//#define PCICRPos  2
//#define PCINTVECT  PCINT2_vect

#define waitForRaiseUS 3


int8_t getKeypadState()
{
	int8_t ch;
	ch = 0x00;
	keyPort = 0b00000010;
	_delay_us(waitForRaiseUS);  // needs appeox 2 us to change value
	if ((ch=keyPin & 0xF0)){
		if (ch & 0b10000000) ch = kp1;
		if (ch & 0b01000000) ch = kp4;
		if (ch & 0b00100000) ch = kp7;
		if (ch & 0b00010000) ch = kpAst;		
	} else {
		keyPort = 0b00000100;
		_delay_us(waitForRaiseUS);  // needs appeox 2 us to change value
		if ((ch=keyPin & 0xF0)){
			if (ch & 0b10000000) ch = kp2;
			if (ch & 0b01000000) ch = kp5;
			if (ch & 0b00100000) ch = kp8;
			if (ch & 0b00010000) ch = kp0;		
		}	else {
				keyPort = 0b00001000;
				_delay_us(waitForRaiseUS);  // needs appeox 2 us to change value
				if ((ch=keyPin & 0xF0)){
					if (ch & 0b10000000) ch = kp3;
					if (ch & 0b01000000) ch = kp6;
					if (ch & 0b00100000) ch = kp9;
					if (ch & 0b00010000) ch = kpNum;		
				}  else {
					keyPort = 0b00000001;
					_delay_us(waitForRaiseUS);  // needs appeox 2 us to change value
					if ((ch=keyPin & 0xF0)){
						if (ch & 0b10000000) ch = kpStart;
						if (ch & 0b01000000) ch = kpStop;
						if (ch & 0b00100000) ch = kpFunction1;
						if (ch & 0b00010000) ch = kpFunction2;		
					}	
				}
			}														
	}
	return ch;
}

int lastValueZero; 

void initKeyPad()
{
	lastCharPressed = 0;
	lastValueZero = 0;

	keyDDR = 0x0F;  // lower four pins as output, higher as Input , resp. Interrupt sources
	keyPort = 0x0F;  // set lower four pins high	
	
	// init PCInt 1 on Port B, 2 on C 
	IntrMsk = 0xF0;  // higher  4 pins will cause interrupt, upper 4 used as output for scanning keyPad
	PCICR = PCICR |( 1 << PCICRPos );  // enable pcint

	sei(); //  enable all interrupts
}


ISR(PCINTVECT)
{
	PCICR = 0b0000000;  // disa pcin interrupts ,evtl will need to reconfigure port ddr.. to be tested
	IntrMsk = 0x00;
	
	if ((keyPin & 0xF0))  {  // any key pressed (toggle down)
		if (lastValueZero) {
			lastCharPressed = getKeypadState();
		}
		lastValueZero = 0;
	} else {
		lastValueZero = 1;
	}
	
	keyPort = 0x0F;

PORTA  =  ~PORTA;
	
	IntrMsk = 0xF0;
	PCIFR = 0x00;
	PCICR = PCICR |( 1 << PCICRPos );  // ena pcint int
}



/*
int8_t keyEntered()  // when polling in main loop
{	int res;
	int ky;
	
	res = 0;
	ky = getKeypadState();
	if ((!anyKeyPressed) && ky){
		res = ky;
		anyKeyPressed = 1;
	} 
	
	if ((!ky) && anyKeyPressed ){
			anyKeyPressed = 0;
	}	
	return res;
}
*/

int8_t keyEntered()  // with interrupts
{
	int8_t res;
	res = lastCharPressed;
	lastCharPressed = 0;
	return res;
}

