/*
 * extiCheck.h
 *
 *  Created on: Jan 22, 2024
 *      Author: Brigitte
 */

#ifndef APPLICATION_USER_TOOLS_HW_EXTICHECK_H_
#define APPLICATION_USER_TOOLS_HW_EXTICHECK_H_

#ifdef  __cplusplus
extern "C"
{
#endif



extern uint8_t extiCheckCnt ;
extern uint32_t amtExtiMissedTotal;
extern uint32_t   maxMissedExti;
extern uint32_t  amtIllegalExti;
extern uint32_t amtSyncMissed;
extern uint32_t syncMissedPeriodStartTick;
extern void stopExtiCheck();
extern void startExtiCheck();

void startHandleMissed();
uint8_t handleMissed();


#ifdef  __cplusplus
}
#endif

#endif /* APPLICATION_USER_TOOLS_HW_EXTICHECK_H_ */