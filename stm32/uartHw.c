///*
// * initUart1.c
// *
// *  Created on: Dec 28, 2023
// *      Author: Brigitte
// */
//
//#include <main.h>
//#include "initUart1.h"
//#include <string.h>
//
//uint32_t txBufferRemain ;
//uint8_t * txBufferPtr  ;
//uint8_t uartBusy;
//
//void setUartJobSemaQ()
//{
//	uartBusy = 0;
//}
//
//uint8_t requireUartJobSemaQ()
//{
//	uint8_t res = 0;
//	if (uartBusy) {
//		res = 0;
//	} else {
//		uartBusy = 1;
//		res = 1;
//	}
//	return res;
//}
//
//#ifdef useOwnUsart
//void USART1_IRQHandler(void)
//#else
//void dummy(void)
//#endif
//{
////	UART_HandleTypeDef * huart = &huart1;
//
//	uint32_t isrflags   = READ_REG(huart1.Instance->ISR);
//	uint32_t cr1its     = READ_REG(huart1.Instance->CR1);
//	uint32_t cr3its     = READ_REG(huart1.Instance->CR3);
//
//	  uint32_t errorflags;
////	  uint32_t errorcode;
////	  uint32_t regval = huart1.Instance->ISR;
//
//	  /* If no error occurs */
//	  errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF));
//
//	  if ((errorflags != 0U)
//	      && (((cr3its & USART_CR3_EIE) != 0U)
//	          || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_RTOIE)) != 0U)))
//	  {
//	    if (((isrflags & USART_ISR_PE) != 0U) && ((cr1its & USART_CR1_PEIE) != 0U))
//	    {
//	      __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_PEF);
//
//	    }
//
//	    if (((isrflags & USART_ISR_FE) != 0U) && ((cr3its & USART_CR3_EIE) != 0U))
//	    {
//	      __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_FEF);
//
//	    }
//	    if (((isrflags & USART_ISR_NE) != 0U) && ((cr3its & USART_CR3_EIE) != 0U))
//	    {
//	      __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_NEF);
//
//	    }
//
//	    if (((isrflags & USART_ISR_ORE) != 0U)
//	        && (((cr1its & USART_CR1_RXNEIE) != 0U) ||
//	            ((cr3its & USART_CR3_EIE) != 0U)))
//	    {
//	      __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_OREF);
//	    }
//
//	    if (((isrflags & USART_ISR_RTOF) != 0U) && ((cr1its & USART_CR1_RTOIE) != 0U))
//	    {
//	      __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_RTOF);
//	    }
//	}
//
//	if ((isrflags & USART_ISR_TXE) != 0U)  {
//	   if (txBufferRemain == 0U)   {
//		   // todo open question why does this get called twice just during first transmit
//		 ATOMIC_CLEAR_BIT(huart1.Instance->CR1, USART_CR1_TXEIE);
//		 ATOMIC_SET_BIT(huart1.Instance->CR1, USART_CR1_TCIE);
//		 setUartJobSemaQ();
//	   }
//	   else {
//		 huart1.Instance->TDR = *txBufferPtr ;
//		 txBufferPtr++;
//		 txBufferRemain--;
//	   }
//	}
//	uint8_t idleDetected = 0;
//
//	if (__HAL_UART_GET_FLAG(&huart1,USART_ISR_IDLE_Msk)  )  {
//		__HAL_UART_CLEAR_IT(&huart1,USART_ISR_IDLE_Msk);
//		idleDetected = 1;
//	//	++ debugIdleCounter;
//	}
//
//	if (__HAL_UART_GET_FLAG(&huart1,USART_ICR_TCCF_Msk)  )  {
//		idleDetected = 1;
//		__HAL_UART_CLEAR_IT(&huart1,USART_ICR_TCCF_Msk);
//	}
//
//	if (idleDetected == 1) {
//
//	#ifdef dmaTxAvailable
//	transferBuffer(fromUartIsr);
//	#endif
//	}
//}
//
//uint8_t UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t size)
//{
//	uint8_t res = 0;
////	HAL_UART_Transmit_IT( huart,pData, Size);
//
//	if (requireUartJobSemaQ() == 1) {
//		txBufferRemain = size;
//		txBufferPtr  =  pData ;
//
//		__HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_TCF);
//		ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_TXEIE);
//		res = 1;
//	}
//	return res;
//}
//
//void initUart1()
//{
//	uartBusy = 0;
//
////	UART_HandleTypeDef* huart = &huart1;
//	 GPIO_InitTypeDef GPIO_InitStruct = {0};
//	 RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
//
//	huart1.Instance = USART1;
//	huart1.Init.BaudRate = 115200;
//	huart1.Init.WordLength = UART_WORDLENGTH_8B;
//	huart1.Init.StopBits = UART_STOPBITS_1;
//	huart1.Init.Parity = UART_PARITY_NONE;
//	huart1.Init.Mode = UART_MODE_TX;
//	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
//	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
////	if (HAL_UART_Init(&huart1) != HAL_OK) {
////		Error_Handler();
////	}
//
//    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
//    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
//      Error_Handler();
//    }
//
//    __HAL_RCC_USART1_CLK_ENABLE();
//
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**USART1 GPIO Configuration
//    PB14     ------> USART1_TX
//    PB15     ------> USART1_RX
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_14;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(USART1_IRQn);
//
//    __HAL_UART_DISABLE(&huart1);
//
//    if (UART_SetConfig(&huart1) == HAL_ERROR)  {
//    	Error_Handler();
//    }
//
//    if (huart1.AdvancedInit.AdvFeatureInit != UART_ADVFEATURE_NO_INIT) {
//        UART_AdvFeatureConfig(&huart1);
//    }
//
//    /* In asynchronous mode, the following bits must be kept cleared:
//    - LINEN and CLKEN bits in the USART_CR2 register,
//    - SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/
//    CLEAR_BIT(huart1.Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
//    CLEAR_BIT(huart1.Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));
//
//    __HAL_UART_ENABLE(&huart1);
//    UART_CheckIdleState(&huart1);
//
//    huart1.Instance->CR1 |= USART_CR1_IDLEIE_Msk;
//    huart1.Instance->CR3 |= USART_CR3_EIE_Msk;
//}
//
//
