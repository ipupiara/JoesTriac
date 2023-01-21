#include "TriacIntr.h"
#include <StateClass.h>
#include <defines.h>
#include <mainJt.h>

#define zeroPassPin_Pin GPIO_PIN_12
#define zeroPassPin_GPIO_Port GPIOA
#define zeroPassPin_EXTI_IRQn EXTI15_10_IRQn
#define triacTriggerPin_Pin GPIO_PIN_14
#define triacTriggerPin_GPIO_Port GPIOB
#define buzzerTimer htim11

#define triacDelayTimer htim5
#define triacRailTimer htim4
#define triacRailPwmTimer htim10

#define stopRailTimer() \
  do { \
        triacRailTimer.Instance->CR1 &= ~(TIM_CR1_CEN);  \
        __HAL_TIM_DISABLE_IT(&triacRailTimer, TIM_IT_UPDATE);\
        HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 0);\
  } while(0)

#define startRailTimer() \
  do { \
        triacRailTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
        __HAL_TIM_ENABLE_IT(&triacRailTimer, TIM_IT_UPDATE);\
  } while(0)

#define stopDelayTimer() \
  do { \
        triacDelayTimer.Instance->CR1 &= ~(TIM_CR1_CEN);  \
        __HAL_TIM_DISABLE_IT(&triacDelayTimer, TIM_IT_UPDATE);\
  } while(0)

#define startDelayTimer() \
  do { \
	    tim5RunState = tim5RunPhase; \
	    __HAL_TIM_ENABLE_IT(&triacDelayTimer, TIM_IT_UPDATE);  \
        triacDelayTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
  } while(0)

#define enableRailTimerPwm() \
  do { \
        triacRailPwmTimer.Instance->CR1  &= ~(TIM_CR1_CEN);  \
  } while(0)


#define disableRailTimerPwm() \
  do { \
        triacRailPwmTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
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
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim10;
uint8_t durationTimerOn;



void setTriggerPinOn();
void setTriggerPinOff();
uint8_t isTriggerPinOn();


uint16_t triacTriggerDelay;

uint32_t secondsDurationTimerRemaining;

uint32_t secondsInDurationTimer;


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


uint32_t  delayCnt0, delayCnt1, railCnt, extiCnt1 , extiCnt0 ;


void TIM4_IRQHandler(void)
{
  	if (__HAL_TIM_GET_FLAG(&triacRailTimer, TIM_FLAG_UPDATE) != 0)  {
		__HAL_TIM_CLEAR_IT(&triacRailTimer, TIM_IT_UPDATE);

		++ railCnt;
		triacRailTimer.Instance->CNT = 0;
		if (HAL_GPIO_ReadPin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin))  {
			HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 0);
			triacRailTimer.Instance->ARR =300;
		}  else  {
			HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 1);
			triacRailTimer.Instance->ARR =50;
		}
	}
}

uint16_t tim5UsedDelay;
#define timerDelta  10

void TIM5_IRQHandler(void)
{
  	if (__HAL_TIM_GET_FLAG(&triacDelayTimer, TIM_FLAG_UPDATE) != 0)  {
		__HAL_TIM_CLEAR_IT(&triacDelayTimer, TIM_IT_UPDATE);

		if (tim5RunState == tim5RunPhase)  {
	  		HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 1);
	  		++ delayCnt0;
//			triacRailTimer.Instance->ARR = 50;
//			startRailTimer();
			triacDelayTimer.Instance->CNT = 0;
			triacDelayTimer.Instance->ARR = stmTriggerDelayMax - tim5UsedDelay -timerDelta ;
			tim5RunState = tim5DelayPhase;
	  	}  else {
	  		++ delayCnt1;
	  		stopDelayTimer();
//			stopRailTimer();
			HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 0);
	  	}
	}
}



void EXTI15_10_IRQHandler(void)
{
	if (extiState == extiRunning) {

	  if(__HAL_GPIO_EXTI_GET_IT(zeroPassPin_Pin) != 0) {
		__HAL_GPIO_EXTI_CLEAR_IT(zeroPassPin_Pin);
		if (HAL_GPIO_ReadPin(zeroPassPin_GPIO_Port,zeroPassPin_Pin))  {
//			HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 1);
			++extiCnt1;
//			if ((extiCnt0 & 0x1) == 1)  {
//				stopRailTimer();
				stopDelayTimer();
				HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 0);
//			}
		}  else  {
			HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 0);
			++extiCnt0;
//			toggleBuzzer();
//			if ((extiCnt0 & 0x1) == 1)  {
				tim5UsedDelay =  triacDelayTimer.Instance->ARR =triacTriggerDelay;
				triacDelayTimer.Instance->CNT =0;
				HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 0);
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
	htim11.Instance ->CR1  |= (TIM_CR1_CEN);
}

