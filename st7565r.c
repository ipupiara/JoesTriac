/*
 * ST7565R LCD controller driver for Atmel (R) AVR (R) 8-Bit RISC
 * Copyright (C) 2009 Alexander Meinke <ameinke@online.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * All "bookmarks" reference the "ST7565R V1.7b" documentation.
 */

#include <avr/io.h>
#define F_CPU 11095200
#include <util/delay.h>

#include "st7565r.h"

#define RS	0x80	// register select aka. A0; H: display, L: control data
#define RW	0x40	// read/write; H: read, L: write (6800 MPU)
#define En1	0x20	// enable lcd1  - impulse; L: active (6800 MPU)
#define En2 0x10    // enable lcd2


#define LCD_CMD PORTD		//control port of uC for LCD display
#define LCD_CMD_IODIR DDRD	//io direction for control port
#define LCD_DATA PORTB		//data port of uC for LCD display
#define LCD_DATA_IODIR DDRB	//io direction for data port




void lcd_write(uint8_t dataW, uint8_t toDataIR, uint8_t Enx) {

	int8_t busy;

	LCD_DATA_IODIR = 0x00; // configure LCD_DATA IOs as input for read Busy Flag
							// pull -up ???
	LCD_CMD = LCD_CMD & 0b00001111 ;
	
	LCD_CMD = LCD_CMD & ~RS; 	//  RS = 0
	LCD_CMD = LCD_CMD |  RW;		//  RW = 1  (means read)

	busy = 1;
	while (busy) {
		LCD_CMD = LCD_CMD | Enx; 	// E = 1
		_delay_us(1);
		busy = LCD_DATA & 0x80 ;
		LCD_CMD	= LCD_CMD & ~Enx;	// E = 0
		_delay_us(1);
	}
	

	LCD_DATA_IODIR = 0xFF;  // configure LCD_DATA as output
	if (toDataIR)  LCD_CMD |= RS;  //  RS = 1
	LCD_CMD = LCD_CMD & ~RW;    // RW = 0  (means write)

	LCD_DATA = dataW ;
	_delay_us(1);
	LCD_CMD = LCD_CMD | Enx; 	// E = 1

	_delay_us(1);
	LCD_CMD	= LCD_CMD & ~Enx;	// E = 0
}




void lcd_init() {

	LCD_CMD_IODIR |= 0xF0;  // highest 4 Pins as output, leave rest as is
	LCD_CMD  = LCD_CMD & 0b00001111 ;

	LCD_DATA_IODIR  = 0x00;  

	lcd_write (0b00000001, 0,En1);   // clear display
//	lcd_write (0b00000001, 0,En2);   // clear display

	lcd_write( 0b00111000,0, En1);   // 8-bit operation, 5x8Font, 2 lines
//	lcd_write( 0b00111000,0, En2);   // 8-bit operation, 5x8Font, 2 lines

	lcd_write( 0b00001100, 0, En1);   // disp on, curs off, space mode (cause of initialization)
//	lcd_write( 0b00001100, 0, En2);   // disp on, curs off, space mode (cause of initialization)

	lcd_write (0b00000110, 0, En1 );  // inc adr, shift curs, no char shift
//	lcd_write (0b00000110, 0, En2 );  // inc adr, shift curs, no char shift


//   lcd_write (0b11000000, 0); // adr of ddram to start 2nd line (cursor move)

}

void lcd_clrscr()
{
	lcd_write (0b00000001, 0, En1); // clr scr and move home
	lcd_write (0b00000001, 0, En2); // clr scr and move home
}

void lcd_Line2(int8_t Enx)
{
	lcd_write (0b11000000, 0, Enx);
}



void lcd_write_str(char* st1, int8_t Enx)
{
	int8_t ch1;

	// acceleration would be possible by writing both screens together
	// (during the wait times for  completion)
	while ((ch1= *st1))
	{
		lcd_write(ch1,1, Enx);
		st1++;
	}
}

void lcd_AskCalibration()
{
	lcd_clrscr();
	lcd_write_str("Calibrate? *=Yes",En1);
	lcd_Line2(En1);
	lcd_write_str("or wait",En1);

}


// set 20 amps (use 1..3,7..9)
// # save, * skip

