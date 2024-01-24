/*
 * extiCheck.c
 *
 *  Created on: Jan 22, 2024
 *      Author: Brigitte
 */


#include <stdlib.h>
#include "TriacIntr.h"
//#include <StateClass.h>
#include <defines.h>
#include <mainJt.h>
#include <extiCheck.h>
#include <TriacIntr.h>

#define extiCheckAmt  4
#define extiCheckDelay  10
#define triacExtiCheckTimer  htim4
#define extiCheckTimerIRQHandler  TIM4_IRQHandler
#define extiCheckTimerIRQn   TIM4_IRQn
#define triacExtiCheckTimerInstance  TIM4
#define  enableExtiCheckTimer()  __HAL_RCC_TIM4_CLK_ENABLE()
#define amtExtiEvChecks   3

uint8_t handleMissed();

TIM_HandleTypeDef triacExtiCheckTimer;



uint32_t uwTickSinceLastOk;
uint32_t amtCountedMissed;
uint32_t lastOkUwTick;
uint32_t syncMissedPeriodStartTick;
uint32_t amtSyncMissed;  //  todo add to astrolabium
uint32_t amtMissed;

uint8_t extiEvTotalCnt;
uint32_t   maxMissedExti;
uint32_t  amtExtiMissedTotal;
uint32_t amtIllegalExti;
uint8_t extiCheckCnt ;


void startHandleMissed()
{
	uwTickSinceLastOk = 10;
	amtExtiMissedTotal = 0;
	maxMissedExti = 0;
	amtCountedMissed = 0;
	lastOkUwTick = uwTick;
	syncMissedPeriodStartTick = 5; //  initialization can only be done by Exti (iE. zeroPass)
	amtMissed = 0;
	extiCheckCnt=0;
	amtSyncMissed = 0;
}


//uint32_t amtSyncMissed()
//{
//	return amtSyncMissedPlusOne -1;
//}

//void resetHandleMissed()
//{
//	uwTickSinceLastOk = 0;
//	lastOkUwTick = uwTick;
//}

void incExtiMissed()
{

}

#define maxInt32  0xFFFFFFFF

#define resetHandleMissed() \
  do { \
	  amtCountedMissed = 0; \
	  lastOkUwTick = uwTick; \
  } while(0)


uint8_t handleMissed()
{
	uint8_t res = 1;

	return res;

	uint8_t currentExtiPinState = isExtiPinSet();

	if (currentExtiPinState == extiZeroPassTriggerStartValue)
	{
		//	if (uwTick >=  lastOkUwTick ) {
				uwTickSinceLastOk = uwTick - lastOkUwTick;
		//	} else {
		//		uwTickSinceLastOk =  uwTick +  ( maxInt32 - lastOkUwTick) + 1;  // overflow start at 0, so needs 1 more for difference
		//	}   //  todo test handle overflow of 32 bit counter
		//		//  or ignore overflow since it will happen first time after 49.710...  days (4294967295 / (1000 * 60 * 60 * 24))

		uint32_t  amtPassed = ((uwTickSinceLastOk +2 )/10 );
		uint32_t  amtMissed = amtPassed - 1;

		 if (amtMissed == 0) {
			 resetHandleMissed();
			 res = 1;
		 }  else  {
			amtExtiMissedTotal +=  amtMissed - amtCountedMissed;
			amtCountedMissed = amtMissed;
			if (amtMissed > maxMissedExti) {maxMissedExti = amtMissed;}

			if (amtPassed & (uint32_t) 0x01) {  //  odd number
				resetHandleMissed();
				res = 1;
			}  else {
				res = 0;    //  prevent short circuit
			}
			if (((uwTick - syncMissedPeriodStartTick ) % 10) >= 2 ) {  //  time difference between measured 10ms (220V) and measured
																	// 10ms (uwTick) > 1ms, should not happen too often. todo make period shorter
				syncMissedPeriodStartTick = uwTick;
				++amtSyncMissed;
			}
		 }
	 }  else {

	 }
	 if (res == 1 ) {
		 syncMissedPeriodStartTick = 0;
	 }
	return res;
}

