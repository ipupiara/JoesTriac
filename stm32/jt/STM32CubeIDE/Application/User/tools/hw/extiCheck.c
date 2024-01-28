/*
 * extiCheck.c
 *
 *  Created on: Jan 22, 2024
 *      Author: Brigitte
 */


#include <stdlib.h>
#include "TriacIntr.h"
#include <defines.h>
#include <mainJt.h>
#include <extiCheck.h>
#include <TriacIntr.h>

#define extiCheckAmt  4
#define extiCheckDelay  10
#define triacExtiCheckTimer  htim3
#define extiCheckTimerIRQHandler  TIM3_IRQHandler
#define extiCheckTimerIRQn   TIM3_IRQn
#define triacExtiCheckTimerInstance  TIM3
#define  enableExtiCheckTimerClock()  __HAL_RCC_TIM3_CLK_ENABLE()
#define amtExtiEvChecks   3


TIM_HandleTypeDef triacExtiCheckTimer;

uint32_t amtCountedMissed;
uint32_t uwTickWhenLastOk;

//uint32_t syncMissedPeriodStartTick;
uint32_t amtSyncMissed;  //  todo add to astrolabium when ever used
uint32_t amtMissed;

uint8_t extiEvTotalCnt;
uint32_t   maxMissedExti;
uint32_t  amtMissedTotal;
uint32_t amountIllegalExti;
uint8_t extiCheckCnt ;
uint8_t extiStarting;
uint8_t currentExtiState;


void startExtiChecking()
{
	uwTickWhenLastOk = 0;
	amtMissedTotal = 0;
	maxMissedExti = 0;
	amtCountedMissed = 0;

//	syncMissedPeriodStartTick = 5; //  initialization can only be done by Exti (iE. zeroPass)
	amtMissed = 0;
	extiCheckCnt=0;
	amtSyncMissed = 0;
	extiStarting= 1;
}


#define maxInt32  0xFFFFFFFF

#define resetHandleMissed() \
  do { \
	  amtCountedMissed = 0; \
	  uwTickWhenLastOk = uwTick; \
  } while(0)

#define incAmtIllegalExti() \
  do { \
	  amountIllegalExti += 1; \
  } while(0)



// todo do not screw-attach buzzer cable
#define debugExti() \
do { \
	  toggleBuzzer(); \
} while(0)

// for later:
// #define debugExti()

void initHandleMissed()
{

}


uint8_t handleMissed()
{
	uint8_t res = 0;

	uint8_t currentExtiPinState = isExtiPinSet();

	if (currentExtiPinState == extiZeroPassTriggerStartValue) {

		uint32_t  amtPassed = ((uwTickWhenLastOk +1 )/10 );
		uint32_t  amtMissed = amtPassed - 1;

		 if ((amtMissed == 0) ||(extiStarting ==1 )) { // extiStarting last needed here as 0
														// for current run
			 extiStarting = 0;
			 resetHandleMissed();
			 res = 1;
		 }  else  {
			amtMissedTotal += ( amtMissed - amtCountedMissed);
			amtCountedMissed = amtMissed;
			if (amtMissed > maxMissedExti) {maxMissedExti = amtMissed;}

			if (amtPassed & (uint32_t) 0x01) {  //  odd number todo to be tested
				resetHandleMissed();
				res = 1;
			}  else {
				res = 0;    //  prevent short circuit
			}
		 }
	 }  else {
		 res = 1;
	 }
	return res;
}


void stopExtiTimer()
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
//	uint8_t res = 0;
//	uint8_t extiState=isExtiPinSet();

	++ extiEvTotalCnt;   // todo maybe later on astrolabium

	if (extiCheckCnt > 0) {
								// another exti happened within short time, probable not valid
								// but maybe a spike within  we loose a valid one.
								// therefor the handleMissed
		stopExtiTimer();
		incAmtIllegalExti();
	} else {
//		if  (currentExtiState == extiZeroPassTriggerStartValue) {
//				if 	((((uwTick- syncMissedPeriodStartTick  ) % 10) >= 2 ) && (extiStarting != 1)) {
//							//  prevent starting outside this time
//							//  time difference between externally measured 10ms (220V) and internally ones
//							//  (uwTick) > 1ms. todo make shorter duration window.
//				++amtSyncMissed;
//					res = 0;
//			}  else {
//				syncMissedPeriodStartTick = uwTick;
//	//			extiStarting = 1;
//				res = 1;
//			}
//		}  else {
//			res =1;   //  todo check that there is only one stop event, but due to timer
						//testing should not+
//			happen, only let legal events happen
		}
//		if (res == 1)  {
			startExtiTimer();
			debugExti();
//		}
//	}
}



/*
 zero pass exti events allways have some hundred usec time difference.
 max measured spike time were approx 10 usec so far approx. what makes the max exitCheck duration
 in case of spikes. A zero pass event in this time could cause problems.
 neglect the case where a 0xEvent happend during this spike- extiCheckTime ?
 else handle Missed might help
*/
void  extiCheckTimerIRQHandler (void)
{
	uint8_t extiPinOk  = (currentExtiState == isExtiPinSet());
	debugExti();
	if (extiCheckCnt < extiCheckAmt) {
		++ extiCheckCnt;
		if (extiPinOk == 0) {
			stopExtiTimer();
			incAmtIllegalExti();
		}
	}  else {
		stopExtiTimer();
		if ((extiPinOk)== 1 ) {
			if(handleMissed()) {
				doJobOnZeroPassEvent(currentExtiState);
			}
		}
	}
}


void initExtiCheckTimer()
{
	// todo measure period time on oscilloscope and leave possibility to do so

	initHandleMissed();
	amountIllegalExti = 0;
	extiCheckCnt= 0;

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	enableExtiCheckTimerClock();

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
	stopExtiTimer();
}

