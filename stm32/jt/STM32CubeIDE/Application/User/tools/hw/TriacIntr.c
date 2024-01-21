#include <stdlib.h>
#include "TriacIntr.h"
#include <StateClass.h>
#include <defines.h>
#include <mainJt.h>

#define zeroPass_Pin GPIO_PIN_12
#define zeroPass_Port GPIOA
#define zeroPassPin_EXTI_IRQn EXTI15_10_IRQn
#define extiZeroPassValue 1


typedef enum {
	delayTimerRailPwmPhase,
	delayTimerExtiEvCnt,
	delayTimerDelayPhase
} delayTimerRunStateType;

#define extiCheckAmt  3
#define extiCheckDelay  10
uint8_t extiCheckCnt ;
uint32_t amtIllegalExti;


// TODO not only missed would be a problem  also to much (emi) is a more probable case
// introduce a eventcounter in exti (additional states) and add short delays therefore
// todo make this file shorter by splitting it in more files eg. one for triacControl, one for all the rest (buzzer, secondTick, durationtimer, alarm  )

#define buzzerTimer htim11

#define triacStopTimer htim2
#define triacDelayTimer htim5
#define triacDelayTimer_IRQn TIM5_IRQn
#define triacRailPwmTimer htim12

#define ampsHigherPort  GPIOB
#define ampsHigherPin   GPIO_PIN_14
#define ampsLowerPort   GPIOB
#define ampsLowerPin    GPIO_PIN_15



/*

#define nvic_enaIrq( IRQn)  NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL))
#define nvic_disaIrq( IRQn)  \
	do {  \
		NVIC->ICER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL)); __DSB(); 	__ISB(); \
	while (0)

*/

uint8_t handleMissed();

#define isPinSet(portx, pinx)  (((portx->IDR) & pinx) != 0) ? 1:0

#define  isExtiPinSet()  isPinSet (zeroPass_Port,zeroPass_Pin)



#define isAmpsZero() ((isPinSet(ampsLowerPort, ampsLowerPin)) && (!(isPinSet(ampsHigherPort,ampsHigherPin))  ))


#define TIM_CCxChannelCommand(TIMx , Channel , ChannelState) \
	do {  \
	  uint32_t  tmp;   \
	  tmp = ~(TIM_CCER_CC1E << Channel);          \
	  TIMx->CCER &=  tmp; \
	  TIMx->CCER |= (uint32_t)(ChannelState << Channel); \
	} while (0)

