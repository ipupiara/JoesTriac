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

//#define debugApp

#define triacApplicationIsrPrio    5
#define triacTriggerIsrPrio   0

#define calibHighAmps  60.0
#define calibLowAmps   20.0

#define eepromI2cAddr   0xA0

#define TABLE_SECTION_SDRAM_PRAGMA LOCATION_PRAGMA("TableSection")

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
	intVar8,
	intVar16
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

uint32_t getDefinesAlarmTime();

uint32_t getDefinesCalibHighAdc();

uint32_t getDefinesCalibLowAdc();

uint8_t getDefinesSwitchPressureNeeded();

uint16_t getDefinesSwitchPressureTime();
tStatus  storeDefinesSwitchPressureData(uint32_t spTime, uint8_t spNeeded);

tStatus saveWeldingTime(uint32_t wTime);
tStatus saveWeldingAmps(float wAmps);

tStatus saveCalibLowAdc(uint32_t cHigh);
tStatus saveCalibHighAdc(uint32_t cLow);

tStatus saveAlarmNeeded(uint8_t aNeeded);
tStatus saveAlarmTime(uint32_t aTime);
tStatus saveAlarmData(uint32_t aTime, uint8_t aNeeded, uint32_t zCalibOn);

void errorHandler(uint32_t  code, errorSeverity severity,const char* errorString, const char* method );

tStatus initDefines();

tStatus isCalibrationReady();

void sendActualValuesToCalibScreen();
void sendActualValuesToRunNStopScreen(uint16_t secondsRemaining,uint16_t secondsb4Return);
void sendActualValuesToJobOkScreen();
void calibTriacDelayChange(int32_t diff);
void storeCalibLowAdc();
void storeCalibHighAdc();

void definesWait(uint32_t ms);

void setCurrentAdcValAsCalibLow();
void setCurrentAdcValAsCalibHigh();

void getDefinesDebugData(uint8_t*  pidP, uint8_t* infoP, uint8_t* exti);

uint8_t getDoExti();
uint8_t getDoInfoPrint();
uint8_t getDoPidPrint();

void setDefinesDebugData(uint8_t  pidP, uint8_t infoP, uint8_t exti);

int32_t i32abs(int32_t val);
#ifdef  __cplusplus
}
#endif

#endif /* APPLICATION_USER_TOOLS_DEFINES_H_ */
