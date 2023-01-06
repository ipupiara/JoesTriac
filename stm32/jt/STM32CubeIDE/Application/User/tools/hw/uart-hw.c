
#include <uart-hw.h>
#include <string.h>
#include <uart-comms.h>
#include <mainJt.h>
#include <dma-tools.h>

enum {
	fromHalfTransferCompleteIsr = 1,
	fromTransferCompleteIsr,
	fromUartIsr
};

UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart6_tx;
//DMA_HandleTypeDef hdma_usart6_rx;

extern uint16_t  feCounter;
extern uint16_t  teCounter;
extern uint16_t  dmeCounter;


uint8_t  commsError;
osMessageQueueId_t uartSendSemaphoreQ;
uint32_t  rxMsgCounter;
uint32_t  txMsgCounter;

uint32_t debugIdleCounter;
uint8_t  uartJobSemSet;

void clearUartInterruptFlags(UART_HandleTypeDef * huart)
{
	__HAL_UART_CLEAR_IT(&huart6,USART_ICR_TCCF_Msk);
	__HAL_UART_CLEAR_IT(&huart6,USART_ICR_IDLECF_Msk);
//	__HAL_UART_CLEAR_IT(&huart6,USART_ICR_TXE_Msk);
}

uint8_t enableUartInterrupts()
{
	uint8_t res = 0;

	clearUartInterruptFlags(&huart6);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

	return res;
}

uint8_t disableUartInterrupts()
{
	uint8_t res = 0;
	HAL_NVIC_DisableIRQ(USART6_IRQn);
	HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);
	HAL_NVIC_DisableIRQ(DMA2_Stream6_IRQn);
	return res;
}




#define bufferCounterType uint8_t
#define halfDmaRxBufferSize   0x20
#define fullDmaRxBufferSize 2 * halfDmaRxBufferSize
#define endBufferPosition ( fullDmaRxBufferSize - 1 )


#define debugArraySize 0xFF
uint8_t  debugArray [debugArraySize];
uint8_t  debugArrayCnt;

void resetDebugArray()
{
	memset(debugArray,0x00,sizeof(debugArray));
	debugArrayCnt = 0;
}

void addToDebugArray(uint8_t from,uint8_t val)
{
/*	if (debugArrayCnt < debugArraySize+3) {
		debugArray[debugArrayCnt] = from;
		debugArray[debugArrayCnt+1] = val;
		debugArrayCnt += 3;
	}*/
}
uint8_t  receiveStringBuffer  [maxUartReceiveDmaStringSize + 1];

typedef struct
{
  uint8_t firsthalf[halfDmaRxBufferSize];
  uint8_t secondhalf[halfDmaRxBufferSize];
} halfBufferTwice_Buffer_Type;

typedef union
{
	halfBufferTwice_Buffer_Type halfTwicebuffer;
	uint8_t byteBuffer [fullDmaRxBufferSize];
} DMA_Buffer_Type;

DMA_Buffer_Type dmaBuffer;

bufferCounterType lastNdtr;
bufferCounterType amtWrittenStringChars;




char txStringBuffer [maxSerialStringSz+1];
uint8_t  txStringBufferPos;
uint8_t  txStringBufferLen;


void resetStringBuffer()
{
	amtWrittenStringChars = 0;
	memset(receiveStringBuffer,0x00,sizeof(receiveStringBuffer));
}

void resetDmaBuffer()
{
	lastNdtr = fullDmaRxBufferSize;
	memset(&dmaBuffer,0x00,sizeof(dmaBuffer));
}

//void transferBuffer(uint8_t  tobeForwardedFrom)
//{
//	bufferCounterType newNdtr= (uint32_t)  (hdma_usart6_rx.Instance->NDTR);
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
//			amtCpy = maxUartReceiveDmaStringSize  - amtWrittenStringChars;
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
//	addToDebugArray(tobeForwardedFrom,amtWrittenStringChars);
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


