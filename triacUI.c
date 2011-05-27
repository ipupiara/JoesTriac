#include <avr/io.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "TriacIntr.h"
#include "triacUI.h"
#include "st7565r.h"
#include "TriacDefines.h"

void initUI()
{
	amps100 = eeprom_read_byte((uint8_t*)amps100EEPROMpos);	
	amps10 = eeprom_read_byte((uint8_t*)amps10EEPROMpos);
	amps = eeprom_read_byte((uint8_t*)ampsEEPROMpos);
	min10 = eeprom_read_byte((uint8_t*)min10EEPROMpos);
	min = eeprom_read_byte((uint8_t*)minEEPROMpos);
	sec10 = eeprom_read_byte((uint8_t*)sec10EEPROMpos);
	sec = eeprom_read_byte((uint8_t*)secEEPROMpos);
}

void displayCurrentAmps()
{
}

void displayCalibratingExplanation()
{
}

void displayCalibrateLow()
{
	lcd_clrscr(LCD1);
	lcd_write_str("Calibrate Low",LCD1);
	lcd_Line2(LCD1);
	lcd_write_str("#,*,R",LCD1);
}

void displayCalibrateHigh()
{
	lcd_clrscr(LCD1);
	lcd_write_str("Calibrate High",LCD1);
	lcd_Line2(LCD1);
	lcd_write_str("#,*,R",LCD1);
}

void displayRunningValues()
{
	
}

void clr_scr()
{
	lcd_clrscr(LCD1);
}


void displayCalibrationPrompt()
{
	lcd_clrscr(LCD1);
	lcd_write_str("Calibrate? *=Yes",LCD1);
	lcd_Line2(LCD1);
	lcd_write_str("or wait",LCD1);
}


void calcDesiredAmps()
{
	desiredAmps = ((amps100 - 0x30) * 100 ) + ((amps10 -0x30) *10) + (amps - 0x30);
}

void calcDesiredTime()
{
	
}

void storeAmps100(int8_t val)
{
	eeprom_write_byte((uint8_t*)amps100EEPROMpos,val);
	amps100 = val;
	calcDesiredAmps();
}

void storeAmps10(int8_t val)
{
	eeprom_write_byte((uint8_t*)amps10EEPROMpos,val);
	amps10 = val;	
	calcDesiredAmps();
}

void storeAmps(int8_t val)
{
	eeprom_write_byte((uint8_t*)ampsEEPROMpos,val);
	amps = val;	
	calcDesiredAmps();
}

void storeMin10(int8_t val)
{
	eeprom_write_byte((uint8_t*)min10EEPROMpos,val);
	min10 = val;	
}

void storeMin(int8_t val)
{
	eeprom_write_byte((uint8_t*)minEEPROMpos,val);
	min = val;	
}

void storeSec10(int8_t val)
{
	eeprom_write_byte((uint8_t*)sec10EEPROMpos,val);
	sec10 = val;	
}

void storeSec(int8_t val)
{
	eeprom_write_byte((uint8_t*)secEEPROMpos,val);
	sec = val;	
}


void setAmps100(int8_t val)
{
	storeAmps100(val);
	// set cursor to amps 10
}

void setAmps10(int8_t val)
{
	storeAmps10(val);
	// as method before
}

void setAmps(int8_t val)
{
	storeAmps(val);
	// amps is the last of this entry, so set cursor off, no abortEntry will be sent
}

void setMin10(int8_t val)
{
	storeMin10(val);
}

void setMin(int8_t val)
{
	storeMin(val);
}

void setSec10(int8_t val)
{
	storeSec10(val);
}

void setSec(int8_t val)
{
	storeSec(val);
}

void displayCountDown()
{
	int16_t secondsRem = getSecondsRemaining();
	int32_t minRem  = secondsRem / 60; ;        //(250 us on 1Mhz Simulator)
	int16_t mRem = minRem;
	int8_t secsRem	= secondsRem - (minRem * 60);  // subtraction + multiply by 60 faster than division
													// 25 us on 1 Mhz Sim 2 
	char buffer [8];
	sprintf((char*)&buffer,"%3im%02i",mRem,secsRem);

	lcd_goto(1,9,LCD1);
	lcd_write_str((char*)&buffer,LCD1);
}

void displayTriacRunning()
{
	lcd_clrscr(LCD1);
	lcd_write_str("Triac Running",LCD1);
	lcd_Line2(LCD1);
	lcd_write_str("wait or H",LCD1);
}

void displayEditAmpsDuration()
{
	lcd_clrscr(LCD1);
	lcd_write_str("A     T",LCD1);
	lcd_Line2(LCD1);
	lcd_write_str("#     *,S",LCD1);
}

void startEditAmps()
{
	lcd_clrscr(LCD1);
	lcd_write_str("Amps  T",LCD1);
	lcd_Line2(LCD1);
	lcd_write_str("#     *,S",LCD1);
}

void startEditDuration()
{
	lcd_clrscr(LCD1);
	lcd_write_str("A     Time",LCD1);
	lcd_Line2(LCD1);
	lcd_write_str("#     *,S",LCD1);
}
