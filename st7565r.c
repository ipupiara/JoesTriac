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
#include <util/delay.h>

#include "st7565r.h"
#include "fontACM.h"


void lcd_init() {

	/* Port A set to output (high impedance) */
	LCD_CMD_IODIR = 0xFF;

	/* Predefine state after hardware reset */
	LCD_CMD = LCD_CMD & ~LI; // background light off
	LCD_CMD = LCD_CMD | PS; // parallel
	LCD_CMD = LCD_CMD | CS; // do not accept data/commands
	LCD_CMD = LCD_CMD | MI; // 6800 MPU interface
	LCD_CMD = LCD_CMD & ~E; // impulse is low

	/* Hardware reset */
	LCD_CMD = LCD_CMD & ~RST;
	_delay_us(2);
	LCD_CMD = LCD_CMD | RST;
	_delay_us(2); // 2 us are save (see page 65)

	/* Display settings */
	lcd_cmd(ADC_NORMAL);
	lcd_cmd(BIAS_1_9);
	lcd_cmd(SCAN_DIR_REVERSE);

	/* Contrast settings */
	lcd_cmd(INTERNAL_RRATIO | (INTERNAL_RRATIO_MASK & 0x07));
	lcd_cmd(INTERNAL_RRATIO_MOD);
	lcd_cmd(INTERNAL_RRATIO_VOL | (INTERNAL_RRATIO_VOL_MASK & 0x00));

	/* Power supply settings */
	lcd_cmd(INTERNAL_POWER | (INTERNAL_POWER_MASK & 0x07));

	lcd_cmd(DISPLAY_ON);
	LCD_CMD = LCD_CMD | LI; // background light on
}

void lcd_cmd(uint8_t cmd) {

	LCD_DATA_IODIR = 0xFF;

	LCD_CMD = LCD_CMD & ~RS; // control data
	LCD_CMD = LCD_CMD & ~RW; // write
	LCD_CMD = LCD_CMD & ~CS; // accept data/commands
	LCD_CMD = LCD_CMD | E;
	_delay_us(1); // 180 ns would be save (see page 62)
	LCD_DATA = cmd;
	_delay_us(1); // 40 ns would be save (see page 62)
	LCD_CMD = LCD_CMD & ~E;
	LCD_DATA = 0x00; // default state
	LCD_CMD = LCD_CMD | CS; // do not accept further data/commands
	LCD_CMD = LCD_CMD | RW; // read
	LCD_CMD = LCD_CMD | RS; // display data

	LCD_DATA_IODIR = 0x00;
}

void lcd_write(uint8_t data) {

	LCD_DATA_IODIR = 0xFF; // configure LCD_DATA IOs as output

	LCD_CMD = LCD_CMD | RS; // display data
	LCD_CMD = LCD_CMD & ~RW; // write
	LCD_CMD = LCD_CMD & ~CS; // accept data/commands
	LCD_CMD = LCD_CMD | E;
	_delay_us(1); // 180 ns would be save (see page 62)
	LCD_DATA = data;
	_delay_us(1); // 40 ns would be save (see page 62)
	LCD_CMD = LCD_CMD & ~E;
	LCD_DATA = 0x00; // default state
	LCD_CMD = LCD_CMD | CS; // do not accept further data/commands
	LCD_CMD = LCD_CMD | RW; // read
	LCD_CMD = LCD_CMD | RS; // display data

	LCD_DATA_IODIR = 0x00; // configure LCD_DATA IOs as intput
}

