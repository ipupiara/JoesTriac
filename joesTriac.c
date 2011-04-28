/*
 * joesTriac.c
 *
 * Created: 18.04.2011 14:34:59
 *  Author: duenda
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "st7565r.h"
#include "triacPID.h"
#include "triacUI.h"
#include "StateClass.h"
#include "TriacIntr.h"
#include "TriacKeyPad.h"

/*

void USART_Init( unsigned int baud )
{
	// Set baud rate 
	
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	// Enable receiver and transmitter
	UCSR0A =  (UCSR0A & 0b11111100) ;
	
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);
//	UCSR0B = 0b00011000;  // rx compl intr ena - tx compl intr ena - dreg empty intr ena - rx ena - tx ena - sz2 (size bit 2)  - 9. bit rx - 9. tx

	UCSR0C = 0b01000110; // "01" async usart - "00" disa parity - 1 (or 2) stop bit - sz1 - sz0 (set t0 8 bit) - clk polarity (sync only)
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

*/

extern const uStInt u32HandlingDone;


extern TStatechart SJoesTriacStateChart;




void secondTimer()
{
	calcNextTriacDelay();
	displayRunningValues();
}


int main(void)
{
	CJoesTriacEvent ev;
	int8_t ky;
	
	
	lcd_init();
	initKeyPad();
	initInterrupts();
	createPID();
	
	startStateCharts();
	
	
	while (1)
	{
		if (runningSecondsTimer){
			cli();
			runningSecondsTimer = 0;
			sei();
			secondTimer();
		}
		if (durationTimerReachead) {
			cli();
			durationTimerReachead = 0;
			sei();
			ev.evType = eTimeOutDurationTimer;
			processTriacEvent(&SJoesTriacStateChart,&ev);
			
		}
/*		if (keyPressed()){
			ev.evType = eKeyPressed;
			processTriacEvent(&SJoesTriacStateChart,&ev);
			
		}  */
		if ((ky = keyEntered())){
			
		}
		
		
		
    }
}