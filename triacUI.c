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
	
	sprintf((char*)&buffer,"%5.2fV/%4iD",VFl,triacFireDurationTcnt2);

	lcd_goto(3, 6);
	lcd_write_str((char*)&buffer);
}

void displayDebugVoltageNTriggerDelay()
{
	double   VFl;
	char buffer [16];
	int16_t adc =  ampsADCValue();

	VFl = adcVoltage();

	sprintf((char*)&buffer,"%5.2fV %4iA %3iD",VFl,adc, triacFireDurationTcnt2);

	lcd_Line2();
	lcd_write_str((char*)&buffer);

}


void displayPotiPersistent()
{
	double   VFl;
	char buffer [16];

	VFl = zeroAdjustDiffVoltage;	

	sprintf((char*)&buffer,"%3i P %6.3f V",zeroPotiPos,VFl);

	lcd_Line2();
	lcd_write_str((char*)&buffer);

}

void displayPotiVolatile()
{
	char buffer[10];
	sprintf((char*) &buffer,"%3iP",zeroPotiPos);
	lcd_goto(3,16);
	lcd_write_str((char*)&buffer);
}

void displayCalibrateZeroPotiPos()
{
	lcd_clrscr();
	lcd_write_str("calib Zero  ,# skip");
	lcd_goto(3,0);
	lcd_write_str("wait to stable + 30s");
}


void displayRmsAvgQuery()
{
	lcd_clrscr();
	lcd_write_str("select");
	lcd_Line2();
	lcd_write_str("RMS *, AVG #");
}

void displayFatalError()
{
	lcd_clrscr();
	lcd_write_str("fatal error");
	lcd_Line2();
	lcd_write_str(lastFatalErrorString);
}

void displayCalibrate(int amps)
{
	char buffer [5];
	lcd_clrscr();
	lcd_write_str("Set ");
	sprintf((char*)&buffer,"%2i",amps);
	lcd_write_str((char*)&buffer);
	lcd_write_str("A #,* Skip");  
	lcd_goto(3,0);
	lcd_write_str("up 1,2,3 - dn 7,8,9");
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
	lcd_clrscr();
}

void displayCalibrationPrompt()
{
	lcd_clrscr();
	lcd_write_str("Calibrate? *=Yes");
	lcd_Line2();
	lcd_write_str("or wait");
	lcd_goto(3,0);
	lcd_write_str("A calib vars");
}

void displayCurrentAmps()
{
	char buffer [8];
	double amps;
	amps = currentAmpsValue;
	sprintf((char *)&buffer,"%5.1fA",amps);
	lcd_goto(1,0);
	lcd_write_str((char *)&buffer);
}

void displayCountDown()
{
	int16_t secondsRem = getSecondsDurationTimerRemaining();
	int32_t minRem  = secondsRem / 60; ;        //(250 us on 1Mhz Simulator)
	int16_t mRem = minRem;
	int8_t secsRem	= secondsRem - (minRem * 60);  // subtraction + multiply by 60 faster than division
													// 25 us on 1 Mhz Sim 2 
	char buffer [8];
	sprintf((char*)&buffer,"%3im%02i",mRem,secsRem);

	lcd_goto(1,9);
	lcd_write_str((char*)&buffer);
}

void displayInDurationTimerSince()
{
	int16_t secondsRel = getSecondsInDurationTimer();
	int32_t minRel  = secondsRel / 60; ;        //(250 us on 1Mhz Simulator)
	int16_t mRel = minRel;
	int8_t secsRel	= secondsRel - (minRel * 60);  // subtraction + multiply by 60 faster than division
													// 25 us on 1 Mhz Sim 2 
	char buffer [8];
	sprintf((char*)&buffer,"%3im%02i",mRel,secsRel);

	lcd_goto(1,6);
	lcd_write_str((char*)&buffer);
}


void displayTriacRunning()
{
	lcd_clrscr();
	lcd_write_str("Running ");
	lcd_goto(0,17);
	lcd_write_str(ARBuffer);
	lcd_goto(3,0);
	lcd_write_str("R stop");
}

void displayEditAmpsDuration()
{
	lcd_clrscr();
	lcd_write_str("A     T    ");
	lcd_Line2();
	lcd_write_str("*     #       ");
	lcd_goto(0,17);
	lcd_write_str(ARBuffer);
	lcd_goto(3,0);
	lcd_write_str("G start,A Setup");
}

void displayEditAmps()
{
	lcd_goto(0,0);
	lcd_write_str("Amps  T   ");
}

