/*
 * triacControl.h
 *
 *  Created on: Jan 29, 2024
 *      Author: Brigitte
 */

#ifndef APPLICATION_USER_TOOLS_HW_TRIACCONTROL_H_
#define APPLICATION_USER_TOOLS_HW_TRIACCONTROL_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include<stdint.h>
#include <math.h>

//	#define stmTriggerDelayMax  5000
#define stmTriggerDelayMax  4920
#define kStepUnitsFactor  5
#define triacDelayPsc  197   // (defaultTriacDelayPsc / kStepUnitsFactor)

#define zeroPass_Pin GPIO_PIN_12
#define zeroPass_Port GPIOA


#define extiZeroPassTriggerStartValue 1
#define isPinSet(portx, pinx)  (((portx->IDR) & pinx) != 0) ? 1:0
#define isExtiPinSet() isPinSet(zeroPass_Port,zeroPass_Pin)

extern int32_t triacTriggerDelay;

void initTriacControl();
void doJobOnZeroPassEvent(uint8_t ev);

extern int32_t triacTriggerDelay;
void setTriacTriggerDelay(int32_t cmsecs);
int32_t getTriacTriggerDelay();
void startTriacRun();
void stopTriacRun();


#ifdef  __cplusplus
}

#endif /* APPLICATION_USER_TOOLS_HW_TRIACCONTROL_H_ */

#endif
