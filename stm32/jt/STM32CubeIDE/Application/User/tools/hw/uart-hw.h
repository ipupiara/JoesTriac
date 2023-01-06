/*
 * uart-comms.h
 *
 *  Created on: Oct 12, 2020
 *      Author: ixchel
 */

#ifndef ST_TEMPIX_TOOLS_UART_HW_H_
#define ST_TEMPIX_TOOLS_UART_HW_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include<stdio.h>
#include  <stm32f7xx_hal.h>
#include "cmsis_os.h"

#define charWidth sizeof(char)

#define dmaTxAvailable  0x01       //  rx dma stream occupied by touchgfx, only tx possible


extern osMessageQueueId_t uartSendSemaphoreQ;

uint8_t initUartHw();

uint8_t startUartHw();

osStatus_t sendUartString(char* sndStr);

#ifdef  __cplusplus
}
#endif


#endif /* ST_TEMPIX_TOOLS_UART_HW_H_ */
