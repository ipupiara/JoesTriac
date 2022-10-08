/*
 * mainJt.h
 *
 *  Created on: Sep 23, 2022
 *      Author: diego
 */

#ifndef APPLICATION_USER_MAINJT_H_
#define APPLICATION_USER_MAINJT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "cmsis_os.h"
#include <defines.h>
#include <string.h>


#define MUTEX_AQUIRE(HANDLE,TIMEOUT)  osMutexAcquire (HANDLE, TIMEOUT)
#define MUTEX_RELEASE(HANDLE) osMutexRelease (HANDLE)
#define amtMessageBuffers 5
#define bufferFull  0xFF


/////////////////  statechart messages //////////////


typedef enum {
	msgFree = 0,
	key,
	zeroAdjust,
	voltage,
	secondTick
}  messageType;



typedef struct  {
	messageType evType;
	union {
		uint32_t keyCode;
		struct {
			float   amps;
			uint16_t  potiPos;
		} zeroAdjustingNVoltageState;
	}  evData;
} mainJtEventT ;

typedef mainJtEventT*  pMainJtEventT;


///////////  presenter messages //////////////

typedef enum {
	changeToRunScreen = 0,
	changeToMainScreen,
	changeToCalibratingScreen,
	changeToRequesStopScreen,
	runScreenSecondUpdate
}  presenterMessageType;


typedef struct  {
	presenterMessageType messageType;
	union {
		uint32_t keyCode;
		struct {
			float   amps;
			uint16_t  potiPos;
			uint16_t  secondsRemaining;
		} secondRunData;
	}  evData;
} CJoesPresenterEventT ;


///////////////  model messages ////////////////






typedef CJoesPresenterEventT*  pJoesPresenterEventT;

extern osMessageQueueId_t    presenterMessageQ;

//void mainJt(void *argument);

void initJt();

osStatus_t sendEventToMainJtMessageQ(pMainJtEventT bufferAddr, uint8_t  fromIsr);

osStatus_t sendPresenterMessage(pJoesPresenterEventT  pMsg);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_MAINJT_H_ */
