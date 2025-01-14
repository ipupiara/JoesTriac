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
#include <defines.h>
#include <string.h>
#include "stm32f7xx_hal.h"

//#ifndef debugApp   // not yet completely implemented to exclude these libs
#include "cmsis_os.h"
#include <task.h>
#include <FreeRTOS.h>
#include <triacPid.h>
#include <canComms.h>
//#endif




#define MUTEX_AQUIRE(HANDLE,TIMEOUT)  osMutexAcquire (HANDLE, TIMEOUT)
#define MUTEX_RELEASE(HANDLE) osMutexRelease (HANDLE)
#define amtMessageBuffers 5
#define bufferFull  0xFF


/////////////////  mainJt messages //////////////


typedef enum {
	msgFree = 0,
	saveCalibLo,
	saveCalibHi,
	saveZPotiPos,
	zCalibAuto,
	calibTriacDelayDelta,
	calibAbortClick,
	calibContinueClick,
	calibSkipClick,
	calibScreenReady,
	configBackPressed,
	storeWeldingTime,
	storeWeldingAmpere,
	storeAlarmData,
	secondTick,
	second100Tick,
	adcTick,
	configPressed,
	runButtonClicked,
	stopButtonClicked,
	continueButtonClicked,
	okPressed,
	autoConfigPressed,
	pidGraphInitializing,
	storeDebugData,
	redrawButtonPressed,
}  mainJtMessageType;

typedef enum {
	isFromIsr,
	isNotFromIsr
} fromIsrParameterType;

typedef struct  {
	mainJtMessageType evType;
	union {
		uint32_t keyCode;
		uint32_t zAuto;
		int32_t calibTriDelayCorrection;
		float   weldingAmps;
		uint32_t  weldingTime;
		uint32_t  calibLow;
		uint32_t calibHigh;
		uint32_t  zPotiPos;
		uint16_t  advV;
		struct {
			uint32_t     alarmTime;
			uint8_t		alarmNeeded;
		} alarmData;
		struct {
			uint8_t     pidPri;
			uint8_t		infoPri;
			uint8_t    extiDo;
		} debugData;
	} mainUnion;

} CMainJtEventT ;

typedef CMainJtEventT*  pMainJtEventT;

osStatus_t sendEventToMainJtMessageQ(pMainJtEventT bufferAddr, fromIsrParameterType  fromIsr);

extern osMessageQueueId_t    mainJtMessageQ;

///////////  presenter messages //////////////

typedef enum {
	calibState,
	calibTriacDelay,
	calibDesiredAmps,
	calibrateZeroPPScreenUpdate,
	calibrationScreenUpdate,
	jobOkScreenUpdate,
	runScreenUpdate,
	paintPidGraph,
	pidGraphFromData,
	requestStop,
	canDebugData,
	doRun
}  presenterMessageType;


typedef struct  {
	presenterMessageType messageType;
	union {
		uint32_t calibTriacDelay;
		float    desiredAmps;
		uint32_t  alarmTimeSince;
		struct {
			float   	amps;
			uint32_t  secondsRemaining;
			uint32_t secondsBeforeReturn;
			int32_t triacDelay;
			int32_t adcValue;
			float adcVoltage;
			uint32_t  amtMissZp;
			uint32_t  maxMissZp;
			uint32_t  amtIllExti;
			uint32_t  amtWrongSyn;
			uint32_t extiEvTotal;
			uint32_t amtSeqErr;
		} runScreenData;
		struct {
			float   	 devV;
			uint32_t  potiPos;
		} calibZeroPPScreenData;
		struct {
			float     adcVolts;
			uint32_t  adcValue;
			uint32_t calibTriacDly;
		} calibrationScreenData;
		struct {
			float ampsF;
			float goalF;
		} pidGraphData;
		struct {
			uint16_t  msgId;
			uint32_t  amtReceived;
			uint32_t  amtErrors;
		} canDebugScreenData ;
		graphDataRec* pidGraphDataArrayPtr;
	}  evData;
} CJoesPresenterEventT ;

typedef CJoesPresenterEventT*  pJoesPresenterEventT;

osStatus_t sendPresenterMessage(pJoesPresenterEventT  pMsg);

void setPresenterQActive();

void setPresenterQInactive();

uint8_t isPresenterQActive();

extern osMessageQueueId_t    presenterMessageQ;

///////////////  model messages ////////////////

typedef enum {
	changeToRunScreen = 0,
	changeToRequestStopScreen,
	changeToMainScreen,
	changeToConfigScreen,
	changeToCalibrateZeroScreen,
	changeToRequesStopScreen,
	changeToJobOkScreen,
	changeToCalibrationScreen,
	restoreModelData,
	pidPrint
}  modelMessageType;


typedef struct  {
	modelMessageType messageType;
	union {
		uint32_t keyCode;
//		struct {
//			float   amps;
//			uint16_t  potiPos;
//			uint16_t  secondsRemaining;
//		} secondRunData;
		struct {
			uint32_t triDelay;
			uint32_t triAdc;
			uint32_t triCorrInt;
			float  ampsV;
			float  Vpa, Vin, Vde;
			doPidAndPrint pidAndPrintBool;
		} pidPrintData;
	}  evData;
} CJoesModelEventT ;

typedef CJoesModelEventT*  pJoesModelEventT;

osStatus_t sendModelMessage(pJoesModelEventT  pMsg);

extern osMessageQueueId_t    modelMessageQ;

//////////////////  end Messages  ///////////////////

void startSec100Timer();
void stopSec100Timer();

void initJt();


#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_MAINJT_H_ */
