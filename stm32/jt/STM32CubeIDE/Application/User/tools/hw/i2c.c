
#include <string.h>
#include <i2c.h>
#include <dma-tools.h>
#include <StateClass.h>
#include <main.h>
#include <mainJt.h>
#include "cmsis_os.h"

#define I2C_FLAG_NACKF   I2C_FLAG_AF

//OS_EVENT *i2cResourceSem;



osSemaphoreId_t   i2cJobSema;
osSemaphoreId_t   i2cResourceSema;

I2C_HandleTypeDef hi2c1;

uint8_t resetOnError;
uint8_t i2cInitialized;

uint8_t  transmitErrorCollectoruint8_t;
uint8_t  jobSemSet;


//void reInitOnError();

typedef enum  {
	sendI2c = 0,
	receiveI2c,
} jobTypes;


typedef struct
{
	jobTypes  jobType;
	uint8_t*  buffer;
	uint8_t	amtChars;
	uint8_t   bufferCnt;
	uint8_t   address;
} i2cJobDataType;

i2cJobDataType i2cJobData;

void setI2cJobSema()
{
	if (jobSemSet == 0)  {    // prevent multiple events by irqs
		osSemaphoreRelease(&i2cJobSema);
		jobSemSet = 1;
	} else  {
			jobSemSet = 2;  //  just for debugging
	}
}

void i2cFinishedOk()
{
	setI2cJobSema();
}

//void flushTXEempty(I2C_HandleTypeDef *hdma)
//{
//	SET_BIT((hdma)->Instance->ISR,I2C_ISR_TXE  );
//}


void i2cError(uint8_t err)
{
	 //log error
	 transmitErrorCollectoruint8_t = err;
	 setI2cJobSema();

}


void enableI2c()
{
	 __HAL_I2C_ENABLE(&hi2c1);
}

void disableI2c()
{
	__HAL_I2C_DISABLE(&hi2c1);
}


#ifdef i2cUseDma

DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;


void DMA1_Stream0_IRQHandler(void)
{
	CPU_SR_ALLOC();
//	uint8_t err = OS_ERR_NONE;

	CPU_CRITICAL_ENTER();
	OSIntEnter();           /* Tell OS that we are starting an ISR           */
	CPU_CRITICAL_EXIT();

	if (__HAL_DMA_GET_FLAG(&hdma_i2c1_rx,DMA_FLAG_TCIF0_4) != 0)  {
//		transferBuffer();
//		i2cFinishedOk();
		__HAL_DMA_CLEAR_FLAG(&hdma_i2c1_rx,DMA_FLAG_TCIF0_4);
	}


    if (__HAL_DMA_GET_FLAG(&hdma_i2c1_rx,DMA_FLAG_HTIF0_4) != 0)  {
//    	transferBuffer();
    	__HAL_DMA_CLEAR_FLAG(&hdma_i2c1_rx,DMA_FLAG_HTIF0_4);
    }

	if ((__HAL_DMA_GET_FLAG(&hdma_i2c1_rx,DMA_FLAG_TEIF0_4))
								| (__HAL_DMA_GET_FLAG(&hdma_i2c1_rx,DMA_FLAG_FEIF0_4))
								| (__HAL_DMA_GET_FLAG(&hdma_i2c1_rx,DMA_FLAG_DMEIF0_4))) {
		incDMAErrorCounter(&hdma_i2c1_rx);
		i2cError(dmaIsr(&hdma_i2c1_rx ));
		__HAL_DMA_CLEAR_FLAG(&hdma_i2c1_rx,DMA_FLAG_TEIF0_4);
		__HAL_DMA_CLEAR_FLAG(&hdma_i2c1_rx,DMA_FLAG_FEIF0_4);
		__HAL_DMA_CLEAR_FLAG(&hdma_i2c1_rx,DMA_FLAG_DMEIF0_4);
	}
	 OSIntExit();
}

