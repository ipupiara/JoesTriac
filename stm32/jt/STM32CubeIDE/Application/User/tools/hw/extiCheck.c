/*
 * extiCheck.c
 *
 *  Created on: Jan 22, 2024
 *      Author: Brigitte
 */


#include <stdlib.h>
//#include "TriacIntr.h"
//#include <StateClass.h>
#include <defines.h>
#include <mainJt.h>
#include <extiCheck.h>
#include <TriacIntr.h>

#define extiZeroPassValue 1
#define extiCheckAmt  3
#define extiCheckDelay  10
#define triacExtiCheckTimer  htim4
#define extiCheckTimerIRQHandler  TIM4_IRQHandler
#define extiCheckTimerIRQn   TIM4_IRQn
#define triacExtiCheckTimerInstance  TIM4
#define amtExtiEvChecks   3



TIM_HandleTypeDef triacExtiCheckTimer;
uint8_t extiEvCnt;
uint8_t handleMissed();
uint8_t extiCheckCnt ;
uint32_t amtIllegalExti;
uint32_t uwTickSinceLastOk;
uint32_t lastUwTick;
uint32_t amtCountedMissed;
uint32_t lastOkUwTick;
uint32_t syncMissedPeriodStartTick;
uint32_t amtSyncMissed;  //  todo add to astrolabium

uint32_t amtMissed;
uint32_t   maxMissedExti;
uint32_t  amtExtiMissedTotal;
uint8_t extiEvCnt;


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

#define maxInt32  0xFFFFFFFF

#define resetHandleMissed() \
  do { \
	  amtCountedMissed = 0; \
	  lastOkUwTick = uwTick; \
  } while(0)

#define resetExtiTimer() \
  do { \
	  amtCountedMissed = 0; \
	  lastOkUwTick = uwTick; \
  } while(0)


uint8_t handleMissed()
{
	uint8_t res = 1;

	return res; //  todo needs be tested first and

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
		if ( (isExtiPinSet() == extiZeroPassValue) &&  ((((uwTick - syncMissedPeriodStartTick ) % 10) >= 2 ))) {
			syncMissedPeriodStartTick = uwTick;
		}
	 }
	return res;
}



//  zero pass pin irq

uint8_t currentExtiState;


void stopExtiCheck()
{
	triacExtiCheckTimer.Instance->CR1 &= ~(TIM_CR1_CEN);
}


void startExtiCheck()
{
	currentExtiState=isExtiPinSet();
	triacExtiCheckTimer.Instance->CR1 |= (TIM_CR1_CEN);
	extiCheckCnt = 0;
}



#define  enableExtiCheckTimer()  __HAL_RCC_TIM4_CLK_ENABLE()

//#define startExtiCheck()\
//	do { \
//		extiCheckCnt = 0; \
//		triacExtiCheckTimer.Instance->CR1 &= ~(TIM_CR1_CEN);  \
//	} while(0)
//
//
//
//#define stopExtiCheck()  \
//	do { \
//		currentExtiState=isExtiPinSet(); \
//		extiCheckCnt = 0; \
//		triacExtiCheckTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
//	} while(0)
//
//


void initHandleMissed()
{

}

void initExtiCheckTimer()
{
	initHandleMissed();
	amtIllegalExti = 0;
	extiCheckCnt= 0;

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	enableExtiCheckTimer();

	triacExtiCheckTimer.Instance = triacExtiCheckTimerInstance;
	triacExtiCheckTimer.Init.Prescaler = triacDelayPsc;
	triacExtiCheckTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	triacExtiCheckTimer.Init.Period = 0;
	triacExtiCheckTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	triacExtiCheckTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
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

void  extiCheckTimerIRQHandler (void)
{
	uint8_t extiValid = 0;

	extiValid = (currentExtiState == isExtiPinSet());
	if (extiCheckCnt < extiCheckAmt) {
		++ extiCheckCnt;
		if (extiValid == 0) {
				stopExtiCheck();
				++amtExtiMissedTotal;
			}
		}  else {
			stopExtiCheck();
			if ((extiValid)== 1 ) {
			extiCheckCnt = 0;
			if(handleMissed()) {
				setJobOnZeroPassEvent();
			}
		}
	}

}


//#define extiZeroPassValue 1
//void EXTI15_10_IRQHandler(void)
//{
//	uint8_t res = 0;
//	UNUSED(res);
//	  if(__HAL_GPIO_EXTI_GET_IT(zeroPass_Pin) != 0) {
//		__HAL_GPIO_EXTI_CLEAR_IT(zeroPass_Pin);
//
//		if (extiCheckCnt > 0 )   {
//			stopExtiCheck();
//			res = 1;
//			extiCheckCnt = 0;
//		}  else {
//			startExtiCheck();
//		}
//
////		if (isExtiPinSet() == extiZeroPassValue)   {
////				tim5UsedDelay =	triacDelayTimer.Instance->ARR =getTriacTriggerDelay();
////				triacDelayTimer.Instance->CNT =0;
//////				delayTimerRunState = delayTimerDelayPhase;
////				triacStopTimer.Instance->ARR=stmTriggerDelayMax;
////				triacStopTimer.Instance->CNT = 0;
////				startStopTimer();
////				startDelayTimer();
////				disableRailTimerPwm();
////		}  else  {
////				disableDelayTimer();
////				disableRailTimerPwm();
////		}
//	  }
//}

