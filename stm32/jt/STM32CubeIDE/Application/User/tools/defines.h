/*
 * defines.h
 *
 *  Created on: Sep 24, 2022
 *      Author: diego
 */

#include <stdio.h>


#ifndef APPLICATION_USER_TOOLS_DEFINES_H_
#define APPLICATION_USER_TOOLS_DEFINES_H_

#define errorStringLen  20

typedef enum{
	goOn,
	stop
} errorSeverity;

typedef enum  {
	tOk,
	tFailed,

} tStatus ;

typedef struct {
	uint32_t weldingTime;
	float    weldingAmps;
	uint32_t  calibLow, calibHigh;
	uint8_t  alarmNeeded;
	uint32_t  alarmTime;
} persistentData;

#ifdef  __cplusplus
extern "C"
{
#endif

tStatus saveWeldingTime(uint32_t wTime);

tStatus saveWeldingAmps(float wAmps);

tStatus saveCalibLow(uint32_t cHigh);

tStatus saveCalibHigh(uint32_t cLow);

tStatus saveAlarmNeeded(uint8_t aNeeded);

tStatus saveWeldingTime(uint32_t aTime);



void errorHandler(uint32_t  code, errorSeverity severity, char* errorString, char* method );

tStatus initDefines();

#ifdef  __cplusplus
}
#endif

#endif /* APPLICATION_USER_TOOLS_DEFINES_H_ */
