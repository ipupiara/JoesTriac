/*
 * miniString.c
 *
 * Created: 22.10.2016 13:45:55
 *  Author: mira
 */ 

#include <avr/io.h>
#include <stdio.h>
//#include <iostream.h>
#include "TriacDefines.h"
#include "miniString.h"

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

int16_t calcMiniString(int16_t eepromPos)
{
	int16_t res = 0;
	int8_t bt;
	int8_t len = miniStringArray[eepromPos].length;
	int16_t pos = miniStringArray[eepromPos].eepromPos;
	int8_t valid = 1;
	int16_t ps ;
	for (int i1 = 0; i1 < len; ++ i1) {
		ps = pos + i1;
		bt = EEPROM_read(ps);
		if ((bt < 0x30) || (bt > 0x39)) { 	
			printf("calcMiniString byte found as %i on eeprom\n", bt);
			eepromWriteByte (ps, 0);
			valid = 0;
		}
		if (valid) {
			res = res * 10 + bt;
		} else {
			res = 0;
		}
	}
	return res;
}