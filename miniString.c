/*
 * miniString.c
 *
 * Created: 22.10.2016 13:45:55
 *  Author: mira
 */ 



#include <avr/io.h>
#include <stdio.h>
#include "miniString.h"
//#include "StateClass.h"
//#include <iostream.h>
#include "TriacDefines.h"
#include "TriacKeyPad.h"
#include "st7565r.h"

extern const uStInt uStIntHandlingDone;
extern const uStInt uStIntNoMatch;

miniStringCalcMenthodType  calcMethod;
miniStringDisplayMethodType  displayMethod;
int8_t       miniStringEditPos;
int16_t		miniStringArrPos;
int8_t       miniStringBusy;
int8_t      numUpperLimit;

int8_t currentMiniStringPageNumber;




void eepromWriteByte (uint16_t adr, uint8_t val)
{
	uint8_t checkRes = 0;
	EEPROM_write(adr,val);
	checkRes = EEPROM_read(adr);
	if (val != checkRes) {
		printf("error eepromWriteByte stored %X, but check returned %X\n",val,checkRes);
		} else {
//		printf("eepromWriteByte ok returned %X\n",checkRes);
	}
}


int16_t calcMiniString(int16_t miniStringArrPos)
{
	int16_t res = 0;
	int8_t bt;
	int8_t len = miniStringArray[miniStringArrPos].length;
	int16_t pos = miniStringArray[miniStringArrPos].eepromPos;
	int16_t ps ;
	for (int i1 = 0; i1 < len; ++ i1) {
		ps = pos + i1;
		bt = EEPROM_read(ps);
		if ((bt < 0x30) || (bt > 0x39)) { 	
			printf("calcMiniString byte found as %i on eeprom\n", bt);
			eepromWriteByte (ps, 0x30);
			bt = 0x30;
		}
		res = res * 10 + (bt - 0x30);
	}
	* miniStringArray[miniStringArrPos].pIntVar = res;
	return res;
}


void calcAllMiniStrings()
{
	int16_t msCnt;
	for (msCnt = 0; msCnt < amtMiniStrings; ++msCnt) {
		calcMiniString(msCnt);
	}
}


void displayMiniString(int8_t pos)
{
	displayMethod(pos);
	inputHintNumeric(numUpperLimit);
	displayMethod(pos); // just a temporary hack to find out if now cursor is ok
}


void resetEditPosLimit()
{
	numUpperLimit = 0x39;
}

void editMiniString(int16_t miniStrArrPos, miniStringDisplayMethodType dispMeth)
{
	miniStringArrPos =	miniStrArrPos;
	displayMethod = dispMeth;
	miniStringBusy = 1;
	miniStringEditPos = 0;
	resetEditPosLimit();
	displayMiniString(miniStringEditPos);
}

void triggerGotoIdle()
{
	editFinished = 1;
}

void endEditMiniString()
{
	displayMethod(-1);
	miniStringBusy = 0;
}



void setNumUpperLimit(int8_t numUpper)
{
	numUpperLimit = numUpper;
}

void incMiniStringEditPos()
{
	miniStringEditPos ++;
	resetEditPosLimit();
}



bool processMiniStringTriacEvent(CJoesTriacEvent* ev)
{
	bool res;
	res = uStIntNoMatch;	
	if (miniStringBusy){
		if (ev->evType == evCharEntered) {
			if (((ev->evData.keyCode <= kp9) && (ev->evData.keyCode >= kp0))  &&
				    (!(ev->evData.keyCode > numUpperLimit  ))) {
				if ((miniStringEditPos >= 0) && (miniStringEditPos < miniStringArray[miniStringArrPos].length) )  {
					EEPROM_write(miniStringArray[miniStringArrPos].eepromPos + miniStringEditPos, ev->evData.keyCode);
					calcMiniString(miniStringArrPos);
				}
				incMiniStringEditPos();
			}
			displayMiniString(miniStringEditPos);
			if (miniStringEditPos >= miniStringArray[miniStringArrPos].length) {
				triggerGotoIdle();
			}
			res =  uStIntHandlingDone;
		}
	} 
	return (res);
}



/****************   mini string UI Component *****************/


void lcdWriteMiniStringWithGap(int16_t miniStringArrPos,  int8_t lineNr, int8_t rowNr, int8_t kInd, int8_t gapPos, int8_t gapLen)
{
	int8_t bt;
	int8_t len = miniStringArray[miniStringArrPos].length;
	int16_t pos = miniStringArray[miniStringArrPos].eepromPos;
	int16_t ps ;
	lcd_goto(lineNr,rowNr );
	for (int i1 = 0; i1 < len; ++ i1) {
		ps = pos + i1;
		bt = EEPROM_read(ps);
		lcd_write_char(bt);
		if (i1 == gapPos) {
			for (int i2 = 0; i2 < gapLen;++i2)  {
				lcd_move_cursor_right();
			}
		}
	}
	int cursorPos = rowNr + kInd;
	if (kInd > gapPos) {
		cursorPos += gapLen;
	}
	if ((kInd >= 0) &&(kInd < len)) lcd_set_cursor(lineNr, cursorPos);
	else lcd_hide_cursor();
}


void lcdWriteMiniString(int16_t miniStringArrPos, int8_t lineNr, int8_t rowNr, int8_t kInd)
{
	lcdWriteMiniStringWithGap(miniStringArrPos,lineNr,rowNr,kInd,-1,0);
}

char* miniStringNToString(int16_t miniStringArrPos, uint8_t maxSize, char* buffer)
{
	uint8_t bt;
	uint8_t i1;
	int8_t len = miniStringArray[miniStringArrPos].length;
	int16_t pos = miniStringArray[miniStringArrPos].eepromPos;
	for (i1 = 0; ((i1 < len) && (i1 < maxSize )); ++ i1)  {
		bt = EEPROM_read(pos + i1);
		buffer [i1] = bt;
	}
	++ i1;
	buffer [i1] = 0;
	return buffer;
}


// needs at least one entry or else the ministring component should not be used,
// what would be worthless anyhow
SetupPageConfigurationStruct   setupPageConfiguration[amtMiniStringEditPages] =
	{
		{
			showCompletionAlarmSetup,
			{1,completionAlarmMinsArrPos, writeCompletionAlarmMinutes  },
			{1,completionAlarmOnArrPos, writeCompletionAlarmOn }
		},
		{	showShortCircuitAlarmSetup,
			{1,shortCircuitAlarmAmpsArrPos, writeShortCircuitAlarmAmps },
			{1,shortCircuitAlarmSecs10ArrPos, writeShortCircuitAlarmSec }	
		},
		{
			showDValueLowHighAlarmSetup,
			{1,dValueAlarmLowArrPos,writeDValueLow},
			{1,dValueAlarmHighArrPos,writeDValueHigh}
		},
		{
			showDValueSec10FatalAlarmSetup,
			{1,dValueAlarmSec10ArrPos,writeDValueSec10},
			{1,dValueAlarmFatalArrPos,writeDValueFatal}	
		}
	};
	
int8_t nextMiniStringPage()
{
	int8_t   res = 0;
	++ currentMiniStringPageNumber;
	if (currentMiniStringPageNumber < amtMiniStringEditPages) {
		pCurrentMiniStringPage = &setupPageConfiguration[currentMiniStringPageNumber];
		res = 1;
	} else  {
		res = 0;
	}
	return res;
}	

int8_t resetMiniStringComponent()
{
	int8_t res;
	currentMiniStringPageNumber = -1;
	res = (nextMiniStringPage());
	return res;
}


void initMiniStringComponent()
{
	miniStringBusy = 0;
	editFinished = 0;
}
