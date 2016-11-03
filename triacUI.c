#include <avr/io.h>
#include <stdio.h>
#include "TriacIntr.h"
#include "triacUI.h"
#include "st7565r.h"
#include "TriacDefines.h"
#include "triacPID.h"

char avgRmsStringBuffer [4];
int8_t actualLine;
int8_t actualTab;
int8_t actualSpaceAftTab;


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
	lcd_write_str(avgRmsStringBuffer);
	lcd_goto(3,0);
	lcd_write_str("R stop");
}


void initUI()
{
	if (ampsInputPin == rms) {
		sprintf((char*) &avgRmsStringBuffer[0],"RMS");
	} else {
		sprintf((char*) &avgRmsStringBuffer[0],"AVG");
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
		lcdWriteMiniString(completionAlarmMinsArrPos, 0, 17, -1);
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



void displayEditAmpsDuration()
{
	lcd_clrscr();
	lcd_write_str("A     T    ");
	lcd_Line2();
	lcd_write_str("*     #       ");
	lcd_goto(0,17);
	lcd_write_str(avgRmsStringBuffer);
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

// TODO   diplayChar(1,ampsTap,kInd,amtchar, char[]+

void displayAmps(int8_t kInd)
{
	actualLine = 1;
	actualTab = ampsTab;
	actualSpaceAftTab = 0;	
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


/****************  using  mini string  *****************/


void showShortCircuitAlarmSetup()
{
	actualTab = 13;
	lcd_clrscr();
	lcd_write_str("Short Circuit Alarm");
	lcd_Line2();
	lcd_write_str("secs limit #");
	lcd_goto(2,0);
	lcd_write_str("Amps limit *");
	lcd_goto(2,19);
	lcd_write_str("A");
	lcd_goto(3,0);
	lcd_write_str("B return");
}


void writeShortCircuitAlarmAmps(int8_t kInd)
{
	actualLine = 2;
	actualSpaceAftTab = 1;
	lcdWriteMiniString( shortCircuitAlarmAmpsArrPos ,actualLine, actualTab + actualSpaceAftTab,  kInd);
}

void writeShortCircuitAlarmSec(int8_t kInd)
{
	actualLine = 1;
	actualSpaceAftTab = 0;
	lcdWriteMiniStringWithGap( shortCircuitAlarmSecsArrPos ,actualLine, actualTab + actualSpaceAftTab, kInd,1,1);
	lcd_goto(actualLine, actualTab + actualSpaceAftTab + 2);
	lcd_write_str(".");
}

#define hintTab 8

void clearSetupInputHint()
{
	lcd_goto(3,hintTab);
	lcd_write_str("          ");
}


void inputHintNumericSetup(int8_t upperLimit)
{
	lcd_goto(3,hintTab);
	char  str [10];
	snprintf(str,19,", 0..%c",upperLimit );
	lcd_write_str(str);
}

void showCompletionAlarmSetup()
{
	actualTab = 13;
	lcd_clrscr();
	lcd_write_str("Setup ");
	lcd_Line2();
	lcd_write_str("Alarm Y/N #");
	lcd_goto(2,0);
	lcd_write_str("Alarm T   *");
	lcd_goto(2,17);
	lcd_write_str("min");
	lcd_goto(3,0);
	lcd_write_str("B continue");
}


void writeCompletionAlarmMinutes(int8_t kInd)
{
	actualLine = 2;
	actualSpaceAftTab = 0;
	lcdWriteMiniString(completionAlarmMinsArrPos,actualLine, actualTab + actualSpaceAftTab,kInd);
}

void writeCompletionAlarmOn(int8_t kInd)
{
	actualLine = 1;
	actualSpaceAftTab = 0;
	setNumUpperLimit(0x31);
	lcdWriteMiniString(completionAlarmOnArrPos,actualLine, actualTab + actualSpaceAftTab,kInd);
}

