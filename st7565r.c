
#include <avr/io.h>
#include "TriacDefines.h"
#include "st7565r.h"

#define RS	0x80	// register select aka. A0; H: display, L: control data
#define RW	0x40	// read/write; H: read, L: write (6800 MPU)
#define En  0x20    // enable line 


#define LCD_CMD PORTD		//control port of uC for LCD display
#define LCD_CMD_IODIR DDRD	//io direction for control port
#define LCD_DATA PORTB		//data port of uC for LCD display
#define LCD_DATA_IODIR DDRB	//io direction for data port
#define LCD_DATA_PIN PINB    // pins for data port


// takes approx  2 us at 10 Mhz with -o0 (no opts. !) ,with all  call/ret, push N Pop
void delayEmptyProc () 
{
}

void lcd_write(uint8_t dataW, uint8_t toDataIR, uint8_t Scr) 
{
	int8_t busy;

	LCD_DATA_IODIR = 0x00; // configure LCD_DATA IOs as input for read Busy Flag
	LCD_DATA    = 0x00;

	LCD_CMD = LCD_CMD & 0b00001111 ;	
	LCD_CMD = LCD_CMD & ~RS; 	//  RS = 0
	LCD_CMD = LCD_CMD |  RW;		//  RW = 1  (means read)

	busy = 1;
	while (busy) {
		LCD_CMD = LCD_CMD | En; 	// E = 1
		delayEmptyProc();
		busy = LCD_DATA_PIN & 0x80 ;
		LCD_CMD	= LCD_CMD & ~En;	// E = 0
	}
	
	LCD_DATA_IODIR = 0xFF;  // configure LCD_DATA as output
	LCD_DATA = dataW ;

	if (toDataIR)  LCD_CMD |= RS;  //  RS = 1
	LCD_CMD = LCD_CMD & ~RW;    // RW = 0  (means write)
	delayEmptyProc();
	LCD_CMD = LCD_CMD | En; 	// E = 1
	delayEmptyProc();
	LCD_CMD	= LCD_CMD & ~En;	// E = 0
	busy = busy + 1;
}




void lcd_init() 
{
	LCD_CMD_IODIR |= 0b11100000;  // highest 3 Pins as output, leave rest as is
	LCD_CMD  = LCD_CMD & 0b00011111 ;
	LCD_DATA_IODIR  = 0x00;  
	LCD_DATA   = 0x00;

	lcd_write (0b00000001, 0, LCD1);   // clear display
	lcd_write( 0b00111000,0, LCD1);   // 8-bit operation, 5x8Font, 2 lines
	lcd_write( 0b00001100, 0, LCD1);   // disp on, curs off, space mode (cause of initialization)
	lcd_write (0b00000110, 0, LCD1 );  // inc adr, shift curs, no char shift
}

void lcd_clrscr(int8_t Scr)
{
	lcd_write (0b00000001, 0, Scr); // clr scr and move home
}

void lcd_Line2(int8_t Scr)
{
	lcd_write (0b11000000, 0, Scr);
}

void lcd_write_str(char* st1, int8_t Scr)
{
	int8_t ch1;

	while ((ch1= *st1))
	{
		lcd_write(ch1,1, Scr);
		st1++;
	}
}

void lcd_goto(int8_t line, int8_t pos,int8_t Scr)
{
	int8_t adrC;
	if (line == 0) adrC = 0x00; else adrC = 0x40;  
	adrC = adrC + pos + 0x80;
	lcd_write(adrC,0,Scr);
}



// set 20 amps (use 1..3,7..9)
// # save, * skip

