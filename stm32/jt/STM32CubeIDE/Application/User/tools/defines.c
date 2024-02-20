/*
 * defines.c
 *
 *  Created on: Sep 24, 2022
 *      Author: diego
 */

#include <stdArg.h>
//#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "cmsis_os.h"
#include <defines.h>
#include <TriacIntr.h>
#include <mainJt.h>
#include <triacPID.h>
#include <i2c.h>
#include <extiCheck.h>
#include <triacControl.h>
#include <uart-comms.h>

//#define  microSdWorking
// despite trying all found examples on google and stm (also for F769Ni)  .... this stm  microSd bu..it did not work

osSemaphoreId_t   waitSema;


void definesWait(uint32_t ms)
{
	osStatus_t status;
	status = osSemaphoreAcquire(waitSema, ms);
	if (status  != osErrorTimeout )  {
		errorHandler(status,goOn," status ","definesWait1");
	}  else  {
//		errorHandler(status,goOn," status ","definesWait2");
	}
}

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
	msg.messageType = calibrationScreenUpdate;
	msg.evData.calibrationScreenData.adcValue = getCurrentAmpsADCValue();
	msg.evData.calibrationScreenData.adcVolts = adcVoltage();
	msg.evData.calibrationScreenData.calibTriacDly = getTriacTriggerDelay();
	sendPresenterMessage(&msg);
}


void sendActualValuesToRunNStopScreen(uint16_t secondsRemaining, uint16_t secondsb4Return)
{
	float amps = getCurrentAmpsValue() ;
	uint32_t time = getSecondsDurationTimerRemaining();

	CJoesPresenterEventT  presenterMessage;
	presenterMessage.messageType=runScreenUpdate;
	presenterMessage.evData.runScreenData.secondsRemaining= time;
	presenterMessage.evData.runScreenData.amps= amps;
	presenterMessage.evData.runScreenData.secondsBeforeReturn = secondsb4Return;
	presenterMessage.evData.runScreenData.adcValue = getCurrentAmpsADCValue();
	presenterMessage.evData.runScreenData.adcVoltage = adcVoltage();
	presenterMessage.evData.runScreenData.triacDelay = getTriacTriggerDelay();
	presenterMessage.evData.runScreenData.amtMissZp =  amtMissedZpTotal;
	presenterMessage.evData.runScreenData.maxMissZp = maxMissedZp;
	presenterMessage.evData.runScreenData.amtIllExti= amountIllegalExti;
	presenterMessage.evData.runScreenData.extiEvTotal = amtExtiEvTotal;
	presenterMessage.evData.runScreenData.amtWrongSyn = amtWrongSync;
	presenterMessage.evData.runScreenData.amtSeqErr = amtExtiSequenceError;
	sendPresenterMessage(&presenterMessage);
}


typedef struct {
	uint32_t weldingTime;
	float   weldingAmps;
	uint32_t  calibLowAdc, calibHighAdc, zeroPotiPos;
	uint8_t  alarmNeeded;
	uint8_t  zCalibOn;
	uint32_t  alarmTime;
	uint8_t switchPressureNeeded;
	uint16_t switchPressureTime;
} persistentData;

persistentData  persistentRec;

