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



#define maxSerialStringSz   80
#define charWidth sizeof(char)




extern osSemaphoreId uartSendSemaphore;

uint8_t initUartHw();

uint8_t startUartHw();

osStatus_t sendUartString(char* sndStr);

#ifdef  __cplusplus
}
#endif


#endif /* ST_TEMPIX_TOOLS_UART_HW_H_ */
