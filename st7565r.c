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


void lcd_init() {

/*





*/


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


void lcd_write_command(unit8_t cmd)
{
}

void lcd_write_data(uint8_t data) {

//	LCD_DATA_IODIR = 0xFF; // configure LCD_DATA IOs as output

	LCD_CMD = LCD_CMD | RS; // display data
	LCD_CMD = LCD_CMD & ~RW; // write
//	LCD_CMD = LCD_CMD & ~CS; // accept data/commands
	LCD_CMD = LCD_CMD | E;
	_delay_us(1); // 180 ns would be save (see page 62)
	LCD_DATA = data;
	_delay_us(1); // 40 ns would be save (see page 62)
	LCD_CMD = LCD_CMD & ~E;
	LCD_DATA = 0x00; // default state
//	LCD_CMD = LCD_CMD | CS; // do not accept further data/commands
	LCD_CMD = LCD_CMD | RW; // read
	LCD_CMD = LCD_CMD | RS; // display data

//	LCD_DATA_IODIR = 0x00; // configure LCD_DATA IOs as intput
}


