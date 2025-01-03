#include <string.h>
#include <mainJt.h>
#include <canComms.h>
#include <defines.h>
#include  <stm32f7xx_hal.h>
#include "cmsis_os.h"
#include <timers.h>
#include <task.h>
#include <FreeRTOS.h>


CAN_HandleTypeDef hcan1;

//#define mailbox0Free   0x01
//#define mailbox1Free   0x02
//#define mailbox2Free   0x03
//OS_FLAG_GRP*   TransmitMailboxStatus;

//OS_EVENT *canQSem;

////////////////////    canTest   /////////////////////////



// todo  messageReceived handling to ui,


#define triacPingId   0x101
#define triacPingRespondId   0x203

#define ammeterPingId    0x201
#define ammeterPingRespondId   0x103

typedef struct {
	uint32_t  rec101Cnt;
	uint32_t  rec203Cnt;
	uint32_t  rec201Cnt;
	uint32_t  rec103Cnt;
	uint32_t  errorCnt;
} canDebugEventCountersType;

canDebugEventCountersType canDebugEventCounters;

void initCanFilters();

canTestTypes currentTestSelection;
canHosts currentHost;
uint32_t msgNrToSend;

osTimerId_t   canTestTimer;

void sendCanPingMessage(uint32_t pingId)
{
	TempixSimpleCommand scmd;
	scmd.commandId = pingId;
	scmd.commandData1 = 0xaaaaaaaa;
	scmd.commandData2 = 0xaaaaaaaa;

	syncSendTempixSimpleCommand( &scmd);
}

void sendTestMessage(canHosts cHost )
{
	if (currentHost == triacHost) {
		sendCanPingMessage(triacPingId);
	}
	if (currentHost == ammeterHost) {
		sendCanPingMessage(ammeterPingId);
	}

}

void canTestTickCallback(void *argument)
{
	sendTestMessage(currentHost);
}


void setCurrentCanSelection(canHosts cHost ,canTestTypes canTestType)
{
	currentHost = cHost;
	currentTestSelection = canTestType;
}

void stopContinuousTest(canHosts cHost )
{
//	osTimerStop(canTestTimer);
	xTimerStop( canTestTimer, 100 );
}

void startContinuousTest(canHosts cHost )
{
	osStatus_t  status;

	status = osTimerStart (canTestTimer, 1000);
	if (status !=  osOK)  {
		errorHandler((uint32_t)status ,goOn," osTimerStart "," osStarted ");
	}
}

void doNoTest(canHosts cHost )
{
	stopContinuousTest(cHost);
}

void doSingleTest(canHosts cHost )
{
	stopContinuousTest(cHost );
	sendTestMessage(cHost);
}


void sendCanTestMessage(canHosts cHost ,canTestTypes canTestType)
{
	if (cHost == triacHost)  {
		switch (canTestType) {
			case noCanTest: {
				doNoTest(triacHost);
				break;
			}
			msgNrToSend = 101;
			case singleCanTest: {
				doSingleTest(triacHost);
				break;
			}
			case continuousCanTest: {
				startContinuousTest(triacHost);
				break;
			}
			default : {
				errorHandler(canTestType ,goOn," sendCanTestMessage "," unknown testtype or host (triacHost)");
			}
		}
	}
	if (cHost == ammeterHost)  {
		switch (canTestType) {
			case noCanTest: {
				doNoTest(ammeterHost);
				break;
			}
			msgNrToSend = 101;
			case singleCanTest: {
				doSingleTest(ammeterHost);
				break;
			}
			case continuousCanTest: {
				startContinuousTest(ammeterHost);
				break;
			}
			default : {
				errorHandler(canTestType ,goOn," sendCanTestMessage "," unknown testtype or host (ammeterHost)");
			}
		}
	}
}

void initCanTest()
{
	canTestTimer = osTimerNew (canTestTickCallback, osTimerPeriodic, (void *) 0x01, NULL);
	if (canTestTimer  == NULL)   {
		errorHandler((uint32_t)canTestTimer ,stop," canTestTimer ","initJt");
	}
	memset(&canDebugEventCounters,0,sizeof(canDebugEventCounters));
}



uint8_t syncSendTempixSimpleCommand( TempixSimpleCommand* scmd)
{
	uint8_t err = osOK;
//	if (err == OS_ERR_NONE ) {
//		if ( sendCanTempixSimpleCommand(&hcan1, scmd) == 1) {
//			osError = 0;
//		}
//	else {err = 1;}

	return err;
}



