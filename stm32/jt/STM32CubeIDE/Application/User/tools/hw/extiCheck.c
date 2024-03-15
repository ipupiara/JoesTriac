/*
 * extiCheck.c
 *
 *
 *
 *
 *  Created on: Jan 22, 2024  pn: this check is not 100% exact in every case, but it gives a good picture of the overall Interrupt / spikess behaviour
 *									moreover, when everything works ok, then it should only set zeros
 *      Author: Brigitte
 */

//   todo tobe tested, only first part tested (exti timer)
#include <stdlib.h>
#include "TriacIntr.h"
#include <defines.h>
#include <mainJt.h>
#include <extiCheck.h>
#include <TriacIntr.h>
#include <triacControl.h>

#define extiCheckAmt  3
#define extiCheckDelay  10
#define triacExtiCheckTimer  htim3
#define extiCheckTimerIRQHandler  TIM3_IRQHandler
#define extiCheckTimerIRQn   TIM3_IRQn
#define triacExtiCheckTimerInstance  TIM3
#define  enableExtiCheckTimerClock()  __HAL_RCC_TIM3_CLK_ENABLE()
#define amtExtiEvChecks   3
#define msTick  uwTick   //  uwTick , debugTick

//#define extiPinValue() debugExtiPin
#define extiPinValue() isExtiPinSet()

uint32_t debugTick;
uint8_t debugExtiPin;

TIM_HandleTypeDef triacExtiCheckTimer;

// external variables
uint32_t   maxMissedZp;
uint32_t  amtMissedZpTotal;
uint32_t amountIllegalExti;
uint32_t amtExtiEvTotal;
uint32_t amtWrongSync;
uint32_t amtExtiSequenceError;

//  internal variables
//uint32_t syncMissedPeriodStartTick;
uint32_t  uwTickWhenLastOk;
uint32_t amtCountedMissed;
uint8_t extiCheckCnt ;
uint8_t extiStarting;
uint8_t currentExtiPinState;
uint32_t extiStateBefore;

uint8_t handleMissed();



#define maxInt32  0xFFFFFFFF


#define resetHandleMissed()   \
  do { \
	  amtCountedMissed = 0; \
	  uwTickWhenLastOk = msTick; \
  } while(0)


// todo do not screw-attach buzzer cable to buzzer when debugging, remove debugExti() after debugging
#define debugExti() \
do { \
	  toggleBuzzer(); \
} while(0)

// for later:
// #define debugExti()

void initHandleMissed()
{

}

#define stopExtiCheck() \
do { \
	triacExtiCheckTimer.Instance->CNT = 0; \
	__HAL_TIM_DISABLE_IT(&triacExtiCheckTimer, TIM_IT_UPDATE); \
	triacExtiCheckTimer.Instance->CR1 &= ~(TIM_CR1_CEN); \
	extiCheckCnt=0;\
} while(0)


#define startExtiTimer() \
	do { \
	triacExtiCheckTimer.Instance->CNT = 0; \
	__HAL_TIM_CLEAR_IT(&triacExtiCheckTimer, TIM_IT_UPDATE); \
	__HAL_TIM_ENABLE_IT(&triacExtiCheckTimer, TIM_IT_UPDATE); \
	triacExtiCheckTimer.Instance->CR1 |= (TIM_CR1_CEN); \
} while(0)



void startExtiChecking()
{
	uwTickWhenLastOk = 0;
	amtMissedZpTotal = 0;
	maxMissedZp = 0;
	amtCountedMissed = 0;
	extiCheckCnt=0;
	amtWrongSync = 0;
	amtExtiSequenceError = 0;
	extiStarting = 1;
}


void startExtiCheck()
{
	currentExtiPinState=extiPinValue();
	++ amtExtiEvTotal;

	if (currentExtiPinState != extiZeroPassTriggerStartValue) {
		++ amtExtiSequenceError;  // just for debugging this if  amtExtiSeq... addition for breakpoint  !!!
	} else {

		if (extiCheckCnt > 0) {
									// another exti happened within short time, both are probable not valid
									// but maybe also a spike  just close a valid one, then we loose the valid one.
									// gives a handleMissed
	//dbg		stopExtiCheck();
			amountIllegalExti += 1;
		} else {
			if (extiStarting  == 1)  {
				extiStateBefore = currentExtiPinState;
			}  else  {
				if (extiStateBefore == currentExtiPinState)  {
					++ amtExtiSequenceError;
				}
				extiStateBefore = currentExtiPinState;
			}
			extiCheckCnt = 1;
			debugExti();
			startExtiTimer();
		}

	}
}


