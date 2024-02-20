
#include <uart-hw.h>
#include <string.h>
#include <uart-comms.h>
#include <mainJt.h>
#include <dma-tools.h>

uint32_t txBufferRemain ;
uint8_t * txBufferPtr  ;

UART_HandleTypeDef huart1;

osMessageQueueId_t uartSendSemaphoreQ;
uint8_t  uartJobSemSet;

uint32_t debugCharCnt, debugStringCnt;

#define receiveBookSize  0x400
uint8_t receiveBook[receiveBookSize];
uint32_t rBookCnt;

void addToTextBook(uint8_t ch)
{
	if (rBookCnt <  receiveBookSize) {
		receiveBook[rBookCnt] = ch;
		++rBookCnt;
	}
}

void initReceiveTextBook()
{
	memset(receiveBook,0x5a,sizeof(receiveBook));
	memset(receiveBook,0,sizeof(receiveBook));
	rBookCnt = 0;
}
void setUartJobSemaQ()
{
#ifndef debugApp
	uint32_t dummy = 0x5a;
	if (uartJobSemSet == 0)  {    // prevent multiple events by irqs
		osMessageQueuePut(uartSendSemaphoreQ, &dummy, 0, 0);   //  after semaphores do not work from isr ?????  replaced by queue as done in OSWrappers.cpp
		uartJobSemSet = 1;
	} else  {
			uartJobSemSet = 2;  //  just for debugging
	}
#endif
}


void USART1_IRQHandler(void)
{
//	UART_HandleTypeDef * huart = &huart1;

	uint32_t isrflags   = READ_REG(huart1.Instance->ISR);
	uint32_t cr1its     = READ_REG(huart1.Instance->CR1);
	uint32_t cr3its     = READ_REG(huart1.Instance->CR3);

	  uint32_t errorflags;
//	  uint32_t errorcode;
//	  uint32_t regval = huart1.Instance->ISR;

	  /* If no error occurs */
	  errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF));

	  if ((errorflags != 0U)
	      && (((cr3its & USART_CR3_EIE) != 0U)
	          || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_RTOIE)) != 0U)))
	  {
	    if (((isrflags & USART_ISR_PE) != 0U) && ((cr1its & USART_CR1_PEIE) != 0U))
	    {
	      __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_PEF);

	    }

	    if (((isrflags & USART_ISR_FE) != 0U) && ((cr3its & USART_CR3_EIE) != 0U))
	    {
	      __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_FEF);

	    }
	    if (((isrflags & USART_ISR_NE) != 0U) && ((cr3its & USART_CR3_EIE) != 0U))
	    {
	      __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_NEF);

	    }

	    if (((isrflags & USART_ISR_ORE) != 0U)
	        && (((cr1its & USART_CR1_RXNEIE) != 0U) ||
	            ((cr3its & USART_CR3_EIE) != 0U)))
	    {
	      __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_OREF);
	    }

	    if (((isrflags & USART_ISR_RTOF) != 0U) && ((cr1its & USART_CR1_RTOIE) != 0U))
	    {
	      __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_RTOF);
	    }
	}

	  if(((isrflags & USART_ISR_TXE) != 0U)
	        && ((cr1its & USART_CR1_TXEIE) != 0U))
	  {
		   if (txBufferRemain == 0U)   {
			   // todo open question why does this get called twice just during first transmit
			 ATOMIC_CLEAR_BIT(huart1.Instance->CR1, USART_CR1_TXEIE);
			 __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_TCF);
			 ATOMIC_SET_BIT(huart1.Instance->CR1, USART_CR1_TCIE);
	   }
	   else {
			 huart1.Instance->TDR = *txBufferPtr ;
			 addToTextBook(*txBufferPtr);
			 txBufferPtr++;
			 txBufferRemain--;
			 ++debugCharCnt;
	   }
	}
	uint8_t idleDetected = 0;