void sendBackGroundCanMessage( CAN_RxHeaderTypeDef *pHeader, uint8_t aData[])
{
//	backGroundEvent bgEv;
//	bgEv.evType = canMessageReadyForSend;
//	bgEv.evData.canMessage.commandId = pHeader->StdId;
//	memset((void*)&bgEv,0,sizeof(bgEv));
//	memmove((char*)bgEv.evData.canMessage.commandData1,(char*)aData,8);
//	proceedBackGroundEvent(&bgEv);
}

void dispatchMsgOfFifo(uint32_t RxFifo)
{
	CAN_RxHeaderTypeDef  mHeader;
	uint8_t mData[8];
	CJoesPresenterEventT  msg;

	if (RxFifo == CAN_RX_FIFO0)  {
		if ( HAL_CAN_GetRxMessage(&hcan1, RxFifo, &mHeader, mData) == HAL_OK) {
			if (mHeader.StdId == thottleActorPingResponse) {
				sendBackGroundCanMessage(&mHeader, mData);
			}
		}  else {
			// handle some Error
		}
	}
	if (RxFifo == CAN_RX_FIFO1)  {

// todo put ammeter messages first in an if to avoid debug handling during calibration

		if ( HAL_CAN_GetRxMessage(&hcan1, RxFifo, &mHeader, mData) == HAL_OK) {
			memset(&msg,0,sizeof(msg));
			switch (mHeader.StdId) {
						case 0x101: {
							++(canDebugEventCounters.rec101Cnt);
							msg.evData.canDebugScreenData.msgId=0x101;
							msg.evData.canDebugScreenData.amtReceived=canDebugEventCounters.rec101Cnt;
							break;
						}
						case 0x103: {
							++(canDebugEventCounters.rec103Cnt);
							msg.evData.canDebugScreenData.msgId=0x103;
							msg.evData.canDebugScreenData.amtReceived=canDebugEventCounters.rec103Cnt;
							break;
						}
						case 0x201: {
							++(canDebugEventCounters.rec201Cnt);
							msg.evData.canDebugScreenData.msgId=0x201;
							msg.evData.canDebugScreenData.amtReceived=canDebugEventCounters.rec201Cnt;
							break;
						}
						case 0x203: {
							++(canDebugEventCounters.rec203Cnt);
							msg.evData.canDebugScreenData.msgId=0x203;
							msg.evData.canDebugScreenData.amtReceived=canDebugEventCounters.rec203Cnt;
							break;
						}
						default : {
//							errorHandler(mJtEv.evType ,goOn," osMessageQueueGet unknown event "," mainJt ");
						}
			}
			if (msg.evData.canDebugScreenData.msgId != 0) {
				msg.messageType = canDebugData;
				msg.evData.canDebugScreenData.amtErrors = canDebugEventCounters.errorCnt;
				sendPresenterMessage(&msg);
			}
		}  else {
			// handle some Error
		}
	}
}

