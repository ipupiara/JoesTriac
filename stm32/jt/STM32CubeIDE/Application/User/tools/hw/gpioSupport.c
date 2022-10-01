
#include <gpioSupport.h>
#include <StateClass.h>



//INT8U   keyPressDelayResetValue;
//INT8U   keyPressDelay;

//OS_TMR  * keyPressedTimer;

TIM_HandleTypeDef htim5;

#define __HAL_GPIO_EXTI_ENABLE_IT(__EXTI_LINE__) (EXTI->IMR |= (__EXTI_LINE__))

#define clutchBreak_Pin GPIO_PIN_1
#define clutchBreak_GPIO_Port GPIOG
#define clutchBreak_EXTI_IRQn EXTI1_IRQn
#define set_acc_Pin GPIO_PIN_2
#define set_acc_GPIO_Port GPIOG
#define set_acc_EXTI_IRQn EXTI2_IRQn
#define res_dec_Pin GPIO_PIN_3
#define res_dec_GPIO_Port GPIOG
#define res_dec_EXTI_IRQn EXTI3_IRQn
#define on_off_Pin GPIO_PIN_4
#define on_off_GPIO_Port GPIOG
#define on_off_EXTI_IRQn EXTI4_IRQn
#define speedometer_Pin GPIO_PIN_5
#define speedometer_GPIO_Port GPIOG
#define speedometer_EXTI_IRQn EXTI9_5_IRQn

#define tim5PrescalerValue 216   //  define it so that 1 sec = 1*E+6 counter values
#define tim5PeriodValue  500000   //  stop counting after 0.5 sec