void disableBuzzerTimerPWM()
{
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

	//htim11.Instance->CCER |= 0x01;  //  1 << CCIE;

	//  TIMx->CCER &= ~TIM_CCER_CC1E;
//	TIMx->CCER |= TIM_CCER_CC1E;

//	TIM_TypeDef *TIMx

	TIM_CCxChannelCmd(htim11.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
}

//uint16_t


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


void initTriacRailTimer()    //  todo urgent check and make sure that timer does not automatically stop
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	__HAL_RCC_TIM4_CLK_ENABLE();

	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 10;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 50;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	{
		errorHandler(1,stop," HAL_TIM_Base_Init ","initRailTimer");
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
	{
		errorHandler(2,stop," HAL_TIM_ConfigClockSource ","initRailTimer");
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
	{
		errorHandler(3,stop," HAL_TIMEx_MasterConfigSynchronization ","initRailTimer");
	}
	htim4.Instance->CR1 &= (~TIM_CR1_OPM_Msk);

	__HAL_TIM_DISABLE(&triacRailTimer);
	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	stopRailTimer();

}

void initTriacRailPwmTimer()
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	  __HAL_RCC_TIM10_CLK_ENABLE();


	  htim10.Instance = TIM10;
	  htim10.Init.Prescaler = 10;
	  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim10.Init.Period = 350;
	  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_PWM_Init(&htim10) != HAL_OK)
	  {
		  errorHandler(4,stop," HAL_TIM_PWM_Init ","initTriacRailPwmTimer");
	  }

	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 50;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if (HAL_TIM_PWM_ConfigChannel(&htim10, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
		  errorHandler(4,stop," HAL_TIM_PWM_ConfigChannel ","initTriacRailPwmTimer");
	  }

	  __HAL_RCC_GPIOF_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		disableRailTimerPwm();
		TIM_CCxChannelCmd(htim10.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

//	    HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
//	    HAL_NVIC_EnableIRQ(TIM5_IRQn);

}




//void initTriacRailPwmTimer()
//{
////  made with firmware 1.17, while above runs so far on 16.2, so better take above
//	  TIM_OC_InitTypeDef sConfigOC = {0};
//
//	  __HAL_RCC_TIM10_CLK_ENABLE();
//
//	  htim10.Instance = TIM10;
//	  htim10.Init.Prescaler = 10;
//	  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
//	  htim10.Init.Period = 350;
//	  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//	  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//	  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
//	  {
//		  errorHandler(3,stop," HAL_TIM_Base_Init ","initTriacRailPwmTimer");
//	  }
//	  if (HAL_TIM_PWM_Init(&htim10) != HAL_OK)
//	  {
//		  errorHandler(4,stop," HAL_TIM_PWM_Init ","initTriacRailPwmTimer");
//	  }
//	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
//	  sConfigOC.Pulse = 50;
//	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//	  if (HAL_TIM_PWM_ConfigChannel(&htim10, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
//	  {
//		  errorHandler(4,stop," HAL_TIM_PWM_ConfigChannel ","initTriacRailPwmTimer");
//	  }
//	  __HAL_RCC_GPIOF_CLK_ENABLE();
//		GPIO_InitStruct.Pin = GPIO_PIN_6;
//		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//		GPIO_InitStruct.Pull = GPIO_NOPULL;
//		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//		GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
//		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
//
////	    HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
////	    HAL_NVIC_EnableIRQ(TIM5_IRQn);
//
//}


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

//void setTriggerPinOn()
//{
//	HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 1);
//}

void setTriggerPinOff()
{
	HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 0);
}

//uint8_t isTriggerPinOn()
//{
//	uint8_t res = 0;
//	res = HAL_GPIO_ReadPin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin);
//	return res;
//}

void initTriacTriggerPin()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = triacTriggerPin_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(triacTriggerPin_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 0);
}




void initInterruptsNPorts()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	initZeroPassDetector();
	initTriacTriggerPin();
	initTriacDelayTimer();
//	initTriacRailTimer();
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
	stopRailTimer();
	setTriggerPinOff();
}

void startDebuggingTriacRun()
{
	HAL_DBGMCU_EnableDBGStandbyMode();
	HAL_DBGMCU_EnableDBGStopMode();
	DBGMCU->APB1FZ |= ( DBGMCU_APB1_FZ_DBG_TIM4_STOP | DBGMCU_APB1_FZ_DBG_TIM5_STOP);

	float multi =1.0 / 4.0;
	triacTriggerDelay = stmTriggerDelayMax*  multi;
	startTriacRun();
}

void initTriacIntr()
{
	delayCnt0 = delayCnt1 = railCnt =  extiCnt1, extiCnt0 =0;
	durationTimerOn = 0;
	triacTriggerDelay = stmTriggerDelayMax;
	initInterruptsNPorts();
#ifdef debugTriac
	startDebuggingTriacRun();
#endif
}




