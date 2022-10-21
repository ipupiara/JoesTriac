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

uint8_t presenterQActive;

void mainJt(void *argument);


osThreadId_t mainJtTaskHandle;
const osThreadAttr_t mainJt_attributes = {
  .name = "mainJtTask",
  .stack_size = 1024 * 8,
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
	CMainJtEventT  mJtEv;
	fsmTriacEvent fsmEv;
	mainJtOsStarted();
	uint8_t  prio = 0;
	do  {
		memset(&mJtEv, 0, sizeof(mJtEv));  //  todo sort ifs for best performance,
		if ((status = osMessageQueueGet(mainJtMessageQ,(void *) &mJtEv, &prio, osWaitForever)) == osOK )  {
			if (mJtEv.evType == secondTick) {
					durationTimerTick();
					fsmEv.evType=evSecondsTick;
					processTriacFsmEvent(PJoesTriacStateChart,&fsmEv);
			} else if (mJtEv.evType == zCalibAuto) {
				setZCalibAuto(mJtEv.mainUnion.zAuto);
			} else if (mJtEv.evType == calibTriacDelayDelta) {
				calibTriacDelayChange(mJtEv.mainUnion.calibTriDelayCorrection);
			} else if (mJtEv.evType == storeAlarmData) {
				saveAlarmData(mJtEv.mainUnion.alarmData.alarmTime,mJtEv.mainUnion.alarmData.alarmNeeded,
						mJtEv.mainUnion.alarmData.zCalibOn);
			} else if (mJtEv.evType == storeWeldingTime) {
				saveWeldingTime(mJtEv.mainUnion.weldingTime);
			} else if (mJtEv.evType == storeWeldingAmpere) {
				saveWeldingAmps(mJtEv.mainUnion.weldingAmps);
			} else if (mJtEv.evType == saveZPotiPos) {
				saveZeroPotiPos(mJtEv.mainUnion.zPotiPos);
			} else if (mJtEv.evType == saveCalibLo) {
				saveCalibLow(mJtEv.mainUnion.calibLow);
			} else if (mJtEv.evType == saveCalibHi) {
				saveCalibHigh(mJtEv.mainUnion.calibHigh);
			} else if (mJtEv.evType == configBackPressed)  {
				fsmEv.evType=configBackPressed;
				processTriacFsmEvent(PJoesTriacStateChart,&fsmEv);
			} else if (mJtEv.evType == configPressed)  {
				fsmEv.evType=evConfigPressed;
				processTriacFsmEvent(PJoesTriacStateChart,&fsmEv);
			} else if (mJtEv.evType == calibAbortClick)  {
				fsmEv.evType=evCalibAbortClick;
				processTriacFsmEvent(PJoesTriacStateChart,&fsmEv);
			} else if (mJtEv.evType == calibContinueClick)  {
				fsmEv.evType=evCalibContinueClick;
				processTriacFsmEvent(PJoesTriacStateChart,&fsmEv);
			} else
			if (mJtEv.evType == autoConfigPressed)  {
				fsmEv.evType=evAutoConfigPressed;
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
	if (presenterQActive != 0)  {
		status = osMessageQueuePut (presenterMessageQ, (void *) pMsg, 0, 10);
		if (status != osOK) {
			errorHandler(status,goOn," status ","sendPresenterMessage");
		}
	}
	return status;
}

void setPresenterQActive()
{
	presenterQActive = 1;
}

void setPresenterQInactive()
{
	presenterQActive = 0;
}

uint8_t isPresenterQActive()
{
	uint8_t res;
	res = (presenterQActive != 0);
	return res;
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
	initDefines();
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
	presenterQActive = 0;

	modelMessageQ =  osMessageQueueNew(3,sizeof(CJoesModelEventT)*memoryMultiplier, NULL);
		if (modelMessageQ  == NULL)   {
			errorHandler((uint32_t)NULL, stop," modelMessageQ ", "initJt");
		}
}


