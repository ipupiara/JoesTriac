/*
 * defines.h
 *
 *  Created on: Sep 24, 2022
 *      Author: diego
 */
#ifndef APPLICATION_USER_TOOLS_DEFINES_H_
#define APPLICATION_USER_TOOLS_DEFINES_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include <stdio.h>

//#define debugTriac

#define triacApplicationIsrPrio    5
#define triacTriggerIsrPrio   0

#define calibHighAmps  80.0
#define calibLowAmps   30.0
#define avrTriggerDelayMaxTcnt  810

#define errorStringLen  20
#define eepromI2cAddr   0xA0

typedef enum{
	goOn,
	stop
} errorSeverity;

typedef enum  {
	tOk,
	tFailed,

} tStatus ;

typedef enum  {
	realVar,
	intVar32,
	intVar8
} varType;

typedef struct  {
	void* pValue;
	varType vType;
	uint8_t EepromPos;
	uint8_t EepromLen;
} varData;

typedef varData* pVarData;


uint32_t getDefinesWeldingTime();

float getDefinesWeldingAmps();

uint8_t getDefinesAlarmNeeded();

uint32_t getDefinesZCalibOn();

uint32_t getDefinesAlarmTime();

uint32_t getDefinesCalibHighAdc();

uint32_t getDefinesCalibLowAdc();

uint32_t getDefinesZeroPotiPos();

uint8_t getDefinesSwitchPressureNeeded();

uint16_t getDefinesSwitchPressureTime();
tStatus  storeDefinesSwitchPressureData(uint32_t spTime, uint8_t spNeeded);

tStatus saveWeldingTime(uint32_t wTime);
tStatus saveWeldingAmps(float wAmps);

tStatus saveCalibLowAdc(uint32_t cHigh);
tStatus saveCalibHighAdc(uint32_t cLow);
tStatus saveZeroPotiPos(uint32_t val);

tStatus saveAlarmNeeded(uint8_t aNeeded);
tStatus saveAlarmTime(uint32_t aTime);
tStatus saveZCalibOn(uint32_t val);
tStatus saveAlarmData(uint32_t aTime, uint8_t aNeeded, uint32_t zCalibOn);

void errorHandler(uint32_t  code, errorSeverity severity, char* errorString, char* method );

tStatus initDefines();

tStatus isCalibrationReady();
void setZCalibAuto(uint32_t on);

void sendActualValuesToCalibScreen();
void sendActualValuesToRunNStopScreen(uint16_t secondsRemaining,uint16_t secondsb4Return);
void sendActualValuesToJobOkScreen();
void calibTriacDelayChange(int32_t diff);
void storeCalibLowAdc();
void storeCalibHighAdc();

void definesWait(uint32_t ms);

void setCurrentAdcValAsCalibLow();
void setCurrentAdcValAsCalibHigh();
#ifdef  __cplusplus
}
#endif

#endif /* APPLICATION_USER_TOOLS_DEFINES_H_ */
