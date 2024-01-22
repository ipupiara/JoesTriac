/*
 * extiCheck.c
 *
 *  Created on: Jan 22, 2024
 *      Author: Brigitte
 */


#define extiCheckAmt  3
#define extiCheckDelay  10
uint8_t extiCheckCnt ;
uint32_t amtIllegalExti;

#define isPinSet(portx, pinx)  (((portx->IDR) & pinx) != 0) ? 1:0

#define  isExtiPinSet()  isPinSet (zeroPass_Port,zeroPass_Pin)


#define  enableExtiCheckTimer()  __HAL_RCC_TIM4_CLK_ENABLE()

#define startExtiCheck()\
	do { \
		extiCheckCnt = 0; \
		triacExtiCheckTimer.Instance->CR1 &= ~(TIM_CR1_CEN);  \
	} while(0)



#define stopExtiCheck()  \
	do { \
		currentExtiState=isExtiPinSet(); \
		extiCheckCnt = 0; \
		triacExtiCheckTimer.Instance->CR1 |= (TIM_CR1_CEN);  \
	} while(0)
#define amtExtiEvChecks   3
uint8_t extiEvCnt;
