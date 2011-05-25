
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TriacKeyPad.h"
#include "TriacDefines.h"


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


int8_t getKeypadState()
{
	int8_t ch;
	ch = 0x00;cv 
	keyPort = 0b00000010;
	if ((ch=keyPin & 0xF0)){
		if (ch & 0b10000000) ch = kp1;
		if (ch & 0b01000000) ch = kp4;
		if (ch & 0b00100000) ch = kp7;
		if (ch & 0b00010000) ch = kpAst;		
	} else {
		keyPort = 0b00000100;
		if ((ch=keyPin & 0xF0)){
			if (ch & 0b10000000) ch = kp2;
			if (ch & 0b01000000) ch = kp5;
			if (ch & 0b00100000) ch = kp8;
			if (ch & 0b00010000) ch = kp0;		
		}	else {
				keyPort = 0b00001000;
				if ((ch=keyPin & 0xF0)){
					if (ch & 0b10000000) ch = kp3;
					if (ch & 0b01000000) ch = kp6;
					if (ch & 0b00100000) ch = kp9;
					if (ch & 0b00010000) ch = kpNum;		
				}  else {
					keyPort = 0b00000001;
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


void initKeyPad()
{
	lastCharPressed = 0;


	keyDDR = 0x0F;  // higher four pins as output, lower as Input , resp. Interrupt sources
	keyPort = 0x0F;  // set higher four pins high	
	
	// init PCInt 2 on Port C , pc pins 16 .. 23
	IntrMsk = 0xF0;  // lower  4 pins will cause interrupt, upper 4 used as output for scanning keyPad
	PCICR = PCICR |( 1 << PCICRPos );  // enable pcint

	sei(); //  enable all interrupts
}

ISR(PCINTVECT)
{
	int8_t  tmp1;
	PCICR = 0b0000000;  // disa pcin interrupts ,evtl will need to reconfigure port ddr.. to be tested
	
	if (( tmp1 = (keyPin & 0xF0)))  {  // any key pressed (toggle down)
		lastCharPressed = getKeypadState();
	}
	keyPort = 0x0F;
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

