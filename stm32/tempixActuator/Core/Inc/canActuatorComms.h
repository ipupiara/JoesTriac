/*
 * canComms.h
 *
 *  Created on: Jun 11, 2021
 *      Author: peetz
 */

#ifndef INC_CANACTUATORCOMMS_H_
#define INC_CANACTUATORCOMMS_H_
#include <stdint.h>
#include <stm32f1xx_hal.h>

void MX_CAN_Init(void);

void dispatchCanMessage( CAN_RxHeaderTypeDef *pHeader, uint8_t aData[]);
void sendPingToTempixController();

#endif /* INC_CANACTUATORCOMMS_H_ */
