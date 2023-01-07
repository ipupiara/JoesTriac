
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

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;
// DMA_HandleTypeDef hdma_usart2_rx;;

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
	__HAL_UART_CLEAR_IT(&huart2,USART_ICR_TCCF_Msk);
	__HAL_UART_CLEAR_IT(&huart2,USART_ICR_IDLECF_Msk);
}

uint8_t enableUartInterrupts()
{
	uint8_t res = 0;

	clearUartInterruptFlags(&huart2);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
//	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	return res;
}

uint8_t disableUartInterrupts()
{
	uint8_t res = 0;
	HAL_NVIC_DisableIRQ(USART2_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Stream6_IRQn);
//	HAL_NVIC_DisableIRQ(DMA1_Stream5_IRQn);
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

void transferBuffer(uint8_t  tobeForwardedFrom)
{
//	bufferCounterType newNdtr= (uint32_t)  (hdma_usart2_rx.Instance->NDTR);
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
}



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


void USART2_IRQHandler(void)
{
    if (__HAL_UART_GET_FLAG(&huart2,USART_ISR_TXE)  )  {
    	__HAL_UART_CLEAR_IT(&huart2,USART_ISR_TXE);

    	huart2.Instance->TDR= txStringBuffer[txStringBufferPos];
        ++ 	txStringBufferPos;

        if (txStringBufferPos == txStringBufferLen) {
            disableUartInterrupts();
            setUartJobSema();
        }
        //  todo handle errors
    }

	uint8_t idleDetected = 0;

    if (__HAL_UART_GET_FLAG(&huart2,USART_ISR_IDLE_Msk)  )  {
    	__HAL_UART_CLEAR_IT(&huart2,USART_ISR_IDLE_Msk);
    	idleDetected = 1;
    	++ debugIdleCounter;
     }
    if (__HAL_UART_GET_FLAG(&huart2,USART_ICR_TCCF_Msk)  )  {
        	//  copy rest of data to receive buffer and signal received event
    	__HAL_UART_CLEAR_IT(&huart2,USART_ICR_TCCF_Msk);
    }

     /* --------------- HANDLER YOUR ISR HERE --------------- */
     if (idleDetected == 1)   {
	    transferBuffer(fromUartIsr);
     }
}


//void DMA1_Stream5_IRQHandler(void)  // RX
//{
//	if (__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_TCIF1_5) != 0)  {
//		transferBuffer(fromTransferCompleteIsr);
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_TCIF1_5);
//	}
//
//
//    if (__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_HTIF1_5) != 0)  {
//    	transferBuffer(fromHalfTransferCompleteIsr);
//    	__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_HTIF1_5);
//    }
//
//	if ((__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_TEIF1_5))
//								| (__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_FEIF1_5))
//								| (__HAL_DMA_GET_FLAG(&hdma_usart2_rx,DMA_FLAG_DMEIF1_5))) {
//		errorHandler((uint32_t)0 ,goOn," DMA_FLAG_TEIF1_5 "," DMA2_Stream2_IRQHandler ");
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_TEIF1_5);
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_FEIF1_5);
//		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_DMEIF1_5);
//	}
//}


