/*
 * miniString.h
 *
 * Created: 22.10.2016 13:46:16
 *  Author: mira
 */ 


#ifndef MINISTRING_H_
#define MINISTRING_H_

#include "StateClass.h"

typedef struct  {
	int16_t   eepromPos;
	int8_t    length;
} miniString ;


typedef int16_t (*calcMenthodType)(void);

typedef void(*displayMethodType)(int8_t);


void editMiniString(int16_t miniStringArrayPos, calcMenthodType calcMeth, displayMethodType dispMeth);

void endEditMiniString();

void initMiniStringComponent();

int16_t calcMiniString(int16_t miniStringArrPos);

bool processMiniStringTriacEvent(CJoesTriacEvent* ev);



#endif /* MINISTRING_H_ */