//uint32_t debugIdleCounter;
//
//
//static void USART1_IRQHandler(void)
//{
//	uint8_t idleDetected = 0;
//
//    if (__HAL_UART_GET_FLAG(&huart6,USART_ISR_IDLE_Msk)  )  {
//    	__HAL_UART_CLEAR_IT(&huart6,USART_ISR_IDLE_Msk);
//    	idleDetected = 1;
//    	++ debugIdleCounter;
//     }
//    if (__HAL_UART_GET_FLAG(&huart6,USART_ICR_TCCF_Msk)  )  {
//        	//  copy rest of data to receive buffer and signal received event
//    	__HAL_UART_CLEAR_IT(&huart6,USART_ICR_TCCF_Msk);
//    }
//
//     /* --------------- HANDLER YOUR ISR HERE --------------- */
//     if (idleDetected == 1)   {
//		CPU_SR_ALLOC();
//
//		CPU_CRITICAL_ENTER();
//		 OSIntEnter();           /* Tell OS that we are starting an ISR           */
//		 CPU_CRITICAL_EXIT();
//
//	    	transferBuffer(fromUartIsr);
//
//		 OSIntExit();
//     }
//}
//

void setUartJobSema()
{

	uint32_t dummy = 0x5a;
	if (uartJobSemSet == 0)  {    // prevent multiple events by irqs
		osMessageQueuePut(uartSendSemaphoreQ, &dummy, 0, 0);   //  after semaphores do not work from isr ?????  replaced by queue as done in OSWrappers.cpp
		uartJobSemSet = 1;
	} else  {
			uartJobSemSet = 2;  //  just for debugging
	}
}


void USART6_IRQHandler(void)
{
    if (__HAL_UART_GET_FLAG(&huart6,USART_ISR_TXE)  )  {
    	__HAL_UART_CLEAR_IT(&huart6,USART_ISR_TXE);

    	huart6.Instance->TDR= txStringBuffer[txStringBufferPos];
        ++ 	txStringBufferPos;

        if (txStringBufferPos == txStringBufferLen) {
            disableUartInterrupts();
            setUartJobSema();
        }
        //  todo handle errors
    }

	uint8_t idleDetected = 0;

    if (__HAL_UART_GET_FLAG(&huart6,USART_ISR_IDLE_Msk)  )  {
    	__HAL_UART_CLEAR_IT(&huart6,USART_ISR_IDLE_Msk);
    	idleDetected = 1;
    	++ debugIdleCounter;
     }
    if (__HAL_UART_GET_FLAG(&huart6,USART_ICR_TCCF_Msk)  )  {
        	//  copy rest of data to receive buffer and signal received event
    	__HAL_UART_CLEAR_IT(&huart6,USART_ICR_TCCF_Msk);
    }

     /* --------------- HANDLER YOUR ISR HERE --------------- */
     if (idleDetected == 1)   {
	    transferBuffer(fromUartIsr);
     }
}


//void DMA2_Stream2_IRQHandler(void)   // RX
//{
//	if (__HAL_DMA_GET_FLAG(&hdma_usart6_rx,DMA_FLAG_TCIF2_6) != 0)  {
//		transferBuffer(fromTransferCompleteIsr);
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart6_rx,DMA_FLAG_TCIF2_6);
//	}
//
//
//    if (__HAL_DMA_GET_FLAG(&hdma_usart6_rx,DMA_FLAG_HTIF2_6) != 0)  {
//    	transferBuffer(fromHalfTransferCompleteIsr);
//    	__HAL_DMA_CLEAR_FLAG(&hdma_usart6_rx,DMA_FLAG_HTIF2_6);
//    }
//
//	if ((__HAL_DMA_GET_FLAG(&hdma_usart6_rx,DMA_FLAG_TEIF2_6))
//								| (__HAL_DMA_GET_FLAG(&hdma_usart6_rx,DMA_FLAG_FEIF2_6))
//								| (__HAL_DMA_GET_FLAG(&hdma_usart6_rx,DMA_FLAG_DMEIF2_6))) {
//		errorHandler((uint32_t)0 ,goOn," DMA_FLAG_TEIF2_6 "," DMA2_Stream2_IRQHandler ");
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart6_rx,DMA_FLAG_TEIF2_6);
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart6_rx,DMA_FLAG_FEIF2_6);
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart6_rx,DMA_FLAG_DMEIF2_6);
//	}
//}


