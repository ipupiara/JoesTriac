/*
 * uosIIsis.h
 *
 *  Created on: Sep 21, 2022
 *      Author: diego
 */

#ifndef APPLICATION_USER_UOSIISIS_H_
#define APPLICATION_USER_UOSIISIS_H_

#include <stdint.h>

extern uint8_t*   perr;


#define WaitForever  0
#define OS_EVENT     uint32_t
#define OS_DEL_ALWAYS  0

OS_EVENT *OSMutexCreate(uint8_t  prio, uint8_t *perr);
uint8_t OSMutexPost(OS_EVENT *pevent);
void OSMutexPend(OS_EVENT *pevent, uint16_t    timeout, uint8_t    *perr);

OS_EVENT *OSSemCreate(uint32_t value);
uint8_t OSSemPost(OS_EVENT *pevent);
void OSSemPend(OS_EVENT *pevent,uint16_t timeout, uint8_t* perr);

#define SEM_TYPE  OS_EVENT
OS_EVENT  SEM_CREATE();
uint8_t SEM_POST(OS_EVENT pevent);
uint8_t SEM_WAIT(OS_EVENT pevent);

/*
#define MUTEX_CREATE() OSMutexCreate(0, &perr)
#define MUTEX_LOCK(m) OSMutexPend(m, WaitForever, &perr)
#define MUTEX_TYPE OS_EVENT
#define MUTEX_UNLOCK(m)  OSMutexPost(m)

#define SEM_CREATE() OSSemCreate((uint32_t) 1)
#define SEM_POST(s) OSSemPost(s)
#define SEM_TYPE  OS_EVENT
#define SEM_WAIT(s)  OSSemPend(s,WaitForever, &perr)
*/

#endif /* APPLICATION_USER_UOSIISIS_H_ */

