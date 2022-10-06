/*
 * defines.c
 *
 *  Created on: Sep 24, 2022
 *      Author: diego
 */

#include <stdio.h>
#include <stdint.h>
#include <defines.h>


persistentData  persistentRec;

void initPersistendData()
{
	persistentRec.weldingTime =  15 *60;
	persistentRec.weldingAmps = 60;
	persistentRec.calibLow = 0;
	persistentRec.calibHigh = 0;
	persistentRec.alarmNeeded = 1;
	persistentRec.alarmTime   = 10 * 60;
}

tStatus savePersistendData()
{
	tStatus success = tFailed;

	return success;
}

tStatus restorePersistenData()
{
	tStatus success = tFailed;

	return success;
}

tStatus saveWeldingTime(uint32_t wTime)
{
	tStatus success = tFailed;
	success = restorePersistenData();
	if (success ==  tOk) {
		persistentRec.weldingTime = wTime;
		success = savePersistendData();
	}
	return success;
}

tStatus saveWeldingAmps(float wAmps)
{
	tStatus success = tFailed;
	success = restorePersistenData();
	if (success ==  tOk) {
		persistentRec.weldingAmps = wAmps;
		success = savePersistendData();
	}
	return success;
}

tStatus saveCalibHigh(uint32_t cHigh)
{
	tStatus success = tFailed;
	success = restorePersistenData();
	if (success ==  tOk) {
		persistentRec.calibHigh = cHigh;
		success = savePersistendData();
	}
	return success;
}

tStatus saveCalibLow(uint32_t cLow)
{
	tStatus success = tFailed;
	success = restorePersistenData();
	if (success ==  tOk) {
		persistentRec.calibLow = cLow;
		success = savePersistendData();
	}
	return success;
}

tStatus saveAlarmNeeded(uint8_t aNeeded)
{
	tStatus success = tFailed;
	success = restorePersistenData();
	if (success ==  tOk) {
		persistentRec.alarmNeeded = aNeeded;
		success = savePersistendData();
	}
	return success;
}


tStatus saveAlarmTime(uint32_t aTime)
{
	tStatus success = tFailed;
	success = restorePersistenData();
	if (success ==  tOk) {
		persistentRec.alarmTime = aTime;
		success = savePersistendData();
	}
	return success;
}



void errorHandler(uint32_t  code, errorSeverity severity, char* errorString, char* method )
{
	char buffer [100];
	snprintf(buffer, 99, "%s, %s, %10lX, %X", method, errorString,code,severity);
	//  todo log to peristency

	if (severity == stop) {  do {} while (1);}
}

tStatus isCalibrationReady()
{
	tStatus res= tOk;

	return res;
}

tStatus initDefines()
{
	tStatus success =  tFailed;
	success = restorePersistenData();
	if (success == tOk) {

	}
	return success;
}

