/*}
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


#define MUTEX_AQUIRE(HANDLE,TIMEOUT)  osMutexAcquire (HANDLE, TIMEOUT)
#define MUTEX_RELEASE(HANDLE) osMutexRelease (HANDLE)

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

void mainJt(void *argument);

void initJt();

uint32_t getNextFreeMessageBufferP(messageType forMessage);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_MAINJT_H_ */
