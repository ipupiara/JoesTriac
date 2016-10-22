/*
 * miniString.c
 *
 * Created: 22.10.2016 13:45:55
 *  Author: mira
 */ 

/*

#include <avr/io.h>
#include <stdio.h>
#include "StateClass.h"
#include "miniString.h"

//#include <iostream.h>
#include "TriacDefines.h"
#include "miniString.h"



calcMenthodType  calcMethod;
displayMethodType  displayMethod;
int8_t       keyInd;
int16_t		miniStringArrPos;
int8_t       miniStringBusy;




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


void editMiniString(int16_t miniStrArrPos, calcMenthodType calcMeth, displayMethodType dispMeth)
{
	miniStringArrPos =	miniStrArrPos;
	calcMethod = calcMeth;
	displayMethod = dispMeth;
	miniStringBusy = 1;
	keyInd = 0;
	displayMethod(keyInd);
}

void endEditMiniString()
{
	displayMethod(-1);
	miniStringBusy = 0;
}

void initMiniStringComponent()
{
	miniStringBusy = 0;
}

bool processMiniStringTriacEvent(CJoesTriacEvent* ev)
{
	bool res;
	res = uStIntNoMatch;
	if (miniStringBusy){
		if (currentEvent->evType == evCharEntered) {
			if ((currentEvent->evData.keyCode <= kp9) && (currentEvent->evData.keyCode >= kp0)) {
				if ((keyInd >= 0) && (keyInd < miniStringArray[miniStringArrPos].length) )  {
					EEPROM_write(miniStringArray[miniStringArrPos].eepromPos + keyInd, currentEvent->evData.keyCode);
					calcMethod();
				}
				keyInd ++;
			}
			displayMethod(keyInd);
			res =  uStIntHandlingDone;
		}
		
	}
	return (res);
}



int16_t calcMiniStringUIntValue(int16_t miniSArrPos)
{
	int16_t res = 0;
	
	for (int8_t i1 = 0; i1 < miniStringArray[miniStringArrPos].length) {
		res =  (res * 10) +   EEPROM_read( miniStringArray[miniStringArrPos].eepromPos );
	}
	
	return res;
}

*/