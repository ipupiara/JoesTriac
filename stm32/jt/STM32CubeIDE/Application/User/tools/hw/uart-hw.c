
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

UART_HandleTypeDef huart;
DMA_HandleTypeDef hdma_usart_tx;
DMA_HandleTypeDef hdma_usart_rx;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;

extern uint16_t  feCounter;
extern uint16_t  teCounter;
extern uint16_t  dmeCounter;


uint8_t  commsError;
osMessageQueueId_t uartSendSemaphoreQ;
uint32_t  rxMsgCounter;
uint32_t  txMsgCounter;

uint32_t debugIdleCounter;
uint8_t  uartJobSemSet;

void uartTriacTest();

void clearUartInterruptFlags(UART_HandleTypeDef * phuart)
{
	__HAL_UART_CLEAR_IT(phuart,USART_ICR_TCCF_Msk);
	__HAL_UART_CLEAR_IT(phuart,USART_ICR_IDLECF_Msk);
}

uint8_t enableUartInterrupts()
{
	uint8_t res = 0;
	clearUartInterruptFlags(&huart);
	enableAllDmaInterrupts(&hdma_usart_tx,withoutHT);
	huart.Instance->CR1 |= USART_CR1_IDLEIE_Msk;
//		  huart2.Instance->CR1 |= USART_CR1_TCIE_Msk;
	huart.Instance->CR3 |= USART_CR3_EIE_Msk;

	HAL_NVIC_EnableIRQ(USART_IRQn);
	HAL_NVIC_EnableIRQ(txDMA_Stream_IRQn);
//	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	return res;
}

uint8_t disableUartInterrupts()
{
	uint8_t res = 0;
	HAL_NVIC_DisableIRQ(USART_IRQn);
	HAL_NVIC_DisableIRQ(txDMA_Stream_IRQn);
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

//void addToDebugArray(uint8_t from,uint8_t val)
//{
///*	if (debugArrayCnt < debugArraySize+3) {
//		debugArray[debugArrayCnt] = from;
//		debugArray[debugArrayCnt+1] = val;
//		debugArrayCnt += 3;
//	}*/
//}
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
	bufferCounterType newNdtr= (uint32_t)  (hdma_usart_rx.Instance->NDTR);
	bufferCounterType amtRcvd;

	if (tobeForwardedFrom == fromTransferCompleteIsr)  {
		amtRcvd = lastNdtr;     // newNdtr already set  to buffer size
	}  else {
		amtRcvd =   lastNdtr  - newNdtr;
	}

	uint8_t receivedAt = fullDmaRxBufferSize - lastNdtr;

	bufferCounterType amtCpy = amtRcvd;
	// evtl if inTc ndtr might already be reset ??

	if (amtWrittenStringChars < maxUartReceiveDmaStringSize )  {
		if (amtWrittenStringChars + amtRcvd > maxUartReceiveDmaStringSize) {
			amtCpy = maxUartReceiveDmaStringSize  - amtWrittenStringChars -1;
		}
	} else {
		amtCpy = 0;
	}

	if (amtCpy > 0) {
		for (uint8_t cnt =0; cnt < amtCpy;++cnt) {
			receiveStringBuffer[amtWrittenStringChars + cnt] = dmaBuffer.byteBuffer[receivedAt + cnt];
		}
	//	strncpy(stringBuffer[amtWrittenStringChars],dmaBuffer[amtReadBufferChars],amtCpy);
	}
	//	memcpy(&stringBuffer[amtWrittenStringChars],&dmaBuffer.byteBuffer[amtReadBufferChars],amtCpy);

	lastNdtr = newNdtr;
	amtWrittenStringChars += amtCpy;
//	addToDebugArray(tobeForwardedFrom,amtWrittenStringChars);

	if (tobeForwardedFrom == fromTransferCompleteIsr)  {
		resetDmaBuffer();
	} else if (tobeForwardedFrom == fromUartIsr) {
		if (amtWrittenStringChars > 0) {
			forwardReceivedStringBuffer((char*)receiveStringBuffer);
			resetStringBuffer();
		}
	}
}



void setUartJobSemaQ()
{
#ifndef debugTriac
	uint32_t dummy = 0x5a;
	if (uartJobSemSet == 0)  {    // prevent multiple events by irqs
		osMessageQueuePut(uartSendSemaphoreQ, &dummy, 0, 0);   //  after semaphores do not work from isr ?????  replaced by queue as done in OSWrappers.cpp
		uartJobSemSet = 1;
	} else  {
			uartJobSemSet = 2;  //  just for debugging
	}
#endif
}


//void USART_IRQHandler(void)
//{
//    if (__HAL_UART_GET_FLAG(&huart,USART_ISR_TXE)  )  {
//    	__HAL_UART_CLEAR_IT(&huart,USART_ISR_TXE);
//
//    	huart.Instance->TDR= txStringBuffer[txStringBufferPos];
//        ++ 	txStringBufferPos;
//
//        if (txStringBufferPos == txStringBufferLen) {
//            disableUartInterrupts();
//            setUartJobSemaQ();
//        }
//        //  todo handle errors
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
//        	//  copy rest of data to receive buffer and signal received event
//    	__HAL_UART_CLEAR_IT(&huart,USART_ICR_TCCF_Msk);
//    }
//
//     /* --------------- HANDLER YOUR ISR HERE --------------- */
//     if (idleDetected == 1)   {
//	    transferBuffer(fromUartIsr);
//     }
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



void startCircReceiver()
{
	DMA_SetTransferConfig(&hdma_usart_rx, (uint32_t)&huart.Instance->RDR , (uint32_t)&dmaBuffer, sizeof(dmaBuffer));
	clearDmaInterruptFlags(&hdma_usart_rx);
	__HAL_DMA_ENABLE(&hdma_usart_rx);
}