void DMA1_Stream6_IRQHandler(void)
{
	CPU_SR_ALLOC();
//	uint8_t err = OS_ERR_NONE;

	CPU_CRITICAL_ENTER();
	OSIntEnter();           /* Tell OS that we are starting an ISR           */
	CPU_CRITICAL_EXIT();

	if (__HAL_DMA_GET_FLAG(&hdma_i2c1_tx,DMA_FLAG_TCIF2_6) != 0)  {
		i2cFinishedOk();
		__HAL_DMA_CLEAR_FLAG(&hdma_i2c1_tx,DMA_FLAG_TCIF2_6);
	}


    if (__HAL_DMA_GET_FLAG(&hdma_i2c1_tx,DMA_FLAG_HTIF2_6) != 0)  {
//    	transferBuffer();
    	__HAL_DMA_CLEAR_FLAG(&hdma_i2c1_tx,DMA_FLAG_HTIF2_6);
    }

	if ((__HAL_DMA_GET_FLAG(&hdma_i2c1_tx,DMA_FLAG_TEIF2_6))
								| (__HAL_DMA_GET_FLAG(&hdma_i2c1_tx,DMA_FLAG_FEIF2_6))
								| (__HAL_DMA_GET_FLAG(&hdma_i2c1_tx,DMA_FLAG_DMEIF2_6))) {
		incDMAErrorCounter(&hdma_i2c1_tx);
		i2cError(dmaIsr(&hdma_i2c1_tx));
		__HAL_DMA_CLEAR_FLAG(&hdma_i2c1_tx,DMA_FLAG_TEIF2_6);
		__HAL_DMA_CLEAR_FLAG(&hdma_i2c1_tx,DMA_FLAG_FEIF2_6);
		__HAL_DMA_CLEAR_FLAG(&hdma_i2c1_tx,DMA_FLAG_DMEIF2_6);
	}
	 OSIntExit();
}

void i2cDmaInit()
{

    /* I2C1 DMA Init */
    /* I2C1_RX Init */
    hdma_i2c1_rx.Instance = DMA1_Stream0;
    hdma_i2c1_rx.Init.Channel = DMA_CHANNEL_1;
    hdma_i2c1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2c1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_i2c1_rx) != HAL_OK)
    {
    	i2cError(0x85);
    }

    __HAL_LINKDMA(&hi2c1,hdmarx,hdma_i2c1_rx);

    /* I2C1_TX Init */
    hdma_i2c1_tx.Instance = DMA1_Stream6;
    hdma_i2c1_tx.Init.Channel = DMA_CHANNEL_1;
    hdma_i2c1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_i2c1_tx) != HAL_OK)
    {
    	i2cError(0x86);
    }
    __HAL_LINKDMA(&hi2c1,hdmatx,hdma_i2c1_tx);

	  __HAL_I2C_TxDmaENABLE(&hi2c1);
	  __HAL_I2C_RxDmaENABLE(&hi2c1);

	  BSP_IntVectSet (DMA1_Stream0_IRQn,tempixIsrPrioLevel,CPU_INT_KA,DMA1_Stream0_IRQHandler);
	  BSP_IntVectSet (DMA1_Stream6_IRQn,tempixIsrPrioLevel,CPU_INT_KA,DMA1_Stream6_IRQHandler);
	  BSP_IntEnable(DMA1_Stream0_IRQn);
	  BSP_IntEnable(DMA1_Stream6_IRQn);

	clearDmaInterruptFlags(&hdma_i2c1_tx);
	clearDmaInterruptFlags(&hdma_i2c1_rx);
	enableAllDmaInterrupts(&hdma_i2c1_tx,withoutHT);
	enableAllDmaInterrupts(&hdma_i2c1_rx,withoutHT);

}

#else

void sendNextI2CByte()
{
	if (i2cJobData.bufferCnt < i2cJobData.amtChars) {
		hi2c1.Instance->TXDR = i2cJobData.buffer[i2cJobData.bufferCnt];
		++i2cJobData.bufferCnt;
	}
}

