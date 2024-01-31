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


extern uint32_t amtMissedZpTotal;
extern uint32_t   maxMissedZp;
extern uint32_t amtExtiEvTotal;
extern uint32_t  amountIllegalExti;
extern uint32_t amtWrongSync;
extern uint32_t amtExtiSequenceError;


extern void startExtiCheck();
void initExtiCheck();

void startExtiChecking();



#ifdef  __cplusplus
}
#endif

#endif /* APPLICATION_USER_TOOLS_HW_EXTICHECK_H_ */