#define disableRailTimerPwm() \
  do { \
	  TIM_CCxChannelCommand(triacRailPwmTimer.Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  \
      triacRailPwmTimer.Instance->CR1  &= ~(TIM_CR1_CEN);  \
  } while(0)


#define enableRailTimerPwm() \
  do { \
	  TIM_CCxChannelCommand(triacRailPwmTimer.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  \
      triacRailPwmTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
  } while(0)

#define disableDelayTimer() \
  do { \
        triacDelayTimer.Instance->CR1 &= ~(TIM_CR1_CEN);  \
        __HAL_TIM_DISABLE_IT(&triacDelayTimer, TIM_IT_UPDATE);\
  } while(0)


#define startDelayTimer() \
  do { \
	    delayTimerRunState = delayTimerDelayPhase; \
        __HAL_TIM_ENABLE_IT(&triacDelayTimer, TIM_IT_UPDATE);  \
        triacDelayTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
  } while(0)

#define disableStopTimer() \
  do { \
        triacStopTimer.Instance->CR1 &= ~(TIM_CR1_CEN);  \
        __HAL_TIM_DISABLE_IT(&triacStopTimer, TIM_IT_UPDATE);\
  } while(0)


#define startStopTimer() \
  do { \
        __HAL_TIM_ENABLE_IT(&triacStopTimer, TIM_IT_UPDATE);  \
        triacStopTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
  } while(0)

#define amtExtiEvChecks   3
uint8_t extiEvCnt;


delayTimerRunStateType delayTimerRunState;

TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim12;
uint8_t durationTimerOn;



void setTriggerPinOn();
void setTriggerPinOff();
uint8_t isTriggerPinOn();
void initBuzzerTimerPWM();

int32_t triacTriggerDelay;

uint32_t secondsDurationTimerRemaining;

uint32_t secondsInDurationTimer;




void checkInterrupts()
{
	uint32_t inr;
	uint32_t prio, subPrio;
	uint32_t grp = HAL_NVIC_GetPriorityGrouping();
	uint32_t ena;
	uint32_t amt = 0;

	for (inr = 0; inr < 109; ++ inr)  {
		ena = NVIC_GetEnableIRQ(inr);
		if (ena != 0) {
			HAL_NVIC_GetPriority((IRQn_Type) inr, grp, &prio, &subPrio);
			if (prio > -10) {
				++ amt;
			}
			if (inr > 108)  {
				++amt;
				--amt;
			}
		}
	}
	++amt;
	--amt;
//	NVIC_GetEnableIRQ(TIM5_IRQn);
//	UNUSED(grp);
//	UNUSED(ena);
//
//
//	HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(TIM5_IRQn);
//	HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(TIM5_IRQn);
}

void setTriacTriggerDelay(int32_t durationTcnt)
{
	taskENTER_CRITICAL();
	if (durationTcnt < stmTriggerDelayMax) {
		if (durationTcnt > 0) {
			triacTriggerDelay = durationTcnt;
		}  else {
			triacTriggerDelay = 1;
		}
	} else {
		triacTriggerDelay = stmTriggerDelayMax - 1;
	}
	taskEXIT_CRITICAL();  // omitted due to isr problems with freertos, should be no problem here due to atomicity, and 1 changer and 1 consumer-only
}


/*
 *
todo make this text human readable :-)

	variable triacTriggerDelay is used and calculated by mainJt thread, running at highest possible freeRtos thread priority.
	So it can can use taskEnter/Exit_CRITICAL  methods, but it is used also by the triac interrupts, which can not use
	freeRtos methods.

	on the other hand triacInterrupts (exti, triacDelay and triggerPWM) run at even higher priority than freeRtod for
	not being delayed by methods of freeRtos and touchGfx. therefore they can not use any freeRtos methods like taskEnter/Exit_CRITICAL.
	(without causing a hanger on an assert(....) method)

	now we have the advantage that on 32-bit processors accesses to 32 bit variables as triacTriggerdelay are atomic.
	so any read or write methods can not be interrupted.

	all settings of this variable are done exclusively by mainJt thread, so do not need concurrency protection
	as on good old real programming under DOSxy.

	as long as all settings of this variable are done with above method  (for setting all changes in a single step !),
	we can be sure that read accesses from the interrupts return a valid value. of
	course the interrupts may not do any other access than read on this variable.
	(if this isrs would do writes, these writes could happen during calculating a new value  or above setTriacDuration method.
	 this could end up in a mess.
*/
int32_t getTriacTriggerDelay()
{
	int32_t res = 0;
//	taskENTER_CRITICAL();
	res = triacTriggerDelay;
						//
//	taskEXIT_CRITICAL();
	return res;
}

int32_t tim5UsedDelay; //  todo put to other global variables

void TIM5_IRQHandler(void)
{
  	if (__HAL_TIM_GET_FLAG(&triacDelayTimer, TIM_FLAG_UPDATE) != 0)  {
		__HAL_TIM_CLEAR_IT(&triacDelayTimer, TIM_IT_UPDATE);

//			extiCheckCnt=0;
			enableRailTimerPwm();
			disableDelayTimer();
//			tim5RunState = tim5RailPwmPhase;

  	}
}

void TIM2_IRQHandler(void)
{
  	if (__HAL_TIM_GET_FLAG(&triacStopTimer, TIM_FLAG_UPDATE) != 0)  {
		__HAL_TIM_CLEAR_IT(&triacStopTimer, TIM_IT_UPDATE);

		disableDelayTimer();
		disableRailTimerPwm();
		disableStopTimer();
	}
}



uint32_t uwTickSinceLastOk;
uint32_t lastUwTick;
uint32_t amtExtiMissedTotal;
uint32_t maxMissedExti;

uint32_t amtCountedMissed;
uint32_t lastOkUwTick;
uint32_t syncMissedPeriodStartTick;
uint32_t amtSyncMissed;  //  todo add to astrolabium

uint32_t amtMissed;

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

//void startExtiCheck()
//{
//
//}
//
//void stopExtiCheck()
//{
//
//}


void startExtiCheck()
{
	currentExtiState=isExtiPinSet();
	extiCheckCnt = 0;
}


#define startExtiCheck()

#define stopExtiCheck()

//#define isAmpsZero() ((isPinSet(ampsLowerPort, ampsLowerPin)) && (!(isPinSet(ampsHigherPort,ampsHigherPin))  ))
//
//

//#define TIM_CCxChannelCommand(TIMx , Channel , ChannelState) \
//	do {  \
//	  uint32_t  tmp;   \
//	  tmp = ~(TIM_CCER_CC1E << Channel);          \
//	  TIMx->CCER &=  tmp; \
//	  TIMx->CCER |= (uint32_t)(ChannelState << Channel); \
//	} while (0)


//  tobe tested, reduces the probability of wrong event, increases it for lost event slightly
//  but accompained by handleMissed

void initHandleMissed()
{

}

void initExtiTimer()
{
	initHandleMissed();
	amtIllegalExti = 0;
	extiCheckCnt= 0;
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
				if (isExtiPinSet() == extiZeroPassValue)   {
					syncMissedPeriodStartTick = 0;
					tim5UsedDelay =	triacDelayTimer.Instance->ARR =getTriacTriggerDelay();
					triacDelayTimer.Instance->CNT =0;
					delayTimerRunState = delayTimerDelayPhase;
					triacStopTimer.Instance->ARR=stmTriggerDelayMax;
					triacStopTimer.Instance->CNT = 0;
					startStopTimer();
					startDelayTimer();
					disableRailTimerPwm();
				} else {
					disableDelayTimer();
					disableRailTimerPwm();
				}
			}
		}
	}

}