void receiveNextI2CByte()
{
	if (i2cJobData.bufferCnt < i2cJobData.amtChars) {
			i2cJobData.buffer[i2cJobData.bufferCnt] = hi2c1.Instance->RXDR ;
			++i2cJobData.bufferCnt;
	}
}

#endif



void establishContactAndRun()
{
#ifdef i2cUseDma
	if (i2cJobData.jobType == sendI2c) {
		DMA_SetTransferConfig(&hdma_i2c1_tx,(uint32_t)i2cJobData.buffer,(uint32_t)&hi2c1.Instance->TXDR,i2cJobData.amtChars);
		clearDmaInterruptFlags(&hdma_i2c1_tx);
		__HAL_DMA_ENABLE(&hdma_i2c1_tx);
	} else {
		DMA_SetTransferConfig(&hdma_i2c1_rx,(uint32_t)&hi2c1.Instance->RXDR,(uint32_t)i2cJobData.buffer,i2cJobData.amtChars);
		clearDmaInterruptFlags(&hdma_i2c1_rx);
		__HAL_DMA_ENABLE(&hdma_i2c1_rx);
	}

#endif

	i2cTransferConfig(&hi2c1,i2cJobData.address,i2cJobData.amtChars,(i2cJobData.jobType == receiveI2c ? 1:0));
	hi2c1.Instance->TXDR = (i2cJobData.address << 1);
//	if (i2cJobData.jobType == receiveI2c) {
//		hi2c1.Instance->TXDR |= 0x01;
//	}  // did also not work ..... ???????
	i2cSendStart(&hi2c1);
}

void enableI2cInterrupts()
{
#ifdef i2cUseDma
	clearDmaInterruptFlags(&hdma_i2c1_rx);
	clearDmaInterruptFlags(&hdma_i2c1_tx);
	BSP_IntEnable(DMA1_Stream6_IRQn);
	BSP_IntEnable(DMA1_Stream0_IRQn);
#endif
//	clearUartInterruptFlags(&huart1);
	HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
	HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);



}

void disableI2cInterrupts()
{
#ifdef i2cUseDma
	BSP_IntDisable(DMA1_Stream6_IRQn);
	BSP_IntDisable(DMA1_Stream0_IRQn);
#endif
	HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
	HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
}

void I2C1_EV_IRQHandler(void)
{
	uint32_t itflags   = READ_REG(hi2c1.Instance->ISR);
#ifndef i2cUseDma
	if ((itflags & I2C_FLAG_TXIS) != 0)   {
		sendNextI2CByte();
	}
	if ((itflags & I2C_FLAG_RXNE) != 0)   {
		receiveNextI2CByte();
	}
#endif
	if ((itflags & I2C_FLAG_TC) != 0)  {
		i2cFinishedOk();
	}
	if (((itflags & I2C_FLAG_STOPF) != 0)| ((itflags & I2C_FLAG_NACKF) != 0) )  {
		__HAL_I2C_CLEAR_FLAG(&hi2c1, I2C_FLAG_STOPF);
		__HAL_I2C_CLEAR_FLAG(&hi2c1, I2C_FLAG_NACKF);
		  i2cError(0x77);
	}
}

