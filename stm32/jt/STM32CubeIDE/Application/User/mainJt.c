/*
 * mainJt.c
 *
 *  Created on: Sep 23, 2022
 *      Author: diego
 */

#include <mainJt.h>
#include <StateClass.h>


//////////////////  types and variables   //////////////////////////////7

void mainJt(void *argument);


osThreadId_t mainJtTaskHandle;
const osThreadAttr_t mainJt_attributes = {
  .name = "mainJtTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osTimerId_t   mainJtTimer;

osMessageQueueId_t    mainJtMessageQ;

osMutexId_t  jtQAccessMutex;
//////////////////////   methods   /////////////////////////////


void secondTickHandler()
{
	errorHandler(0,goOn," status ","secondTick");  // just something for a breakpoint for first tests
}


osStatus_t sendEventToMainJtMessageQ(pJoesTriacEventT pEv, uint8_t  fromIsr)
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

	CJoesTriacEventT  ev;
	memset(&ev, 0x0, sizeof(ev));
	ev.evType = secondTick;
//	ev.evData.keyCode = 0x12345678;
	ev.evData.zeroAdjustingNVoltageState.voltage = 3.1415926535897932384;
	ev.evData.zeroAdjustingNVoltageState.potiPos = 0x1234;
	osStatus_t status =  sendEventToMainJtMessageQ( &ev, 0);
	if (status != osOK) {
		errorHandler(status,goOn," status ","mainJtSecondTickCallback");
	}
}

void osStarted()
{
	osStatus_t  status;

	status = osTimerStart (mainJtTimer, 1000);
	if (status !=  osOK)  {
		errorHandler((uint32_t)status ,goOn," osTimerStart "," osStarted ");
	}
}


void mainJt(void *argument)
{
	osStatus_t status;
	osStarted();
	CJoesTriacEventT  ev;
	do  {
		memset(&ev, 0, sizeof(ev));
		if ((status = osMessageQueueGet(mainJtMessageQ,(void *) &ev, 0, osWaitForever)) == osOK )  {
			if (ev.evType == secondTick) {
				secondTickHandler();
			}
		}  else {
			errorHandler((uint32_t)status ,goOn," osMessageQueueGet "," mainJt ");
		}
	}  while (1);
}



void initJt()
{
	startStateCharts();

	mainJtTaskHandle = osThreadNew(mainJt, NULL, &mainJt_attributes);
	if (mainJtTaskHandle  == NULL)   {
		errorHandler((uint32_t)mainJtTaskHandle ,stop," mainJtTaskHandle ","initJt");
	}

	mainJtMessageQ =  osMessageQueueNew(1,sizeof(pJoesTriacEventT)*4, NULL);
	if (mainJtMessageQ  == NULL)   {
		errorHandler((uint32_t)mainJtMessageQ ,stop," mainJtMessageQ ","initJt");
	}


	if ((jtQAccessMutex = osMutexNew(NULL)) == NULL) {
			errorHandler((uint32_t)jtQAccessMutex ,stop," jtQAccessMutex ","initJt");
	}

	mainJtTimer = osTimerNew (mainJtSecondTickCallback, osTimerPeriodic, (void *) 0x01, NULL);
	if (mainJtTimer  == NULL)   {
		errorHandler((uint32_t)mainJtTimer ,stop," mainJtTimer ","initJt");
	}

}