void CAN1_TX_IRQHandler(void)
{
	uint32_t tsrflags = (uint32_t) hcan1.Instance->TSR;

//	CPU_SR_ALLOC();
////	INT8U err = OS_ERR_NONE;
//
//	CPU_CRITICAL_ENTER();
//	OSIntEnter();           /* Tell OS that we are starting an ISR           */
//	CPU_CRITICAL_EXIT();


//	uint8_t err;

	if ((tsrflags & CAN_TSR_RQCP0) != 0)		{
		// Clear the Transmission Complete flag (and TXOK0,ALST0,TERR0 bits)
		__HAL_CAN_CLEAR_FLAG(&hcan1, CAN_FLAG_RQCP0);

////		err = OSFlagPost(TransmitMailboxStatus,mailbox0Free,OS_FLAG_SET, &err);
//		err = OSSemPost(canQSem);
//		if (err != OS_ERR_NONE) {
//			//  report error
//		}

		if ((tsrflags & CAN_TSR_TXOK0) != 0) {
			// Transmission Mailbox 0 complete callback
		}
		else  {
			if ((tsrflags & CAN_TSR_ALST0) != 0) {
			  // Update error code
			} else if ((tsrflags & CAN_TSR_TERR0) != 0) {
			  /* Update error code */
			} else  {// Transmission Mailbox 0 abort callback  HAL_CAN_TxMailbox0AbortCallback(hcan);
			}
		}
	}

	if ((tsrflags & CAN_TSR_RQCP1) != 0)		{
		// Clear the Transmission Complete flag (and TXOK0,ALST0,TERR0 bits)
		__HAL_CAN_CLEAR_FLAG(&hcan1, CAN_FLAG_RQCP0);

////		err = OSFlagPost(TransmitMailboxStatus,mailbox1Free,OS_FLAG_SET, &err);
//		err = OSSemPost(canQSem);
//		if (err != OS_ERR_NONE) {
//			//  report error
//		}

		if ((tsrflags & CAN_TSR_TXOK1) != 0) {
			// Transmission Mailbox 0 complete callback
		}
		else  {
			if ((tsrflags & CAN_TSR_ALST1) != 0) {
			  // Update error code
			} else if ((tsrflags & CAN_TSR_TERR1) != 0) {
			  /* Update error code */
			} else  {// Transmission Mailbox 0 abort callback  HAL_CAN_TxMailbox0AbortCallback(hcan);
			}
		}
	}

	if ((tsrflags & CAN_TSR_RQCP2) != 0)		{
		// Clear the Transmission Complete flag (and TXOK0,ALST0,TERR0 bits)
		__HAL_CAN_CLEAR_FLAG(&hcan1, CAN_FLAG_RQCP2);

////		err = OSFlagPost(TransmitMailboxStatus,mailbox2Free,OS_FLAG_SET, &err);
//		err = OSSemPost(canQSem);
//		if (err != OS_ERR_NONE) {
//			//  report error
//		}

		if ((tsrflags & CAN_TSR_TXOK2) != 0) {
			// Transmission Mailbox 0 complete callback
		}
		else  {
			if ((tsrflags & CAN_TSR_ALST2) != 0) {
			  // Update error code
			} else if ((tsrflags & CAN_TSR_TERR2) != 0) {
			  /* Update error code */
			} else  {// Transmission Mailbox 0 abort callback  HAL_CAN_TxMailbox0AbortCallback(hcan);
			}
		}
	}

//   todo consider something like this:
//	if ((__HAL_CAN_GET_FLAG(&hcan1,CAN_FLAG_TME0)) && (__HAL_CAN_GET_FLAG(&hcan1,CAN_FLAG_TME1))&&(__HAL_CAN_GET_FLAG(&hcan1,CAN_FLAG_TME2)))  {
//		checkSemaphoreStateFull();
//	}

//	OSIntExit();
}



void CAN1_RX0_IRQHandler(void)
{

	uint32_t interrupts = (uint32_t) hcan1.Instance->IER;

//	CPU_SR_ALLOC();
////	INT8U err = OS_ERR_NONE;
//
//	CPU_CRITICAL_ENTER();
//	OSIntEnter();           /* Tell OS that we are starting an ISR           */
//	CPU_CRITICAL_EXIT();
//

	// Receive FIFO 0 message pending interrupt management
	if ((interrupts & CAN_IT_RX_FIFO0_MSG_PENDING) != 0) {
		// Check if message is still pending
		if ((hcan1.Instance->RF0R & CAN_RF0R_FMP0) != 0) {
			dispatchMsgOfFifo(CAN_RX_FIFO0);
		}
	}


//
//	  /* Receive FIFO 0 overrun interrupt management *****************************/
//	  if ((interrupts & CAN_IT_RX_FIFO0_OVERRUN) != RESET)
//	  {
//	    if ((rf0rflags & CAN_RF0R_FOVR0) != RESET)
//	    {
//	      /* Set CAN error code to Rx Fifo 0 overrun error */
//	      errorcode |= HAL_CAN_ERROR_RX_FOV0;
//
//	      /* Clear FIFO0 Overrun Flag */
//	      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV0);
//	    }
//	  }
//
//	  /* Receive FIFO 0 full interrupt management ********************************/
//	  if ((interrupts & CAN_IT_RX_FIFO0_FULL) != RESET)
//	  {
//	    if ((rf0rflags & CAN_RF0R_FULL0) != RESET)
//	    {
//	      /* Clear FIFO 0 full Flag */
//	      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FF0);
//
//	      /* Receive FIFO 0 full Callback */
//	      /* Call weak (surcharged) callback */
//	      HAL_CAN_RxFifo0FullCallback(hcan);
//	    }
//	  }

//	 OSIntExit();
}

