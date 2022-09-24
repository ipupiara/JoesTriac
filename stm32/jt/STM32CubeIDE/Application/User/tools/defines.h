/*
 * defines.h
 *
 *  Created on: Sep 24, 2022
 *      Author: diego
 */

#include <stdio.h>


#ifndef APPLICATION_USER_TOOLS_DEFINES_H_
#define APPLICATION_USER_TOOLS_DEFINES_H_

#define errorStringLen  20

typedef enum{
	goOn,
	stop
} errorSeverity;


void errorHandler(uint32_t  code, errorSeverity severity, char* errorString, char* method );




#endif /* APPLICATION_USER_TOOLS_DEFINES_H_ */
