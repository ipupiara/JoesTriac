#include "TriacIntr.h"
#include <StateClass.h>
#include <defines.h>
#include <adcControl.h>
#include <triacPID.h>
#include <mainJt.h>

#define zeroPassPin_Pin GPIO_PIN_12
#define zeroPassPin_GPIO_Port GPIOA
#define zeroPassPin_EXTI_IRQn EXTI15_10_IRQn
#define triacTriggerPin_Pin GPIO_PIN_14
#define triacTriggerPin_GPIO_Port GPIOB
#define buzzerTimer htim11




typedef enum {
	extiRunning,
	extiStopped
} extiStateType;

extiStateType extiState;

TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim11;

uint8_t durationTimerOn;

uint16_t currentAmpsADCValue;

void setTriggerPinOn();
void setTriggerPinOff();
uint8_t isTriggerPinOn();

uint32_t getCurrentAmpsADCValue()
{
	uint32_t res;
	taskENTER_CRITICAL();
	res = currentAmpsADCValue;
	taskEXIT_CRITICAL();
	return res;
}

void setCurrentAmpsADCValueNonIsr(uint32_t adcV )
{
	taskENTER_CRITICAL();
	currentAmpsADCValue = adcV;
	taskEXIT_CRITICAL();
}

void adcValueReceived(uint16_t adcVal)
{
	taskENTER_CRITICAL();
	setCurrentAmpsADCValueNonIsr(adcVal);
	taskEXIT_CRITICAL();
}

float adcVoltage()
{
	int16_t ampsAdcHex;
	float   ampsAdcF;
	float   adcMaxF = 0x0FFF;

	float    Vf;

	ampsAdcHex = getCurrentAmpsADCValue();
	ampsAdcF  = ampsAdcHex;
	Vf = (ampsAdcF * 3.3) / adcMaxF;  //  todo set final ref voltage here

	return Vf;
}


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

void setRailTimerArr(int16_t  period)
{
	htim4.Instance->ARR = period;
}

void setRailTimerCnt(uint16_t val)
{
	htim4.Instance->CNT = val;
}


void setDelayTimerArr(int16_t  period)
{
	htim5.Instance->ARR = period;
}

void setDelayTimerCnt(uint16_t val)
{
	htim5.Instance->CNT = val;
}

void startRailTimer()
{

	__HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
	__HAL_TIM_ENABLE(&htim4);
}

void stopRailTimer()
{
	__HAL_TIM_DISABLE(&htim4);
	__HAL_TIM_DISABLE_IT(&htim4, TIM_IT_UPDATE);
}

void startDelayTimerFromIsr()
{
	setDelayTimerArr(triacTriggerDelay); //  unprotected (cli sei) cause in isr anyhow and no interrupt nesting !
	setDelayTimerCnt(0);
	setTriggerPinOff();
	__HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);
	__HAL_TIM_ENABLE(&htim5);
}

void stopDelayTimer()
{
	__HAL_TIM_DISABLE(&htim5);
	__HAL_TIM_DISABLE_IT(&htim5, TIM_IT_UPDATE);
	setTriggerPinOff();

//	__HAL_TIM_DISABLE(&htim5);
//	__HAL_TIM_DISABLE_IT(&htim5, TIM_IT_UPDATE);  // todo reimplemented after problems with triactrigger
}

void TIM4_IRQHandler(void)
{
  	if (__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE) != 0)  {
		__HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);

	  	stopRailTimer();
		setRailTimerCnt(0);

		if (isTriggerPinOn())  {
			setTriggerPinOff();
			setRailTimerArr(300);
		}  else  {
			setTriggerPinOn();
			setRailTimerArr(50);
		}
		startRailTimer();
	}
}


void TIM5_IRQHandler(void)
{
//  HAL_TIM_IRQHandler(&htim5);

  	if (__HAL_TIM_GET_FLAG(&htim5, TIM_FLAG_UPDATE) != 0)  {
		__HAL_TIM_CLEAR_IT(&htim5, TIM_IT_UPDATE);

	  	__HAL_TIM_DISABLE(&htim5);
	  	__HAL_TIM_DISABLE_IT(&htim5, TIM_IT_UPDATE);

	  	setTriggerPinOn();

	  	setRailTimerArr(50);
	  	setRailTimerCnt(0);
	  	startRailTimer();

//		__HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);
//		__HAL_TIM_ENABLE(&htim5);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if (extiState == extiRunning) {
	  if(__HAL_GPIO_EXTI_GET_IT(zeroPassPin_Pin) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(zeroPassPin_Pin);
		if (HAL_GPIO_ReadPin(zeroPassPin_GPIO_Port,zeroPassPin_Pin))  {
			stopDelayTimer();
			stopRailTimer();
			setTriggerPinOff();
		}  else  {
			startDelayTimerFromIsr();
		}
	  }
	}  else {
		stopTriacRun();
		stopDelayTimer();
		stopRailTimer();
		setTriggerPinOff();
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
	htim11.Init.Prescaler = 8;
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
	htim5.Init.Period = 100;
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

	HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
}


void initTriacRailTimer()
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

	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
}


void disableZeroPassDetector()
{
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);   // ok as long as we have only one line on this ISR, else use exti interrupt mask register
}

void enableZeroPassDetector()
{
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

void setTriggerPinOn()
{
	HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 1);
}

void setTriggerPinOff()
{
	HAL_GPIO_WritePin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin, 0);
}

uint8_t isTriggerPinOn()
{
	uint8_t res = 0;
	res = HAL_GPIO_ReadPin(triacTriggerPin_GPIO_Port, triacTriggerPin_Pin);
	return res;
}

void initTriacTriggerPin()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = triacTriggerPin_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(triacTriggerPin_GPIO_Port, &GPIO_InitStruct);
}




void initInterruptsNPorts()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	initZeroPassDetector();
	initTriacTriggerPin();
	initTriacDelayTimer();
	initTriacRailTimer();
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

void startAmpsADC()
{
	setCurrentAmpsADCValueNonIsr( 0);
	startADC();
}

void stopAmpsADC()
{
	stopADC();
	setCurrentAmpsADCValueNonIsr(0);
}

void startTriacRun()
{
	startADC();
	resetPID();
	extiState = extiRunning;
	enableZeroPassDetector();    //  todo added this line after problems
}

void stopTriacRun()
{
//	disableZeroPassDetector();    // ok as long as we have only one line on this ISR, else use exti interrupt mask register
//	stopDelayTimer();
//	stopRailTimer();
//	stopAmpsADC();
//	setTriggerPinOff();    // todo commented after problems

	extiState = extiStopped;
	disableZeroPassDetector();
	stopDelayTimer();
	stopRailTimer();
	stopADC();
	setTriggerPinOff();
}


void initTriacIntr()
{
	durationTimerOn = 0;
	currentAmpsADCValue = 0;  // in isr ok With arm cortex stn32F769 (st least)
	triacTriggerDelay = 0;
	initAdc();
	initInterruptsNPorts();
}