//INT8U   setAccPressed;
//INT8U   resDecPressed;
//INT8U   switchedOn;
//INT8U	switchedOff;
//INT8U   clutchBreakPressed;
//
//INT8U  turnCounter;
//
//void setKeyEvent(INT8U kyEv)
//{
//	if (keyPressDelay == 0) {
//		postTempixEvent(kyEv);
//		keyPressDelay = keyPressDelayResetValue;
//	}
//}
//
//void EXTI1_IRQHandler(void)
//{
//	if(__HAL_GPIO_EXTI_GET_IT(clutchBreak_Pin) != RESET) {
//	    __HAL_GPIO_EXTI_CLEAR_IT(clutchBreak_Pin);
//	    setKeyEvent(evClutchBreakPressed);
//	}
//}
//
//void EXTI2_IRQHandler(void)   // set_acc_Pin
//{
//	if(__HAL_GPIO_EXTI_GET_IT(set_acc_Pin) != RESET) {
//	    __HAL_GPIO_EXTI_CLEAR_IT(set_acc_Pin);
//	    setKeyEvent(evAccSetPressed);
//	}
//}
//
//
//void EXTI3_IRQHandler(void)      //   res_dec_Pin
//{
//	if(__HAL_GPIO_EXTI_GET_IT(res_dec_Pin) != RESET)  {
//		__HAL_GPIO_EXTI_CLEAR_IT(res_dec_Pin);
//		setKeyEvent(evDecResPressed);
//	}
//}
//
//
//void EXTI4_IRQHandler(void)    //  on_off_Pin
//{
//	if(__HAL_GPIO_EXTI_GET_IT(on_off_Pin) != RESET) {
//		__HAL_GPIO_EXTI_CLEAR_IT(on_off_Pin);
//		if (on_off_GPIO_Port->IDR &  on_off_Pin   )  {   //  todo tobe tested is this way or versa polarity of pin
//			setKeyEvent(evSwitchedOn);
//		}  else {
//			setKeyEvent(evSwitchedOn);
//		}
//	}
//}
//
//void EXTI9_5_IRQHandler(void)   //  speedometer_Pin
//{
//	INT32U spd;
//	  if(__HAL_GPIO_EXTI_GET_IT(speedometer_Pin) != RESET) {
//			__HAL_GPIO_EXTI_CLEAR_IT(speedometer_Pin);
//			spd = __HAL_TIM_GET_COUNTER(&htim5);
//			__HAL_TIM_SET_COUNTER(&htim5,0);
//			++ turnCounter;
//			if (turnCounter >= turnsToMeasure) {
//				if (getSpeedTimeout() != 0){
//					setSpeedTimeout(1);
//					spd = 0;
//				}  else { }
//				setCurrentSpeedInv(spd);
//				turnCounter = 0;
//			}
//	  }
//}
//
//void TIM5_IRQHandler(void)
//{
//	if(__HAL_TIM_GET_FLAG(&htim5, TIM_FLAG_UPDATE) != RESET)
//	  {
//	    if(__HAL_TIM_GET_IT_SOURCE(&htim5, TIM_IT_UPDATE) !=RESET)
//	    {
//	      __HAL_TIM_CLEAR_IT(&htim5, TIM_IT_UPDATE);
//	      setSpeedTimeout(1);
//	    }
//	  }
//}
//
//static void MX_GPIO_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOH_CLK_ENABLE();
//  __HAL_RCC_GPIOG_CLK_ENABLE();
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//
//  /*Configure GPIO pins : set_acc_Pin res_dec_Pin on_off_Pin */
//  GPIO_InitStruct.Pin = set_acc_Pin|res_dec_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
//
//  GPIO_InitStruct.Pin = clutchBreak_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(clutchBreak_GPIO_Port, &GPIO_InitStruct);
//
//  /*Configure GPIO pins : set_acc_Pin res_dec_Pin on_off_Pin */
//  GPIO_InitStruct.Pin = on_off_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
//
//
//  /*Configure GPIO pin : speedometer_Pin */
//  GPIO_InitStruct.Pin = speedometer_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(speedometer_GPIO_Port, &GPIO_InitStruct);
//
//  /* EXTI interrupt init*/
//  BSP_IntVectSet (clutchBreak_EXTI_IRQn,tempixIsrPrioLevel,CPU_INT_KA,EXTI1_IRQHandler);
//  BSP_IntEnable(clutchBreak_EXTI_IRQn);
//  __HAL_GPIO_EXTI_ENABLE_IT(clutchBreak_Pin);
//
//  BSP_IntVectSet (set_acc_EXTI_IRQn,tempixIsrPrioLevel,CPU_INT_KA,EXTI2_IRQHandler);
//  BSP_IntEnable(set_acc_EXTI_IRQn);
//  __HAL_GPIO_EXTI_ENABLE_IT(set_acc_Pin);
//
//  BSP_IntVectSet (res_dec_EXTI_IRQn,tempixIsrPrioLevel,CPU_INT_KA,EXTI3_IRQHandler);
//  BSP_IntEnable(res_dec_EXTI_IRQn);
//  __HAL_GPIO_EXTI_ENABLE_IT(res_dec_Pin);
//
//  BSP_IntVectSet (on_off_EXTI_IRQn,tempixIsrPrioLevel,CPU_INT_KA,EXTI4_IRQHandler);
//  BSP_IntEnable(on_off_EXTI_IRQn);
//  __HAL_GPIO_EXTI_ENABLE_IT(on_off_Pin);
//
//  BSP_IntVectSet (speedometer_EXTI_IRQn,tempixIsrPrioLevel,CPU_INT_KA,EXTI9_5_IRQHandler);
//  BSP_IntEnable(speedometer_EXTI_IRQn);
//  __HAL_GPIO_EXTI_ENABLE_IT(speedometer_Pin);
//
//}
//
//
//void startTim5()
//{
//	__HAL_TIM_ENABLE(&htim5);
//}
//
//void stopTim5()
//{
//	__HAL_TIM_DISABLE(&htim5);
//}
//
//static void MX_TIM5_Init(void)
//{
//  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//  TIM_MasterConfigTypeDef sMasterConfig = {0};
//
//  htim5.Instance = TIM5;
//  htim5.Init.Prescaler = tim5PrescalerValue;
//  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim5.Init.Period = tim5PeriodValue;
//  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
//  if (HAL_TIM_Base_Init(&htim5) != HAL_OK) {
//   }
//  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK) {
//
//  }
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK) {
//
//  }
//  BSP_IntVectSet (TIM5_IRQn,tempixIsrPrioLevel,CPU_INT_KA,TIM5_IRQHandler);
//  BSP_IntEnable(TIM5_IRQn);
//  __HAL_TIM_ENABLE_IT(&htim5,TIM_IT_UPDATE);
//  __HAL_TIM_URS_ENABLE(&htim5);   //  only overflow as updatte event
//  //  create start stop methods
//  //  check priorities, preemption grouping allover the project
//}
//
//
//void keyPressedTimerCallback( void *parg)
//{
//    if (keyPressDelay > 0) {
//    	--keyPressDelay;
//    }
//}
//
//void initGpioSupport ()
//{
//	INT8U err;
//	if ((OS_TMR_CFG_TICKS_PER_SEC / 10) > 1 )  {
//		keyPressDelayResetValue  = OS_TMR_CFG_TICKS_PER_SEC / 10;
//	} else {
//		keyPressDelayResetValue = 2;  // at least one full timer tick cycle
//	}
//	keyPressedTimer = OSTmrCreate( 1,1,OS_TMR_OPT_PERIODIC,
//								(OS_TMR_CALLBACK)keyPressedTimerCallback,NULL,
//										(INT8U *)"workingTimer",&err);
//   setAccPressed = 0;
//   resDecPressed = 0;
//   switchedOn = 0;
//   switchedOff = 0;
//   turnCounter = 0;
//   MX_GPIO_Init();
//   MX_TIM5_Init();
//}
//
//INT8U isClutchBreakReleased()
//{
//	INT8U res = 0;
//	res = HAL_GPIO_ReadPin(clutchBreak_GPIO_Port,clutchBreak_Pin);
//	return res;
//}
//
//INT8U isTempixSwitchedOn()
//{
//	//  todo tobe tested is this way or versa polarity of pin
//	INT8U res = 0;
//	if (HAL_GPIO_ReadPin(on_off_GPIO_Port,on_off_Pin)) {
//		res = 0;
//	} else {
//		res = 1;
//	}
//	return res;
//}
//
