/*
 * defines.c
 *
 *  Created on: Sep 24, 2022
 *      Author: diego
 */

#include <defines.h>


void errorHandler(uint32_t  code, errorSeverity severity, char* errorString, char* method )
{
	char buffer [100];
	snprintf(buffer, 99, "%s, %s, %10lX, %X", method, errorString,code,severity);
	//  todo log to peristency

	if (severity == stop) {  do {} while (1);}
}



