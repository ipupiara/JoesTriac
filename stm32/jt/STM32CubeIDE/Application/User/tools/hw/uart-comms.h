/*
 * uart-comms.h
 *
 *  Created on: Oct 12, 2020
 *      Author: ixchel
 */

#ifndef ST_TEMPIX_TOOLS_UART_COMMS_H_
#define ST_TEMPIX_TOOLS_UART_COMMS_H_

#ifdef  __cplusplus
extern "C"
{
#endif


#define maxSerialStringSz   80
#define charWidth sizeof(char)


void init_printf();
void err_printf ( char *emsg, ...);
void info_printf( char *emsg, ...);


//void start_printf();
void printStartMessage();

void forwardReceivedStringBuffer(char* strBuffer);

#ifdef  __cplusplus
}
#endif


#endif /* ST_TEMPIX_TOOLS_UART_COMMS_H_ */