void displayEditDuration()
{
	lcd_goto(0,0);
	lcd_write_str("A     Time");
}

#define ampsTab  2

void displayAmps(int8_t kInd)
{  
	lcd_goto(1,ampsTab);
	lcd_write_char(amps100);
	lcd_write_char(amps10);
	lcd_write_char(amps);
	if ((kInd >= 0) &&(kInd <= 2)) lcd_set_cursor(1, ampsTab + kInd);
		else lcd_hide_cursor();	
}

#define timeTab  8

void displayTime(int8_t kInd)
{  
	lcd_goto(1,timeTab);
	lcd_write_char(min10);
	lcd_write_char(min);
	lcd_write_char(0x6D);
	lcd_write_char(sec10);
	lcd_write_char(sec);
	if ((kInd >= 0) &&(kInd <= 4)) lcd_set_cursor(1, timeTab + kInd);
		else lcd_hide_cursor();	
}

#define setupTab  13

void displaySetup()
{
	lcd_clrscr();
	lcd_write_str("Setup ");
	lcd_Line2();
	lcd_write_str("Alarm Y/N #");
	lcd_goto(2,0);
	lcd_write_str("Alarm T   *");
	lcd_goto(2,17);
	lcd_write_str("min");
	lcd_goto(3,0);
	lcd_write_str("B return");
}

void displaySetupShortCircuitAlarm()
{
	lcd_clrscr();
	lcd_write_str("Setup Short Circuit Alarm");
	lcd_Line2();
	lcd_write_str("secs limit #");
	lcd_goto(2,0);
	lcd_write_str("Amps limit *");
	lcd_goto(2,17);
	lcd_write_str("A");
	lcd_goto(3,0);
	lcd_write_str("B return");
}

void displayAlarmMinutes(int8_t kInd)
{  
	lcd_goto(2,setupTab);
	lcd_write_char(completionAlarmMins10);
	lcd_write_char(completionAlarmMins);
	if ((kInd >= 0) &&(kInd <= 1)) lcd_set_cursor(2, setupTab + kInd);
		else lcd_hide_cursor();	
}

void displayAlarmYesNo(int8_t kInd)
{
	lcd_goto(1,setupTab);
	if (completionAlarmOn) {
		lcd_write_str("On ");
	} else {
		lcd_write_str("Off");
	}
	if ((kInd >= 0) &&(kInd <= 0)) lcd_set_cursor(1, setupTab + kInd);
		else lcd_hide_cursor();	
}

void displayAlarmShortCircuitSecs(int8_t kInd)
{
	lcd_goto(2,setupTab);
	lcd_write_char(shortCircuitAlarmSecs100)
	lcd_write_char(shortCircuitAlarmSecs10);
	lcd_write_char(shortCircuitAlarmSecs);
	if ((kInd >= 0) &&(kInd <= 2)) lcd_set_cursor(2, setupTab + kInd);
	else lcd_hide_cursor();
}

void displayAlarmShortCircuitSecondsBarrier()
{
	lcd_goto(1,setupTab);
	lcd_write_char(shortCircuitAlarmSecondBarrier);
	if ((kInd >= 0) &&(kInd <= 0)) lcd_set_cursor(1, setupTab + kInd);
	else lcd_hide_cursor();
}

#define hintTab 8

void clearSetupInputHint()
{
	lcd_goto(3,hintTab);
	lcd_write_str("          ");
}

void toggleSetupInputHint()
{
	lcd_goto(3,hintTab);
	lcd_write_str(", 0..1");
}

void numericSetupInputHint()
{
	lcd_goto(3,hintTab);
	lcd_write_str(", 0..9");
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
	lcd_clrscr();
	lcd_write_str("Job OK");
	if (completionAlarmOn) {
		lcd_goto(0,14);
		lcd_write_str("al ");
		lcd_write_char(completionAlarmMins10);
		lcd_write_char(completionAlarmMins);
		lcd_write_str("m");
	}
	lcd_goto(1,0);
	lcd_write_str("since");
	lcd_goto(3,0);
	lcd_write_str("press * to continue");
}

void currentVarChanged()
{
	char buffer [6];
	lcd_goto(1,0);
	sprintf((char*)&buffer,"%4i",currentVarVal);
	lcd_write_str((char*)&buffer);
}

void displayCurrentVar()
{	
	lcd_clrscr();
	lcd_write_str(currentTitle);
	lcd_goto(2,0);
	lcd_write_str(" #save,* skip"); 	 
	lcd_goto(3,0);
	lcd_write_str("up 1,2,3 - dn 7,8,9");
	currentVarChanged();
}



