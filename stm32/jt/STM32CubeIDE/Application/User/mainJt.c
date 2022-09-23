/*
 * mainJt.c
 *
 *  Created on: Sep 23, 2022
 *      Author: diego
 */
#include <stdint.h>

#define amtMessageBuffers 5
#define bufferFull  0xFF



typedef enum {
	msgFree = 0,
	key,
	zeroAdjust,
	voltage
}  messageType;


typedef struct  {
	messageType evType;
	union {
		uint32_t keyCode;
		struct {
			float   voltage;
			uint16_t  potiPos;
		} zeroAdjustingNVoltageState;
	}  evData;
} CJoesTriacEvent ;

typedef CJoesTriacEvent CJoesTriacEventT;
typedef CJoesTriacEventT*  pCJoesTriacEventT;

CJoesTriacEventT  jtMessageQBuffer  [amtMessageBuffers];

uint8_t getNextFreeMessageBuffer(messageType forMessage)
{
	uint8_t  found = 0;
	uint8_t  cnt   = 0;
	//   todo put mutex around to hopefully prevent any consistency violation
	//   hopefully = depends finally on os  and ev. interrupt priorities
	while ((!found) && (cnt < amtMessageBuffers) ) {
		if (jtMessageQBuffer[cnt].evType == msgFree) {
			if (!found) {
				++ cnt;
			}
		}
	}
	if (cnt >= amtMessageBuffers) {
		cnt = bufferFull;
	}
	return cnt;
}




#include <mainJt.h>
#include "cmsis_os.h"


void mainJtSecondTickCallback(void *argument)
{


	/// Get a Message from a Queue or timeout if Queue is empty.
	/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
	/// \param[out]    msg_ptr       pointer to buffer for message to get from a queue.
	/// \param[out]    msg_prio      pointer to buffer for message priority or NULL.
	/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
	/// \return status code that indicates the execution status of the function.
//																						NULL    portMAX_DELAY   or waitForEver32
//	osStatus_t osMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);
}


osThreadId_t mainJtTaskHandle;
const osThreadAttr_t mainJt_attributes = {
  .name = "mainJtTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};



osTimerId_t   mainJtTimer;
//osTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr);

osMessageQueueId_t    mainJtMessageQ;
//const osMessageQueueAttr_t  mainJtMessageQAttr = {
//
//};


void mainJt(void *argument)
{

	/// Get a Message from a Queue or timeout if Queue is empty.
	/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
	/// \param[out]    msg_ptr       pointer to buffer for message to get from a queue.
	/// \param[out]    msg_prio      pointer to buffer for message priority or NULL.
	/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
	/// \return status code that indicates the execution status of the function.
	//																						0			portMAX_DELAY   or waitForEver32
//mainJtMessageQ =	osStatus_t osMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);

}



void initJt()
{
	mainJtTaskHandle = osThreadNew(mainJt, NULL, &mainJt_attributes);


// mainJtMessageQ = osMessageQueueId_t osMessageQueueNew (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr);
}