void CAN1_RX1_IRQHandler(void)
{
	uint32_t interrupts = (uint32_t) hcan1.Instance->IER;

//	CPU_SR_ALLOC();
////	INT8U err = OS_ERR_NONE;
//
//	CPU_CRITICAL_ENTER();
//	OSIntEnter();           /* Tell OS that we are starting an ISR           */
//	CPU_CRITICAL_EXIT();

	// Receive FIFO 0 message pending interrupt management
	if ((interrupts & CAN_IT_RX_FIFO1_MSG_PENDING) != 0) {
		// Check if message is still pending
		if ((hcan1.Instance->RF1R & CAN_RF1R_FMP1) != 0) {
			dispatchMsgOfFifo(CAN_RX_FIFO1);
		}
	}

//	 OSIntExit();
}

void CAN1_SCE_IRQHandler(void)
{
	// empty handler for method installation
	// uint32_t errorcode = HAL_CAN_ERROR_NONE;
	uint32_t interrupts = hcan1.Instance->IER;
	uint32_t msrflags = hcan1.Instance->MSR;
//	uint32_t tsrflags = hcan1.Instance->TSR;
//	uint32_t rf0rflags = hcan1.Instance->RF0R;
//	uint32_t rf1rflags = hcan1.Instance->RF1R;
	uint32_t esrflags = hcan1.Instance->ESR;



	  //
	  if ((interrupts & CAN_IT_ERROR) != 0)
	  {
	    if ((msrflags & CAN_MSR_ERRI) != 0)
	    {
	    	++canDebugEventCounters.errorCnt;
	      if (((interrupts & CAN_IT_ERROR_WARNING) != 0) &&
	          ((esrflags & CAN_ESR_EWGF) != 0))
	      {

	      }

	      //
	      if (((interrupts & CAN_IT_ERROR_PASSIVE) != 0) &&
	          ((esrflags & CAN_ESR_EPVF) != 0))
	      {

	      }

	      //
	      if (((interrupts & CAN_IT_BUSOFF) != 0) &&
	          ((esrflags & CAN_ESR_BOFF) != 0))
	      {

	      }


	      if (((interrupts & CAN_IT_LAST_ERROR_CODE) != 0) &&
	          ((esrflags & CAN_ESR_LEC) != 0))
	      {
	        switch (esrflags & CAN_ESR_LEC)
	        {
	          case (CAN_ESR_LEC_0):
	            //
	            //errorcode |= HAL_CAN_ERROR_STF;
	            break;
	          case (CAN_ESR_LEC_1):
	            //
	            //errorcode |= HAL_CAN_ERROR_FOR;
	            break;
	          case (CAN_ESR_LEC_1 | CAN_ESR_LEC_0):
	            //
	            //errorcode |= HAL_CAN_ERROR_ACK;
	            break;
	          case (CAN_ESR_LEC_2):
	            //
	            //errorcode |= HAL_CAN_ERROR_BR;
	            break;
	          case (CAN_ESR_LEC_2 | CAN_ESR_LEC_0):
	            //
	            //errorcode |= HAL_CAN_ERROR_BD;
	            break;
	          case (CAN_ESR_LEC_2 | CAN_ESR_LEC_1):
	            //
	            //errorcode |= HAL_CAN_ERROR_CRC;
	            break;
	          default:
	            break;
	        }

	        // Clear Last error code Flag
	        CLEAR_BIT(hcan1.Instance->ESR, CAN_ESR_LEC);
	      }
	    }

	    // Clear ERRI Flag
	    __HAL_CAN_CLEAR_FLAG(&hcan1, CAN_FLAG_ERRI);
	  }

	  //  /* Sleep interrupt management *********************************************/
	  //  if ((interrupts & CAN_IT_SLEEP_ACK) != RESET)
	  //  {
	  //    if ((msrflags & CAN_MSR_SLAKI) != RESET)
	  //    {
	  //      /* Clear Sleep interrupt Flag */
	  //      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_SLAKI);
	  //
	  //      /* Sleep Callback */
	  //      /* Call weak (surcharged) callback */
	  //      HAL_CAN_SleepCallback(hcan);
	  //    }
	  //  }
	  //
	  //  /* WakeUp interrupt management *********************************************/
	  //  if ((interrupts & CAN_IT_WAKEUP) != RESET)
	  //  {
	  //    if ((msrflags & CAN_MSR_WKUI) != RESET)
	  //    {
	  //      /* Clear WakeUp Flag */
	  //      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_WKU);
	  //
	  //      /* WakeUp Callback */
	  //      /* Call weak (surcharged) callback */
	  //      HAL_CAN_WakeUpFromRxMsgCallback(hcan);
	  //    }
	  //  }

}



