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
#include <triacControl.h>

#define extiCheckAmt  4
#define extiCheckDelay  10
#define triacExtiCheckTimer  htim3
#define extiCheckTimerIRQHandler  TIM3_IRQHandler
#define extiCheckTimerIRQn   TIM3_IRQn
#define triacExtiCheckTimerInstance  TIM3
#define  enableExtiCheckTimerClock()  __HAL_RCC_TIM3_CLK_ENABLE()
#define amtExtiEvChecks   3
#define msTick  debugTick //  uwTick

uint32_t debugTick;


TIM_HandleTypeDef triacExtiCheckTimer;

// external variables
uint32_t   maxMissedZp;
uint32_t  amtMissedZpTotal;
uint32_t amountIllegalExti;
uint32_t amtExtiEvTotal;
uint32_t amtWrongSync;  //  todo add to astrolabium when ever used



//  internal variables
uint32_t syncMissedPeriodStartTick;
uint32_t  uwTickWhenLastOk;
uint32_t amtCountedMissed;
uint8_t extiCheckCnt ;
uint8_t extiStarting;
uint8_t currentExtiState;
uint32_t amtMissed;
uint32_t extiStateBefore;
uint32_t amtExtiSequenceError;

void startExtiChecking()
{
	uwTickWhenLastOk = 0;
	amtMissedZpTotal = 0;
	maxMissedZp = 0;
	amtCountedMissed = 0;
	syncMissedPeriodStartTick = 0;
	amtMissed = 0;
	extiCheckCnt=0;
	amtWrongSync = 0;
	amtExtiSequenceError = 0;
	extiStarting = 1;
}


#define maxInt32  0xFFFFFFFF

#define resetHandleMissed() \
  do { \
	  amtCountedMissed = 0; \
	  uwTickWhenLastOk = msTick; \
  } while(0)

#define incAmtIllegalExti() \
  do { \
	  amountIllegalExti += 1; \
  } while(0)



// todo do not screw-attach buzzer cable to buzzer
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

		uint32_t  amtPassed = ((msTick - uwTickWhenLastOk + 2 )/10 );   // we assume that never more than 2 events can get missed,
																		// two just outside the border, due to slightly different clock speeds.
																		// not very likely, but also not impossible. anyhow amtPassed value will be correct.
																		// later we need a better clock (cpu clock counter or timer)
		uint32_t  amtMissed = amtPassed - 1;

		 if ((amtMissed == 0) ||(extiStarting ==1 )) {
			 extiStarting = 0;         // extiStarting last needed here as 1 for current run
			 res = 1;
		 }  else  {
			amtMissedZpTotal += ( amtMissed - amtCountedMissed);
			amtCountedMissed = amtMissed;
			if (amtMissed > maxMissedZp) {maxMissedZp= amtMissed;}

			if (amtPassed & (uint32_t) 0x01) {  //  odd number todo to be tested
				res = 1;
			}  else {
				res = 0;    //  prevent short circuit
			}
		 }
	 }  else {
		 res = 1;
	 }
	if (res == 1) {
		doJobOnZeroPassEvent(currentExtiPinState);
		resetHandleMissed();
	}
	return res;
}

void stopExtiTimer()
{
	triacExtiCheckTimer.Instance->CNT = 0;
	__HAL_TIM_DISABLE_IT(&triacExtiCheckTimer, TIM_IT_UPDATE);
	triacExtiCheckTimer.Instance->CR1 &= ~(TIM_CR1_CEN);
}

void startExtiTimer()
{
	triacExtiCheckTimer.Instance->CNT = 0;
	__HAL_TIM_ENABLE_IT(&triacExtiCheckTimer, TIM_IT_UPDATE);
	triacExtiCheckTimer.Instance->CR1 |= (TIM_CR1_CEN);
}

void startExtiCheck()
{
	uint8_t res = 0;
//	uint8_t extiState=isExtiPinSet();

	++ amtExtiEvTotal;   // todo maybe later on astrolabium

	if (extiCheckCnt > 0) {
								// another exti happened within short time, both are probable not valid
								// but maybe a spike  just before a valid one, then we loose the valid one.
								// gives a handleMissed
		stopExtiTimer();
		incAmtIllegalExti();
		res = 0;
	} else {
//		if  (currentExtiState == extiZeroPassTriggerStartValue) {
//				if 	((((msTick- syncMissedPeriodStartTick  ) % 10) >= 2 ) && (extiStarting != 1)) {
//							//  prevent starting outside this time
//							//  time difference between externally measured 10ms (220V) and internally ones
//							//  (msTick) > 1ms. todo make shorter duration window.
//				++amtWrongExti;
//					res = 0;
//			}  else {
//				syncMissedPeriodStartTick = msTick;
//	//			extiStarting = 1;
//				res = 1;
//			}
//		}  else {
			currentExtiState=isExtiPinSet();
			if (extiStarting)  {
				extiStateBefore = currentExtiState;
			}  else  {
				if (extiStateBefore == currentExtiState)  {
					++ amtExtiSequenceError;
					extiStateBefore = currentExtiState;
				}
			}
		}
//	}

	if (res == 1)  {        // one side is always stable, but within this short time is probable a spike return
		currentExtiState=isExtiPinSet();
		extiCheckCnt = 1;
		startExtiTimer();
//		debugExti();   // for debug
	}
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
//	debugExti();
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

void initExtiCheck()
{
	initHandleMissed();
	amountIllegalExti = 0;
	extiCheckCnt= 0;
	initExtiCheckTimer();

	//  general todo smt32F769 check if it needs an external powersupply for debuging...
	//  or 800ma power over usb

}

