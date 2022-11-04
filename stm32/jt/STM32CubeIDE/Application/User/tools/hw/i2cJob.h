/*
 * i2cJob.h
 *
 *  Created on: Nov 1, 2022
 *      Author: Brigitte
 */

#ifndef APPLICATION_USER_TOOLS_HW_I2CJOB_H_
#define APPLICATION_USER_TOOLS_HW_I2CJOB_H_
#ifdef  __cplusplus
extern "C"
{
#endif
#include <defines.h>

typedef enum {
	save,
	restore,
	setAddr
} i2cJobTopic;

typedef void(*t_fvoid)(void);
//typedef void(*t_fPar)(void* pCmdLine);

typedef uint32_t (*t_fPar) (void* param);

//typedef struct {
//	uint16_t waitS1ms;
//	uint8_t xPos;
//	uint8_t yPos;
//	t_fvoid  stepMethod ;
//} i2cJobStepType ;


void i2cCentiSecTimer ();
void initI2cJob();

uint32_t requestAndWaitForCurrentI2cJob();
void addToCurrentI2cJob(uint32_t keyRand,pVarData pVarD, i2cJobTopic jobTopic);
void startCurrentI2cJob(uint32_t keyRand);

void addSetAddress(uint8_t adr);

#ifdef  __cplusplus
}
#endif


#endif /* APPLICATION_USER_TOOLS_HW_I2CJOB_H_ */
