/*
 * mainJt.c
 *
 *  Created on: Sep 23, 2022
 *      Author: diego
 */

#include <mainJt.h>


//////////////////  types and variables   //////////////////////////////7



CJoesTriacEventT  jtMessageQBuffer  [amtMessageBuffers];


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

void returnMessageBufferP(pJoesTriacEventT  mPtr)
{
	osStatus_t  status;
	status = MUTEX_AQUIRE(jtQAccessMutex, osWaitForever);
	if (status !=  osOK)  {
		//  todo  log error since it is a fatal error and raise fatal error state
		errorHandler(status,stop," MUTEX_AQUIRE ","returnMessageBufferP");
	}
	mPtr->evType= msgFree;
	status = MUTEX_RELEASE(jtQAccessMutex);
	if (status !=  osOK)  {
		//  todo  log error since it is a fatal error and raise fatal error state
		errorHandler(status,stop," MUTEX_RELEASE ","returnMessageBufferP");
	}
}

pJoesTriacEventT getNextFreeMessageBufferP(messageType forMessage)
{
	uint8_t  found = 0;
	uint8_t  cnt   = 0;
	osStatus_t  status;
	pJoesTriacEventT res =  NULL;

	if ((status = MUTEX_AQUIRE(jtQAccessMutex, osWaitForever)) !=  osOK)  {
		errorHandler(status,stop," MUTEX_AQUIRE ","getNextFreeMessageBufferP");
	}
	while ((found == 0) && (cnt < amtMessageBuffers) ) {
		if (jtMessageQBuffer[cnt].evType == msgFree) {
			found = 1;
			jtMessageQBuffer[cnt].evType = forMessage;
			res = & jtMessageQBuffer[cnt];
		}
	}
	if ((status = MUTEX_RELEASE(jtQAccessMutex)) !=  osOK)  {
		errorHandler(status,stop," MUTEX_RELEASE ","getNextFreeMessageBufferP");
	}
	if (cnt >= amtMessageBuffers) {
		errorHandler(cnt,goOn," bufferFull ","getNextFreeMessageBufferP");
	}
	return res;
}

osStatus_t sendMainJtMessageQ(pJoesTriacEventT bufferAddr, uint8_t  fromIsr)
{
	osStatus_t  status = osError;

	if (fromIsr == 1)  {
		status = osMessageQueuePut (mainJtMessageQ, (void *) bufferAddr, 0, 0);
	}  else  {
		status = osMessageQueuePut (mainJtMessageQ, (void *) bufferAddr, 0, 1000);
	}
	if (status != osOK) {
		errorHandler(status,goOn," status ","sendMainJtMessageQ");
	}
	return status;
}


void mainJtSecondTickCallback(void *argument)
{
//   todo check argument  and timer duration of one tick

	pJoesTriacEventT mPtr = getNextFreeMessageBufferP(secondTick);
	osStatus_t status =  sendMainJtMessageQ(mPtr, 0);
	if ((status != osOK) || (mPtr == 0)  ) {
		errorHandler(status,goOn," status ","mainJtSecondTickCallback");
		errorHandler((uint32_t)mPtr,stop," mptr ","mainJtSecondTickCallback");
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
	pJoesTriacEventT mPtr = 0;
	osStatus_t status;
	osStarted();
	uint32_t* msgPtr ;
	uint32_t** mPPtr =  &msgPtr;
	do  {
		if ((status = osMessageQueueGet(mainJtMessageQ,(void *) mPPtr, 0, osWaitForever)) == osOK )  {
			mPtr = (pJoesTriacEventT) msgPtr;
			if (mPtr->evType == secondTick) {
				secondTickHandler();
			}
			returnMessageBufferP(mPtr);
		}  else {
			errorHandler((uint32_t)status ,goOn," osMessageQueueGet "," mainJt ");
		}
	}  while (1);
}



void initJt()
{
	mainJtTaskHandle = osThreadNew(mainJt, NULL, &mainJt_attributes);
	if (mainJtTaskHandle  == NULL)   {
		errorHandler((uint32_t)mainJtTaskHandle ,stop," mainJtTaskHandle ","initJt");
	}

	mainJtMessageQ =  osMessageQueueNew(1, 4, NULL);
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
