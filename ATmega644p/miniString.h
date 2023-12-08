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

enum {
		astPos = 0,
		numPos
	};

typedef struct miniString {
	int16_t   eepromPos;
	int8_t    length;
	int16_t*  pIntVar;
} miniString ;

typedef miniString*  pMiniString;

typedef int16_t (*miniStringCalcMenthodType)(void);

typedef void(*miniStringDisplayMethodType)(int8_t);

#define amtMiniStringEditPages  1
//#define amtMiniStringEditPages  4

// contains every information needed to edit the miniString
typedef struct SetupMiniStringConfigurationStruct {
	uint8_t      inUse;
	uint16_t     miniStringArrayPos;
	miniStringDisplayMethodType dispMethod;
} SetupMiniStringConfigurationStruct  ;

typedef SetupMiniStringConfigurationStruct* pMiniStringSetupConfigurationStruct;


// contains every info for editing a setup page
typedef struct SetupPageConfigurationStruct {
	setupPageDisplayMethodType	pageDisplayMethod;
	SetupMiniStringConfigurationStruct		miniStringSetupAstConfiguration;
	SetupMiniStringConfigurationStruct		miniStringSetupNumConfiguration;
} SetupPageConfigurationStruct ;

typedef SetupPageConfigurationStruct* pSetupPageConfigurationStruct;

pSetupPageConfigurationStruct  pCurrentMiniStringPage;

// needed for the event when editing of a miniString ended (especially if the last character has been entered)
int8_t editFinished;

void editMiniString(int16_t miniStringArrayPos, miniStringDisplayMethodType dispMeth);

void endEditMiniString();

void initMiniStringComponent();

int16_t calcMiniString(int16_t miniStringArrPos);

void calcAllMiniStrings();

bool processMiniStringTriacEvent(CJoesTriacEvent* ev);

void setNumUpperLimit(int8_t numUpper);  // can be called by the write...... method for a specific miniStringEditPos value

void lcdWriteMiniString(int16_t miniStringArrPos, int8_t lineNr, int8_t rowNr, int8_t kInd);

void lcdWriteMiniStringWithGap(int16_t miniStringArrPos,  int8_t lineNr, int8_t rowNr, int8_t kInd, int8_t gapPos, int8_t gapLen);

char* miniStringNToString(int16_t miniStringArrPos, uint8_t maxSize, char* buffer);

int8_t resetMiniStringComponent();

int8_t nextMiniStringPage();

void triggerGotoIdle();

#endif /* MINISTRING_H_ */