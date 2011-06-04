/*
 * joesTriac.c
 *
 * Created: 03.05.2011 09:20:38
 *  Author: duenda
 */ 

#define F_CPU 11095200

#include <avr/io.h>
#include <stdio.h>
#include "st7565r.h"
#include "TriacDefines.h"
#include "triacPID.h"
#include "triacUI.h"
#include "StateClass.h"
#include "TriacIntr.h"
#include "TriacKeyPad.h"


void initBlink()
{
	DDRA = 0xFF;
	PORTA = 0x00;
}

void blink()
{
	int dummyInt;

	while (1) {
		dummyInt = 5;
		PORTA  =  ~PORTA;
		dummyInt = 4;
	}

}


void USART_Init( unsigned int baud )
{
	// Set baud rate 
	
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	// Enable receiver and transmitter
	UCSR0A =  (UCSR0A & 0b11111100) ;
	
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);
//	UCSR0B = 0b00011000;  // rx compl intr ena - tx compl intr ena - dreg empty intr ena - rx ena - tx ena - sz2 (size bit 2)  - 9. bit rx - 9. tx

	UCSR0C = 0b00000110; // "00" async usart - "00" disa parity - 1 (or 2) stop bit - sz1 - sz0 (set t0 8 bit) - clk polarity (sync only)
}


static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);  // first opened will be stdout and errout

//stdout = fdevopen((FILE *)uart_putchar, NULL, 0);

static int uart_putchar(char c, FILE *stream)
{
   while (!(UCSR0A & (1<<UDRE0)));                    
   if (c == '\n')
   {
      uart_putchar('\r',&mystdout);
   }
   UDR0 = c;
   return 0;
}


int main(void)
{
	CJoesTriacEvent ev;
	int8_t ky;
//	int16_t dummyI;

//	initBlink();
//	blink();

//	USART_Init( 143 );   // baud 4800 at 11.0592 mhz, single uart speed
	USART_Init( 71 );   // baud 9600 at 11.0592 mhz, single uart speed
	stdout = &mystdout;
	printf("startup\n");
	restorePersistentData();
	lcd_init();
	initKeyPad();
	initInterrupts();

/*	dummyI = 0;
	while(1) {
		++ dummyI;
		if (dummyI == 0) {
			if ((ky = keyEntered())){
					printf("\nmain() ky %c %X",ky,ky);
			}
		}
	}   */

	

//	InitializePID(real kp, real ki, real kd, real error_thresh, real step_time);   
//	InitializePID(-0.01, 0.3, 0.3, 0.3, 1);
	initUI();
	
	startStateCharts();	

/*	 ev.evType = evAstPressed;
	processTriacEvent(&SJoesTriacStateChart,&ev);  */

	while (1)
	{
		if (runningSecondsTick){
	//		cli();    // 8-bit access is already atomic
			runningSecondsTick = 0;
	//		sei();
			ev.evType = evSecondsTick;
			processTriacEvent(&SJoesTriacStateChart,&ev);	
		}
		if (adcTick){
			adcTick = 0; // 8-bit access is atomic
			ev.evType = evAdcTick;
			processTriacEvent(&SJoesTriacStateChart,&ev);	
		}
		if (durationTimerReachead) {
	//		cli();   // 8-bit access is alread atomic
			durationTimerReachead = 0;
	//		sei();
			ev.evType = evTimeOutDurationTimer;
			processTriacEvent(&SJoesTriacStateChart,&ev);	
		}

		if ((ky = keyEntered())){
			printf("\nmain() ky %c %X",ky,ky);
			if (ky == kpFunction1) ev.evType = evFunction1Pressed;
//			if (ky == kpFunction2) ev.evType = eFunction2Pressed;   // not yet in use
			if (ky == kpStart) ev.evType = evStartPressed;
			if (ky == kpStop) ev.evType = evStopPressed;
			if (ky == kpAst) ev.evType = evAstPressed;
			if (ky == kpNum) ev.evType = evNumPressed;
			if (ky >= kp0){
				ev.evType = evCharEntered;
				ev.keyCode = ky;			
			}	
			processTriacEvent(&SJoesTriacStateChart,&ev);	
		}	
    }
}