//	if (__HAL_UART_GET_FLAG(&huart1,USART_ISR_IDLE_Msk)  )  {
//		__HAL_UART_CLEAR_IT(&huart1,USART_ISR_IDLE_Msk);
//		idleDetected = 1;
//	//	++ debugIdleCounter;
//	}

	if (__HAL_UART_GET_FLAG(&huart1,USART_ICR_TCCF_Msk)  )  {
		__HAL_UART_CLEAR_IT(&huart1,USART_ICR_TCCF_Msk);
		idleDetected = 1;
		ATOMIC_CLEAR_BIT(huart1.Instance->CR1, USART_CR1_TCIE);
		huart1.Instance->CR3 &= ~USART_CR3_EIE_Msk;
	}

	if (idleDetected == 1) {
		 setUartJobSemaQ();
	}
}

uint8_t UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t size)
{
	uint8_t res = 0;
	txBufferRemain = size;
	txBufferPtr  =  pData ;
	huart1.Instance->CR3 |= USART_CR3_EIE_Msk;
	__HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_TCF);
	ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_TXEIE);
	res = 1;
	return res;
}

uint8_t startUartHw()
{
	return 0;
}

osStatus_t sendUartString(char* sndStr)
{
	osStatus_t res = osOK;
	++ debugStringCnt;
	UART_Transmit(&huart1, (uint8_t*) sndStr, strlen(sndStr));

	return res;
}

//void uartTest()
//{
//	char * teststr  = "hello you there, hee duu tettä\n";
//	  while (1)
//	  {
//		  uint32_t halVersion = HAL_GetHalVersion();
//		  UNUSED(halVersion);
//			uint32_t lastUwTick  = uwTick;
//			do {
//				do {} while (((uwTick - lastUwTick) < 100 ) || (uwTick == 0) );
//				sendUartString(teststr);
////				UART_Transmit( &huart1,(uint8_t *) teststr, strlen(teststr));
//				lastUwTick = uwTick;
//			}   while (1);
//	  }
//}



//   todo check looping interrupts in the whole application ( --->>>>>>>  UI not reacting !!!!!!!!!!!!!!!!!!!!!!!!!!)

uint8_t initUartHw()
{
	uint8_t res = 0;

	debugCharCnt = debugStringCnt = 0;
	initReceiveTextBook();

	uartJobSemSet = 0;
	#ifndef debugApp
		uartSendSemaphoreQ =  osMessageQueueNew(1,4, NULL);
		if (uartSendSemaphoreQ  == NULL)   {
			errorHandler(0xff ,stop," osMessageQueueNew ","initUartHw");
		}
		setUartJobSemaQ();
	#endif

//	UART_HandleTypeDef* huart = &huart1;
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200; //19200;  115200; 57600; 9600
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//	if (HAL_UART_Init(&huart1) != HAL_OK) {
//		Error_Handler();
//	}

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
//      Error_Handler();
    }

    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB14     ------> USART1_TX
    PB15     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    ATOMIC_CLEAR_BIT(huart1.Instance->CR1, (USART_CR1_IDLEIE));
    ATOMIC_CLEAR_BIT(huart1.Instance->CR1, (USART_CR1_TXEIE));  // todo put into better structure macros

