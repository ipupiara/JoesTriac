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

#define RS	0x02	// register select aka. A0; H: display, L: control data
#define RW	0x04	// read/write; H: read, L: write (6800 MPU)
#define En	0x01	// enable - impulse; L: active (6800 MPU)


#define LCD_CMD PORTD		//control port of uC for LCD display
#define LCD_CMD_IODIR DDRD	//io direction for control port
#define LCD_DATA PORTB		//data port of uC for LCD display
#define LCD_DATA_IODIR DDRB	//io direction for data port




void lcd_write(uint8_t dataW, uint8_t toDataIR) {

	int8_t busy;

	LCD_DATA_IODIR = 0x00; // configure LCD_DATA IOs as input for read Busy Flag
							// pull -up ???
	
	LCD_CMD &= ~RS; 	//  RS = 0
	LCD_CMD |= RW;		//  RW = 1  (means read)

	busy = 1;
	while (busy) {
		LCD_CMD |= En; 	// E = 1
		_delay_us(1);
		busy = LCD_DATA & 0x80 ;
		LCD_CMD	&= ~En;	// E = 0
		_delay_us(1);
	}
	

	LCD_DATA_IODIR = 0xFF;  // configure LCD_DATA as output
	if (toDataIR)  LCD_CMD |= RS;  //  RS = 1
	LCD_CMD &= ~RW;    // RW = 0  (means write)

		LCD_CMD |= En; 	// E = 1
		_delay_us(1);
		LCD_DATA = dataW ;
		_delay_us(1);
		LCD_CMD	&= ~En;	// E = 0
}




void lcd_init() {

	LCD_CMD_IODIR |= 0x03;  // lowest 3 Pins as output, leave rest as is
	LCD_CMD  &= 0b11111000 ;

	LCD_DATA_IODIR  = 0x00;  
	
// init as read-port
//	LCD_DATA_IODIR  = 0xFF  // all are used as output only in this application	
//	LCD_DATA = 0x00;

/*
	// Port A set to output (high impedance) 
	LCD_CMD_IODIR = 0xFF;

	// Predefine state after hardware reset 
	LCD_CMD = LCD_CMD & ~LI; // background light off
	LCD_CMD = LCD_CMD | PS; // parallel
	LCD_CMD = LCD_CMD | CS; // do not accept data/commands
	LCD_CMD = LCD_CMD | MI; // 6800 MPU interface
	LCD_CMD = LCD_CMD & ~E; // impulse is low

	// Hardware reset 
	LCD_CMD = LCD_CMD & ~RST;
	_delay_us(2);
	LCD_CMD = LCD_CMD | RST;
	_delay_us(2); // 2 us are save (see page 65)

	// Display settings 
	lcd_cmd(ADC_NORMAL);
	lcd_cmd(BIAS_1_9);
	lcd_cmd(SCAN_DIR_REVERSE);

	// Contrast settings 
	lcd_cmd(INTERNAL_RRATIO | (INTERNAL_RRATIO_MASK & 0x07));
	lcd_cmd(INTERNAL_RRATIO_MOD);
	lcd_cmd(INTERNAL_RRATIO_VOL | (INTERNAL_RRATIO_VOL_MASK & 0x00));

	// Power supply settings 
	lcd_cmd(INTERNAL_POWER | (INTERNAL_POWER_MASK & 0x07));

	lcd_cmd(DISPLAY_ON);
	LCD_CMD = LCD_CMD | LI; // background light on

*/
}

void lcd_clrscr()
{

}