#define extiZeroPassValue 1
void EXTI15_10_IRQHandler(void)
{
	uint8_t res = 0;
	UNUSED(res);
	  if(__HAL_GPIO_EXTI_GET_IT(zeroPass_Pin) != 0) {
		__HAL_GPIO_EXTI_CLEAR_IT(zeroPass_Pin);

		if (extiCheckCnt > 0 )   {
			stopExtiCheck();
			res = 1;
			extiCheckCnt = 0;
		}  else {
			startExtiCheck();
		}

//		if (isExtiPinSet() == extiZeroPassValue)   {
//				tim5UsedDelay =	triacDelayTimer.Instance->ARR =getTriacTriggerDelay();
//				triacDelayTimer.Instance->CNT =0;
////				delayTimerRunState = delayTimerDelayPhase;
//				triacStopTimer.Instance->ARR=stmTriggerDelayMax;
//				triacStopTimer.Instance->CNT = 0;
//				startStopTimer();
//				startDelayTimer();
//				disableRailTimerPwm();
//		}  else  {
//				disableDelayTimer();
//				disableRailTimerPwm();
//		}
	  }
}


void initTriacDelayTimer()
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	__HAL_RCC_TIM5_CLK_ENABLE();

	htim5.Instance = TIM5;
	htim5.Init.Prescaler = triacDelayPsc;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 0;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
	{
		errorHandler(1,stop," HAL_TIM_Base_Init ","initTriacTimer");
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
	{
		errorHandler(2,stop," HAL_TIM_ConfigClockSource ","initTriacTimer");
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
	{
		errorHandler(3,stop," HAL_TIMEx_MasterConfigSynchronization ","initTriacTimer");
	}

	htim5.Instance->CR1 &= (~TIM_CR1_OPM_Msk);
	HAL_NVIC_SetPriority(TIM5_IRQn, triacTriggerIsrPrio, 0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
	disableDelayTimer();
}

void initTriacStopTimer()
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	__HAL_RCC_TIM2_CLK_ENABLE();

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = triacDelayPsc;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		errorHandler(1,stop," HAL_TIM_Base_Init ","initTriacTimer");
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		errorHandler(2,stop," HAL_TIM_ConfigClockSource ","initTriacTimer");
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		errorHandler(3,stop," HAL_TIMEx_MasterConfigSynchronization ","initTriacTimer");
	}

	htim2.Instance->CR1 &= (~TIM_CR1_OPM_Msk);
	HAL_NVIC_SetPriority(TIM2_IRQn, triacTriggerIsrPrio, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	disableStopTimer();
}

void TIM8_BRK_TIM12_IRQHandler(void)
{
//	uint8_t doit = 0;
//	if (__HAL_TIM_GET_FLAG(&htim12, TIM_FLAG_UPDATE) != 0)       {
//		__HAL_TIM_CLEAR_IT(&htim12, TIM_IT_UPDATE);
//		doit = 1;
//	}
//	if (__HAL_TIM_GET_FLAG(&htim12, TIM_FLAG_CC1) != 0)   {
//		__HAL_TIM_CLEAR_IT(&htim12, TIM_IT_CC1);
//		doit = 1;
//	}
//
//	if (doit == 1) {
//		if (isAmpsZero()) {
//			htim12.Instance->CCR1 = 70;
//		}  else {
//			htim12.Instance->CCR1 = 0;
//		}
//	}
}

void initTriacRailPwmTimer()
{
	 TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	 TIM_OC_InitTypeDef sConfigOC = {0};
	 GPIO_InitTypeDef GPIO_InitStruct = {0};

	 __HAL_RCC_TIM12_CLK_ENABLE();

	  htim12.Instance = TIM12;
	  htim12.Init.Prescaler = 10;
	  htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim12.Init.Period = 600;
	  htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	  if (HAL_TIM_Base_Init(&htim12) != HAL_OK)
	  {
		  errorHandler(4,stop," HAL_TIM_Base_Init ","initTriacRailPwmTimer");
	  }
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&htim12, &sClockSourceConfig) != HAL_OK)
	  {
		  errorHandler(5,stop," HAL_TIM_ConfigClockSource ","initTriacRailPwmTimer");
	  }
	  if (HAL_TIM_PWM_Init(&htim12) != HAL_OK)
	  {
		  errorHandler(6,stop," HAL_TIM_PWM_Init ","initTriacRailPwmTimer");
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 70;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if (HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
		  errorHandler(7,stop," HAL_TIM_PWM_ConfigChannel ","initTriacRailPwmTimer");
	  }
	   __HAL_RCC_GPIOH_CLK_ENABLE();
	    /**TIM12 GPIO Configuration
	    PH6     ------> TIM12_CH1
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
	    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

		htim12.Instance->CR1 &= (~TIM_CR1_OPM_Msk);
		htim12.Instance->CR1 &= (~TIM_CR1_UDIS_Msk);

//	    HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, triacTriggerIsrPrio, 0);
//	    HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
//	    htim12.Instance->DIER |= (TIM_DIER_UIE |TIM_DIER_CC1IE) ;

	    disableRailTimerPwm();
}


void disableZeroPassDetector()
{
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);   // ok as long as we have only one line on this ISR, else use exti interrupt mask register}
}

void enableZeroPassDetector()
{
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void initZeroPassDetector()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = zeroPass_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(zeroPass_Port, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, triacTriggerIsrPrio, 0);
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
}

void initAmpsZeroPassDetect()
{
	  __HAL_RCC_GPIOH_CLK_ENABLE();  // ex cubemx
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = ampsHigherPin | ampsLowerPin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ampsHigherPort, &GPIO_InitStruct);

}


void startTriacRun()
{
	startHandleMissed();
	setTriacTriggerDelay(stmTriggerDelayMax);
	enableZeroPassDetector();
//	checkInterrupts();
}

void stopTriacRun()
{
	disableZeroPassDetector();
	// ok as long as we have only one line on this ISR, else use exti interrupt mask register
	disableDelayTimer();
	disableRailTimerPwm();
}

void stopTimersWhenDebugHalt()
{
	HAL_DBGMCU_EnableDBGStandbyMode();
	HAL_DBGMCU_EnableDBGStopMode();
	DBGMCU->APB1FZ |= ( DBGMCU_APB1_FZ_DBG_TIM12_STOP | DBGMCU_APB1_FZ_DBG_TIM5_STOP);
	DBGMCU->APB2FZ |= DBGMCU_APB2_FZ_DBG_TIM11_STOP;
}

void startDebuggingTriacRun()
{
	stopTimersWhenDebugHalt();

	checkInterrupts();
	float multi = 5.0 / 8.0;
	triacTriggerDelay = stmTriggerDelayMax*  multi;
	startTriacRun();
}



void initInterruptsNPorts()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	printf("inside initInterruptsNPorts\n");

	initZeroPassDetector();
	initTriacDelayTimer();
	initTriacStopTimer();
	initTriacRailPwmTimer();
	initBuzzerTimerPWM();
	initAmpsZeroPassDetect();
	initExtiTimer();
}


void initTriacIntr()
{
//	stopTimersWhenDebugHalt();

	durationTimerOn = 0;
	triacTriggerDelay = stmTriggerDelayMax;
	initInterruptsNPorts();
#ifdef debugApp
	startDebuggingTriacRun();
#endif
}


void enableBuzzerTimerPWM()
{
	TIM_CCxChannelCmd(htim11.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
	htim11.Instance ->CR1  |= (TIM_CR1_CEN);
}

void disableBuzzerTimerPWM()
{
	TIM_CCxChannelCmd(htim11.Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
	htim11.Instance ->CR1  &= ~(TIM_CR1_CEN);
}

uint8_t isEnabledBuzzerTimerPWM()
{
	uint8_t res = ((htim11.Instance->CR1 & TIM_CR1_CEN) == 1) ;
	return res;
}

void setBuzzerOn()
{
	enableBuzzerTimerPWM();
}

void setBuzzerOff()
{
	disableBuzzerTimerPWM();
}

void toggleBuzzer()
{
	if (isEnabledBuzzerTimerPWM()) {
		disableBuzzerTimerPWM();
	}  else {
		enableBuzzerTimerPWM();
	}
}


void initBuzzerTimerPWM()
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_TIM11_CLK_ENABLE();

	htim11.Instance = TIM11;
	htim11.Init.Prescaler = 8;   //  todo increase presc and different periods for better overall performance
	htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim11.Init.Period = 25000;
	htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	if (HAL_TIM_PWM_Init(&htim11) != HAL_OK)
	{
		errorHandler(1,stop," HAL_TIM_PWM_Init ","initBuzzerTimerPWM");
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 12500;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim11, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		errorHandler(1,stop," HAL_TIM_PWM_ConfigChannel ","initBuzzerTimerPWM");
	}
	 __HAL_RCC_GPIOF_CLK_ENABLE();
	/**TIM11 GPIO Configuration
	PF7     ------> TIM11_CH1
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF3_TIM11;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	disableBuzzerTimerPWM();
}


void setCompletionAlarmOff()
{
	setBuzzerOff();
}

void setCompletionAlarmOn()
{
	setBuzzerOn();
}

void toggleCompletionAlarm()
{
	toggleBuzzer();
}



void durationTimerTick()
{
	if (durationTimerOn == 1) {
		if (secondsDurationTimerRemaining == 0) {
			stopDurationTimer();
			fsmTriacEvent ev;
			ev.evType = evTimeOutDurationTimer;
			processTriacFsmEvent(PJoesTriacStateChart,&ev);
		}
		if (secondsDurationTimerRemaining > 0)  {
			--secondsDurationTimerRemaining;
			++secondsInDurationTimer;
		}
	}
}


void startDurationTimer(uint32_t secs)
{
//	durationTimerReachead = 0;
	secondsDurationTimerRemaining = secs;
	secondsInDurationTimer = 0;
	durationTimerOn=1;
}

void stopDurationTimer()
{
	durationTimerOn = 0;
	secondsDurationTimerRemaining = 0;
	secondsInDurationTimer = 0;
}

void haltDurationTimer()
{
	durationTimerOn = 0;  // CLARIFY WITH Team if amps shall stop or not yet.
}

uint32_t getSecondsDurationTimerRemaining()
{
	uint32_t res;
	taskENTER_CRITICAL();
	res = secondsDurationTimerRemaining;
	taskEXIT_CRITICAL();
	return res;
}

uint32_t getSecondsInDurationTimer()
{
	uint32_t res;
	taskENTER_CRITICAL();
	res = secondsInDurationTimer;
	taskEXIT_CRITICAL();
	return res;
}

