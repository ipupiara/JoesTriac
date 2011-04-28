
#include <avr/io.h>
#include <avr/interrupt.h>

int anyKeyPressed;

int8_t lastCharPressed;

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

/*
int8_t keyPressed()
{
	return 0;
} */

int8_t getKeypadState()
{
	return 0;
	
	
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

