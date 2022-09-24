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
#include "cmsis_os.h"
#include <defines.h>
#include <string.h>


#define MUTEX_AQUIRE(HANDLE,TIMEOUT)  osMutexAcquire (HANDLE, TIMEOUT)
#define MUTEX_RELEASE(HANDLE) osMutexRelease (HANDLE)
#define amtMessageBuffers 5
#define bufferFull  0xFF

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
			float   voltage;
			uint16_t  potiPos;
		} zeroAdjustingNVoltageState;
	}  evData;
} CJoesTriacEventT ;

typedef CJoesTriacEventT*  pJoesTriacEventT;


//void mainJt(void *argument);

void initJt();

osStatus_t sendEventToMainJtMessageQ(pJoesTriacEventT bufferAddr, uint8_t  fromIsr);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_MAINJT_H_ */
