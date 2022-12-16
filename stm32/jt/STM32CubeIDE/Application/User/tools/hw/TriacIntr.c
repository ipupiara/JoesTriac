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
#define buzzerPin_Pin GPIO_PIN_15
#define buzzerPin_GPIO_Port GPIOB

TIM_HandleTypeDef htim5;

uint8_t durationTimerOn;

uint16_t currentAmpsADCValue;

uint32_t getCurrentAmpsADCValue()
{
	uint32_t res;
	taskENTER_CRITICAL();
	res = currentAmpsADCValue;
	taskEXIT_CRITICAL();
	return res;
}


void adcValueReceived(uint16_t adcVal)
{
	taskENTER_CRITICAL();
	currentAmpsADCValue = adcVal;
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


//int64_t  secondCount;

//
//int16_t remainingTriacTriggerDelayCounts;
//
uint32_t triacTriggerTimeTcnt;

uint32_t triacFireDurationTcnt;

uint32_t secondsDurationTimerRemaining;

uint32_t secondsInDurationTimer;

//int8_t adcCnt;
//
int32_t amtInductiveRepetitions;

//#define ocra2aValueMax 0XFC  // still to be defined
//
//void setTcnt2AndOcra2a(int16_t newTcnt2Val,int16_t newOcra2a)
//{
//	// timer must be stopped to set tcnt, because else, on an
//	// unprotected set, the timer itself could interfere with the *non double buffered feature" write access.
//	// resulting in a more or less random set value.
//	int8_t tccr2bStack;
//	tccr2bStack = TCCR2B;
//	TCCR2B = 0b00000000  ;  // CTC, timer stopped
//	if (TCNT2 != newTcnt2Val) {  // dont set if not needed , because  .....
//		TCNT2 = newTcnt2Val;
//		if (newOcra2a == (TCNT2 + 1)) {++ newOcra2a; }  // .... updating avoids triggering of next clock cycle, but needs overnext.
//	}
//	OCR2A = newOcra2a;
//	TCCR2B = tccr2bStack  ; // set previous value
//}
//
//void setTriacTriggerDelayValues()
//{
//	if (remainingTriacTriggerDelayCounts < ocra2aValueMax) {
//		setTcnt2AndOcra2a (0, remainingTriacTriggerDelayCounts);
//		triacTriggerTimeTcnt2 += remainingTriacTriggerDelayCounts;
//		remainingTriacTriggerDelayCounts = 0;
//	} else {
//		remainingTriacTriggerDelayCounts -= ocra2aValueMax;
//		setTcnt2AndOcra2a(0, ocra2aValueMax);
//		triacTriggerTimeTcnt2 +=  ocra2aValueMax;
//	}
//}
//
//void startTimer2()
//{
//	TIFR2 = 0x00;
//	TIMSK2   = 0b00000010;  //  Output Compare A Match Interrupt Enable
//	TCCR2B = 0b00000101  ; // CTC on CC2A , set clk / 128, timer 2 started
//}
//
//void stopTimer2()
//{
//	TCCR2B = 0b00000000  ;  // CTC, timer stopped
//	TIMSK2  = 0x00;
//	TIFR2 = (1<< OCF2A);    // cleared by writing a "logic" one to the flag
//}
//
//
//void startTriacTriggerDelay( int16_t delayDuration)  // must run protected between cli and sei
//{
//	if (delayDuration <= 0) {
//		delayDuration = 1;   // just a very short duration, but one that will happen in future
//	}
//	remainingTriacTriggerDelayCounts = delayDuration;
//	setTriacTriggerDelayValues();
//	startTimer2();
//}
//
void setTriacFireDuration(int32_t durationTcnt)
{
	taskENTER_CRITICAL();
	if (durationTcnt < avrTriggerDelayMaxTcnt) {
		if (durationTcnt > 0) {
			triacFireDurationTcnt = durationTcnt;
		}  else {
			triacFireDurationTcnt = 0;
		}
	} else {
		triacFireDurationTcnt = avrTriggerDelayMaxTcnt;
	}
	taskEXIT_CRITICAL();
}

uint32_t getTriacFireDuration()
{
	uint32_t res = 0;
	taskENTER_CRITICAL();
	res = avrTriggerDelayMaxTcnt;
	taskEXIT_CRITICAL();
	return res;
}
//
//void calcAmtInductiveRepetitions(int16_t triacFireDurationTcnt2)
//{
//	if ( inductiveLoad)  {
//		float amtInductiveRepetitionsF = 0.0;
//		float triacFireDurationTcnt2F = triacFireDurationTcnt2;
////		amtInductiveRepetitions = ((triacFireDurationTcnt2 * ( 1  /(11.0592e+6  /128) )) * 1.0e+6  ) /  measuredRepetitionIntervalus;
//		amtInductiveRepetitionsF = (triacFireDurationTcnt2F * 11.63  )  /  measuredRepetitionIntervalus;
//		// always cut off modulo part when converting to int
//		amtInductiveRepetitions = amtInductiveRepetitionsF;   // tobe  debugged
//	} else {
//		amtInductiveRepetitions = 1;
//	}
//
//}
//
//ISR(TIMER2_COMPA_vect)
//{
//	cli();
//	if (remainingTriacTriggerDelayCounts <= 0) {
//		PORTD |= 0x10;
//		delay6pnt2d5us(triacTriggerLength);   // approx 5 us of triac trigger , try later half or even less, measured 7 with oscilloscope
//		PORTD &= ~0x10;			// handled synchronous
//		if ((triacTriggerTimeTcnt2 >= triggerDelayMaxTcnt2) ) {   //  || (amtInductiveRepetitions <= 0)  ) {
//			stopTimer2();
//		} else {
//			startTriacTriggerDelay(delayBetweenTriacTriggers);
//			// --amtInductiveRepetitions;
//		}
//	} else {
//		setTriacTriggerDelayValues();
//	}
//	sei();
//}
//
//ISR(INT0_vect)
//{
//	cli();
//	if ((PIND & 0x04) != 0) {
//		stopTimer2();
//	} else {
//		triacTriggerTimeTcnt2 = 0;
//		if (triacFireDurationTcnt2 > 0)  {
//			startTriacTriggerDelay(  triggerDelayMaxTcnt2 - triacFireDurationTcnt2);
////			calcAmtInductiveRepetitions(triacFireDurationTcnt2);
//		}
//	}
//	sei();
//}
//
//ISR(TIMER0_COMPA_vect)
//{    // needed for ADC so far..
//}
//
//int8_t sec10Counter;
//int8_t shortCircuitSec10Counter;
//uint8_t  shortCircuitAlarmOn;
//int16_t  dValueSec10Counter;
//uint8_t  dValueAlarmOn;
//
//
//ISR(TIMER1_COMPA_vect)
//{
//#ifdef shortCircuitAlarmSupported
//
//		cli();
//		if (shortCircuitSec10Counter > 0)  {
//			-- shortCircuitSec10Counter;
//			if (shortCircuitSec10Counter == 0)  {
//				shortCircuitSec10Counter = -1;
//				shortCircuitAlarmOn = 1;
//				sprintf((char *) &lastFatalErrorString,"short circuit");
//				fatalErrorOccurred = 1;
//			}
//		}
//		if (dValueSec10Counter > 0)  {
//			-- dValueSec10Counter;
//			if (dValueSec10Counter == 0)  {
//				dValueSec10Counter = -1;
//				if (dValueAlarmFatal > 0) {
//					sprintf((char *) &lastFatalErrorString,"DValue low/high");
//					fatalErrorOccurred = 1;
//				} else {
//					dValueAlarmOn = 1;
//				}
//			}
//		}
//		sei();
//		if ((shortCircuitAlarmOn > 0) || (dValueAlarmOn > 0)) {
////			if ((sec10Counter == 5) || (sec10Counter ==  10 ) || (sec10Counter == 3) || (sec10Counter ==  8 )) {sec
//			if ((sec10Counter & 0x1) == 0 )  {
//				toggleCompletionAlarm();
//			}
//		}  else {
//			setCompletionAlarmOff();
//		}
//
//#endif
//	if ( sec10Counter >= 10)  {
//		sec10Counter = 1;
//		secondsDurationTimerRemaining --;
//		secondsInDurationTimer ++;
//		if (secondsDurationTimerRemaining <= 0) {
//			stopDurationTimer();
//			durationTimerReachead = 1;
//		} else {
//			runningSecondsTick = 1;
//		}
//	}  else {
//		++ sec10Counter;
//	}
//}
//

void TIM5_IRQHandler(void)
{

  HAL_TIM_IRQHandler(&htim5);

}

void setDelayTimer(int16_t  len)
{
	//  set ARR
}

void startDelayTimer()
{
	// enable Timer, interrupts and start
}

void stopDelayTimer()
{
	// stop the timer anyhow if passed or not

}

void initTriacTimer()
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	__HAL_RCC_TIM5_CLK_ENABLE();

	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 1000;
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


void EXTI15_10_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(zeroPassPin_Pin) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(zeroPassPin_Pin);
    if (HAL_GPIO_ReadPin(zeroPassPin_GPIO_Port,zeroPassPin_Pin))  {

    }  else  {

    }


  }
}


