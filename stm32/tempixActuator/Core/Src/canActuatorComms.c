/*
 * canComms.c
 *
 *  Created on: Jun 11, 2021
 *      Author: peetz
 */

#include <canActuatorComms.h>
#include <canRelated.h>
#include <main.h>

CAN_HandleTypeDef hcan;

void respondPingRequest(uint8_t aData[])
{

}

void dispatchCanMessage( CAN_RxHeaderTypeDef *pHeader, uint8_t aData[])
{
	if (isCanMessageType(pHeader,controllerPingRequest)) {
		respondPingRequest(aData);
	}
}


void dispatchMsgOfFifo(uint32_t RxFifo)
{
	CAN_RxHeaderTypeDef  mHeader;
	uint8_t mData[8];

	if ( HAL_CAN_GetRxMessage(&hcan, RxFifo, &mHeader, mData) == HAL_OK) {
		dispatchCanMessage(&mHeader, mData);
	}  else {
		// handle some Error
	}
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	uint32_t interrupts = (uint32_t) hcan.Instance->IER;

	// Receive FIFO 0 message pending interrupt management
	if ((interrupts & CAN_IT_RX_FIFO0_MSG_PENDING) != 0) {
		// Check if message is still pending
		if ((hcan.Instance->RF0R & CAN_RF0R_FMP0) != 0) {
			dispatchMsgOfFifo(CAN_RX_FIFO0);
		}
	}

}

void CAN1_RX1_IRQHandler(void)
{
	uint32_t interrupts = (uint32_t) hcan.Instance->IER;

	// Receive FIFO 0 message pending interrupt management
	if ((interrupts & CAN_IT_RX_FIFO1_MSG_PENDING) != 0) {
		// Check if message is still pending
		if ((hcan.Instance->RF1R & CAN_RF1R_FMP1) != 0) {
			dispatchMsgOfFifo(CAN_RX_FIFO1);
		}
	}
}


void CAN1_SCE_IRQHandler(void)
{
	// empty handler for method installation
	// uint32_t errorcode = HAL_CAN_ERROR_NONE;
	uint32_t interrupts = hcan.Instance->IER;
	uint32_t msrflags = hcan.Instance->MSR;
//	uint32_t tsrflags = hcan1.Instance->TSR;
//	uint32_t rf0rflags = hcan1.Instance->RF0R;
//	uint32_t rf1rflags = hcan1.Instance->RF1R;
	uint32_t esrflags = hcan.Instance->ESR;

	  //
	  if ((interrupts & CAN_IT_ERROR) != 0)
	  {
	    if ((msrflags & CAN_MSR_ERRI) != 0)
	    {
	      //
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
	        CLEAR_BIT(hcan.Instance->ESR, CAN_ESR_LEC);
	      }
	    }

	    // Clear ERRI Flag
	    __HAL_CAN_CLEAR_FLAG(&hcan, CAN_FLAG_ERRI);
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


void USB_HP_CAN1_TX_IRQHandler(void)
{
	uint32_t tsrflags = (uint32_t) hcan.Instance->TSR;
	if ((tsrflags & CAN_TSR_RQCP0) != 0)		{
		// Clear the Transmission Complete flag (and TXOK0,ALST0,TERR0 bits)
		__HAL_CAN_CLEAR_FLAG(&hcan, CAN_FLAG_RQCP0);

//		err = OSFlagPost(TransmitMailboxStatus,mailbox0Free,OS_FLAG_SET, &err);
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
		__HAL_CAN_CLEAR_FLAG(&hcan, CAN_FLAG_RQCP0);

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
		__HAL_CAN_CLEAR_FLAG(&hcan, CAN_FLAG_RQCP2);

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
}


void initCanFilters()
{
	HAL_StatusTypeDef  initState;
	CAN_FilterTypeDef  filterTypeDef;
	filterTypeDef.FilterScale = CAN_FILTERSCALE_16BIT;
	filterTypeDef.FilterMode = CAN_FILTERMODE_IDMASK;
	filterTypeDef.FilterIdHigh = 0x0100;   // 0x0100 on receiver side
	filterTypeDef.FilterMaskIdHigh =  0x0700;
	filterTypeDef.FilterIdLow  = 0x07FF;
	filterTypeDef.FilterMaskIdLow =	0x07FF;
	filterTypeDef.FilterBank = 0;
	filterTypeDef.SlaveStartFilterBank = 1;
	filterTypeDef.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	initState = HAL_CAN_ConfigFilter(&hcan, &filterTypeDef);
	if (initState != HAL_OK) {
		Install_Error_Handler();
	}
}


void MX_CAN_Init(void)
{


	GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  hcan.Instance = CAN1;
//  hcan.Init.Prescaler =  clockRelValueVsMaxClk(uint16_t valAtMax);
  hcan.Init.Prescaler =  48; //   125 kb/s;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }

  initCanFilters();

  HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
  HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
  HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

  __HAL_CAN_ENABLE_IT(&hcan,CAN_IER_TMEIE);

  __HAL_CAN_ENABLE_IT(&hcan,CAN_IER_FMPIE0);
  __HAL_CAN_ENABLE_IT(&hcan,CAN_IER_FMPIE1);

  __HAL_CAN_ENABLE_IT(&hcan,CAN_IER_ERRIE);

  __HAL_CAN_ENABLE_IT(&hcan,CAN_IER_EWGIE);
  __HAL_CAN_ENABLE_IT(&hcan,CAN_IER_EPVIE);
  __HAL_CAN_ENABLE_IT(&hcan,CAN_IER_BOFIE);
  __HAL_CAN_ENABLE_IT(&hcan,CAN_IER_LECIE);

  HAL_CAN_Start(&hcan);  // clears CAN_MCR_INRQ

}




void sendPingToTempixController()
{
	TempixSimpleCommand scmd;
	scmd.commandId = thottleActorPingRquest;
	scmd.commandData1 = 0xaaaaaaaa;
	scmd.commandData2 = 0xaaaaaaaa;
	sendCanTempixSimpleCommand(&hcan,&scmd);

}