//static void MX_DMA_Init(void)
//{
//
//  /* DMA controller clock enable */
//  __HAL_RCC_DMA2_CLK_ENABLE();
//  __HAL_RCC_DMA1_CLK_ENABLE();
//
//  /* DMA interrupt init */
//  /* DMA1_Stream5_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn,triacApplicationIsrPrio, 0);
//  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
//  /* DMA1_Stream6_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, triacApplicationIsrPrio, 0);
//  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
//  /* DMA2_Stream3_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, triacApplicationIsrPrio, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
//  /* DMA2_Stream6_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, triacApplicationIsrPrio, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
//
//}
//
//
//static void MX_USART2_UART_Init(void)
//{
//
//  /* USER CODE BEGIN USART2_Init 0 */
//
//  /* USER CODE END USART2_Init 0 */
//
//  /* USER CODE BEGIN USART2_Init 1 */
//
//  /* USER CODE END USART2_Init 1 */
//  huart2.Instance = USART2;
//  huart2.Init.BaudRate = 115200;
//  huart2.Init.WordLength = UART_WORDLENGTH_8B;
//  huart2.Init.StopBits = UART_STOPBITS_1;
//  huart2.Init.Parity = UART_PARITY_NONE;
//  huart2.Init.Mode = UART_MODE_TX_RX;
//  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
//  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//  if (HAL_UART_Init(&huart2) != HAL_OK)
//  {
//	  errorHandler(0xff ,stop," HAL_UART_Init ","MX_USART2_UART_Init");
//  }
//  /* USER CODE BEGIN USART2_Init 2 */
//
//  /* USER CODE END USART2_Init 2 */
//
//}
//
//


uint32_t debugIdleCounter;

uint8_t initUartHw()
{
	uint8_t res = osOK ;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

//	MX_DMA_Init();
//	MX_USART2_UART_Init();


	debugIdleCounter = 0;
	 commsError = 0;
	rxMsgCounter = 0;
	txMsgCounter = 0;
	uartJobSemSet = 0;
#ifndef debugTriac
	uartSendSemaphoreQ =  osMessageQueueNew(3,4, NULL);
	if (uartSendSemaphoreQ  == NULL)   {
		errorHandler(0xff ,stop," osMessageQueueNew ","initUartHw");
	}
	setUartJobSemaQ();
#endif
	resetStringBuffer();



	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART;
	PeriphClkInitStruct.UsartClockSelection = RCC_USARTCLKSOURCE_PCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)  {
		errorHandler(0xfe,goOn," HAL_RCCEx_PeriphCLKConfig  ","initUartHw");
	}

	    /* Peripheral clock enable */
	usart__HAL_RCC_USART_CLK_ENABLE();

	usart__HAL_RCC_GPIO_CLK_ENABLE();

	GPIO_InitStruct.Pin = txGPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF_USART;
	HAL_GPIO_Init(uartPort, &GPIO_InitStruct);



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

	usart__HAL_RCC_DMA_CLK_ENABLE();

	/* USART2_TX Init */
	hdma_usart_tx.Instance = txDMA_Stream;
	hdma_usart_tx.Init.Channel = txDMA_CHANNEL;
	hdma_usart_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart_tx.Init.Mode = DMA_NORMAL;
	hdma_usart_tx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_usart_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_usart_tx) != HAL_OK)
	{
		errorHandler(0xfe,goOn," HAL_DMA_Init 2 ","initUartHw");
	}
	__HAL_LINKDMA(&huart,hdmatx,hdma_usart_tx);

	huart.Instance = USART_Number;
	huart.Init.BaudRate = 57600;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_TX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;
	huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart) != HAL_OK)
	{
		errorHandler(0xfe,goOn," HAL_UART_Init","initUartHw");
	}

//	huart2.Instance->CR3 |= (USART_CR3_DMAR_Msk | USART_CR3_DMAT_Msk);
	huart.Instance->CR3 |=  USART_CR3_DMAT_Msk;

	huart.Instance->CR1 |= USART_CR1_IDLEIE_Msk;
//		  huart2.Instance->CR1 |= USART_CR1_TCIE_Msk;
	huart.Instance->CR3 |= USART_CR3_EIE_Msk;

//	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_SetPriority(txDMA_Stream_IRQn, triacApplicationIsrPrio, 0);
	HAL_NVIC_SetPriority(USART_IRQn, triacApplicationIsrPrio, 0);

	disableUartInterrupts();

#ifdef debugTriac
	startUartHw();
	uartTriacTest();
#endif
	return res;
}

uint8_t startUartHw()
{
	uint8_t res = 0;
//	enableUartInterrupts();
//	startCircReceiver();
	return res;
}

osStatus_t sendUartString(char* sndStr)
{
	uint8_t res = 0;
	++ txMsgCounter;
//	commsError = 0;

	enableUartInterrupts();

	DMA_SetTransferConfig(&hdma_usart_tx, (uint32_t)sndStr, (uint32_t)&huart.Instance->TDR, strlen(sndStr));
	clearDmaInterruptFlags(&hdma_usart_tx);
	__HAL_DMA_ENABLE(&hdma_usart_tx);

	return res;
}

#ifdef debugTriac
void uartTriacTest()
{
	uint32_t lastUwTick  = uwTick;
	char * teststr  = "hello you there";

	do {
		do {} while (((uwTick - lastUwTick) < 100 ) || (uwTick == 0xffffffff) );
		lastUwTick = uwTick;
		sendUartString(teststr);
//		HAL_UART_Transmit_DMA(&huart, (uint8_t*) teststr, sizeof(teststr));

	}   while (1);
}
#endif


