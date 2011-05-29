
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TriacDefines.h"
#include "TriacKeyPad.h"



int8_t lastCharPressed;

//#define KeyPortB

#ifdef KeyPortB

	#define keyPort PORTB
	#define keyPin  PINB
	#define keyDDR  DDRB
	#define IntrMsk  PCMSK1
	#define PCICRPos  1
	#define PCINTVECT  PCINT1_vect

#else

	#define keyPort PORTC
	#define keyPin PINC
	#define keyDDR  DDRC
	#define IntrMsk  PCMSK2
	#define PCICRPos  2
	#define PCINTVECT  PCINT2_vect

#endif


int8_t getKeypadState()
{
	int8_t ch;
	int8_t chr;
	ch = 0x00;
	chr = 0x00;
	keyPort = 0b00000010;  
	delayEmptyProc ();
	// delay6pnt2d5us(2);
	if ((ch=keyPin & 0xF0)){
		if (ch & 0b10000000) chr = kp2;
		if (ch & 0b01000000) chr = kp0;
		if (ch & 0b00100000) chr = kp8;  
		if (ch & 0b00010000) chr = kp5;		
	} else {
		keyPort = 0b00000100;
		delayEmptyProc ();
		if ((ch=keyPin & 0xF0)){
			if (ch & 0b10000000) chr = kp1;
			if (ch & 0b01000000) chr = kpAst;
			if (ch & 0b00100000) chr = kp7;
			if (ch & 0b00010000) chr = kp4;		
		}	else {
				keyPort = 0b00001000;
				delayEmptyProc ();
				if ((ch=keyPin & 0xF0)){
					if (ch & 0b10000000) chr = kp3;
					if (ch & 0b01000000) chr = kpNum;
					if (ch & 0b00100000) chr = kp9;
					if (ch & 0b00010000) chr = kp6;		
				}  else {
					keyPort = 0b00000001; 
					delayEmptyProc ();
					if ((ch=keyPin & 0xF0)){
						if (ch & 0b10000000) chr = kpStart;
						if (ch & 0b01000000) chr = kpStop;
						if (ch & 0b00100000) chr = kpFunction1;
						if (ch & 0b00010000) chr = kpFunction2;		
					}	
				}
			}														
	}
	return chr;
}

int8_t lastValueZero; 


void initKeyPad()
{
	lastCharPressed = 0;
	lastValueZero = 1;

	keyDDR = 0x0F;  // lower four pins as output, higher as Input , resp. Interrupt sources
	keyPort = 0x0F;  // set lower four pins high	
	
	// init PCInt 1 on Port B, 2 on C 
	IntrMsk = 0xF0;  // higher  4 pins will cause interrupt, upper 4 used as output for scanning keyPad
	PCICR = PCICR |( 1 << PCICRPos );  // enable pcint

	sei(); //  enable all interrupts
}


ISR(PCINTVECT)
{
	PCICR = 0b0000000;  
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
	delayEmptyProc ();
	PCIFR = 0x00;
	IntrMsk = 0xF0;
	PCICR = PCICR |( 1 << PCICRPos );  // ena pcint int
}


int8_t keyEntered()  // with interrupts
{
	int8_t res;
	cli();  
	res = lastCharPressed;
	lastCharPressed = 0;
	sei();
	return res;
}

