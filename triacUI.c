#include <avr/io.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "TriacIntr.h"
#include "triacUI.h"
#include "st7565r.h"
#include "TriacDefines.h"

void calcDesiredAmps()
{
	desiredAmps = ((amps100 - 0x30) * 100 ) + ((amps10 -0x30) *10) + (amps - 0x30);
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

void calcDesiredTime()
{
	desiredTimeS = ((min10 - 0x30) * 600 ) + ((min - 0x30) *60) + ((sec10 - 0x30) * 10) + (sec - 0x30);
}

void storeMin10(int8_t val)
{
	eeprom_write_byte((uint8_t*)min10EEPROMpos,val);
	min10 = val;	
	calcDesiredTime();
}

void storeMin(int8_t val)
{
	eeprom_write_byte((uint8_t*)minEEPROMpos,val);
	min = val;	
	calcDesiredTime();
}

void storeSec10(int8_t val)
{
	eeprom_write_byte((uint8_t*)sec10EEPROMpos,val);
	sec10 = val;	
	calcDesiredTime();
}

void storeSec(int8_t val)
{
	eeprom_write_byte((uint8_t*)secEEPROMpos,val);
	sec = val;
	calcDesiredTime();		
}


                              

void initUI()
{
	amps100 = eeprom_read_byte((uint8_t*)amps100EEPROMpos);	
	if ((amps100 < 0x30) || (amps100 > 0x39)) { storeAmps100(0x30);}   // use lazy initialization
	amps10 = eeprom_read_byte((uint8_t*)amps10EEPROMpos);
	if ((amps10 < 0x30) || (amps10 > 0x39)) { storeAmps10(0x30);}
	amps = eeprom_read_byte((uint8_t*)ampsEEPROMpos);
	if ((amps < 0x30) || (amps > 0x39)) { storeAmps(0x30);}
	min10 = eeprom_read_byte((uint8_t*)min10EEPROMpos);
	if ((min10 < 0x30) || (min10 > 0x39)) { storeMin10(0x30);}
	min = eeprom_read_byte((uint8_t*)minEEPROMpos);
	if ((min < 0x30) || (min > 0x39)) { storeMin(0x30);}
	sec10 = eeprom_read_byte((uint8_t*)sec10EEPROMpos);
	if ((sec10 < 0x30) || (sec10 > 0x35)) { storeSec10(0x30);}
	sec = eeprom_read_byte((uint8_t*)secEEPROMpos);
	if ((sec < 0x30) || (sec > 0x39)) { storeSec(0x30);}
	calcDesiredTime();
	calcDesiredAmps();
}

void displayCurrentAmps()
{
}

void displayCalibratingExplanation()   // on LCD2
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
	lcd_write_str("*     #       ,S",LCD1);
}



void displayEditAmps()
{
	lcd_goto(0,0,LCD1);
	lcd_write_str("Amps  T",LCD1);
}

void displayEditDuration()
{
	lcd_goto(0,0,LCD1);
	lcd_write_str("A     Time",LCD1);
}


#define ampsTab  2

void displayAmps(int8_t kInd)
{  
	lcd_goto(1,ampsTab,LCD1);
	lcd_write_char(amps100,LCD1);
	lcd_write_char(amps10,LCD1);
	lcd_write_char(amps,LCD1);
	if ((kInd >= 0) &&(kInd <= 2)) lcd_set_cursor(1, ampsTab + kInd, LCD1);
		else lcd_hide_cursor();	
}

#define timeTab  8

void displayTime(int8_t kInd)
{  
	lcd_goto(1,timeTab,LCD1);
	lcd_write_char(min10,LCD1);
	lcd_write_char(min,LCD1);
	lcd_write_char(0x6D,LCD1);
	lcd_write_char(sec10,LCD1);
	lcd_write_char(sec,LCD1);
	if ((kInd >= 0) &&(kInd <= 4)) lcd_set_cursor(1, timeTab + kInd, LCD1);
		else lcd_hide_cursor();	
}
