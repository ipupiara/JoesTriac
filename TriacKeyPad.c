
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TriacKeyPad.h"
#include "TriacDefines.h"

int anyKeyPressed;

int8_t lastCharPressed;

#define keyPort PORTC
#define keyPortC

//#define keyPort PORTB
//#define keyPortB


int8_t getKeypadState()
{
	int8_t ch;
	keyPort = 0b1000000;
	if ((ch=keyPort & 0x0F)){
		if (ch & 0b00001000) ch = kp1;
		if (ch & 0b00000100) ch = kp4;
		if (ch & 0b00000010) ch = kp7;
		if (ch & 0b00000001) ch = kpRed;		
	} else {
		keyPort = 0b0100000;
		if ((ch=keyPort & 0x0F)){
			if (ch & 0b00001000) ch = kp2;
			if (ch & 0b00000100) ch = kp5;
			if (ch & 0b00000010) ch = kp8;
			if (ch & 0b00000001) ch = kp0;		
		}	else {
				keyPort = 0b00100000;
				if ((ch=keyPort & 0x0F)){
					if (ch & 0b00001000) ch = kp3;
					if (ch & 0b00000100) ch = kp6;
					if (ch & 0b00000010) ch = kp9;
					if (ch & 0b00000001) ch = kpWhite;		
				}  else {
					keyPort = 0b00010000;
					if ((ch=keyPort & 0x0F)){
						if (ch & 0b00001000) ch = kpStart;
						if (ch & 0b00000100) ch = kpStop;
						if (ch & 0b00000010) ch = kpFunction1;
						if (ch & 0b00000001) ch = kpFunction2;		
					}	
				}
			}														
	}
	return ch;
}

#ifdef keyPortC

void initKeyPad()
{
	anyKeyPressed = 0;
	lastCharPressed = 0;


	DDRC = 0xF0;  // higher four pins as output, lower as Input , resp. Interrupt sources
	PORTC = 0xF0;  // set higher four pins high	
	
	// init PCInt 2 on Port C , pc pins 16 .. 23
	PCMSK2 = 0x0F;  // lower  4 pins will cause interrupt, upper 4 used as output for scanning keyPad
	PCICR = 0b0000100;  // enable pcint2

	sei(); //  enable all interrupts
}

ISR(PCINT2_vect)
{
	PCICR = 0b0000000;  // disa pcin interrupts ,evtl will need to reconfigure port ddr.. to be tested
	
	if ((PORTC & 0x0F))  {  // any key pressed (toggle down)
		lastCharPressed = getKeypadState();
	}
	PORTC = 0xF0;
	PCICR = 0b0000100;  // ena pcint int
}

#endif


#ifdef keyPortB

void initKeyPad()
{
	anyKeyPressed = 0;
	lastCharPressed = 0;


	DDRB = 0xF0;  // higher four pins as output, lower as Input , resp. Interrupt sources
	PORTB = 0xF0;  // set higher four pins high	
	
	// init PCInt 2 on Port C , pc pins 16 .. 23
	PCMSK1 = 0x0F;  // lower  4 pins will cause interrupt, upper 4 used as output for scanning keyPad
	PCICR = 0b0000010;  // enable pcint2

	sei(); //  enable all interrupts
}

ISR(PCINT1_vect)
{
	PCICR = 0b0000000;  // disa pcin interrupts ,evtl will need to reconfigure port ddr.. to be tested
	
	if ((PORTB & 0x0F))  {  // any key pressed (toggle down)
		lastCharPressed = getKeypadState();
	}
	PORTB = 0xF0;
	PCICR = 0b0000010;  // ena pcint int
}

#endif


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

