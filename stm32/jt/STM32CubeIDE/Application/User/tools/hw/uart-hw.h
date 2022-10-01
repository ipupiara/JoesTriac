/*
 * uart-comms.h
 *
 *  Created on: Oct 12, 2020
 *      Author: ixchel
 */

#ifndef ST_TEMPIX_TOOLS_UART_HW_H_
#define ST_TEMPIX_TOOLS_UART_HW_H_

#include<stdio.h>
#include  <stm32f7xx_hal.h>

#define maxUartReceiveDmaStringSize  0x80


#define dmaAvailable  0x01

uint8_t initUartHw();

uint8_t startUartHw();

uint8_t sendUartString(char* sndStr);


uint8_t enableUartInterrupts();

uint8_t disableUartInterrupts();


#endif /* ST_TEMPIX_TOOLS_UART_HW_H_ */
