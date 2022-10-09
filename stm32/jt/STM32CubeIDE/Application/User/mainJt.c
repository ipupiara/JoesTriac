/*
 * mainJt.c
 *
 *  Created on: Sep 23, 2022
 *      Author: diego
 */

#include <mainJt.h>
#include <StateClass.h>
#include <TriacIntr.h>


//////////////////  types and variables   //////////////////////////////7

void mainJt(void *argument);


osThreadId_t mainJtTaskHandle;
const osThreadAttr_t mainJt_attributes = {
  .name = "mainJtTask",
  .stack_size = 512 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};

osTimerId_t   mainJtTimer;

osMessageQueueId_t    mainJtMessageQ;

osMessageQueueId_t    presenterMessageQ;

osMessageQueueId_t    modelMessageQ;

osMutexId_t  jtQAccessMutex;
//////////////////////   methods   /////////////////////////////



osStatus_t sendEventToMainJtMessageQ(pMainJtEventT pEv, uint8_t  fromIsr)
{
	osStatus_t  status = osError;

	if (fromIsr == 1)  {
		status = osMessageQueuePut (mainJtMessageQ, (void *) pEv, 0, 0);
	}  else  {
		status = osMessageQueuePut (mainJtMessageQ, (void *) pEv, 0, 1000);
	}
	if (status != osOK) {
		errorHandler(status,goOn," status ","sendMainJtMessageQ");
	}
	return status;
}


void mainJtSecondTickCallback(void *argument)
{
//   todo check argument  and timer duration of one tick

	CMainJtEventT  ev;
	memset(&ev, 0x0, sizeof(ev));
	ev.evType = secondTick;
	ev.evData.zeroAdjustingNVoltageState.amps = 3.1415926535897932384;
	ev.evData.zeroAdjustingNVoltageState.potiPos = 0x1234;
	osStatus_t status =  sendEventToMainJtMessageQ( &ev, 0);
	if (status != osOK) {
		errorHandler(status,goOn," status ","mainJtSecondTickCallback");
	}
}

void startMainJtTimer()
{
	osStatus_t  status;

	status = osTimerStart (mainJtTimer, 1000);
	if (status !=  osOK)  {
		errorHandler((uint32_t)status ,goOn," osTimerStart "," osStarted ");
	}
}

void mainJtOsStarted()
{
	startStateCharts();
	startMainJtTimer();
}


void mainJt(void *argument)
{
	osStatus_t status;
	CMainJtEventT  mainJtEv;
	fsmTriacEvent fsmEv;
	mainJtOsStarted();
	uint8_t  prio = 0;
	do  {
		memset(&mainJtEv, 0, sizeof(mainJtEv));
		if ((status = osMessageQueueGet(mainJtMessageQ,(void *) &mainJtEv, &prio, osWaitForever)) == osOK )  {
			if (mainJtEv.evType == secondTick) {
				durationTimerTick();
				fsmEv.evType=secondTick;
				processTriacFsmEvent(PJoesTriacStateChart,&fsmEv);
			}
		}  else {
			errorHandler((uint32_t)status ,goOn," osMessageQueueGet "," mainJt ");
		}
	}  while (1);
}

osStatus_t sendPresenterMessage(pJoesPresenterEventT  pMsg)
{
	osStatus_t  status = osError;
	status = osMessageQueuePut (presenterMessageQ, (void *) pMsg, 0, 10);
	if (status != osOK) {
		errorHandler(status,goOn," status ","sendPresenterMessage");
	}
	return status;
}

osStatus_t sendModelMessage(pJoesModelEventT  pMsg)
{
	osStatus_t  status = osError;
	status = osMessageQueuePut (modelMessageQ, (void *) pMsg, 0, 10);
	if (status != osOK) {
		errorHandler(status,goOn," status ","sendModelMessage");
	}
	return status;
}

#define memoryMultiplier 1

void initJt()
{
	mainJtTaskHandle = osThreadNew(mainJt, NULL, &mainJt_attributes);
	if (mainJtTaskHandle  == NULL)   {
		errorHandler((uint32_t)mainJtTaskHandle ,stop," mainJtTaskHandle ","initJt");
	}


	if ((jtQAccessMutex = osMutexNew(NULL)) == NULL) {
			errorHandler((uint32_t)jtQAccessMutex ,stop," jtQAccessMutex ","initJt");
	}

	mainJtTimer = osTimerNew (mainJtSecondTickCallback, osTimerPeriodic, (void *) 0x01, NULL);
	if (mainJtTimer  == NULL)   {
		errorHandler((uint32_t)mainJtTimer ,stop," mainJtTimer ","initJt");
	}

	mainJtMessageQ =  osMessageQueueNew(8,sizeof(CMainJtEventT)*memoryMultiplier, NULL);   //  todo check multiplication with 4 ? needed ?
	if (mainJtMessageQ  == NULL)   {
		errorHandler((uint32_t)mainJtMessageQ ,stop," mainJtMessageQ ","initJt");
	}

	presenterMessageQ =  osMessageQueueNew(5,sizeof(CJoesPresenterEventT)*memoryMultiplier, NULL);
	if (presenterMessageQ  == NULL)   {
		errorHandler((uint32_t)NULL, stop," presenterMessageQ ", "initJt");
	}

	modelMessageQ =  osMessageQueueNew(3,sizeof(CJoesModelEventT)*memoryMultiplier, NULL);
		if (modelMessageQ  == NULL)   {
			errorHandler((uint32_t)NULL, stop," modelMessageQ ", "initJt");
		}
}