void initZeroPassDetector()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = zeroPassPin_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(zeroPassPin_GPIO_Port, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void initBuzzerPin()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = buzzerPin_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(buzzerPin_GPIO_Port, &GPIO_InitStruct);
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
	initBuzzerPin();
	initTriacTriggerPin();

}

void startTriacRun()
{
	startADC();
	resetPID();
	// todo start timers and zero pass detector.... pid etc....
	// and stop in stopTriacRun
}

void stopTriacRun()
{
	stopADC();

}

///*
//int16_t  valueFrom6Bit2Complement(int16_t adcV)
//{
//	if (adcV & 0x0200) {
//		adcV--;
//		adcV = ~(adcV | 0xFC00);
//		adcV = - adcV;
//	}
//	return adcV;
//}
//
//int16_t diffADCValue()
//{
//	int16_t res;
//	res = ampsADCValue();
//	res = valueFrom6Bit2Complement(res);
//	return res;
//}
//*/
//





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
	durationTimerOn = 0;
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
	HAL_GPIO_WritePin(buzzerPin_GPIO_Port, buzzerPin_Pin, 1);
}

void setBuzzerOff()
{
	HAL_GPIO_WritePin(buzzerPin_GPIO_Port, buzzerPin_Pin, 1);
}

void toggleBuzzer()
{
	HAL_GPIO_TogglePin(buzzerPin_GPIO_Port, buzzerPin_Pin);
}

void setCompletionAlarmOff()
{
	setBuzzerOn();
}

void setCompletionAlarmOn()
{
	setBuzzerOff();
}

void toggleCompletionAlarm()
{
	toggleBuzzer();
}

void startAmpsADC()
{
	startADC();
}

void stopAmpsADC()
{
	stopADC();
	currentAmpsADCValue = 0;
}

void initTriacIntr()
{
	durationTimerOn = 0;
	currentAmpsADCValue = 0;
	triacFireDurationTcnt = 0;
	initAdc();
	initInterruptsNPorts();
}