void I2C1_ER_IRQHandler(void)
{
	uint32_t itflags   = READ_REG(hi2c1.Instance->ISR);
	uint32_t itsources = READ_REG(hi2c1.Instance->CR1);
	  /* I2C Bus error interrupt occurred ------------------------------------*/
	  if (((itflags & I2C_FLAG_BERR) != RESET) && ((itsources & I2C_IT_ERRI) != RESET))
	  {
	    __HAL_I2C_CLEAR_FLAG(&hi2c1, I2C_FLAG_BERR);
	  }

	  /* I2C Over-Run/Under-Run interrupt occurred ----------------------------------------*/
	  if (((itflags & I2C_FLAG_OVR) != RESET) && ((itsources & I2C_IT_ERRI) != RESET))
	  {
	    __HAL_I2C_CLEAR_FLAG(&hi2c1, I2C_FLAG_OVR);
	  }

	  /* I2C Arbitration Loss error interrupt occurred -------------------------------------*/
	  if (((itflags & I2C_FLAG_ARLO) != RESET) && ((itsources & I2C_IT_ERRI) != RESET))
	  {
	    __HAL_I2C_CLEAR_FLAG(&hi2c1, I2C_FLAG_ARLO);
	  }
	  i2cError(0x82);  //  todo implement refined error message with above details....

	  // next two ifs are just for debugging reasons
	  if ((itflags & I2C_FLAG_AF) != 0) {   //  should actually be named I2C_FLAG_NACKF. how this name ?
		  i2cError(0x69);
	  }
	  if ((itflags & I2C_FLAG_STOPF) != 0) {
		  i2cError(0x96);
	  }
}

uint8_t transmitI2cByteArray(uint8_t adr,uint8_t* pResultString,uint8_t amtChars, uint8_t doSend)
{
	uint8_t res = 0xFF;
	osStatus_t status;

	if ((i2cInitialized == 1) )  {
		status = osSemaphoreAcquire(i2cResourceSema, osWaitForever);
		if (status == osOK) {
			transmitErrorCollectoruint8_t = 0;
			jobSemSet = 0;
			i2cJobData.buffer = pResultString;
			i2cJobData.amtChars = amtChars;
			i2cJobData.bufferCnt = 0;
			i2cJobData.address = adr;
			if (doSend == 1) {
				i2cJobData.jobType = sendI2c;
			} else {
				i2cJobData.jobType = receiveI2c;
				if (pResultString != 0) {
				memset(pResultString,0,amtChars);  // todo check if this work correct (not content of pointer variable is changed)
				}
			}
			establishContactAndRun();

			status = osSemaphoreAcquire(i2cJobSema, osWaitForever);
			if (status != osOK) {
				errorHandler((uint32_t)status ,stop," i2cJobSema "," transmitI2cByteArray ");
			}
			osSemaphoreRelease(i2cResourceSema);
			res = transmitErrorCollectoruint8_t;
		}  else {
			errorHandler((uint32_t)status ,stop," i2cResourceSema "," transmitI2cByteArray ");
		}
	}
	return res;
}

//  todo delay should not be done on transmit level on i2c side,
//  but on client side that needs the delay
//  else traffic will be blocked for all
uint8_t sendI2cByteArray(uint8_t adr,uint8_t* pString,uint8_t amtChars)
{
	return transmitI2cByteArray(adr, pString, amtChars, 1);
}

uint8_t receiveI2cByteArray(uint8_t adr,uint8_t* pString,uint8_t amtChars)
{
	return transmitI2cByteArray(adr, pString, amtChars, 0);
}



//uint8_t pollForReady(uint8_t adr, uint8_t delay)
//{
//	int8_t res = 0xFF;
////	int8_t resOnErrStack = resetOnError;
////	resetOnError = 0;
////	uint8_t dummyBuffer [1];
//
//	while (res != 0) {
//		//sendI2cByteArray(adr,&dummyBuffer[0],0, delay);  // do just a very short delay if desired
////		resetOnError = resOnErrStack;
//	}
//    return res;
//}

