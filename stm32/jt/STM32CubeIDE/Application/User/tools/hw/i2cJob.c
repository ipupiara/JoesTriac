/*
 * i2cJob.c
 *
 *  Created on: Nov 1, 2022
 *      Author: Brigitte
 */



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <i2cJob.h>
#include <main.h>
#include <mainJT.h>
#include <string.h>
#include <uart-comms.h>     //  todo added temporarely,  tobe erased later
#include <i2c.h>

#include <TriacIntr.h>
#include <FreeRTOS.h>
#include <task.h>


#define waitShortCs   	1
#define waitMediumCs		2
#define waitLongCs		3
*/

#define byteArrayMaxSz   80
osSemaphoreId_t   i2cJobResourceSema;

typedef enum {
	jobActive,
	jobInactive
} jobStateEnum;

typedef struct {
	uint16_t waitS1ms;
	void* param;
	t_fPar   stepParMethod;
} i2cJobStepT ;


typedef struct {
	uint8_t   amtJobSteps;
	i2cJobStepT  i2cJobSteps [];

}i2cJobType;


typedef uint8_t commandLineType [];

typedef struct  {
	uint8_t len;
	uint8_t buffer [byteArrayMaxSz];
} byteArrayT;

typedef byteArrayT* pByteArrayT;

jobStateEnum jobState;
i2cJobType *  currentI2cJob;
uint8_t  currentStepIndex;
uint8_t  currentWaitCycle;

uint32_t currentRand;

i2cJobType i2cJob = {10,{{0,NULL,},{},{},{},{},{},{},{},{},{}}};

//#define maxStateNameLen  20
//char  stateName[maxStateNameLen+1];
//char  appStateName[maxStateNameLen+1];
//
//void setStateName(uint8_t* stName)
//{
//	memset(stateName,0,maxStateNameLen);
//	strncpy((char*)stateName,(char*) stName,maxStateNameLen);
//}
//
//void appendStateName(uint8_t* stName)
//{
//	memset(appStateName,0,maxStateNameLen);
//	strncpy(appStateName,(char*)stName,maxStateNameLen);
//}


//void setHelloPaintJob();
//
void clear(pByteArrayT pBary)
{
	memset(pBary,0,byteArrayMaxSz);
}

void addToByteArray(pByteArrayT pBary,uint8_t sz, uint8_t  arr [sz])
{
	uint8_t toMove;
	if (pBary->len + sz <= byteArrayMaxSz)  {
		toMove = sz;
	}else  {
		toMove = byteArrayMaxSz - pBary->len ;
	}
//	uint8_t i1;
//	for (i1 = 0; i1 < toMove; ++ i1) {
//		pBary->buffer[pBary->len] = arr[i1];
//		++pBary->len;
//	}

	memmove(&(pBary->buffer[pBary->len]),arr,toMove);
	pBary->len += toMove;
}

byteArrayT byteBuffer;

void startI2cCentiSecTimer()
{
	startSec100Timer();
}

void stopI2cCentiSecTimer()
{
	stopSec100Timer();
}


uint8_t sendI2cScreenCommand()
{
	uint8_t res = 0;
	res = sendI2cByteArray(screenI2cAddress,byteBuffer.buffer ,byteBuffer.len);
	return res;
}

uint8_t setNextScreenJob(i2cJobType* sJob)
{
	uint8_t res = 0;  // todo  check that used inside privileged code, else no effect of method
						//  see also F103 programming manual  cpsid instruction, also check primask values... confusings??...
	taskENTER_CRITICAL();  //  todo check that own assembler uses same constants as code in HAL (not only same value)
	if ( jobState == jobInactive) {
		currentI2cJob = sJob;
		currentStepIndex = 0;
		currentWaitCycle = 0;
		clear(&byteBuffer);  //  could be omitted here after debugging
		jobState = jobActive;
		res = 1;
	}
	taskEXIT_CRITICAL();
	startI2cCentiSecTimer();
	return res;
}

