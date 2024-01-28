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




extern uint32_t amtMissedTotal;
extern uint32_t   maxMissedExti;
extern uint32_t  amountIllegalExti;
extern uint32_t amtSyncMissed;


extern void startExtiCheck();
void initExtiCheckTimer();

void startExtiChecking();



#ifdef  __cplusplus
}
#endif

#endif /* APPLICATION_USER_TOOLS_HW_EXTICHECK_H_ */
