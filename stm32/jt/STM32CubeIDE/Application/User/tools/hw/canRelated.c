#include <string.h>
#include <canRelated.h>
#include  <stm32f7xx_hal.h>

void Install_Error_Handler()
{
	while(1) {}
}

uint8_t isCanMessageType(CAN_RxHeaderTypeDef *pHeader   , uint32_t msgT)
{
	uint8_t res = 0;
	res = (pHeader->StdId == msgT);

	return res;
}

uint8_t addTxMessage(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t aData[])
{
  uint32_t transmitmailbox;

    /* Check that not all the Tx mailboxes are  full */
    if (((hcan->Instance->TSR & CAN_TSR_TME0) != 0) ||
        ((hcan->Instance->TSR & CAN_TSR_TME1) != 0) ||
        ((hcan->Instance->TSR & CAN_TSR_TME2) != 0))
    {
      /* Select an empty transmit mailbox */
      transmitmailbox = (hcan->Instance->TSR & CAN_TSR_CODE) >> CAN_TSR_CODE_Pos;



      /* Set up the Id */
      if (pHeader->IDE == CAN_ID_STD)
      {
        hcan->Instance->sTxMailBox[transmitmailbox].TIR = ((pHeader->StdId << CAN_TI0R_STID_Pos) |
                                                           pHeader->RTR);
      }
      else
      {
        hcan->Instance->sTxMailBox[transmitmailbox].TIR = ((pHeader->ExtId << CAN_TI0R_EXID_Pos) |
                                                           pHeader->IDE |
                                                           pHeader->RTR);
      }

      /* Set up the DLC */
      hcan->Instance->sTxMailBox[transmitmailbox].TDTR = (pHeader->DLC);

      /* Set up the Transmit Global Time mode */
      if (pHeader->TransmitGlobalTime == ENABLE)
      {
        SET_BIT(hcan->Instance->sTxMailBox[transmitmailbox].TDTR, CAN_TDT0R_TGT);
      }

      /* Set up the data field */
      WRITE_REG(hcan->Instance->sTxMailBox[transmitmailbox].TDHR,
                ((uint32_t)aData[7] << CAN_TDH0R_DATA7_Pos) |
                ((uint32_t)aData[6] << CAN_TDH0R_DATA6_Pos) |
                ((uint32_t)aData[5] << CAN_TDH0R_DATA5_Pos) |
                ((uint32_t)aData[4] << CAN_TDH0R_DATA4_Pos));
      WRITE_REG(hcan->Instance->sTxMailBox[transmitmailbox].TDLR,
                ((uint32_t)aData[3] << CAN_TDL0R_DATA3_Pos) |
                ((uint32_t)aData[2] << CAN_TDL0R_DATA2_Pos) |
                ((uint32_t)aData[1] << CAN_TDL0R_DATA1_Pos) |
                ((uint32_t)aData[0] << CAN_TDL0R_DATA0_Pos));

      /* Request transmission */
      SET_BIT(hcan->Instance->sTxMailBox[transmitmailbox].TIR, CAN_TI0R_TXRQ);

      /* Return function status */
      return 1;
    }
    else
    {
      return 0;
    }
}

uint8_t  sendCanData(CAN_HandleTypeDef* pCan, uint32_t stdID, uint8_t aData[])
{
	uint8_t res = 0;
	CAN_TxHeaderTypeDef header;
	memset(&header,0x00,sizeof(header));
	header.StdId= stdID;
	header.IDE = CAN_ID_STD;
	header.DLC = 8;
	header.RTR = CAN_RTR_DATA;

	res= addTxMessage(pCan, &header, aData);
	return res;
}

uint8_t sendCanTempixSimpleCommand(CAN_HandleTypeDef* pCan,  TempixSimpleCommand* scmd)
{
	uint8_t res = 0;
	uint8_t msgData[8];
	memcpy(msgData,&scmd->commandData1,4);  // tobe tested
	memcpy(&msgData[4],&scmd->commandData2,4);


	res = sendCanData(pCan, scmd->commandId, msgData);
	return res;
}

