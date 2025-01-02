#ifndef canRelated_h
#define canRelated_h
#include <stdint.h>
#include  <stm32f7xx_hal.h>

#ifdef  __cplusplus
extern "C"
{
#endif

#ifdef STM32F756xx
#include <stm32f7xx_hal.h>
#endif

#ifdef  STM32F103xB
#include <stm32f1xx_hal.h>
#endif
// lowest 11 bits allowed, ie.  Masked against  0000 07FF
#define controllerCommandMessage   0x00000101

#define controllerPingRequest      0x00000121
#define controllerPingResponse     0x00000121

#define thottleActorStateMessage	 0x00000201

#define thottleActorAlarmMessage	 0x00000210

#define thottleActorPingResponse	 0x00000221
#define thottleActorPingRquest		 0x00000222




//typedef struct
//{
//	uint32_t   canId;
//	uint8_t     message [8];
//}  TempixCanMessage;


typedef struct
{
	uint32_t   commandId;
	uint32_t   commandData1;
	uint32_t   commandData2;     // datasize always 8
}  TempixSimpleCommand;

void Install_Error_Handler();

uint8_t isCanMessageType(CAN_RxHeaderTypeDef *pHeader   , uint32_t msgT);

//void dispatchMsgOfFifo(uint32_t RxFifo);

uint8_t addTxMessage(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t aData[]);

uint8_t sendCanTempixSimpleCommand(CAN_HandleTypeDef* pCan, TempixSimpleCommand* scmd);


#ifdef  __cplusplus
}
#endif

#endif

