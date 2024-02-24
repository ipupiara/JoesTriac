/*
 * triacControl.c
 *
 *  Created on: Jan 29, 2024
 *      Author: Brigitte
 */

#include <triacControl.h>
//#include <TriacIntr.h>
#include <extiCheck.h>

#include <stdlib.h>
#include <defines.h>
#include <mainJt.h>
#include <stm32f7xx_hal.h>



//#define zeroPass_Pin GPIO_PIN_12   moved to TriacIntr.h filet
//#define zeroPass_Port GPIOA
#define zeroPassPin_EXTI_IRQn EXTI15_10_IRQn

TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim12;

#define stopTimerIrqHandler TIM2_IRQHandler
#define  delayTimerIrq handler TIM5_IRQHandler

#define triacStopTimer htim2
#define triacDelayTimer htim5
#define triacDelayTimer_IRQn TIM5_IRQn
#define triacRailPwmTimer htim12


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
      amtTim12IrqCallsPerCyleCC = tim12IrqCntCC ; \
      amtTim12IrqCallsPerCyleUI = tim12IrqCntUI ; \
  } while(0)


#define enableRailTimerPwm() \
  do { \
	  TIM_CCxChannelCommand(triacRailPwmTimer.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  \
      triacRailPwmTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
      tim12IrqCntCC = 0;  \
      tim12IrqCntUI = 0; \
  } while(0)

#define disableDelayTimer() \
  do { \
        triacDelayTimer.Instance->CR1 &= ~(TIM_CR1_CEN);  \
        __HAL_TIM_DISABLE_IT(&triacDelayTimer, TIM_IT_UPDATE);\
  } while(0)

int32_t triacTriggerDelay;

uint32_t tim12IrqCntCC, amtTim12IrqCallsPerCyleCC;
uint32_t tim12IrqCntUI, amtTim12IrqCallsPerCyleUI;

//#define ampsHigherPort  GPIOB
//#define ampsHigherPin   GPIO_PIN_14
//#define ampsLowerPort   GPIOB
//#define ampsLowerPin    GPIO_PIN_15
//







void TIM8_BRK_TIM12_IRQHandler(void)
{
	uint8_t doit = 0;
	UNUSED(doit);
	if (__HAL_TIM_GET_FLAG(&htim12, TIM_FLAG_UPDATE) != 0)       {
		__HAL_TIM_CLEAR_IT(&htim12, TIM_IT_UPDATE);
		tim12IrqCntUI += 1;
		doit = 1;
	}
	if (__HAL_TIM_GET_FLAG(&htim12, TIM_FLAG_CC1) != 0)   {
		__HAL_TIM_CLEAR_IT(&htim12, TIM_IT_CC1);
		tim12IrqCntCC += 1;
		doit = 1;
	}

//	if (doit == 1) {
//		if (isAmpsZero()) {
//			htim12.Instance->CCR1 = 70;
//		}  else {
//			htim12.Instance->CCR1 = 0;
//		}
//	}
}
//
//void initAmpsZeroPassDetect()
//{
//	  __HAL_RCC_GPIOH_CLK_ENABLE();  // ex cubemx
//	  __HAL_RCC_GPIOB_CLK_ENABLE();
//
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = ampsHigherPin | ampsLowerPin;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(ampsHigherPort, &GPIO_InitStruct);
//
//}



void startDelayTimer ()
{
    __HAL_TIM_ENABLE_IT(&triacDelayTimer, TIM_IT_UPDATE);
     triacDelayTimer.Instance->ARR =	getTriacTriggerDelay();
     triacDelayTimer.Instance->CNT = 0;
     triacDelayTimer.Instance->CR1 |= (TIM_CR1_CEN);
}

//#define startDelayTimer() \
//  do { \
//        __HAL_TIM_ENABLE_IT(&triacDelayTimer, TIM_IT_UPDATE);  \
//        triacDelayTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
//

//  } while(0)

 //  ex startDelayTimer:   delayTimerRunState = delayTimerDelayPhase; \



#define disableStopTimer() \
  do { \
        triacStopTimer.Instance->CR1 &= ~(TIM_CR1_CEN);  \
        __HAL_TIM_DISABLE_IT(&triacStopTimer, TIM_IT_UPDATE);\
  } while(0)


#define startStopTimer() \
  do { 	triacStopTimer.Instance->ARR= stmTriggerDelayMax;\
  	  	  triacStopTimer.Instance->CNT = 0;\
        __HAL_TIM_ENABLE_IT(&triacStopTimer, TIM_IT_UPDATE);  \
        triacStopTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
  } while(0)

void setTriacTriggerDelay(int32_t durationTcnt)
{
//	taskENTER_CRITICAL();
	if (durationTcnt < stmTriggerDelayMax) {
		if (durationTcnt > 0) {
			triacTriggerDelay = durationTcnt;
		}  else {
			triacTriggerDelay = 1;
		}
	} else {
		triacTriggerDelay = stmTriggerDelayMax - 1;
	}
//	taskEXIT_CRITICAL();  // omitted due to isr problems with freertos, should be no problem here due to atomicity, and 1 changer and 1 consumer-only
}


/*
	variable triacTriggerDelay is used and calculated by mainJt thread, running at highest possible freeRtos thread priority.
	So it can can use taskEnter/Exit_CRITICAL  methods.

	but on the other hand this Variable is used also by the triac interrupts, which run at higher priority for not being delayed
	by methods of freeRtos and touchgfx, and therefore can not use freeRtos methods.

	now we have the advantage that on 32-bit processors, accesses to 32 bit variables, as is triacTriggerdelay, are atomic.
	and so any read or write methods can anyhow not be interrupted.
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



//int32_t tim5UsedDelay; //  todo put to other global variables

void TIM5_IRQHandler(void)      // delaytimer
{
  	if (__HAL_TIM_GET_FLAG(&triacDelayTimer, TIM_FLAG_UPDATE) != 0)  {
		__HAL_TIM_CLEAR_IT(&triacDelayTimer, TIM_IT_UPDATE);

			disableDelayTimer();
			enableRailTimerPwm();
  	}
}

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


void TIM2_IRQHandler(void)    // stoptimer
{
  	if (__HAL_TIM_GET_FLAG(&triacStopTimer, TIM_FLAG_UPDATE) != 0)  {
		__HAL_TIM_CLEAR_IT(&triacStopTimer, TIM_IT_UPDATE);

		disableDelayTimer();
		disableRailTimerPwm();
		disableStopTimer();
	}
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

	    HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, triacTriggerIsrPrio, 0);
	    HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
	    htim12.Instance->DIER |= (TIM_DIER_UIE |TIM_DIER_CC1IE) ;

	    disableRailTimerPwm();
}


void disableZeroPassDetector()
{
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);   // ok as long as we have only one line on this ISR, else use exti interrupt mask register}
}

void enableZeroPassDetector()
{
	__HAL_GPIO_EXTI_CLEAR_IT(zeroPass_Pin);
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


void EXTI15_10_IRQHandler(void)
{
	uint8_t res = 0;
	UNUSED(res);
	  if(__HAL_GPIO_EXTI_GET_IT(zeroPass_Pin) != 0) {
		__HAL_GPIO_EXTI_CLEAR_IT(zeroPass_Pin);

	//	startExtiCheck();
	//  todo needs be tested first and

		doJobOnZeroPassEvent(isExtiPinSet());
	  }
}

void startTriacRun()
{
	startExtiChecking();
	setTriacTriggerDelay(stmTriggerDelayMax-200);
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


void stopTriacTimersWhenDebugHalt()
{
	HAL_DBGMCU_EnableDBGStandbyMode();
	HAL_DBGMCU_EnableDBGStopMode();
	DBGMCU->APB1FZ |= ( DBGMCU_APB1_FZ_DBG_TIM12_STOP | DBGMCU_APB1_FZ_DBG_TIM5_STOP | DBGMCU_APB1_FZ_DBG_TIM2_STOP
						| DBGMCU_APB1_FZ_DBG_TIM3_STOP |  DBGMCU_APB1_FZ_DBG_WWDG_STOP | DBGMCU_APB1_FZ_DBG_TIM6_STOP);
	DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM11_STOP ) ;
}

void startDebuggingTriacRun()
{
	stopTriacTimersWhenDebugHalt();
//	startTriacRun();
}

void doJobOnZeroPassEvent(uint8_t ev)
{
	if (ev == extiZeroPassTriggerStartValue)   {
		disableRailTimerPwm();
		startStopTimer();
		startDelayTimer();
	} else {
		disableDelayTimer();
		disableRailTimerPwm();
	}
}

void initTriacControl()
{
	startDebuggingTriacRun();   //  todo comment this out after debugging
	triacTriggerDelay = stmTriggerDelayMax;
//	initExtiCheck();
	initTriacDelayTimer();
	initTriacStopTimer();
	initTriacRailPwmTimer();
	initZeroPassDetector();

//	initAmpsZeroPassDetect();
#ifdef debugApp
	startDebuggingTriacRun();
#endif
}
