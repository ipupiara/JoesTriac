
#include <uart-hw.h>
#include <string.h>
#include <uart-comms.h>
#include <mainJt.h>



UART_HandleTypeDef huart6;


uint8_t  commsError;
osSemaphoreId uartSendSemaphore;
osSemaphoreDef_t  uartSendSemaphoreDef;
uint32_t  rxMsgCounter;
uint32_t  txMsgCounter;

uint32_t debugIdleCounter;

void clearUartInterruptFlags(UART_HandleTypeDef * huart)
{
	__HAL_UART_CLEAR_IT(&huart6,USART_ICR_TCCF_Msk);
	__HAL_UART_CLEAR_IT(&huart6,USART_ICR_IDLECF_Msk);
//	__HAL_UART_CLEAR_IT(&huart6,USART_ICR_TXE_Msk);
}

char txStringBuffer [maxSerialStringSz+1];
uint8_t  txStringBufferPos;
uint8_t  txStringBufferLen;

uint8_t enableUartInterrupts()
{
	uint8_t res = 0;

	clearUartInterruptFlags(&huart6);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
	return res;
}

uint8_t disableUartInterrupts()
{
	uint8_t res = 0;
	HAL_NVIC_DisableIRQ(USART6_IRQn);
	return res;
}

void USART6_IRQHandler(void)
{
    if (__HAL_UART_GET_FLAG(&huart6,USART_ISR_TXE)  )  {
    	huart6.Instance->TDR= txStringBuffer[txStringBufferPos];
        ++ 	txStringBufferPos;
        if (txStringBufferPos == txStringBufferLen) {
        	disableUartInterrupts();
        	osSemaphoreRelease(&uartSendSemaphore);
        }
        //  todo handle errors
//    	__HAL_UART_CLEAR_IT(&huart6,USART_ICR_TXE);
    }
}





uint32_t debugIdleCounter;



void usart6_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOG_CLK_ENABLE();
	  __HAL_RCC_GPIOH_CLK_ENABLE();

	    __HAL_RCC_USART6_CLK_ENABLE();

	    __HAL_RCC_GPIOG_CLK_ENABLE();
	    /**USART6 GPIO Configuration
	    PG14     ------> USART6_TX
	    PG9     ------> USART6_RX
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
	    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}




uint8_t initUartHw()
{
	uint8_t res ;// =  OS_ERR_NONE;

	debugIdleCounter = 0;
	 commsError = 0;

	rxMsgCounter = 0;
	txMsgCounter = 0;

	uartSendSemaphoreDef.name="uart send sema"  ;
	uartSendSemaphoreDef.attr_bits= 0;
	uartSendSemaphoreDef.cb_mem = NULL;
	uartSendSemaphoreDef.cb_size = 0;
	uartSendSemaphore =  osSemaphoreNew(1,1,&uartSendSemaphoreDef);


	 __HAL_RCC_USART6_CLK_ENABLE();

		usart6_GPIO_Init();

		  huart6.Instance = USART6;
//		  huart1.Init.BaudRate = 9600;
		  huart6.Init.BaudRate = 57600;
		  huart6.Init.WordLength = UART_WORDLENGTH_8B;
		  huart6.Init.StopBits = UART_STOPBITS_1;
		  huart6.Init.Parity = UART_PARITY_NONE;
//		  huart6.Init.Mode = UART_MODE_TX_RX;
		  huart6.Init.Mode = UART_MODE_TX;
		  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
		  if (HAL_UART_Init(&huart6) != HAL_OK)
		  {
		    //  Error_Handler();
			  res = 0xFE;
		  }

		  clearUartInterruptFlags(&huart6);
//		  huart6.Instance->CR1 |= USART_CR1_IDLEIE_Msk;
		  huart6.Instance->CR1 |= USART_CR1_TXEIE_Msk;
//		  huart6.Instance->CR1 |= USART_CR1_TCIE_Msk;

		  HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
		  disableUartInterrupts();
	return res;
}

uint8_t startUartHw()
{
	uint8_t res = 0;
//	enableUartInterrupts();
	return res;
}

osStatus_t sendUartString(char* sndStr)
{
	osStatus_t res = 0;
	++ txMsgCounter;
	commsError = 0;
	txStringBufferPos = 0;
	txStringBufferLen = strlen(sndStr);

	strncpy(txStringBuffer,sndStr,maxSerialStringSz);
	enableUartInterrupts();
	// evtl. send first char if interrupt does not get fired now when txe
	return res;
}