/*
 zero pass exti events always have some hundred usec time difference.
 max measured spike times were approx 10 usec, what gives us an idea for the
 needed exitCheck duration. A zero pass event in this time could cause problems.
 neglect the case where a 0xEvent happend during this spike- extiCheckTime
*/
void  extiCheckTimerIRQHandler (void)
{
	__HAL_TIM_CLEAR_IT(&triacExtiCheckTimer, TIM_IT_UPDATE);
	uint8_t extiPinOk  = (currentExtiPinState == extiPinValue());
	debugExti();
	++ extiCheckCnt;
	if (extiCheckCnt < extiCheckAmt) {
		if (extiPinOk == 0) {
//dbg			stopExtiCheck();
			amountIllegalExti += 1;
		}
	}  else {
		stopExtiCheck();
		if ((extiPinOk)== 1 ) {
//dbg			if(handleMissed()) {
//				doJobOnZeroPassEvent(currentExtiPinState);
//			}
		}  else {
			amountIllegalExti += 1;
		}
	}
}

uint8_t min1Diff, max1Diff;

void check1TimeDiff()
{
	uint8_t diff = msTick - uwTickWhenLastOk;
	if (diff < 15) {
		if (diff < min1Diff) {min1Diff = diff;}
		if (diff > max1Diff) {max1Diff = diff;}
	}
}

void init1TimeDiff()
{
	min1Diff = 0xff;
	max1Diff = 0;
}

uint8_t handleMissed()
{
	uint8_t res = 0;

	if (currentExtiPinState == extiZeroPassTriggerStartValue)  {
		if (extiStarting == 1) {
			extiStarting = 0;
			resetHandleMissed();
			init1TimeDiff();
			res = 1;
		} else {  // todo later we should use a better clock base (cpu clock counter or timer)
			check1TimeDiff();
			if((((msTick - uwTickWhenLastOk + 1 ) % 10 )  )  > 2 )  {  // tobe tested evtl > 2 needed or 1 possible
		//			if(((msTick - uwTickWhenLastOk ) % 10   ) != 0 )  {  // tobe tested
				++amtWrongSync;
				resetHandleMissed();
				res = 0; 	// take it as a new correct one, but do not fire unless the next is in time.
			}  else {
				int32_t  amtPassed = ((msTick - uwTickWhenLastOk + 1 )/10 );
				if (amtPassed > 0) {
					int32_t  amtMissed = amtPassed - 1;
					 if (amtMissed == 0) {
						 resetHandleMissed();
						 res = 1;
					 }  else  {
						amtMissedZpTotal += ( amtMissed - amtCountedMissed);
						amtCountedMissed = amtMissed;
						if (amtMissed > maxMissedZp) { maxMissedZp= amtMissed; }


						if ((amtPassed & ((uint32_t) 0x01)) == 1) {  //  odd number todo to be tested
							resetHandleMissed();
							res = 1;
						}  else {
							res = 0;    //  prevent short circuit
						}
					 }
				}  else {
					res = 0;
					resetHandleMissed();
					++amtWrongSync;
				}
			}
		 }
	 }  else {
		 res = 1;
	 }
	return res;
}





void initExtiCheckTimer()
{
	// todo measure period time on oscilloscope and leave possibility to do so

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	enableExtiCheckTimerClock();

	triacExtiCheckTimer.Instance = triacExtiCheckTimerInstance;
	triacExtiCheckTimer.Init.Prescaler = 2;
	triacExtiCheckTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	triacExtiCheckTimer.Init.Period = 4;
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

void initExtiCheck()
{
	initHandleMissed();
	amountIllegalExti = 0;
	extiCheckCnt= 0;
	initExtiCheckTimer();

	//  general todo smt32F769 check if it needs an external powersupply for debuging...
	//  or 800ma power over usb

}