/*

void lcd_read_dummy(void) {

	//lcd_set_port_read(  );
	LCD_DATA_IODIR = 0x00;

  	LCD_CMD = LCD_CMD | RS; // display data
  	LCD_CMD = LCD_CMD | RW; // read
  	LCD_CMD = LCD_CMD & ~CS; // accept data/commands
  	LCD_CMD = LCD_CMD | E;
  //	_delay_us(1); // 140 ns would be save (see page 62)
  	LCD_CMD = LCD_CMD & ~E;
  //	_delay_us(1); // 40 ns would be save (see page 62)

  	LCD_CMD = LCD_CMD | CS; // do not accept further data/commands
  	LCD_CMD = LCD_CMD | RW; // read
  	LCD_CMD = LCD_CMD | RS; // display data
}

uint8_t lcd_read() {

	uint8_t data = 0x00;

	lcd_read_dummy();

	LCD_DATA = 0xFF;
    LCD_DATA_IODIR  = 0x00;

	LCD_CMD = LCD_CMD | RS; // display data
	LCD_CMD = LCD_CMD | RW; // read
	LCD_CMD = LCD_CMD & ~CS; // accept data/commands
	LCD_CMD = LCD_CMD | E;
//	_delay_us(1); // 140 ns would be save (see page 62)

	LCD_CMD = LCD_CMD & ~E;

//	_delay_us(1); // 40 ns would be save (see page 62)
	data = LCD_DATA_IN;// LCD_DATA;
	LCD_CMD = LCD_CMD | CS; // do not accept further data/commands
	LCD_CMD = LCD_CMD | RW; // read
	LCD_CMD = LCD_CMD | RS; // display data

	LCD_DATA = 0x00;

	return data;
}

uint8_t lcd_status() {

	return 0;
}

void show_icon(const uint8_t *bitmap,
					 uint8_t w,
					 uint8_t h,
					 uint8_t x,
					 uint8_t y,
					 uint8_t mode) {

	uint8_t offset_top = 0u;
	uint8_t offset_bottom = 0u;
	uint8_t currentByte = 0u;
    uint8_t currentByteTop = 0u;
    uint8_t pageByte = 0u;
    uint8_t hi8 = 0u;

    //loop variable
	uint8_t i = 0u;
	uint8_t j = 0u;

	uint8_t pageloops = 0u;

	//wenn y nicht ganzahlig teilbar durch 8
	offset_top = (uint8_t) (y % 8u);

	//
	offset_bottom = 8u-(y+h)%8u;
	if (offset_bottom == 8u)
	{
		offset_bottom = 0u;
	}

	//calculate loop, how many pages are needed
	pageloops = (uint8_t) (h/8u);
	hi8 = (uint8_t) (h/8u);

	if ((h%8u)!=0u)
	{
		pageloops++;
	}
	//calculate loop inc
	if ((y%8)!=0u)
	{
		if ((y%8)>(h%8))
		{
			pageloops++;
		}
	}

	//i for pages
	for (i = 0u; (i < 8u) && (i < pageloops); i++)
	{
	  //j for width
      for (j = 0u; j<w; j++)
      {
    	  currentByte = 0u;
    	  currentByteTop = 0u;
		if (offset_top != 0u) {
			//wenn es die erste zeile ist, die zu zeichnen ist
			if (i==0u)
			{

				//addressierung evtl. rausziehbar, da beim naechsten lesen naechste spalte ist
				lcd_cmd(COLUMN_ADDRESS_LOW | (COLUMN_ADDRESS_MASK & (uint8_t)(x+j)));  //column offset lower
				lcd_cmd(COLUMN_ADDRESS_HIGH | (COLUMN_ADDRESS_MASK & (uint8_t)(uint8_t)((x+j)>>4))); //column offset higher

				//nehme n bits von page (y / 8u)+i
				lcd_cmd(PAGE_ADDRESS | (PAGE_ADDRESS_MASK & ((y/8u)+i)));		//row in 8th
				currentByteTop = lcd_read();

				//zero new pixel for merge
				currentByteTop = (uint8_t) (currentByteTop << offset_bottom);
				currentByteTop = (uint8_t) (currentByteTop >> offset_bottom);

			}
			else
			{
			//else use pixel of bitmap's previous pageline
			currentByteTop = (uint8_t)((bitmap[(i-1u)*w+j]) >> offset_bottom);
			}
		}

		//if not overflow pageloop
		if (i!=hi8)
		{   //use byte of bitmap and shift with offset_top
			currentByte = (uint8_t)(bitmap[(i)*w+j]);
			currentByte = (uint8_t)(currentByte << offset_top);
		}

		currentByte |= currentByteTop; //add top part

		if (offset_bottom != 0u) {
			//if it is last page
			if (i == (pageloops-1u))
			{

			// use pixel of last page
			lcd_cmd(COLUMN_ADDRESS_LOW | (COLUMN_ADDRESS_MASK & (uint8_t)(x+j)));  //column offset lower
			lcd_cmd(COLUMN_ADDRESS_HIGH | (COLUMN_ADDRESS_MASK & (uint8_t)(uint8_t)((x+j)>>4))); //column offset higher
			lcd_cmd(PAGE_ADDRESS | (PAGE_ADDRESS_MASK & ((y/8u)+i)));		//row in 8th
			pageByte = lcd_read();

			//loesche offset_top bits
             pageByte = (uint8_t) (pageByte >> offset_top);  //shift down bits
             pageByte = (uint8_t) (pageByte << offset_top);  //shift up bits and fill with zero

             //merge with currentByte
             currentByte |= pageByte;
			}

		}

		//write to display
		lcd_cmd(COLUMN_ADDRESS_LOW | (COLUMN_ADDRESS_MASK & (uint8_t)(x+j)));
		lcd_cmd(COLUMN_ADDRESS_HIGH | (COLUMN_ADDRESS_MASK & (uint8_t)((x+j)>>4)));

		lcd_cmd(PAGE_ADDRESS | (PAGE_ADDRESS_MASK & ((y/8u)+i)));
		lcd_write(currentByte);

      }
	}
}

void lcd_putc(uint8_t c) {

	uint8_t column = 0;

	if (c < 32)
		return;
	else if (c > 126)
		return;
	else {
		c = c - 32;	lcd_write(0xFF);
		for (column = 0; column < 8; column++) {
			lcd_write(font[c][column]);
		}
	}
}


*/