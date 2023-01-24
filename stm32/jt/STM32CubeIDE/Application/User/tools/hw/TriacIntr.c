#include "TriacIntr.h"
#include <StateClass.h>
#include <defines.h>
#include <mainJt.h>
#include <core_cm7.h>

#define zeroPassPin_Pin GPIO_PIN_12
#define zeroPassPin_GPIO_Port GPIOA
#define zeroPassPin_EXTI_IRQn EXTI15_10_IRQn
#define buzzerTimer htim11

#define triacDelayTimer htim5
#define triacRailPwmTimer htim12

#define TIM_CCxChannelCommand(TIMx , Channel , ChannelState) \
	do {  \
	  TIMx->CCER &=  ~(TIM_CCER_CC1E << (Channel & 0x1FU)); \
	  TIMx->CCER |= (uint32_t)(ChannelState << (Channel & 0x1FU)); \
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

#define stopDelayTimer() \
  do { \
        triacDelayTimer.Instance->CR1 &= ~(TIM_CR1_CEN);  \
        __HAL_TIM_DISABLE_IT(&triacDelayTimer, TIM_IT_UPDATE);\
  } while(0)


#define startDelayTimer() \
  do { \
	    tim5RunState = tim5RunPhase; \
        triacDelayTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
        __HAL_TIM_ENABLE_IT(&triacDelayTimer, TIM_IT_UPDATE);  \
  } while(0)

typedef enum {
	tim5DelayPhase,
	tim5RunPhase
} tim5RunStateType;

typedef enum {
	extiRunning,
	extiStopped
} extiStateType;

extiStateType extiState;
tim5RunStateType tim5RunState;

TIM_HandleTypeDef htim5;
//TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim12;
uint8_t durationTimerOn;

void setTriggerPinOn();
void setTriggerPinOff();
uint8_t isTriggerPinOn();

uint16_t triacTriggerDelay;

uint32_t secondsDurationTimerRemaining;
uint32_t secondsInDurationTimer;
uint16_t tim5UsedDelay;

void checkInterrupts()
{
	uint32_t inr;
	uint32_t prio, subPrio;
	uint32_t grp = HAL_NVIC_GetPriorityGrouping();
	uint32_t ena;
	for (inr = 0; inr < 109; ++ inr)  {
		ena = NVIC_GetEnableIRQ(inr);
		if (ena != 0) {
			HAL_NVIC_GetPriority((IRQn_Type) inr, grp, &prio, &subPrio);
		}
	}
	NVIC_GetEnableIRQ(TIM5_IRQn);
	UNUSED(grp);
	UNUSED(ena);


	HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
	HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
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
		triacTriggerDelay = stmTriggerDelayMax;
	}
	taskEXIT_CRITICAL();
}


uint16_t getTriacTriggerDelay()
{
	uint32_t res = 0;
	taskENTER_CRITICAL();
	res = triacTriggerDelay;
	taskEXIT_CRITICAL();
	return res;
}

uint32_t  delayCnt0, delayCnt1, extiCnt1 , extiCnt0 ;


void TIM5_IRQHandler(void)
{
  	if (__HAL_TIM_GET_FLAG(&triacDelayTimer, TIM_FLAG_UPDATE) != 0)  {
		__HAL_TIM_CLEAR_IT(&triacDelayTimer, TIM_IT_UPDATE);

		if (tim5RunState == tim5RunPhase)  {
	  		++ delayCnt0;
	  		enableRailTimerPwm();
			triacDelayTimer.Instance->CNT = 0;
			triacDelayTimer.Instance->ARR = stmTriggerDelayMax - tim5UsedDelay;
			tim5RunState = tim5DelayPhase;
	  	}  else {
	  		++ delayCnt1;
	  		stopDelayTimer();
	  		disableRailTimerPwm();
	  	}
	}
}


void EXTI15_10_IRQHandler(void)
{
	if (extiState == extiRunning) {

	  if(__HAL_GPIO_EXTI_GET_IT(zeroPassPin_Pin) != 0) {
		__HAL_GPIO_EXTI_CLEAR_IT(zeroPassPin_Pin);
		if (HAL_GPIO_ReadPin(zeroPassPin_GPIO_Port,zeroPassPin_Pin))  {
			++extiCnt1;
//			if ((extiCnt0 & 0x1) == 1)  {
				disableRailTimerPwm();
				stopDelayTimer();
//			}
		}  else  {
			++extiCnt0;
;
//			if ((extiCnt0 & 0x1) == 1)  {
				tim5UsedDelay =  triacDelayTimer.Instance->ARR =triacTriggerDelay;
				triacDelayTimer.Instance->CNT =0;
				startDelayTimer();
//			}
		}
	  }
	}  else {
//		stopTriacRun();
	}
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


void initBuzzerTimerPWM()
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_TIM11_CLK_ENABLE();

	htim11.Instance = TIM11;
	htim11.Init.Prescaler = 8;   //  todo increase presc and lower periods for better overall performance
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
	HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
	stopDelayTimer();
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
	  htim12.Init.Period = 350;
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
	  sConfigOC.Pulse = 50;
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

	    disableRailTimerPwm();
//		TIM_CCxChannelCmd(htim12.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

//	    HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
//	    HAL_NVIC_EnableIRQ(TIM5_IRQn);
}


void disableZeroPassDetector()
{
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);   // ok as long as we have only one line on this ISR, else use exti interrupt mask register}
	extiState = extiStopped;
}

void enableZeroPassDetector()
{
	extiState = extiRunning;
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void initZeroPassDetector()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = zeroPassPin_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(zeroPassPin_GPIO_Port, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	extiState = extiStopped;
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
}


void initInterruptsNPorts()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	initZeroPassDetector();
	initTriacDelayTimer();
	initTriacRailPwmTimer();
	initBuzzerTimerPWM();
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

void resumeDurationTimer()
{
	durationTimerOn = 1;
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


void startTriacRun()
{
	enableZeroPassDetector();
}

void stopTriacRun()
{
	disableZeroPassDetector();
	// ok as long as we have only one line on this ISR, else use exti interrupt mask register
	stopDelayTimer();
	disableRailTimerPwm();
}

void startDebuggingTriacRun()
{
	HAL_DBGMCU_EnableDBGStandbyMode();
	HAL_DBGMCU_EnableDBGStopMode();
	DBGMCU->APB1FZ |= ( DBGMCU_APB1_FZ_DBG_TIM12_STOP | DBGMCU_APB1_FZ_DBG_TIM5_STOP);
	DBGMCU->APB2FZ |= DBGMCU_APB2_FZ_DBG_TIM11_STOP;

	float multi = 5.0 / 8.0;
	triacTriggerDelay = stmTriggerDelayMax*  multi;
	startTriacRun();
}

void initTriacIntr()
{
	delayCnt0 = delayCnt1 =  extiCnt1, extiCnt0 =0;
	durationTimerOn = 0;
	triacTriggerDelay = stmTriggerDelayMax;
	initInterruptsNPorts();
#ifdef debugTriac
	startDebuggingTriacRun();
#endif
}