void initI2c()
{
	osStatus_t status;
	osSemaphoreDef_t  i2cSendSemaphoreDef;
	i2cInitialized = 0;
//	resetOnError = 1;
	resetOnError = 0;
	GPIO_InitTypeDef GPIO_InitStruct = {0};


	i2cSendSemaphoreDef.name="i2c send sema"  ;
	i2cSendSemaphoreDef.attr_bits= 0;
	i2cSendSemaphoreDef.cb_mem = NULL;
	i2cSendSemaphoreDef.cb_size = 0;
	i2cResourceSema=  osSemaphoreNew(1,1,&i2cSendSemaphoreDef);
	i2cJobSema =  osSemaphoreNew(1,1,&i2cSendSemaphoreDef);

	status = osSemaphoreAcquire(i2cJobSema, osWaitForever);  // set event to zero . todo replace with event group type ,
							//implemented this out of time to market reasons and at this time unknown cmsis_os eventGroup  interface
	if (status != osOK) {
		errorHandler((uint32_t)status ,stop," i2cResourceSema "," transmitI2cByteArray ");
	}


	__HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
/*
    //  I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
*/
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);



    __HAL_RCC_I2C1_CLK_ENABLE();

       disableI2c();
     //   	OSTimeDlyHMSM(0u, 0u, 1u, 0u);  // wait for uart/dma ready,  else fe happens when immediately sending a msg
     //     use this block if reset of i2c should be needed

     // //  enableI2c();

  hi2c1.Instance = I2C1;
//hi2c1.Init.Timing = 0x20404768;  // proposed by mx   FOR 100kHz
//hi2c1.Init.Timing = 0xC042C3C7;  // proposed by 756 datasheet for 10 kHz
  hi2c1.Init.Timing = 0xC0426164;	// calculated by 756 datasheet for 50 kHz
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
	  i2cError(0x87);
  }


  //  problem:  i2c works, but address and RW are not forwarded on SDA line, SDA held low
  //            during address and RW bits.
  //              as seen on oscilloscope, but releasing SDA line for slave ack bit works
  //                BUT slave sends NACK (lets SDA high), what produces an
  //                       nack / stop condition on EV_IRQ
  //
  //  (PN 18.oct 21) still possible  todo
  //    disable filters since they produce delay,
  //    check clk conditions
  //          (30.4.2  I2C clock requirements,30.4.4  I2C initialization-->I2C timings ff)
  //    PN, 21.oct 21: changed according to datasheet, but did not help, same mess
  //
  //  if this does not help enable i2c2 to 4
  //      (shift clock for data on SDA line might eventually come from other sources...)
  //  PN, 21.Oct21: as expected also this did not work
  //
  //  if this also does not help transfer this very same code as
  //        close as possible on stm32F103
  //         (with the minimum needed changes due to hw differences in interface)

  //  if still not running contact stm and check entirely why also their EEPROM i2c example
  //    specially made for stm32F756..   does not work on stm32F756zg nucleo board  ???



  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_DISABLE) != HAL_OK)
  {
	  i2cError(0x88);
  }

  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
	  i2cError(0x89);
  }
  HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
  HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

  __HAL_I2C_ENABLE_IT(&hi2c1,(I2C_IT_ERRI | I2C_IT_TCI));
  __HAL_I2C_ENABLE_IT(&hi2c1,(I2C_IT_STOPI | I2C_IT_NACKI));

  __HAL_I2C_AutoEndENABLE(&hi2c1);

#ifdef i2cUseDma
  i2cDmaInit();
#else
  __HAL_I2C_ENABLE_IT(&hi2c1,(I2C_IT_RXI | I2C_IT_TXI));
#endif

  enableI2c();
  i2cInitialized = 1;

}


//void initI2c()
//{
//	initI2c1();
////	initI2cTo4();
//}

//void reInitI2cAfterError()   // called from backgroundEventQ
//{
////	uint8_t err = OS_ERR_NONE;
////	OSSemDel(i2cJobSem, OS_DEL_ALWAYS, &err);
////	OSSemDel(i2cResourceSem, OS_DEL_ALWAYS, &err);
//	initI2c();
//}


//void reInitOnError()
//{
////	i2cSendStop(&hi2c1);
//	if (resetOnError != 0)  {
////		backGroundEvent   bgEv;
////		bgEv.evType = i2cReinitNeeded;
////		proceedBackGroundEvent(&bgEv);
//	}
//}