void DMA2_Stream6_IRQHandler(void)   // TX
{
//	CMainJtEventT  ev;
//	osStatus_t err = osOK;
	if (__HAL_DMA_GET_FLAG(&hdma_usart6_tx,DMA_FLAG_TCIF3_7) != 0)  {

		setUartJobSema();
//
//		osSemaphoreRelease(&uartSendSemaphore);
//		if (err != osOK) {
//			errorHandler((uint32_t)err ,goOn," uartSendSemaphore "," DMA2_Stream7_IRQHandler ");
//		}
		__HAL_DMA_CLEAR_FLAG(&hdma_usart6_tx,DMA_FLAG_TCIF3_7);
	}

    if (__HAL_DMA_GET_FLAG(&hdma_usart6_tx,DMA_FLAG_HTIF3_7) != 0)  {
    	__HAL_DMA_CLEAR_FLAG(&hdma_usart6_tx,DMA_FLAG_HTIF3_7);
    }

	if ((__HAL_DMA_GET_FLAG(&hdma_usart6_tx,DMA_FLAG_TEIF3_7))
								| (__HAL_DMA_GET_FLAG(&hdma_usart6_tx,DMA_FLAG_FEIF3_7))
								| (__HAL_DMA_GET_FLAG(&hdma_usart6_tx,DMA_FLAG_DMEIF3_7))) {
		errorHandler((uint32_t)1 ,goOn," DMA_FLAG_TEIF3_7 "," DMA2_Stream7_IRQHandler ");
	  //   todo carefully abort job and deinit if possible

		__HAL_DMA_CLEAR_FLAG(&hdma_usart6_tx,DMA_FLAG_TEIF3_7);
		__HAL_DMA_CLEAR_FLAG(&hdma_usart6_tx,DMA_FLAG_FEIF3_7);
		__HAL_DMA_CLEAR_FLAG(&hdma_usart6_tx,DMA_FLAG_DMEIF3_7);
	}
}



void uart_DMA_Init(void)
{
	/* DMA controller clock enable */
	  __HAL_RCC_DMA2_CLK_ENABLE();

	    /* USART1 DMA Init */
	    /* USART1_RX Init */
	    hdma_usart6_rx.Instance = DMA2_Stream2;
	    hdma_usart6_rx.Init.Channel = DMA_CHANNEL_5;
//	    hdma_usart1_rx.Instance = DMA2_Stream2;
//	    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
	    hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	    hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	    hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
	    hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	    hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	    hdma_usart6_rx.Init.Mode = DMA_CIRCULAR;
	    hdma_usart6_rx.Init.Priority = DMA_PRIORITY_LOW;
	    hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	    if (HAL_DMA_Init(&hdma_usart6_rx) != HAL_OK)
	    {
	    	errorHandler((uint32_t)3 ,goOn," hdma_usart6_rx "," uart_DMA_Init ");

	    }
//	    __HAL_LINKDMA(&huart6,hdmarx,hdma_usart6_rx);
	    clearDmaInterruptFlags(&hdma_usart6_rx);
	    enableAllDmaInterrupts(&hdma_usart6_rx,withHT);
	    resetDmaBuffer();

	    /* USART1_TX Init */
	    hdma_usart6_tx.Instance = DMA2_Stream6;
	    hdma_usart6_tx.Init.Channel = DMA_CHANNEL_5;
//	    hdma_usart1_tx.Instance = DMA2_Stream7;
//	    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
	    hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	    hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	    hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
	    hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	    hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	    hdma_usart6_tx.Init.Mode = DMA_NORMAL;
	    hdma_usart6_tx.Init.Priority = DMA_PRIORITY_HIGH;
	    hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	    if (HAL_DMA_Init(&hdma_usart6_tx) != HAL_OK)
	    {
	    	errorHandler((uint32_t)3 ,goOn," hdma_usart6_tx "," uart_DMA_Init ");
	    }
//	    __HAL_LINKDMA(&huart6,hdmatx,hdma_usart6_tx);
	    clearDmaInterruptFlags(&hdma_usart6_tx);
	    enableAllDmaInterrupts(&hdma_usart6_tx,withoutHT);

	    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
	    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
}

