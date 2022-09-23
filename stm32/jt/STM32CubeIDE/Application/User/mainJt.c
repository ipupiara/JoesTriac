/*
 * mainJt.c
 *
 *  Created on: Sep 23, 2022
 *      Author: diego
 */

#include <mainJt.h>

#include "cmsis_os.h"


#define amtMessageBuffers 5
#define bufferFull  0xFF


//////////////////  types and variables   //////////////////////////////7



CJoesTriacEventT  jtMessageQBuffer  [amtMessageBuffers];


osThreadId_t mainJtTaskHandle;
const osThreadAttr_t mainJt_attributes = {
  .name = "mainJtTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osTimerId_t   mainJtTimer;

osMessageQueueId_t    mainJtMessageQ;

osMutexId_t  jtQAccessMutex;
//////////////////////   methods   /////////////////////////////

uint32_t getNextFreeMessageBufferP(messageType forMessage)
{
	uint8_t  found = 0;
	uint8_t  cnt   = 0;
	osStatus_t  status;
	//   todo put mutex around to hopefully prevent any consistency violation
	//   hopefully = depends finally on os  and ev. interrupt priorities

	status = MUTEX_AQUIRE(jtQAccessMutex, osWaitForever);
	if (status ==  osOK)  {
		while ((found == 0) && (cnt < amtMessageBuffers) ) {
			if (jtMessageQBuffer[cnt].evType == msgFree) {
				if (!found) {
					++ cnt;
				}  else {
					jtMessageQBuffer[cnt].evType = forMessage;
					found = 1;
				}
			}
		}
		status = MUTEX_RELEASE(jtQAccessMutex);
		if (cnt >= amtMessageBuffers) {
			cnt = bufferFull;
		}
	}  else {
		//  todo  log error since it is a fatal error and raise fatal error state
		cnt = bufferFull;
	}
	return cnt;
}

osStatus_t sendMainJtMessageQ(uint32_t bufferAddr, uint8_t  fromIsr)
{
	osStatus_t  status = osError;

	if (fromIsr == 1)  {
		status = osMessageQueuePut (mainJtMessageQ, (void *) bufferAddr, 0, 0);
	}  else  {
		status = osMessageQueuePut (mainJtMessageQ, (void *) bufferAddr, 0, 1000);
	}

	return status;
}


void mainJtSecondTickCallback(void *argument)
{
//   todo check argument  and timer duration of one tick

}

void osStarted()
{
	osStatus_t  status;

	status = osTimerStart (mainJtTimer, 1000);
	if (status !=  osOK)  {
		//	osStatus_t   todo check return value, log error
	}
}


void mainJt(void *argument)
{
	uint32_t param;
	osStarted();
	do  {
			if (osMessageQueueGet(mainJtMessageQ, &param, 0, osWaitForever) == osOK )  {

			}  else
			{
				//  todo  handle and log
			}

	}  while (1);
}



void initJt()
{
	mainJtTaskHandle = osThreadNew(mainJt, NULL, &mainJt_attributes);

	mainJtMessageQ =  osMessageQueueNew(1, 4, NULL);

	if ((jtQAccessMutex = osMutexNew(NULL)) == NULL) {
		//  todo check NULL return on all four object creations
	}

	mainJtTimer = osTimerNew (mainJtSecondTickCallback, osTimerPeriodic, (void *) 0x01, NULL);


}
