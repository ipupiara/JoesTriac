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

/*
 * Terminals
 */

#define CS	0x01	// chip select; H: accept data/commands, L: do not
#define RST	0x02	// hardware reset; H: disabled, L: enabled
#define RS	0x04	// register select aka. A0; H: display, L: control data
#define RW	0x08	// read/write; H: read, L: write (6800 MPU)
#define E	0x10	// enable - impulse; L: active (6800 MPU)
#define LI	0x20	// background light; H: on, L: off
#define MI	0x40	// mpu interface; H: 6800, L: 8080
#define PS	0x80	// parallel/serial; H: parallel, L: serial

#define LCD_CMD PORTA		//control port of uC for LCD display
#define LCD_CMD_IODIR DDRA	//io direction for control port
#define LCD_DATA PORTC		//data port of uC for LCD display
#define LCD_DATA_IODIR DDRC	//io direction for data port
#define LCD_DATA_IN PINC 	//register for readout lcd pixel data

/*
 * Commands
 */

// Display property commands
#define ADC_NORMAL				0xA0
#define ADC_REVERSE				0xA1

#define BIAS_1_9				0xA2
#define BIAS_1_7				0xA3

#define ALL_POINTS_OFF			0xA4
#define ALL_POINTS_ON			0xA5

#define DISPLAY_NORMAL			0xA6
#define DISPLAY_REVERSE			0xA7

#define SLEEP_MODE_SLEEP		0xAC
#define SLEEP_MODE_NORMAL		0xAD

#define DISPLAY_OFF				0xAE
#define DISPLAY_ON				0xAF

// RAM access commands
#define COLUMN_ADDRESS_LOW		0x00
#define COLUMN_ADDRESS_HIGH		0x10
#define COLUMN_ADDRESS_MASK		0x0F

#define START_LINE				0x40
#define START_LINE_MASK			0x3F

#define PAGE_ADDRESS			0xB0
#define PAGE_ADDRESS_MASK		0x0F

#define SCAN_DIR_NORMAL			0xC0
#define SCAN_DIR_REVERSE		0xC8

#define RW_MOD_ON				0xE0
#define RW_MOD_OFF				0xEE

// Internal power supply commands
#define INTERNAL_POWER			0x28
#define INTERNAL_POWER_MASK		0x07

#define INTERNAL_RRATIO			0x20
#define INTERNAL_RRATIO_MASK	0x07
#define INTERNAL_RRATIO_MOD		0x81
#define INTERNAL_RRATIO_VOL		0x00
#define INTERNAL_RRATIO_VOL_MASK	0x3F

#define INTERNAL_BRATIO			0xF8
#define INTERNAL_BRATIO_MASK	0x02
#define BRATIO_2x3x4x			0x00
#define BRATIO_5x				0x01
#define BRATIO_6x				0x03

// Misc commands
#define RESET					0xE2
#define NOP						0xE3
#define TEST					0xF0


/*
 * Functions
 */

void lcd_init(void);
void lcd_cmd(uint8_t cmd);
void lcd_write(uint8_t data);
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