void  screenCentiStepExecution( uint8_t sz, i2cJobStepT  sJob [sz] )
{
	uint16_t waitTime = sJob[currentStepIndex].waitS1ms;
	if (currentWaitCycle < waitTime) {
		if (currentWaitCycle == 0) {
			clear(&byteBuffer);
			sJob[currentStepIndex].method.uni2.stepParMethod( sJob[currentStepIndex].uni1.param  );
			sendI2cScreenCommand();
		}
		++ currentWaitCycle;
//		if (currentWaitCycle == waitTime) {
//			if (currentWaitCycle != waitTime) {
//				sendI2cScreenCommand();
//			}
//		}
	} else {
		currentWaitCycle = 0;
		++ currentStepIndex;
	}
}


void i2cCentiSecTimer ()
{
	i2cJobType*  screenJob = NULL;

	taskENTER_CRITICAL();
		if (jobState == jobActive) {
			screenJob = currentI2cJob;
		}
	taskEXIT_CRITICAL();

	if (screenJob != NULL) {
		screenCentiStepExecution(screenJob->amtJobSteps,screenJob->i2cJobSteps);

		if (currentStepIndex >= screenJob->amtJobSteps) {
			taskENTER_CRITICAL();
			currentI2cJob = NULL;
			currentWaitCycle = 0;
			currentStepIndex = 0;
			jobState = jobInactive;
			taskEXIT_CRITICAL();
			stopI2cCentiSecTimer();
		}
	}
}

uint32_t requestAndWaitForCurrentI2cJob()
{
	uint32_t res = 0;
	osStatus_t status;
	status = osSemaphoreAcquire(i2cJobResourceSema, osWaitForever);
	if (status == osOK) {
		while (res == 0) {
			res= currentRand = (uint32_t) (rand());
		}
	}
	return res;
}


//
//uint8_t  setEepromAddress(INT8U i2cAdr,INT8U memAdr)
//{
//	uint8_t res = 0;
//	uint8_t byteArr [1];
//	byteArr[0] = memAdr;
//	do {
////		res = receiveI2cByteArray(i2cAdr, &byteArr[0], 1, 1);   // just once used for debugging
//		res = sendI2cByteArray(i2cAdr, &byteArr[0], 1, 3);   // set 1 ms delay for debugging in the do while loop
//		if (res == 0) {
//			pollForReady(i2cAdr, 0);  // todo test if this is even needed here, without something to write
//		}
//	} while (1);
//	return res;
//}


void addSetAddress(uint8_t adr)
{

}

void addToCurrentI2cJob(uint32_t keyRand, pVarData pVarD, i2cJobTopic jobTopic)
{
	if ((keyRand == currentRand) && (keyRand != 0)) {

	}
}

void startCurrentI2cJob(uint32_t keyRand)
{
	if ((keyRand == currentRand) && (keyRand != 0)) {

		keyRand = 0;
	}
}

void storeFloatToEeprom(float val, uint16_t pos)
{

}

float getFloatFromEeporm(uint16_t pos )
{
	float res = 0.0;
	return res;
}