//    uint8_t ena = NVIC_GetEnableIRQ(USART1_IRQn);
    HAL_NVIC_SetPriority(USART1_IRQn, triacApplicationIsrPrio, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
//    ena = NVIC_GetEnableIRQ(USART1_IRQn);

    __HAL_UART_DISABLE(&huart1);

    if (UART_SetConfig(&huart1) == HAL_ERROR)  {
//    	Error_Handler();
    }

    if (huart1.AdvancedInit.AdvFeatureInit != UART_ADVFEATURE_NO_INIT) {
        UART_AdvFeatureConfig(&huart1);
    }

    /* In asynchronous mode, the following bits must be kept cleared:
    - LINEN and CLKEN bits in the USART_CR2 register,
    - SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/
    CLEAR_BIT(huart1.Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
    CLEAR_BIT(huart1.Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

    ATOMIC_CLEAR_BIT(huart1.Instance->CR1, (USART_CR1_IDLEIE));
    __HAL_UART_ENABLE(&huart1);
    UART_CheckIdleState(&huart1);

//    huart1.Instance->CR3 |= USART_CR3_EIE_Msk;
    huart1.Instance->CR3 |= USART_CR3_EIE_Msk;
#ifdef debugApp
    uartTest();
#endif
    return res;
}











//
//enum {
//	fromHalfTransferCompleteIsr = 1,
//	fromTransferCompleteIsr,
//	fromUartIsr
//};
//
//UART_HandleTypeDef huart;
//
//#ifdef dmaTxAvailable
//DMA_HandleTypeDef hdma_usart_tx;
//DMA_HandleTypeDef hdma_usart_rx;
//
//extern uint16_t  feCounter;
//extern uint16_t  teCounter;
//extern uint16_t  dmeCounter;
//#endif
//
//
//uint8_t  commsError;
//osMessageQueueId_t uartSendSemaphoreQ;
//uint32_t  rxMsgCounter;
//uint32_t  txMsgCounter;
//
//char txStringBuffer [maxSerialStringSz+1];
//uint8_t *   txStringBufferPtr;
//uint8_t  txStringBufferRemain;
//
//uint32_t debugIdleCounter;
//uint8_t  uartJobSemSet;
//
//#define bufferCounterType uint8_t
//bufferCounterType amtWrittenStringChars;
//
//void uartTriacTest();
//
////void clearUartInterruptFlags(UART_HandleTypeDef * phuart)
////{
////	__HAL_UART_CLEAR_IT(phuart,USART_ICR_TCCF_Msk);
////	__HAL_UART_CLEAR_IT(phuart,USART_ICR_IDLECF_Msk);
////}
//
////uint8_t enableUartInterrupts()
////{
////	uint8_t res = 0;
////	clearUartInterruptFlags(&huart);
////#ifdef dmaTxAvailable
////	enableAllDmaInterrupts(&hdma_usart_tx,withoutHT);
////	HAL_NVIC_EnableIRQ(txDMA_Stream_IRQn);
//////	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
////#endif
////	huart.Instance->CR1 |= USART_CR1_IDLEIE_Msk;
////	huart.Instance->CR1 |= USART_CR1_TCIE_Msk;
////	huart.Instance->CR3 |= USART_CR3_EIE_Msk;      // error interrupt endable
////	HAL_NVIC_EnableIRQ(USART_IRQn);
////	return res;
////}
//
////uint8_t disableUartInterrupts()
////{
////	uint8_t res = 0;
////	HAL_NVIC_DisableIRQ(USART_IRQn);
////#ifdef dmaTxAvailable
////	HAL_NVIC_DisableIRQ(txDMA_Stream_IRQn);
//////	HAL_NVIC_DisableIRQ(DMA1_Stream5_IRQn);
////#endif
////	return res;
////}
//
//
//
////void resetDebugArray()
////{
////	memset(debugArray,0x00,sizeof(debugArray));
////	debugArrayCnt = 0;
////}
//
////void addToDebugArray(uint8_t from,uint8_t val)
////{
/////*	if (debugArrayCnt < debugArraySize+3) {
////		debugArray[debugArrayCnt] = from;
////		debugArray[debugArrayCnt+1] = val;
////		debugArrayCnt += 3;
////	}*/
////}
//uint8_t  receiveStringBuffer  [maxUartReceiveDmaStringSize + 1];
//
//#ifdef dmaTxAvailable
//
//
//#define halfDmaRxBufferSize   0x20
//#define fullDmaRxBufferSize 2 * halfDmaRxBufferSize
//#define endBufferPosition ( fullDmaRxBufferSize - 1 )
//
//
//#define debugArraySize 0xFF
//uint8_t  debugArray [debugArraySize];
//uint8_t  debugArrayCnt;
//
//typedef struct
//{
//  uint8_t firsthalf[halfDmaRxBufferSize];
//  uint8_t secondhalf[halfDmaRxBufferSize];
//} halfBufferTwice_Buffer_Type;
//
//typedef union
//{
//	halfBufferTwice_Buffer_Type halfTwicebuffer;
//	uint8_t byteBuffer [fullDmaRxBufferSize];
//} DMA_Buffer_Type;
//
//DMA_Buffer_Type dmaBuffer;
//
//bufferCounterType lastNdtr;
//
//
//void resetDmaBuffer()
//{
//	lastNdtr = fullDmaRxBufferSize;
//	memset(&dmaBuffer,0x00,sizeof(dmaBuffer));
//}
//
//void transferBuffer(uint8_t  tobeForwardedFrom)
//{
//	bufferCounterType newNdtr= (uint32_t)  (hdma_usart_rx.Instance->NDTR);
//	bufferCounterType amtRcvd;
//
//	if (tobeForwardedFrom == fromTransferCompleteIsr)  {
//		amtRcvd = lastNdtr;     // newNdtr already set  to buffer size
//	}  else {
//		amtRcvd =   lastNdtr  - newNdtr;
//	}
//
//	uint8_t receivedAt = fullDmaRxBufferSize - lastNdtr;
//
//	bufferCounterType amtCpy = amtRcvd;
//	// evtl if inTc ndtr might already be reset ??
//
//	if (amtWrittenStringChars < maxUartReceiveDmaStringSize )  {
//		if (amtWrittenStringChars + amtRcvd > maxUartReceiveDmaStringSize) {
//			amtCpy = maxUartReceiveDmaStringSize  - amtWrittenStringChars -1;
//		}
//	} else {
//		amtCpy = 0;
//	}
//
//	if (amtCpy > 0) {
//		for (uint8_t cnt =0; cnt < amtCpy;++cnt) {
//			receiveStringBuffer[amtWrittenStringChars + cnt] = dmaBuffer.byteBuffer[receivedAt + cnt];
//		}
//	//	strncpy(stringBuffer[amtWrittenStringChars],dmaBuffer[amtReadBufferChars],amtCpy);
//	}
//	//	memcpy(&stringBuffer[amtWrittenStringChars],&dmaBuffer.byteBuffer[amtReadBufferChars],amtCpy);
//
//	lastNdtr = newNdtr;
//	amtWrittenStringChars += amtCpy;
////	addToDebugArray(tobeForwardedFrom,amtWrittenStringChars);
//
//	if (tobeForwardedFrom == fromTransferCompleteIsr)  {
//		resetDmaBuffer();
//	} else if (tobeForwardedFrom == fromUartIsr) {
//		if (amtWrittenStringChars > 0) {
//			forwardReceivedStringBuffer((char*)receiveStringBuffer);
//			resetStringBuffer();
//		}
//	}
//}
//
//void txDMA_Stream_IRQHandler(void)   // TX
//{
//	if (__HAL_DMA_GET_FLAG(&hdma_usart_tx,txDMA_FLAG_TCIF) != 0)  {
//		setUartJobSemaQ();
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart_tx,txDMA_FLAG_TCIF);
//	}
//
//    if (__HAL_DMA_GET_FLAG(&hdma_usart_tx,txDMA_FLAG_HTIF) != 0)  {
//    	__HAL_DMA_CLEAR_FLAG(&hdma_usart_tx,txDMA_FLAG_HTIF);
//    }
//
//	if ((__HAL_DMA_GET_FLAG(&hdma_usart_tx,txDMA_FLAG_TEIF))
//								| (__HAL_DMA_GET_FLAG(&hdma_usart_tx,txDMA_FLAG_FEIF))
//								| (__HAL_DMA_GET_FLAG(&hdma_usart_tx,txDMA_FLAG_DMEIF))) {
//		errorHandler((uint32_t)1 ,goOn," DMA_FLAG_TEIF2_6 "," txDMA_Stream_IRQHandler ");
//	  //   todo carefully abort job and deinit if possible
//
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart_tx,txDMA_FLAG_TEIF);
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart_tx,txDMA_FLAG_FEIF);
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart_tx,txDMA_FLAG_DMEIF);
//	}
//}
//
//
////void DMA1_Stream5_IRQHandler(void)  // RX
////{
////	if (__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_TCIF1_5) != 0)  {
////		transferBuffer(fromTransferCompleteIsr);
////		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_TCIF1_5);
////	}
////
////
////    if (__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_HTIF1_5) != 0)  {
////    	transferBuffer(fromHalfTransferCompleteIsr);
////    	__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_HTIF1_5);
////    }
////
////	if ((__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_TEIF1_5))
////								| (__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_FEIF1_5))
////								| (__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_DMEIF1_5))) {
////		errorHandler((uint32_t)0 ,goOn," DMA_FLAG_TEIF1_5 "," DMA2_Stream2_IRQHandler ");
////		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_TEIF1_5);
////		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_FEIF1_5);
////		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_DMEIF1_5);
////	}
////}
//
//
//
//void startCircReceiver()
//{
//	DMA_SetTransferConfig(&hdma_usart_rx, (uint32_t)&huart.Instance->RDR , (uint32_t)&dmaBuffer, sizeof(dmaBuffer));
//	clearDmaInterruptFlags(&hdma_usart_rx);
//	__HAL_DMA_ENABLE(&hdma_usart_rx);
//}
//
//
//void usartDmaInit()
//{
//	usart__HAL_RCC_DMA_CLK_ENABLE();
//	//	    hdma_usart2_rx.Instance = DMA1_Stream5;
//	//	    hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
//	//	    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//	//	    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//	//	    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
//	//	    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//	//	    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//	//	    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
//	//	    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
//	//	    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//	//	    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
//	//	    {
//	//	      errorHandler(0xfe,goOn," HAL_DMA_Init 1 ","initUartHw");
//	//	    }
//	//
//	//	    __HAL_LINKDMA(huart,hdmarx,hdma_usart2_rx);
//
//
//	/* USART2_TX Init */
//	hdma_usart_tx.Instance = txDMA_Stream;
//	hdma_usart_tx.Init.Channel = txDMA_CHANNEL;
//	hdma_usart_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//	hdma_usart_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//	hdma_usart_tx.Init.MemInc = DMA_MINC_ENABLE;
//	hdma_usart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//	hdma_usart_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//	hdma_usart_tx.Init.Mode = DMA_NORMAL;
//	hdma_usart_tx.Init.Priority = DMA_PRIORITY_LOW;
//	hdma_usart_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//	if (HAL_DMA_Init(&hdma_usart_tx) != HAL_OK)
//	{
//		errorHandler(0xfe,goOn," HAL_DMA_Init 2 ","initUartHw");
//	}
//	__HAL_LINKDMA(&huart,hdmatx,hdma_usart_tx);
//
//	//	huart2.Instance->CR3 |= (USART_CR3_DMAR_Msk | USART_CR3_DMAT_Msk);
//		huart.Instance->CR3 |=  USART_CR3_DMAT_Msk;
//
//	//	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
//		HAL_NVIC_SetPriority(txDMA_Stream_IRQn, triacApplicationIsrPrio, 0);
//}
//
//
//#endif
//
//
//void resetStringBuffer()
//{
//	amtWrittenStringChars = 0;
//	memset(receiveStringBuffer,0x00,sizeof(receiveStringBuffer));
//}
//
//
//
//
//void setUartJobSemaQ()
//{
//#ifndef debugApp
//	uint32_t dummy = 0x5a;
//	if (uartJobSemSet == 0)  {    // prevent multiple events by irqs
//		osMessageQueuePut(uartSendSemaphoreQ, &dummy, 0, 0);   //  after semaphores do not work from isr ?????  replaced by queue as done in OSWrappers.cpp
//		uartJobSemSet = 1;
//	} else  {
//			uartJobSemSet = 2;  //  just for debugging
//	}
//#endif
//}
//
//
////void USART_IRQHandler(void)
//void USART1_IRQHandler(void)
//{
////	UART_HandleTypeDef *huart
////uint32_t isrflags   = READ_REG(huart->Instance->ISR);
//
//	uint32_t isrflags   = READ_REG(huart.Instance->ISR);
////	uint32_t cr1its     = READ_REG(huart.Instance->CR1);
////	uint32_t cr3its     = READ_REG(huart.Instance->CR3);
//
//	uint32_t errorflags;
////	uint32_t errorcode;
//	errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF));
//
//	if (errorflags != 0)   {
//		// todo continue implementation
//		commsError = 1;
//	    if (__HAL_UART_GET_FLAG(&huart,USART_ISR_PE_Msk)  )  {
//	       	__HAL_UART_CLEAR_IT(&huart,USART_ISR_PE_Msk);
//		}
//	    if (__HAL_UART_GET_FLAG(&huart,USART_ISR_FE_Msk)  )  {
//	   	       	__HAL_UART_CLEAR_IT(&huart,USART_ISR_FE_Msk);
//	   	}
//	    if (__HAL_UART_GET_FLAG(&huart,USART_ISR_ORE_Msk)  )  {
//	   	       	__HAL_UART_CLEAR_IT(&huart,USART_ISR_ORE_Msk);
//	   	}
//
//	    if (__HAL_UART_GET_FLAG(&huart,USART_ISR_NE_Msk)  )  {
//	    	__HAL_UART_CLEAR_IT(&huart,USART_ISR_NE_Msk);
//	    }
//	    if (__HAL_UART_GET_FLAG(&huart,USART_ISR_RTOF_Msk)  )  {
//	    	__HAL_UART_CLEAR_IT(&huart,USART_ISR_RTOF_Msk);
//	    }
//	}
//
//    if (__HAL_UART_GET_FLAG(&huart,USART_ISR_TXE)  )  {
//    	__HAL_UART_CLEAR_IT(&huart,USART_ISR_TXE);
//
//        if (txStringBufferRemain == 0) {
//			 ATOMIC_CLEAR_BIT(huart.Instance->CR1, USART_CR1_TXEIE);
//			 ATOMIC_SET_BIT(huart.Instance->CR1, USART_CR1_TCIE);
//			 setUartJobSemaQ();
//        } else {
//			huart.Instance->TDR= *txStringBufferPtr ;;
//			++ 	txStringBufferPtr;
//			--  txStringBufferRemain;
//		}
//    }
//
//	uint8_t idleDetected = 0;
//
//    if (__HAL_UART_GET_FLAG(&huart,USART_ISR_IDLE_Msk)  )  {
//    	__HAL_UART_CLEAR_IT(&huart,USART_ISR_IDLE_Msk);
//    	idleDetected = 1;
//    	++ debugIdleCounter;
//     }
//    if (__HAL_UART_GET_FLAG(&huart,USART_ICR_TCCF_Msk)  )  {
//    	idleDetected = 1;
//    	__HAL_UART_CLEAR_IT(&huart,USART_ICR_TCCF_Msk);   // for timing reasons so far only tx without dma, rx tobe implemented later
//    }
//
//    if (idleDetected == 1) {
//#ifdef dmaTxAvailable
//	    transferBuffer(fromUartIsr);
//#endif
//    }
//
//}
//
//
//uint32_t debugIdleCounter;
//
//uint8_t initUartHw()
//{
//	uint8_t res = osOK ;
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
//
//	debugIdleCounter = 0;
//	 commsError = 0;
//	rxMsgCounter = 0;
//	txMsgCounter = 0;
//	uartJobSemSet = 0;
//#ifndef debugApp
//	uartSendSemaphoreQ =  osMessageQueueNew(3,4, NULL);
//	if (uartSendSemaphoreQ  == NULL)   {
//		errorHandler(0xff ,stop," osMessageQueueNew ","initUartHw");
//	}
//	setUartJobSemaQ();
//#endif
//
//#ifdef dmaTxAvailable
//	resetStringBuffer();
//	usartDmaInit();
//#endif
//
//	huart.Instance = USART_Number;
//	huart.Init.BaudRate = 115200;  // 57600;      //   115200;
//	huart.Init.WordLength = UART_WORDLENGTH_8B;
//	huart.Init.StopBits = UART_STOPBITS_1;
//	huart.Init.Parity = UART_PARITY_NONE;
//	huart.Init.Mode = UART_MODE_TX;
//	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//	huart.Init.OverSampling = UART_OVERSAMPLING_16;
//	huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//	huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//	if (HAL_UART_Init(&huart) != HAL_OK)
//	{
//		errorHandler(0xfe,goOn," HAL_UART_Init","initUartHw");
//	}
//
//	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART;
//	PeriphClkInitStruct.UsartClockSelection = RCC_USARTCLKSOURCE_PCLK;
//	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)  {
//		errorHandler(0xfe,goOn," HAL_RCCEx_PeriphCLKConfig  ","initUartHw");
//	}
//
//	    /* Peripheral clock enable */
//	usart__HAL_RCC_USART_CLK_ENABLE();
//
//	usart__HAL_RCC_GPIO_CLK_ENABLE();
//
//	GPIO_InitStruct.Pin = txGPIO_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	GPIO_InitStruct.Alternate = GPIO_AF_USART;
//	HAL_GPIO_Init(uartPort, &GPIO_InitStruct);
//
//	HAL_NVIC_SetPriority(USART_IRQn, triacApplicationIsrPrio, 0);
//	HAL_NVIC_EnableIRQ(USART_IRQn);
//
//	 __HAL_UART_DISABLE(&huart);
//
//	if (UART_SetConfig(&huart) == HAL_ERROR)  {
//		errorHandler(0xfe,goOn," UART_SetConfig  ","initUartHw");
//	}
//
//	if (huart.AdvancedInit.AdvFeatureInit != UART_ADVFEATURE_NO_INIT) {
//		UART_AdvFeatureConfig(&huart);
//	}
//
//	/* In asynchronous mode, the following bits must be kept cleared:
//	- LINEN and CLKEN bits in the USART_CR2 register,
//	- SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/
//	CLEAR_BIT(huart.Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
//	CLEAR_BIT(huart.Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));
//
//	__HAL_UART_ENABLE(&huart);
//	UART_CheckIdleState(&huart);
//
//	huart.Instance->CR1 |= USART_CR1_IDLEIE_Msk;
//	huart.Instance->CR3 |= USART_CR3_EIE_Msk;
//
//#ifdef debugApp
//	uartTriacTest();
//#endif
//	return res;
//}
//
//
//osStatus_t sendUartString(char* sndStr) {
//	uint8_t res = 0;
//	++ txMsgCounter;
//	commsError = 0;
//
//    uint8_t ena = __NVIC_GetEnableIRQ(USART1_IRQn);
//
//#ifdef dmaTxAvailable
//	DMA_SetTransferConfig(&hdma_usart_tx, (uint32_t)sndStr, (uint32_t)&huart.Instance->TDR, strlen(sndStr));
//	clearDmaInterruptFlags(&hdma_usart_tx);
//	__HAL_DMA_ENABLE(&hdma_usart_tx);
//#else
//	txStringBufferRemain = strlen(sndStr);
//	txStringBufferPtr = (uint8_t*) sndStr;
//
//	__HAL_UART_CLEAR_FLAG(&huart, UART_CLEAR_TCF);
//	ATOMIC_SET_BIT(huart.Instance->CR1, USART_CR1_TXEIE);
//#endif
//	return res;
//}
//
//
//
//#ifdef debugApp
//void uartTriacTest()
//{
//	uint32_t lastUwTick  = uwTick;
//	char * teststr  = "hello you there, hee duu tettä\n";
//
//	do {
//		do {} while (((uwTick - lastUwTick) < 100 ) || (uwTick == 0xffffffff) );
//		sendUartString(teststr);
//		lastUwTick = uwTick;
//	}   while (1);
//}
//#endif
//

