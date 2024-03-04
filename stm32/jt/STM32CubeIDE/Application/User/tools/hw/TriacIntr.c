#include <stdlib.h>
#include "TriacIntr.h"
#include <uart-comms.h>
#include <StateClass.h>
#include <defines.h>
#include <mainJt.h>
#include <extiCheck.h>
#include <stm32f7xx_hal.h>
#include <triacControl.h>



// TODO not only missed would be a problem  also to much (emi) is a more probable case
// introduce a eventcounter in exti (additional states) and add short delays therefore
// todo make this file shorter by splitting it in more files eg. one for triacControl, one for all the rest (buzzer, secondTick, durationtimer, alarm  )

TIM_HandleTypeDef htim11;

#define buzzerTimer htim11



/*

#define nvic_enaIrq( IRQn)  NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL))
#define nvic_disaIrq( IRQn)  \
	do {  \
		NVIC->ICER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL)); __DSB(); 	__ISB(); \
	while (0)

*/



uint8_t durationTimerOn;




void initBuzzerTimerPWM();


uint32_t secondsDurationTimerRemaining;

uint32_t secondsInDurationTimer;



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


void initInterruptsNPorts()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	printf("inside initInterruptsNPorts\n");

	initBuzzerTimerPWM();
	initTriacControl();
}

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
			info_printf("interrupt %d, prio %d, subprio % d",inr,prio, subPrio);
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


void stopTimersWhenDebugHalt()
{
	HAL_DBGMCU_EnableDBGStandbyMode();
	HAL_DBGMCU_EnableDBGStopMode();
	DBGMCU->APB1FZ |= ( DBGMCU_APB1_FZ_DBG_TIM12_STOP | DBGMCU_APB1_FZ_DBG_TIM5_STOP | DBGMCU_APB1_FZ_DBG_TIM2_STOP
						| DBGMCU_APB1_FZ_DBG_TIM3_STOP |  DBGMCU_APB1_FZ_DBG_WWDG_STOP | DBGMCU_APB1_FZ_DBG_TIM6_STOP);
	DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM11_STOP ) ;
}

void startDebugRun()
{
	stopTriacTimersWhenDebugHalt();
//  checkInterrupts();
}


void initTriacIntr()
{
//	startDebugRun();

	durationTimerOn = 0;
	initInterruptsNPorts();
#ifdef debugApp
	startDebuggingTriacRun();
#endif
}