void initPersistendData()
{
	persistentRec.weldingTime =  15 *60;
	persistentRec.weldingAmps = 60.00;
	persistentRec.calibLowAdc = 0;
	persistentRec.calibHighAdc = 0;
	persistentRec.alarmNeeded = 1;
	persistentRec.alarmTime   = 12;
	persistentRec.zCalibOn = 0;
	persistentRec.switchPressureNeeded = 0;
	persistentRec.switchPressureTime = 0;
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
	float amps;
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

uint32_t getDefinesCalibHighAdc()
{
	uint32_t cal;
	taskENTER_CRITICAL();
	cal = persistentRec.calibHighAdc;
	taskEXIT_CRITICAL();
	return cal;
}

uint32_t getDefinesCalibLowAdc()
{
	uint32_t cal;
	taskENTER_CRITICAL();
	cal = persistentRec.calibLowAdc;
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

uint8_t getDefinesSwitchPressureNeeded()
{
	uint8_t val;
	taskENTER_CRITICAL();
	val = persistentRec.switchPressureNeeded;
	taskEXIT_CRITICAL();
	return val;
}

uint16_t getDefinesSwitchPressureTime()
{
	uint16_t val;
	taskENTER_CRITICAL();
	val = persistentRec.switchPressureTime;
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

tStatus saveCalibHighAdc(uint32_t cHigh)
{
	tStatus success = tFailed;
	success = restorePersistenData();
	if (success ==  tOk) {
		persistentRec.calibHighAdc = cHigh;
		success = savePersistendData();
	}
	return success;
}

tStatus saveCalibLowAdc(uint32_t cLow)
{
	tStatus success = tFailed;
	success = restorePersistenData();
	if (success ==  tOk) {
		persistentRec.calibLowAdc = cLow;
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

#define amountPersistentValues  8

#define wTimePos  0
#define wAmpsPos wTimePos+ 4
#define cLowPos  wTimePos+ 8
#define cHighPos wTimePos+ 12
#define zPotiPos wTimePos+ 16
#define aNeededPos wTimePos+ 20
#define zOnPos wTimePos+ 24
#define aTimePos  wTimePos+ 28
#define spTimePos wTimePos + 32
#define spNeededPos wTimePos + 36

#define amtPersistentVariables  10
varData  variableData  [amtPersistentVariables]= {
		{(void *)(&persistentRec.weldingTime), intVar32, wTimePos,sizeof(persistentRec.weldingTime)},
		{(void *)(&persistentRec.weldingAmps), realVar,wAmpsPos, sizeof(persistentRec.weldingAmps)},
		{(void *)(&persistentRec.calibLowAdc), intVar32, cLowPos, sizeof(persistentRec.calibLowAdc)},
		{(void *)(&persistentRec.calibHighAdc), intVar32, cHighPos, sizeof(persistentRec.calibHighAdc)},
		{(void *)(&persistentRec.zeroPotiPos), intVar32, zPotiPos, sizeof(persistentRec.zeroPotiPos)},
		{(void *)(&persistentRec.zCalibOn), intVar8, zOnPos, sizeof(persistentRec.zCalibOn)},
		{(void *)(&persistentRec.alarmNeeded), intVar8, aNeededPos, sizeof(persistentRec.alarmNeeded)},
		{(void *)(&persistentRec.alarmTime), intVar32, aTimePos, sizeof(persistentRec.alarmTime)},
		{(void *)(&persistentRec.switchPressureNeeded), intVar8, spNeededPos, sizeof(persistentRec.switchPressureNeeded)},
		{(void *)(&persistentRec.switchPressureTime), intVar32, spTimePos, sizeof(persistentRec.switchPressureTime)}};


void eepromSave(pVarData pVD)
{
	uint8_t  buff [10];
	memset (buff,0,sizeof(buff));
	buff[0]= pVD->EepromPos;
	memcpy(buff,&pVD->EepromPos,1);  //  *(uint32_t *)(pVD->pValue)  returns value
	memcpy(&buff[1],pVD->pValue,pVD->EepromLen);
	sendI2cByteArray(eepromI2cAddr,(uint8_t*) buff  ,pVD->EepromLen +1);
	definesWait(2);
}

void eepromRestore(pVarData pVD)
{
	uint8_t  buff [10];
	memset (buff,0,sizeof(buff));
	buff[0]= pVD->EepromPos;
	sendI2cByteArray(eepromI2cAddr,&buff[0],1);
	memset (buff,0,sizeof(buff));
	definesWait(2);
	receiveI2cByteArray(eepromI2cAddr,buff,pVD->EepromLen);
	definesWait(2);
	memcpy(pVD->pValue,buff,pVD->EepromLen);
	definesWait(1);  // just to place a reliable debug halt
}

tStatus restoreAll()
{
	tStatus success = tOk;
	uint8_t cnt;
	for (cnt=0; cnt < amountPersistentValues;++cnt)  {
		eepromRestore(&variableData[cnt]);
	}
	return success;
}

tStatus saveAll()
{
	tStatus success = tOk;
	uint8_t cnt;
	for (cnt=0; cnt < amountPersistentValues;++cnt)  {
		eepromSave(&variableData[cnt]);
	}
	return success;
}

tStatus saveWeldingTime(uint32_t wTime)
{
	tStatus success = tOk;
	persistentRec.weldingTime = wTime;
	eepromSave(&variableData[0]);

	return success;
}

tStatus saveWeldingAmps(float wAmps)
{
	tStatus success = tOk;
	persistentRec.weldingAmps = wAmps;
	eepromSave(&variableData[1]);
	return success;
}

tStatus saveCalibLowAdc(uint32_t cLow)
{
	tStatus success = tOk;
	persistentRec.calibLowAdc = cLow;
	eepromSave(&variableData[2]);
	return success;
}

tStatus saveCalibHighAdc(uint32_t cHigh)
{
	tStatus success = tOk;
	persistentRec.calibHighAdc = cHigh;
	eepromSave(&variableData[3]);
	return success;
}

tStatus saveZeroPotiPos(uint32_t val)
{
	tStatus success = tOk;
	persistentRec.zeroPotiPos = val;
	eepromSave(&variableData[4]);
	return success;
}

tStatus saveZCalibOn(uint32_t val)
{
	tStatus success = tOk;
	persistentRec.zCalibOn = val;
	eepromSave(&variableData[5]);
	return success;
}

tStatus saveAlarmNeeded(uint8_t aNeeded)
{
	tStatus success = tOk;
	persistentRec.alarmNeeded = aNeeded;
	eepromSave(&variableData[6]);
	return success;
}

tStatus saveAlarmTime(uint32_t aTime)
{
	tStatus success = tOk;
	persistentRec.alarmTime = aTime;
	eepromSave(&variableData[7]);
	return success;
}

tStatus saveSwitchPressureNeeded(uint8_t aNeeded)
{
	tStatus success = tOk;
	persistentRec.switchPressureNeeded = aNeeded;
	eepromSave(&variableData[8]);
	return success;
}

tStatus	 saveSwitchPressureTime(uint32_t aTime)
{
	tStatus success = tOk;
	persistentRec.switchPressureTime = aTime;
	eepromSave(&variableData[9]);
	return success;
}


tStatus saveAlarmData(uint32_t aTime, uint8_t aNeeded, uint32_t zCalibOn)
{
	tStatus success = tOk;
	success= saveZCalibOn(zCalibOn);
	success= saveAlarmNeeded(aNeeded);
	success= saveAlarmTime(aTime);
	return success;
}

tStatus storeDefinesSwitchPressureData(uint32_t spTime, uint8_t spNeeded)
{
	tStatus success = tOk;
	success= saveSwitchPressureNeeded(spNeeded);
	success= saveSwitchPressureTime(spTime);
	return success;
}


tStatus savePersistendData()
{
	tStatus success = tFailed;
	success = saveAll();
	return success;
}


tStatus restorePersistenData()
{
	tStatus success = tFailed;
	success = restoreAll();
	if (success != tOk)  {
		initPersistendData();
		success = tOk;
	}
	return success;
}

#endif


void errorHandler(uint32_t  code, errorSeverity severity, char* errorString, char* method )
{
//	char buffer [100];
//	err_printf(buffer, 99, "%s, %s, %10lX, %X", method, errorString,code,severity);
	//  todo log to persistency

	if (severity == stop) {  do {} while (1);}
}

int32_t i32abs(int32_t val)
{
	int32_t res = val;
	if (res < 0) { res = -res; }
	return res;
}

void calibTriacDelayChange(int32_t diff)
{
//	printf("calibTriacDelayChange\n");
   int32_t res = getTriacTriggerDelay();
   if ( i32abs (diff) != 1)  {
	   res += diff * kStepUnitsFactor;   //  todo change also on ui
   } else  {
	   res += diff;
   }
   setTriacTriggerDelay(res);
   res =  getTriacTriggerDelay();

   info_printf("calibTriacDelayChange %4d \n", res);
   CJoesPresenterEventT msg;
	msg.messageType = calibTriacDelay;
	msg.evData.calibTriacDelay =res;
	sendPresenterMessage(&msg);

//	printf("calibTriacDelayChange\n");
}

tStatus isCalibrationReady()
{
	tStatus res= tFailed;
	if ((getDefinesCalibHighAdc() != 0)  && (getDefinesCalibLowAdc() != 0))   {
		res = tOk;
	}
	return res;
}

void setCurrentAdcValAsCalibLow()
{
	uint32_t cL;
	cL = getCurrentAmpsADCValue();
	saveCalibLowAdc(cL);
}

void setCurrentAdcValAsCalibHigh()
{
	uint32_t cL;
	cL = getCurrentAmpsADCValue();
	saveCalibHighAdc(cL);
}


tStatus initDefines()
{
	osSemaphoreDef_t  waitSemaphoreDef;
	waitSemaphoreDef.name="i2c send sema"  ;
	waitSemaphoreDef.attr_bits= 0;
	waitSemaphoreDef.cb_mem = NULL;
	waitSemaphoreDef.cb_size = 0;
	waitSema=  osSemaphoreNew(1,0,&waitSemaphoreDef);

	tStatus success =  tFailed;
	success = restorePersistenData();

	return success;
}