/*

uint8_t lines [4] = {0x00, 0x40, 0x14, 0x54};

uint8_t getDdramAdr(uint8_t xPos, uint8_t yPos)
{
	uint8_t res = lines [yPos - 1];
	res += xPos -1;
	return res;
}

void initScreenFuntionSet(void)
{
	commandLineType cmd = {LCD_LastControlByte + LCD_CommandControlByte, LCD_FUNCTIONSET + LCD_8BITMODE + LCD_2LINE + LCD_5x8DOTS};
	addToByteArray(&byteBuffer, 2, cmd);
}

void initDisplayControl(void)
{
	commandLineType cmd = {LCD_LastControlByte + LCD_CommandControlByte,LCD_DISPLAYCONTROL+ LCD_DISPLAYON + LCD_CURSOROFF + LCD_BLINKOFF};
	addToByteArray(&byteBuffer, 2, cmd);
}

void clearDisplay(void)
{
	commandLineType cmd = {LCD_LastControlByte + LCD_CommandControlByte,LCD_CLEARDISPLAY};
	addToByteArray(&byteBuffer, 2, cmd);
}

void initEntryModeSet(void)
{
	commandLineType cmd = {LCD_LastControlByte + LCD_CommandControlByte,LCD_ENTRYMODESET + LCD_CURSORRIGHTADRINC };
	addToByteArray(&byteBuffer, 2, cmd);
}

void addCursorToByteArray(uint8_t xPos, uint8_t yPos)
{
	uint8_t adr = 0x00;
	adr = getDdramAdr(xPos, yPos);
	commandLineType cmd = {LCD_LastControlByte + LCD_CommandControlByte,LCD_SETDDRAMADDR + adr };
	addToByteArray(&byteBuffer, 2, cmd);
}

void setCursor(void)
{
	addCursorToByteArray(currentScreenJob->i2cJobSteps[currentStepIndex].xPos,
			currentScreenJob->i2cJobSteps[currentStepIndex].yPos);
//	uint8_t adr = 0x00;
//	adr = getDdramAdr(currentScreenJob->screenJobSteps[currentStepIndex].xPos, currentScreenJob->screenJobSteps[currentStepIndex].yPos);
//	commandLineType cmd = {LCD_LastControlByte + LCD_CommandControlByte,LCD_SETDDRAMADDR + adr };
//	addToByteArray(&byteBuffer, 2, cmd);
}

void returnHome(void)
{
	commandLineType cmd = {LCD_LastControlByte + LCD_CommandControlByte,LCD_RETURNHOME};
	addToByteArray(&byteBuffer, 2, cmd);
}

void helloScreen(void)
{
	commandLineType cmd = {LCD_ContinuousControlByte + LCD_CommandControlByte,LCD_RETURNHOME,LCD_LastControlByte + LCD_AsciiControlByte};
	addToByteArray(&byteBuffer, 3, cmd);
	char* stri = "tubeVoltageRegulator";
	addToByteArray(&byteBuffer, strlen(stri), (uint8_t*) stri);
}
//  experimental   question: how much can be sent together with i2c without performing wait states,
//   handling of needed waitStates could as well be done on display itself (encapsulation principle) ...  ??
void anotherScreen (commandLineType** pCmdLine)
{
	commandLineType cmd = {LCD_LastControlByte + LCD_AsciiControlByte};
	addToByteArray(&byteBuffer, 1, cmd);
	char* stri = "tubeVoltageRegulator";
	addToByteArray(&byteBuffer, strlen(stri), (uint8_t*) stri);
}

void emptyWait()
{

}

void paintHello()
{
	commandLineType cmd = {LCD_LastControlByte + LCD_AsciiControlByte};
	addToByteArray(&byteBuffer, 1, cmd);
	char* stri = "screen initialized";
	addToByteArray(&byteBuffer, strlen(stri), (uint8_t*) stri);
}

void paintCanScr()
{

}

void displayTemperatureLine()
{
//	double   tmp;
//	double   hyd;
//	char buffer [20+1];
//	tmp = getCurrentTemperature();
//	hyd = getCurrentHumidity();
//	commandLineType cmd = {LCD_LastControlByte + LCD_AsciiControlByte};
//	addToByteArray(&byteBuffer, 1, cmd);
////	snprintf(buffer, sizeof(buffer), "T %6.2f H %6.2f",tmp, hyd);
//	addToByteArray(&byteBuffer, strlen(buffer) , (uint8_t*) buffer);
}

void displayTimeLine()
{
//	uint8_t secs;
//	uint8_t mins;
//	uint16_t hrs;
//	char buffer [20+1];
////	getTimeValues(&hrs, &mins, &secs);
//	commandLineType cmd = {LCD_LastControlByte + LCD_AsciiControlByte};
//	addToByteArray(&byteBuffer, 1, cmd);
////	float cntf = hygrosenseMsgCnt;
////	snprintf(buffer, sizeof(buffer), "%4i:%02i.%02i/%7.0f",hrs,mins,secs,cntf);
//	addToByteArray(&byteBuffer, strlen(buffer) , (uint8_t*) buffer);
}

//void displayStatechartLine()
//{
//	commandLineType cmd = {LCD_LastControlByte + LCD_AsciiControlByte};
//	addToByteArray(&byteBuffer, 1, cmd);
//	uint8_t stLen = strlen(stateName);
//	uint8_t appLen = strlen(appStateName);
//	addToByteArray(&byteBuffer, stLen, (uint8_t*) stateName);
//	uint8_t maxLen = maxStateNameLen -stLen;
//	if(appLen < maxLen) { maxLen = appLen; }
//	addToByteArray(&byteBuffer, maxLen, (uint8_t*) appStateName);
//	char* spc = " ";
//	for(uint8_t i1= stLen + appLen; i1 < 20 ; ++ i1 ) {
//		addToByteArray(&byteBuffer,1,(uint8_t*)spc);
//	}
//

//	snprintf(buffer, sizeof(buffer), "T %6.2f H %6.2f",tmp, hyd);
//	addToByteArray(&byteBuffer, strlen(buffer) , (uint8_t*) buffer);

//}

void displayErrorStateLine()
{
//	double   tmp;
//	double   hyd;
//	char buffer [20+1];
//	tmp = getCurrentTemperature();
//	hyd = getCurrentHumidity();
//	commandLineType cmd = {LCD_LastControlByte + LCD_AsciiControlByte};
//	addToByteArray(&byteBuffer, 1, cmd);
////	snprintf(buffer, sizeof(buffer), "T %6.2f H %6.2f",tmp, hyd);
//	addToByteArray(&byteBuffer, strlen(buffer) , (uint8_t*) buffer);
}


screenJobType  initJob = {5, {{waitLongCs,0,0,initScreenFuntionSet}, {waitLongCs,0,0, initDisplayControl},
								{waitShortCs,0,0, clearDisplay},{waitLongCs,0,0, initEntryModeSet},
								{waitShortCs,0,0,returnHome}}};

screenJobType testPaint = {8, {{waitShortCs,1,1,setCursor}, {waitShortCs,0,0, paintHello}
							, {waitShortCs,2,1,setCursor}, {waitShortCs,0,0, paintHello}
							, {waitShortCs,3,1,setCursor}, {waitShortCs,0,0, paintHello}
							, {waitShortCs,4,1,setCursor}, {waitLongCs,0,0, paintHello}}};

screenJobType halloPaint = {2, {{waitShortCs,1,1,setCursor}, {waitShortCs,0,0, paintHello}}};

//screenJobType growboxScreenPaint = {6, {{waitShortCs,1,1,setCursor},{waitLongCs,1,1,displayTemperatureLine},
//										{waitShortCs,1,2,setCursor},{waitLongCs,1,1,displayTimeLine} ,
//										{waitShortCs,1,3,setCursor},{waitLongCs,1,1,displayStatechartLine}}};
////										{waitShortCs,1,4,setCursor},{waitLongCs,1,1,displayErrorStateLine}}};


//void paintCanScreen()
//{
//	setNextScreenJob(&canScreen);
//}

//void setDebugScreenJob()
//{
//	setNextScreenJob(&testPaint);
//}
//
//void setHelloPaintJob()
//{
//	setNextScreenJob(&halloPaint);
//}
//
//void setGrowboxScreen()
//{
//	setNextScreenJob(&growboxScreenPaint);
//}
//
//
//void displayFatalError()
//{
//
//}
*/

void initI2cJob()
{
	time_t tim;
	srand((unsigned) time(&tim));
	osSemaphoreDef_t  i2cJobSendSemaphoreDef;
	i2cJobSendSemaphoreDef.name="i2c send sema"  ;
	i2cJobSendSemaphoreDef.attr_bits= 0;
	i2cJobSendSemaphoreDef.cb_mem = NULL;
	i2cJobSendSemaphoreDef.cb_size = 0;
	i2cJobResourceSema=  osSemaphoreNew(1,1,&i2cJobSendSemaphoreDef);


	currentI2cJob = NULL;
	currentStepIndex = 0;
	jobState = jobInactive;
	currentWaitCycle = 0;
	clear(&byteBuffer);
//	setNextScreenJob(&initJob);
}