void DMA1_Stream6_IRQHandler(void)   // TX
{
//	CMainJtEventT  ev;
//	osStatus_t err = osOK;
	if (__HAL_DMA_GET_FLAG(&hdma_usart2_tx,DMA_FLAG_TCIF2_6) != 0)  {

		setUartJobSema();
//
//		osSemaphoreRelease(&uartSendSemaphore);
//		if (err != osOK) {
//			errorHandler((uint32_t)err ,goOn," uartSendSemaphore "," DMA2_Stream7_IRQHandler ");
//		}
		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_tx,DMA_FLAG_TCIF2_6);
	}

    if (__HAL_DMA_GET_FLAG(&hdma_usart2_tx,DMA_FLAG_HTIF2_6) != 0)  {
    	__HAL_DMA_CLEAR_FLAG(&hdma_usart2_tx,DMA_FLAG_HTIF2_6);
    }

	if ((__HAL_DMA_GET_FLAG(&hdma_usart2_tx,DMA_FLAG_TEIF2_6))
								| (__HAL_DMA_GET_FLAG(&hdma_usart2_tx,DMA_FLAG_FEIF2_6))
								| (__HAL_DMA_GET_FLAG(&hdma_usart2_tx,DMA_FLAG_DMEIF2_6))) {
		errorHandler((uint32_t)1 ,goOn," DMA_FLAG_TEIF2_6 "," DMA2_Stream7_IRQHandler ");
	  //   todo carefully abort job and deinit if possible

		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_tx,DMA_FLAG_TEIF2_6);
		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_tx,DMA_FLAG_FEIF2_6);
		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_tx,DMA_FLAG_DMEIF2_6);
	}
}


//void startCircReceiver()
//{
//	DMA_SetTransferConfig(&hdma_usart2_rx, (uint32_t)&huart2.Instance->RDR , (uint32_t)&dmaBuffer, sizeof(dmaBuffer));
//	clearDmaInterruptFlags(&hdma_usart2_rx);
//	__HAL_DMA_ENABLE(&hdma_usart2_rx);
//}


uint32_t debugIdleCounter;

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

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};


	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
	PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		errorHandler(0xfe,goOn," HAL_RCCEx_PeriphCLKConfig ","initUartHw");
	}

	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**USART2 GPIO Configuration
	PA2     ------> USART2_TX
	PA3     ------> USART2_RX
	*/

	GPIO_InitStruct.Pin = GPIO_PIN_2;
//	    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//	    /* USART2 DMA Init */
//	    /* USART2_RX Init */
//	    hdma_usart2_rx.Instance = DMA1_Stream5;
//	    hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
//	    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//	    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//	    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
//	    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//	    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//	    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
//	    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
//	    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//	    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
//	    {
//	      errorHandler(0xfe,goOn," HAL_DMA_Init 1 ","initUartHw");
//	    }
//
//	    __HAL_LINKDMA(huart,hdmarx,hdma_usart2_rx);

	/* USART2_TX Init */
	hdma_usart2_tx.Instance = DMA1_Stream6;
	hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart2_tx.Init.Mode = DMA_NORMAL;
	hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
	{
		errorHandler(0xfe,goOn," HAL_DMA_Init 2 ","initUartHw");
	}

	__HAL_LINKDMA(&huart2,hdmatx,hdma_usart2_tx);
//	huart2.Instance->CR3 |= (USART_CR3_DMAR_Msk | USART_CR3_DMAT_Msk);
	huart2.Instance->CR3 |=  USART_CR3_DMAT_Msk;

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 57600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		errorHandler(0xfe,goOn," HAL_UART_Init","initUartHw");
	}

	clearDmaInterruptFlags(&hdma_usart2_tx);
	enableAllDmaInterrupts(&hdma_usart2_tx,withoutHT);

	clearUartInterruptFlags(&huart2);
	huart2.Instance->CR1 |= USART_CR1_IDLEIE_Msk;
//		  huart2.Instance->CR1 |= USART_CR1_TCIE_Msk;

//	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  //  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  //  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);

	disableUartInterrupts();
	return res;
}

uint8_t startUartHw()
{
	uint8_t res = 0;
	enableUartInterrupts();
	return res;
}

osStatus_t sendUartString(char* sndStr)
{
	uint8_t res = 0;
	++ txMsgCounter;
//	commsError = 0;

	DMA_SetTransferConfig(&hdma_usart2_tx, (uint32_t)sndStr, (uint32_t)&huart2.Instance->TDR, strlen(sndStr));
	clearDmaInterruptFlags(&hdma_usart2_tx);
	__HAL_DMA_ENABLE(&hdma_usart2_tx);

	return res;
}


