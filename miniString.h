/*
 * miniString.h
 *
 * Created: 22.10.2016 13:46:16
 *  Author: mira
 */ 


#ifndef MINISTRING_H_
#define MINISTRING_H_

#include "StateClass.h"
#include "triacUI.h"

typedef struct miniString {
	int16_t   eepromPos;
	int8_t    length;
} miniString ;

typedef int16_t (*miniStringCalcMenthodType)(void);

typedef void(*miniStringDisplayMethodType)(int8_t);

typedef void(*inputHintDisplayMethodType)(void);

#define amtMiniStringEditPages  2

typedef struct miniStringSetupConfigurationStruct {
	uint16_t     miniStringArrayPos;
	miniStringCalcMenthodType calcMethod, 
	miniStringDisplayMethodType dispMethod,
	inputHintDisplayMethodType   inputHintMethod
} miniStringSetupConfigurationStruct ;


typedef struct SetupConfigurationStruct {
	setupPageDisplayMethodType	pageDisplayMethod;
	miniStringSetupConfigurationStruct		miniStringSetupAstConfiguration;
	miniStringSetupConfigurationStruct		miniStringSetupNumConfiguration;
} SetupConfigurationStruct ;


int8_t editFinished;

void editMiniString(int16_t miniStringArrayPos, miniStringCalcMenthodType calcMeth, miniStringDisplayMethodType dispMeth);

void endEditMiniString();

void initMiniStringComponent();

int16_t calcMiniString(int16_t miniStringArrPos);

bool processMiniStringTriacEvent(CJoesTriacEvent* ev);

void setNumUpperLimit(int8_t numUpper, int8_t atKey);

void resetNumUpperLimit();

void lcdWriteMiniString(int16_t miniStringArrPos, int8_t lineNr, int8_t rowNr, int8_t kInd);

void lcdWriteMiniStringWithGap(int16_t miniStringArrPos,  int8_t lineNr, int8_t rowNr, int8_t kInd, int8_t gapPos, int8_t gapLen);

char* miniStringNToString(int16_t miniStringArrPos, uint8_t maxSize, char* buffer);

#endif /* MINISTRING_H_ */