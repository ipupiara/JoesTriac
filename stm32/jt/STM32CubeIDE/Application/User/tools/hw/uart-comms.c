/*
 * uart-comms.c
 *
 *  Created on: Oct 12, 2020
 *      Author: ixchel
 */


#include <string.h>
#include <math.h>
#include <stdarg.h>
#include "cmsis_os.h"
#include <mainJt.h>
#include "cmsis_os.h"
#include <mainJt.h>
#include <uart-comms.h>

//#define debugPid
#define useLogging   //  todo in productive system use variable for this which can be set in configutation

uint32_t  amtErr;			// amt calls to err_printf
uint32_t  amtPrintErr;   // errors during print out, where err_printf should not be called

uint8_t  serialOn;


osThreadId_t serialQMethodThread;
const osThreadAttr_t serialQMethod_attributes = {
  .name = "SerialQMethod",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
osMessageQueueId_t    serialMessageQ;
char receiveBuffer [maxSerialStringSz+1];
char transmitBuffer  [maxSerialStringSz+1];

void OnPrintError()
{
  ++amtPrintErr;  // at least keep the number for debugging, maybe later an led could be switched on or so...
}


void  SerialQMethod (void *p_arg)
{
	osStatus_t status;
	uint32_t dummyGet;
	do  {
		memset(&receiveBuffer, 0, sizeof(maxSerialStringSz));
		if ((status = osMessageQueueGet(serialMessageQ,(void *) &receiveBuffer, 0, osWaitForever)) == osOK )  {
			osMessageQueueGet(uartSendSemaphoreQ, &dummyGet, 0, osWaitForever);
			if (status == osOK) {
			sendUartString((char*)&receiveBuffer);
			}  else {
				errorHandler((uint32_t)status ,goOn," osSemaphoreAcquire "," SerialQMethod ");
			}
		} else {
			errorHandler((uint32_t)status ,goOn," osMessageQueueGet "," SerialQMethod ");
		}
	}  while (1);
}

void init_printf()
{

	serialOn = 0;
	amtErr = 0;
	amtPrintErr = 0;
	uint8_t err = osOK;


	serialMessageQ =  osMessageQueueNew(5,maxSerialStringSz * charWidth, NULL);
	if (serialMessageQ  == NULL)   {
		errorHandler((uint32_t)serialMessageQ ,stop," serialMessageQ ","init_printf");
	}


	serialQMethodThread =  osThreadNew(SerialQMethod, NULL, &serialQMethod_attributes);
	if (serialQMethodThread  == NULL)   {
		errorHandler((uint32_t)serialQMethodThread ,stop," serialQMethodThread ","initJt");
	}

	err=initUartHw();

	if (err == osOK)  {
		serialOn = 1;
	}
}

void private_printf( char *emsg, ...)
{

	//  todo did not work with dma using touchgfx and freertos ????????   but worked with testmethod in uarthw.c and under debugApp definition
	//  crashed into an ??? wwdg ??? loop in startup.s  why what how ?????
	//  todo further debug the version without dma, whose interrupts worked on productive system, what means a rather good omen

	osStatus_t status;
	va_list ap;
	va_start(ap, emsg);

	if (serialOn == 1) {

		vsnprintf((char *)&transmitBuffer, maxSerialStringSz-1,  emsg, ap);
		transmitBuffer[maxSerialStringSz-1] = 0;

		status = osMessageQueuePut(serialMessageQ,&transmitBuffer,0,0);
		if (status != osOK)  {
			errorHandler(status ,goOn," osMessageQueuePut ","info_printf");
		}
	}
	va_end(ap);
}


void info_printf( char *emsg, ...)
{
	va_list ap;
	va_start(ap, emsg);
#ifdef	useLogging
#ifndef debugPid
	private_printf(emsg, ap);
#endif
#endif
	va_end(ap);
}

void pid_printf( char *emsg, ...)
{
	va_list ap;
	va_start(ap, emsg);
#ifdef	useLogging
#ifdef debugPid
	private_printf(emsg, ap);
#endif
#endif
	va_end(ap);
}


// just for usage with short strings, otherwise sizes of buffers need to be increased
void  err_printf ( char *emsg, ...)
{
	va_list ap;
	va_start(ap, emsg);
	++ amtErr;
#ifdef	useLogging
	private_printf(emsg, ap);
#endif
	va_end(ap);
}

//void printStartMessage()
//{
//	info_printf("serial print ready for use\n");
//	info_printf("second test ok\n");
//	info_printf("third test ok\n");
////	info_printf("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
//}



//  todo forwardReceivedStringBuffer happens within interrupt may not use
//  the put to queue method of info_printf which waits for ever if no place in queue available
//  might be done with an own queue for forwarding if ever needed
//  currently (25. dez 2003) not used code

void forwardReceivedStringBuffer(char* strBuffer)
{
	if (strlen(strBuffer) > 4) {
		if (strncmp(strBuffer,"@ping:",6) == 0) {
//			info_printf("tempixController ping received: %s \n",strBuffer);
		}  else {
//			info_printf("tempixController received unknown String %s\n",strBuffer);
		}
	}
}


