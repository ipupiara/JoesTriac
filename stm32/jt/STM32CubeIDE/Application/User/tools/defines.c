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

void sendActualValuesToJobOkScreen()
{
	CJoesPresenterEventT  msg;
	msg.messageType = jobOkScreenUpdate;
	msg.evData.alarmTimeSince = getSecondsInDurationTimer();
	sendPresenterMessage(&msg);
}

void sendActualValuesToCalibScreen()
{
	CJoesPresenterEventT  msg;
	msg.messageType = calibScreenData;
	msg.evData.calibrationScreenData.adcValue = 300;
	msg.evData.calibrationScreenData.adcVolts = 0.68;
	sendPresenterMessage(&msg);
}


void sendActualValuesToRunScreen()
{
	float amps = getCurrentAmpsValue() ;
	uint32_t time = getSecondsDurationTimerRemaining();

	CJoesPresenterEventT  presenterMessage;
	presenterMessage.messageType=runScreenUpdate;
	presenterMessage.evData.runScreenData.secondsRemaining= time;
	presenterMessage.evData.runScreenData.amps= amps;

	sendPresenterMessage(&presenterMessage);
}

void sendActualValuesToRequestStopScreen()
{
	float amps = getCurrentAmpsValue() ;
	uint32_t time = getSecondsDurationTimerRemaining();

	CJoesPresenterEventT  presenterMessage;
	presenterMessage.messageType=runScreenUpdate;
	presenterMessage.evData.runScreenData.secondsRemaining= time;
	presenterMessage.evData.runScreenData.amps= amps;
	presenterMessage.evData.runScreenData.potiPos= 50;

	sendPresenterMessage(&presenterMessage);
}


typedef struct {
	uint32_t weldingTime;
	float   weldingAmps;
	uint32_t  calibLow, calibHigh, zeroPotiPos;
	uint8_t  alarmNeeded;
	uint8_t  zCalibOn;
	uint32_t  alarmTime;
} persistentData;

persistentData  persistentRec;

void initPersistendData()
{
	persistentRec.weldingTime =  15 *60;
	persistentRec.weldingAmps = 60.00;
	persistentRec.calibLow = 0;
	persistentRec.calibHigh = 0;
	persistentRec.alarmNeeded = 1;
	persistentRec.alarmTime   = 12;
	persistentRec.zCalibOn = 0;
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

float getDefinesWeldingAmps()
{
	uint32_t amps;
	taskENTER_CRITICAL();
	amps = persistentRec.weldingAmps;
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

uint32_t getDefinesZCalibOn()
{
	uint32_t res;
	taskENTER_CRITICAL();
	res = persistentRec.zCalibOn;
	taskEXIT_CRITICAL();
	return res;
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

tStatus saveWeldingAmps(float wAmps)
{
	tStatus success = tOk;
	persistentRec.weldingAmps = wAmps;

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

tStatus saveZCalibOn(uint32_t val)
{
	tStatus success = tOk;
	persistentRec.zCalibOn = val;

	return success;
}

tStatus saveAlarmData(uint32_t aTime, uint8_t aNeeded, uint32_t zCalibOn)
{
	tStatus success = tOk;
	persistentRec.alarmTime = aTime;
	persistentRec.alarmNeeded = aNeeded;
	persistentRec.zCalibOn = (uint8_t) zCalibOn;

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

uint32_t  triacTriggerDelay;

void addToTriggerDelay(int32_t val)
{
	uint32_t res;
	if ((res = (triacTriggerDelay + val)) < 1000) {
		triacTriggerDelay = res;
	}  else  {
		triacTriggerDelay = 1000;
	}
}

void storeCalibLowAdc()
{
	saveCalibLow(100); //  for debugging only
}

void storeCalibHighAdc()
{
	saveCalibHigh(500);
}

void calibTriacDelayChange(int32_t diff)
{
   //   change variable

	CJoesPresenterEventT msg;

	addToTriggerDelay(diff);

	msg.messageType = calibTriacDelay;
	msg.evData.calibTriacDelay = triacTriggerDelay;
	sendPresenterMessage(&msg);
}

tStatus isCalibrationReady()
{
	tStatus res= tFailed;
	if ((getDefinesCalibHigh() != 0)  && (getDefinesCalibLow() != 0))   {
		res = tOk;
	}
	return res;
}

void setZCalibAuto(uint32_t on)
{

}

tStatus initDefines()
{
	triacTriggerDelay = 0;
	tStatus success =  tFailed;
	success = restorePersistenData();

	return success;
}



