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

osStatus_t sendEventToMainJtMessageQ(pMainJtEventT bufferAddr, uint8_t  fromIsr);

extern osMessageQueueId_t    mainJtMessageQ;

///////////  presenter messages //////////////

typedef enum {
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

typedef CJoesPresenterEventT*  pJoesPresenterEventT;

osStatus_t sendPresenterMessage(pJoesPresenterEventT  pMsg);

extern osMessageQueueId_t    presenterMessageQ;

///////////////  model messages ////////////////

typedef enum {
	changeToRunScreen = 0,
	changeToMainScreen,
	changeToCalibratingScreen,
	changeToRequesStopScreen
}  modelMessageType;


typedef struct  {
	modelMessageType messageType;
	union {
		uint32_t keyCode;
		struct {
			float   amps;
			uint16_t  potiPos;
			uint16_t  secondsRemaining;
		} secondRunData;
	}  evData;
} CJoesModelEventT ;

typedef CJoesModelEventT*  pJoesModelEventT;

osStatus_t sendModelMessage(pJoesModelEventT  pMsg);

extern osMessageQueueId_t    modelMessageQ;

//////////////////  end Messages  ///////////////////


void initJt();


#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_MAINJT_H_ */
