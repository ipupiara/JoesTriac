#include <avr/io.h>
#include <stdio.h>
#include "TriacIntr.h"
#include "triacUI.h"
#include "st7565r.h"
#include "TriacDefines.h"
#include "triacPID.h"

char ARBuffer [4];


void displayVoltage()
{
	double   VFl;
	char buffer [13];
	
	VFl = adcVoltage();
	
	sprintf((char*)&buffer,"%5.2fV/%4iD",VFl,triacTriggerDelayCms);

	lcd_goto(3, 6, LCD1);
	lcd_write_str((char*)&buffer,LCD1);
}

void displayDebugVoltageNTriggerDelay()
{
	double   VFl;
	char buffer [16];

	VFl = adcVoltage();

	sprintf((char*)&buffer,"%5.2fV %5i D",VFl,triacTriggerDelayCms);

	lcd_Line2(LCD1);
	lcd_write_str((char*)&buffer,LCD1);

}


void displayPotiPersistent()
{
	double   VFl;
	char buffer [16];

	VFl = zeroAdjustDiffVoltage;	

	sprintf((char*)&buffer,"%3i P %6.3f V",zeroPotiPos,VFl);

	lcd_Line2(LCD1);
	lcd_write_str((char*)&buffer,LCD1);

}

void displayPotiVolatile()
{
	char buffer[16];
	sprintf((char*) &buffer,"%3iP (20-80)",zeroPotiPos);
	lcd_goto(3,8,LCD1);
	lcd_write_str((char*)&buffer,LCD1);
}

void displayCalibrateZeroPotiPos()
{
	lcd_clrscr(LCD1);
	lcd_write_str("calib Zero  ,# skip",LCD1);
	lcd_goto(3,0,LCD1);
	lcd_write_str("wait to stable + 30s",LCD1);
}


void displayRmsAvgQuery()
{
	lcd_clrscr(LCD1);
	lcd_write_str("select",LCD1);
	lcd_Line2(LCD1);
	lcd_write_str("RMS *, AVG #",LCD1);
}

void displayFatalError()
{
	lcd_clrscr(LCD1);
	lcd_write_str("fatal error",LCD1);
	lcd_Line2(LCD1);
	lcd_write_str(lastFatalErrorString,LCD1);
}

void displayCalibrate(int amps)
{
	char buffer [2];
	lcd_clrscr(LCD1);
	lcd_write_str("Set ",LCD1);
	sprintf((char*)&buffer,"%2i",amps);
	lcd_write_str((char*)&buffer,LCD1);
	lcd_write_str("A #,* Skip",LCD1);  
	lcd_goto(3,0,LCD1);
	lcd_write_str("up 1,2,3 - dn 7,8,9",LCD1);
} 

void displayCalibrateLow()
{
	displayCalibrate(calibLowAmps);
}

void displayCalibrateHigh()
{
	displayCalibrate(calibHighAmps);
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

void displayCurrentAmps()
{
	char buffer [8];
	double amps;
	amps = currentAmpsValue;
	sprintf((char *)&buffer,"%5.1fA",amps);
	lcd_goto(1,0,LCD1);
	lcd_write_str((char *)&buffer, LCD1);
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
	lcd_write_str("Running ",LCD1);
	lcd_goto(0,17,LCD1);
	lcd_write_str(ARBuffer,LCD1);
	lcd_goto(3,0,LCD1);
	lcd_write_str("R=stop",LCD1);
}

void displayEditAmpsDuration()
{
	lcd_clrscr(LCD1);
	lcd_write_str("A     T    ",LCD1);
	lcd_Line2(LCD1);
	lcd_write_str("*     #       ",LCD1);
	lcd_goto(0,17,LCD1);
	lcd_write_str(ARBuffer,LCD1);
	lcd_goto(3,0,LCD1);
	lcd_write_str("G=start",LCD1);
}

void displayEditAmps()
{
	lcd_goto(0,0,LCD1);
	lcd_write_str("Amps  T   ",LCD1);
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

void initUI()
{
	if (ampsInputPin == rms) {
		sprintf((char*) &ARBuffer[0],"RMS");
	} else {
		sprintf((char*) &ARBuffer[0],"AVG");
	}
//	printf("initUI ARBuffer: %s\n",ARBuffer);
}

void displayJobOk()
{
	lcd_clrscr(LCD1);
	lcd_write_str("Job OK",LCD1);
	lcd_goto(3,0,LCD1);
	lcd_write_str("press * to continue",LCD1);
}