void initHandleMissed()
{

}

//  zero pass pin irq

uint8_t currentExtiState;

//#define resetExtiTimer() \
//  do { \
//	  amtCountedMissed = 0; \
//	  lastOkUwTick = uwTick; \
//  } while(0)


void stopExtiCheck()
{
	extiCheckCnt = 0;
	triacExtiCheckTimer.Instance->CNT = 0;
	triacExtiCheckTimer.Instance->CR1 &= ~(TIM_CR1_CEN);
}

void startExtiTimer()
{
	currentExtiState=isExtiPinSet();
	extiCheckCnt = 1;
	triacExtiCheckTimer.Instance->CNT = 0;
	triacExtiCheckTimer.Instance->CR1 |= (TIM_CR1_CEN);

}

void startExtiCheck()
{
	uint8_t res = 1;
	++ extiEvTotalCnt;   // maybe later on astrolabium
	if (extiCheckCnt > 0) {
								// an exti happened within short time, probable not valid
								// but maybe a spike within  we loose a valid one.
								// therefor the handleMissed
		stopExtiCheck();
		incExtiMissed();
		res = 0;
	} else {
		if  (currentExtiState == extiZeroPassTriggerStartValue) {
				if 	(((uwTick- syncMissedPeriodStartTick ) % 10) >= 2 ) {
											//  time difference between measured 10ms (220V) and measured	(uwtick)
											// 10ms (uwTick) > 1ms, should not happen too often. todo make shorter
											//  prevent starting outside the time
				++amtSyncMissed;
				res = 0;
			}  else {
				syncMissedPeriodStartTick = uwTick;
			}
		}
		if (res == 1)  {
			startExtiTimer();
		}
	}
}



/*
 zero pass exti events allways have some hundred usec time difference.
 max measured spike time were approx 10 usec so far approx. what makes the max exitCheck duration
 in case of spikes. A zero pass event in this time could cause problems.
 neglect the case where a 0xEvent happend during this spike- extiCheckTime ?
 else handle Missed might help
*/
uint8_t  extiCheckTimerIRQHandler (void)
{
	uint8_t extiPinOk = 0;
	uint8_t res = 0;

	extiPinOk = (currentExtiState == isExtiPinSet());
	if (extiCheckCnt < extiCheckAmt) {
		++ extiCheckCnt;
		if (extiPinOk == 0) {
				stopExtiCheck();
				++amtExtiMissedTotal;
			}
		}  else {
			stopExtiCheck();
			if ((extiPinOk)== 1 ) {
			extiCheckCnt = 0;
			if(handleMissed()) {
				doJobOnZeroPassEvent(currentExtiState);
			}
		}
	}
	return res;
}


void initExtiCheckTimer()
{
	// todo measure period time on oscilloscope and leave possibility to do so

	initHandleMissed();
	amtIllegalExti = 0;
	extiCheckCnt= 0;

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	enableExtiCheckTimer();

	triacExtiCheckTimer.Instance = triacExtiCheckTimerInstance;
	triacExtiCheckTimer.Init.Prescaler = 10;
	triacExtiCheckTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	triacExtiCheckTimer.Init.Period = 20;
	triacExtiCheckTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	triacExtiCheckTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&triacExtiCheckTimer) != HAL_OK)
	{
		errorHandler(1,stop," HAL_TIM_Base_Init ","initTriacTimer");
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&triacExtiCheckTimer, &sClockSourceConfig) != HAL_OK)
	{
		errorHandler(2,stop," HAL_TIM_ConfigClockSource ","initTriacTimer");
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&triacExtiCheckTimer, &sMasterConfig) != HAL_OK)
	{
		errorHandler(3,stop," HAL_TIMEx_MasterConfigSynchronization ","initTriacTimer");
	}

	triacExtiCheckTimer.Instance->CR1 &= (~TIM_CR1_OPM_Msk);
	HAL_NVIC_SetPriority(extiCheckTimerIRQn, triacTriggerIsrPrio, 0);
	HAL_NVIC_EnableIRQ(extiCheckTimerIRQn);
	stopExtiCheck();
}

