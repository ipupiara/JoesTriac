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


#ifndef ST7565R_H
	#define ST7565R_H



void lcd_init(void);

void lcd_AskCalibration();

// void lcd_write(uint8_t data);
//uint8_t lcd_read(void);
/*
uint8_t lcd_status(void);
void lcd_read_dummy(void);

void show_icon(const uint8_t *bitmap,
					 uint8_t w,
					 uint8_t h,
					 uint8_t x,
					 uint8_t y,
					 uint8_t mode);
void lcd_putc(uint8_t c);
*/

#endif
