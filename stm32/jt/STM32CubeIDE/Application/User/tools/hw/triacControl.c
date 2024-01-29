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

TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim12;

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