void startCircReceiver()
{
	DMA_SetTransferConfig(&hdma_usart6_rx, (uint32_t)&huart6.Instance->RDR , (uint32_t)&dmaBuffer, sizeof(dmaBuffer));
	clearDmaInterruptFlags(&hdma_usart6_rx);
	__HAL_DMA_ENABLE(&hdma_usart6_rx);
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
	    /* planned
	     *
	     * usart 2
	     *
	     * PA2   TX
	     * PA3   RX
	     *
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
	uint8_t res = osOK ;

	debugIdleCounter = 0;
	 commsError = 0;
	rxMsgCounter = 0;
	txMsgCounter = 0;
	uartJobSemSet = 0;

	uartSendSemaphoreQ =  osMessageQueueNew(3,4, NULL);
	if (uartSendSemaphoreQ  == NULL)   {
		errorHandler(0xff ,stop," osMessageQueueNew ","initUartHw");
	}

	resetStringBuffer();

	 __HAL_RCC_USART6_CLK_ENABLE();

		usart6_GPIO_Init();

		  huart6.Instance = USART6;
//		  huart6.Init.BaudRate = 9600;
		  huart6.Init.BaudRate = 57600;
		  huart6.Init.WordLength = UART_WORDLENGTH_8B;
		  huart6.Init.StopBits = UART_STOPBITS_1;
		  huart6.Init.Parity = UART_PARITY_NONE;
		  huart6.Init.Mode = UART_MODE_TX_RX;
		  huart6.Init.Mode = UART_MODE_TX;
		  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
		  if (HAL_UART_Init(&huart6) != HAL_OK)
		  {
		    //  Error_Handler();
			  res = 0xFE;
		  }
		  huart6.Instance->CR3 |= (USART_CR3_DMAR_Msk | USART_CR3_DMAT_Msk);
		  clearUartInterruptFlags(&huart6);
		  huart6.Instance->CR1 |= USART_CR1_IDLEIE_Msk;
//		  huart6.Instance->CR1 |= USART_CR1_TXEIE_Msk;
//		  huart6.Instance->CR1 |= USART_CR1_TCIE_Msk;

		  HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);

		  uart_DMA_Init();

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
	uint8_t res = 0;
	++ txMsgCounter;
//	commsError = 0;

	DMA_SetTransferConfig(&hdma_usart6_tx, (uint32_t)sndStr, (uint32_t)&huart6.Instance->TDR, strlen(sndStr));
	clearDmaInterruptFlags(&hdma_usart6_tx);
	__HAL_DMA_ENABLE(&hdma_usart6_tx);

	return res;
}

//osStatus_t sendUartString(char* sndStr)
//{
//	osStatus_t res = 0;
//	++ txMsgCounter;
//	commsError = 0;
//	txStringBufferPos = 0;
//	txStringBufferLen = strlen(sndStr);
//
//	strncpy(txStringBuffer,sndStr,maxSerialStringSz);
//	enableUartInterrupts();
//	// evtl. send first char if interrupt does not get fired now when txe
//	return res;
//}
