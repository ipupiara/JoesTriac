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
int8_t       keyInd;
int16_t		miniStringArrPos;
int8_t       miniStringBusy;
int8_t      numUpperLimit;
int8_t      numUpperKeyInd;



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
	return res;
}

void editMiniString(int16_t miniStrArrPos, miniStringCalcMenthodType calcMeth, miniStringDisplayMethodType dispMeth)
{
	miniStringArrPos =	miniStrArrPos;
	calcMethod = calcMeth;
	displayMethod = dispMeth;
	miniStringBusy = 1;
	keyInd = 0;
	numUpperKeyInd = -1;
	numUpperLimit  = 0x39;
	displayMethod(keyInd);
}

void endEditMiniString()
{
	displayMethod(-1);
	miniStringBusy = 0;
	resetNumUpperLimit();
}

void initMiniStringComponent()
{
	miniStringBusy = 0;
	editFinished = 0;
}


bool processMiniStringTriacEvent(CJoesTriacEvent* ev)
{
	bool res;
	res = uStIntNoMatch;	
	if (miniStringBusy){
		if (ev->evType == evCharEntered) {
			if ((ev->evData.keyCode <= kp9) && (ev->evData.keyCode >= kp0)  &&
				    (!((keyInd == numUpperKeyInd) && (ev->evData.keyCode > numUpperLimit ) ))) {
				if ((keyInd >= 0) && (keyInd < miniStringArray[miniStringArrPos].length) )  {
					EEPROM_write(miniStringArray[miniStringArrPos].eepromPos + keyInd, ev->evData.keyCode);
					calcMethod();
				}
				keyInd ++;
				
			}
			displayMethod(keyInd);
			if (keyInd >= miniStringArray[miniStringArrPos].length) {
				endEditMiniString();
				editFinished = 1;
			}
			res =  uStIntHandlingDone;
		}
	} 
	return (res);
}



void setNumUpperLimit(int8_t numUpper, int8_t atKey)
{
	numUpperKeyInd = atKey ;
	numUpperLimit = numUpper;
}

void resetNumUpperLimit()
{
	numUpperKeyInd = -1 ;
	numUpperLimit = 0x39;
}



/****************   mini string UI Component *****************/


void lcdWriteMiniStringWithGap(int16_t miniStringArrPos,  int8_t lineNr, int8_t rowNr, int8_t kInd, int8_t gapPos, int8_t gapLen)
{
	int8_t bt;
	int8_t len = miniStringArray[miniStringArrPos].length;
	int16_t pos = miniStringArray[miniStringArrPos].eepromPos;
	int16_t ps ;
	int8_t  gapJumped = 0;
	lcd_goto(lineNr,rowNr );
	for (int i1 = 0; i1 < len; ++ i1) {
		ps = pos + i1;
		bt = EEPROM_read(ps);
		lcd_write_char(bt);
		if (i1 == gapPos) {
			for (int i2 = 0; i2 < gapLen;++i2)  {
				lcd_move_cursor_right();
				++ gapJumped;
			}
		}
	}
	if ((kInd >= 0) &&(kInd < len)) lcd_set_cursor(lineNr, rowNr +  gapJumped + kInd);
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

/*

miniStringSetupConfigurationStruct   miniStringSetupConfiguration[amtMiniStringEditPages] =
	{
		{
			displayCompletionAlarmSetup,
			13, 2, {{completionAlarmOnArrPos, }  }
		}
	}
	
	*/