void initCanInterrupts(CAN_HandleTypeDef* hcan)
{
	//   todo implement right behaviour
//	BSP_IntVectSet(CAN1_TX_IRQn,tempixIsrPrioLevel ,CPU_INT_KA,CAN1_TX_IRQHandler);
//	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
//	BSP_IntVectSet(CAN1_RX0_IRQn,tempixIsrPrioLevel ,CPU_INT_KA,CAN1_RX0_IRQHandler);
//	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
//	BSP_IntVectSet(CAN1_RX1_IRQn,tempixIsrPrioLevel,CPU_INT_KA,CAN1_RX1_IRQHandler);
//	HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
//	BSP_IntVectSet(CAN1_SCE_IRQn,tempixIsrPrioLevel ,CPU_INT_KA,CAN1_SCE_IRQHandler);
//	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

	__HAL_CAN_ENABLE_IT(&hcan1,CAN_IER_TMEIE);

	__HAL_CAN_ENABLE_IT(&hcan1,CAN_IER_FMPIE0);
	__HAL_CAN_ENABLE_IT(&hcan1,CAN_IER_FMPIE1);

	__HAL_CAN_ENABLE_IT(&hcan1,CAN_IER_ERRIE);

	__HAL_CAN_ENABLE_IT(&hcan1,CAN_IER_EWGIE);
	__HAL_CAN_ENABLE_IT(&hcan1,CAN_IER_EPVIE);
	__HAL_CAN_ENABLE_IT(&hcan1,CAN_IER_BOFIE);
	__HAL_CAN_ENABLE_IT(&hcan1,CAN_IER_LECIE);
}


void initCanFilters()
{
	HAL_StatusTypeDef  initState;
	CAN_FilterTypeDef  filterTypeDef;
	filterTypeDef.FilterScale = CAN_FILTERSCALE_16BIT;
	filterTypeDef.FilterMode = CAN_FILTERMODE_IDMASK;
	filterTypeDef.FilterIdHigh = 0x200;   // 0x0100 on receiver side
	filterTypeDef.FilterMaskIdHigh =  0x0701;
	filterTypeDef.FilterIdLow  = 0x07FF;
	filterTypeDef.FilterMaskIdLow =	0x07FF;
	filterTypeDef.FilterBank = 0;
//	filterTypeDef.SlaveStartFilterBank = 1;
	filterTypeDef.FilterFIFOAssignment = CAN_FILTER_FIFO0;

	filterTypeDef.FilterScale = CAN_FILTERSCALE_16BIT;
	filterTypeDef.FilterMode = CAN_FILTERMODE_IDLIST;
	filterTypeDef.FilterIdHigh = 0x0101;   // 0x0100 on receiver side
	filterTypeDef.FilterMaskIdHigh =  0x0103;
	filterTypeDef.FilterIdLow  = 0x0201;
	filterTypeDef.FilterMaskIdLow =	0x203;
	filterTypeDef.FilterBank = 1;
	filterTypeDef.SlaveStartFilterBank = 2;
	filterTypeDef.FilterFIFOAssignment = CAN_FILTER_FIFO0;


	initState = HAL_CAN_ConfigFilter(&hcan1, &filterTypeDef);
	if (initState != HAL_OK) {
		Install_Error_Handler();
	}
}


void initCanComms()
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};

//	canQSem = OSSemCreate(3);
//	if (canQSem == NULL) {
//		Install_Error_Handler();
//	}

   __HAL_RCC_CAN1_CLK_ENABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**CAN1 GPIO Configuration
	PA11     ------> CAN1_RX
	PA12     ------> CAN1_TX
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



	hcan1.Instance = CAN1;
	hcan1.Init.Prescaler = 72;              // 125.00   kb/s
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_3TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.AutoBusOff = DISABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.AutoRetransmission = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TransmitFifoPriority = ENABLE;
	if (HAL_CAN_Init(&hcan1) != HAL_OK){
		Install_Error_Handler();
	}
	initCanFilters();
	initCanInterrupts(&hcan1);
	HAL_CAN_Start(&hcan1);

}


