/*
 * uart-comms.h
 *
 *  Created on: Oct 12, 2020
 *      Author: ixchel
 */

#ifndef ST_TEMPIX_TOOLS_UART_COMMS_H_
#define ST_TEMPIX_TOOLS_UART_COMMS_H_
#include <cmsis_os.h>
#include <uart-hw.h>

#ifdef  __cplusplus
extern "C"
{
#endif

#define serialStringSz   0x80
#define serialBufferSize serialStringSz + 1

extern uint8_t uartInputBuffer [serialBufferSize];

void init_printf();
void err_printf ( char *emsg, ...);
void info_printf( char *emsg, ...);
void pid_printf( char *emsg, ...);
void private_printf( char *emsg, ...);

//void start_printf();
void printStartMessage();

void forwardReceivedStringBuffer(char* strBuffer);

#ifdef  __cplusplus
}
#endif


#endif /* ST_TEMPIX_TOOLS_UART_COMMS_H_ */
