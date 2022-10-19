/*
 * defines.c
 *
 *  Created on: Sep 24, 2022
 *      Author: diego
 */

#include <stdio.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

#include <defines.h>
#include <TriacIntr.h>
#include <mainJt.h>
#include <triacPID.h>

//#define  microSdWorking
// despite trying all found examples on google and stm (also for F769Ni)  .... this stm  microSd bu..it did not work



extern float currentAmpsValue;

float getCurrentAmpsValue()
{
	float  res;
	taskENTER_CRITICAL();
	res = currentAmpsValue;
	taskEXIT_CRITICAL();
	return res;
}

void sendActualValuesToRunScreen()
{
	uint32_t ampsI = (uint32_t) ( getCurrentAmpsValue() * 100);
	uint32_t time = getSecondsInDurationTimer();

	CJoesPresenterEventT  presenterMessage;
	presenterMessage.messageType=runScreenUpdate;
	presenterMessage.evData.runScreenData.secondsRemaining= getSecondsDurationTimerRemaining();
	presenterMessage.evData.runScreenData.amps= ampsI;
	presenterMessage.evData.runScreenData.potiPos= 50;

	sendPresenterMessage(&presenterMessage);
}


typedef struct {
	uint32_t weldingTime;
	uint32_t   weldingAmps100;
	uint32_t  calibLow, calibHigh, zeroPotiPos;
	uint8_t  alarmNeeded;
	uint32_t  alarmTime;
} persistentData;

persistentData  persistentRec;

void initPersistendData()
{
	persistentRec.weldingTime =  15 *60;
	persistentRec.weldingAmps100 = 6000;
	persistentRec.calibLow = 0;
	persistentRec.calibHigh = 0;
	persistentRec.alarmNeeded = 1;
	persistentRec.alarmTime   = 12;
}

tStatus savePersistendData()
{
	tStatus success = tFailed;
	success = tOk;
	return success;
}


tStatus restorePersistenData()
{
	tStatus success = tFailed;

	if (success != tOk)  {
		initPersistendData();
		success = tOk;
	}
	return success;
}

uint32_t getDefinesWeldingTime()
{
	uint32_t wTime;
	taskENTER_CRITICAL();
	wTime = persistentRec.weldingTime;
	taskEXIT_CRITICAL();
	return wTime;
}

uint32_t getDefinesWeldingAmps()
{
	uint32_t amps;
	taskENTER_CRITICAL();
	amps = persistentRec.weldingAmps100;
	taskEXIT_CRITICAL();
	return amps;
}

uint8_t getDefinesAlarmNeeded()
{
	uint8_t needed;
	taskENTER_CRITICAL();
	needed = persistentRec.alarmNeeded;
	taskEXIT_CRITICAL();
	return needed;
}

uint32_t getDefinesAlarmTime()
{
	uint32_t aTime;
	taskENTER_CRITICAL();
	aTime = persistentRec.alarmTime;
	taskEXIT_CRITICAL();
	return aTime;
}

uint32_t getDefinesCalibHigh()
{
	uint32_t cal;
	taskENTER_CRITICAL();
	cal = persistentRec.calibHigh;
	taskEXIT_CRITICAL();
	return cal;
}

uint32_t getDefinesCalibLow()
{
	uint32_t cal;
	taskENTER_CRITICAL();
	cal = persistentRec.calibLow;
	taskEXIT_CRITICAL();
	return cal;
}

uint32_t getDefinesZeroPotiPos()
{
	uint32_t val;
	taskENTER_CRITICAL();
	val = persistentRec.zeroPotiPos;
	taskEXIT_CRITICAL();
	return val;
}


#ifdef microSdWorking

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

tStatus saveAlarmData(uint32_t aTime, uint8_t aNeeded)
{
	tStatus success = tFailed;
//	success = restorePersistenData();
	success = tOk;
	if (success ==  tOk) {
		persistentRec.alarmTime = aTime;
		persistentRec.alarmNeeded = aNeeded;
		success = savePersistendData();
	}
	return success;
}

#else

tStatus saveWeldingTime(uint32_t wTime)
{
	tStatus success = tOk;
	persistentRec.weldingTime = wTime;

	return success;
}

tStatus saveWeldingAmps(uint32_t wAmps)
{
	tStatus success = tOk;
	persistentRec.weldingAmps100 = wAmps;

	return success;
}

tStatus saveCalibHigh(uint32_t cHigh)
{
	tStatus success = tOk;
	persistentRec.calibHigh = cHigh;

	return success;
}

tStatus saveCalibLow(uint32_t cLow)
{
	tStatus success = tOk;
	persistentRec.calibLow = cLow;

	return success;
}

tStatus saveAlarmNeeded(uint8_t aNeeded)
{
	tStatus success = tOk;
	persistentRec.alarmNeeded = aNeeded;

	return success;
}


tStatus saveAlarmTime(uint32_t aTime)
{
	tStatus success = tOk;
	persistentRec.alarmTime = aTime;

	return success;
}

tStatus saveAlarmData(uint32_t aTime, uint8_t aNeeded)
{
	tStatus success = tOk;
	persistentRec.alarmTime = aTime;
	persistentRec.alarmNeeded = aNeeded;

	return success;
}

tStatus saveZeroPotiPos(uint32_t val)
{
	tStatus success = tOk;
	persistentRec.zeroPotiPos = val;
	return success;
}
#endif


void errorHandler(uint32_t  code, errorSeverity severity, char* errorString, char* method )
{
	char buffer [100];
	snprintf(buffer, 99, "%s, %s, %10lX, %X", method, errorString,code,severity);
	//  todo log to persistency

	if (severity == stop) {  do {} while (1);}
}

tStatus isCalibrationReady()
{
	tStatus res= tFailed;
	if ((getDefinesCalibHigh() != 0)  && (getDefinesCalibLow() != 0))   {
		res = tOk;
	}
	return res;
}



tStatus initDefines()
{
	tStatus success =  tFailed;
	success = restorePersistenData();

	return success;
}

