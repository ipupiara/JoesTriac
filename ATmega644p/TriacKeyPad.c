
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "TriacIntr.h"
#include "TriacDefines.h"
#include "TriacKeyPad.h"



int8_t lastCharPressed;

#define keyPort PORTC
#define keyPin PINC
#define keyDDR  DDRC
#define keyPortOut2  PORTA
#define keyDDROut2   DDRA
#define IntrMsk  PCMSK2
#define PCICRPos  PCIE2
#define PCINTVECT  PCINT2_vect   

//#define printKeybState

#define keypadSignalDelayFaktor  8

// #define counting

#ifdef counting
  int64_t pcintCnt;
#endif
int8_t getKeypadState()
{
	int8_t ch;
	int8_t chr;
	ch = 0x00;
	chr = 0x00;
//	keyPort = 0b00000010; 

	keyPort &= ~0x0C;  
	keyPortOut2	 &= ~0xC0;
 
	keyPortOut2  |= 0b10000000;  
	delay6pnt2d5us(keypadSignalDelayFaktor);
	// delay6pnt2d5us(2);
	if ((ch=keyPin & 0xF0)){
		if (ch & 0b10000000) chr = kp2;
		if (ch & 0b01000000) chr = kp0;
		if (ch & 0b00100000) chr = kp8;  
		if (ch & 0b00010000) chr = kp5;		
	} else {
		keyPortOut2  &= ~0b10000000; 
		keyPort |= 0b00000100;
		delay6pnt2d5us(keypadSignalDelayFaktor);
		if ((ch=keyPin & 0xF0)){
			if (ch & 0b10000000) chr = kp1;
			if (ch & 0b01000000) chr = kpAst;
			if (ch & 0b00100000) chr = kp7;
			if (ch & 0b00010000) chr = kp4;		
		}	else {
				keyPort &= ~0b00000100;
				keyPort |= 0b00001000;
				delay6pnt2d5us(keypadSignalDelayFaktor);
				if ((ch=keyPin & 0xF0)){
					if (ch & 0b10000000) chr = kp3;
					if (ch & 0b01000000) chr = kpNum;
					if (ch & 0b00100000) chr = kp9;
					if (ch & 0b00010000) chr = kp6;		
				}  else {
//					keyPort = 0b00000001;
					keyPort &= ~0b00001000; 
					keyPortOut2 |= 0b01000000; 
					delay6pnt2d5us(keypadSignalDelayFaktor);
					if ((ch=keyPin & 0xF0)){
						if (ch & 0b10000000) chr = kpStart;
						if (ch & 0b01000000) chr = kpStop;
						if (ch & 0b00100000) chr = kpF1;
						if (ch & 0b00010000) chr = kpF2;		
					}	
				}
			}														
	}
	return chr;
}

int8_t lastValueZero; 


#ifdef counting
	void testIncCnt()
	{
		pcintCnt++;
		if (pcintCnt == INT64_MAX)  {
			sprintf((char *) &lastFatalErrorString,"pcintCnt overflow\n");
			fatalErrorOccurred = 1;
		}
	}
#endif


int16_t  getKeybIntCnt()
{
	int16_t res= 0;
#ifdef counting
	cli();	
	res = pcintCnt;
	sei();
#endif	
	return res;
}

void onTriacIdleSecondTick_Keyb()
{
#ifdef printKeybState
	int16_t secs;
	secs = getSecondsDurationTimerRemaining();
	if ((secs & 0x001f) == 0x0000) {
		int16_t res = getKeybIntCnt();
		printf("kybIntCnt: %i lastCh: %X lastV0: %X\n",res , lastCharPressed, lastValueZero);
	}
#endif
}

void initKeyboardPins()
{

	//	keyDDR = 0x0F;  // lower four pins as output, higher as Input , resp. Interrupt sources
	keyDDR  |= 0x0C;
	keyDDR  &= ~0xF0;
	keyDDROut2 |= 0xC0;

	//	keyPort = 0x0F;  // set lower four pins high
	keyPort |= 0x0C;  // set upper2 of lower four pins high
	keyPort &= ~0xF0;  // make sure no pullup resistors switched
	keyPortOut2	 |= 0xC0;

	// init PCInt 1 on Port B, 2 on C
	IntrMsk = 0xF0;  // higher  4 pins will cause interrupt, upper 4 used as output for scanning keyPad
	PCICR = PCICR |( 1 << PCICRPos );  // enable pcint
	
}

void initUsartInput()
{
	// USART_init()  has already set most of usart setup, so only a few has to be done
	UCSR0B |= (1<< RXCIE0) | (1<<RXEN0) ;
	
}

void initKeyPad()
{
	lastCharPressed = 0;
	lastValueZero = 1;
#ifdef counting	
	pcintCnt = 0;
#endif	
	
#ifdef jtagDebugKeyboardMode
	printf("jtagDebugKeyboardMode\n");
	initUsartInput();
#else
	printf("not jtagDebugKeyboardMode\n");
	initKeyboardPins();
#endif

	sei(); //  enable all interrupts if not yet done so
}

#ifndef jtagDebugKeyboardMode

ISR(PCINTVECT)
{
	cli();
	PCICR = 0b0000000;  
	IntrMsk = 0x00; 

#ifdef counting
	testIncCnt();
#endif
	
	sei(); 
	
	if ((keyPin & 0xF0))  {  // any key pressed (toggle down)
		if (lastValueZero) {
			lastCharPressed = getKeypadState();
		}
		lastValueZero = 0;
	} else {
		lastValueZero = 1;
	}
	
//	keyPort = 0x0F;

	keyPort |= 0x0C;  // set upper2 of lower four pins high
	keyPortOut2	 |= 0xC0;

	delayEmptyProc ();
	PCIFR = 0x00;
	IntrMsk = 0xF0;
	PCICR = PCICR |( 1 << PCICRPos );  // ena pcint int
}

#else



ISR(USART0_RX_vect)
{
	int8_t rxCh;
	rxCh = UDR0;
	
#ifdef counting
testIncCnt();
#endif	

	switch (rxCh)
	{
		case '1' :	{
			lastCharPressed = kp1;
			break;
		}
		case '2' : {
			lastCharPressed = kp2;
			break;
		}
		case '3' : {
			lastCharPressed = kp3;
			break;
		}
		case '4' :	{
			lastCharPressed = kp4;
			break;
		}
		case '5' : {
			lastCharPressed = kp5;
			break;
		}
		case '6' : {
			lastCharPressed = kp6;
			break;
		}
		case '7' :	{
			lastCharPressed = kp7;
			break;
		}
		case '8' : {
			lastCharPressed = kp8;
			break;
		}
		case '9' : {
			lastCharPressed = kp9;
			break;
		}
		case '0' : {
			lastCharPressed = kp0;
			break;
		}
		case '#' : {
			lastCharPressed = kpNum;
			break;
		}		
		case '*' : {
			lastCharPressed = kpAst;
			break;
		}
		case 'A' : {
			lastCharPressed = kpF1;
			break;
		}
		case 'B' : {
			lastCharPressed = kpF2;
			break;
		}
		case 'R' : {
			lastCharPressed = kpStop;
			break;
		}
		case 'G' : {
			lastCharPressed = kpStart;
			break;
		}
		default : {
			lastCharPressed = 0;
			break;
		}
	}
	
	
}

#endif


int8_t keyEntered()  // with interrupts
{
	int8_t res;
	cli();  
	res = lastCharPressed;
	lastCharPressed = 0;
	sei();
	return res;
}

