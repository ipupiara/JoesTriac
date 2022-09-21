/*
 * uosIIsis.h
 *
 *  Created on: Sep 21, 2022
 *      Author: diego
 */

#ifndef APPLICATION_USER_UOSIISIS_H_
#define APPLICATION_USER_UOSIISIS_H_


extern uint8_t*   perr;

#define WaitForever  0
#define OS_EVENT     uint32_t
#define OS_DEL_ALWAYS  0

#define MUTEX_CREATE() OSMutexCreate(0, &perr)
#define MUTEX_LOCK(m) OSMutexPend(m, WaitForever, &perr)
#define MUTEX_TYPE OS_EVENT
#define MUTEX_UNLOCK(m)  OSMutexPost(m)

#define SEM_CREATE() OSSemCreate(1)
#define SEM_POST(s) OSSemPost(s)
#define SEM_TYPE  OS_EVENT
#define SEM_WAIT(s)  OSSemPend(s,WaitForever, &perr)


OS_EVENT *OSMutexCreate(INT8U  prio, INT8U *perr);
INT8U OSMutexPost(OS_EVENT *pevent);
void OSMutexPend(OS_EVENT *pevent, INT16U    timeout, INT8U    *perr);

OS_EVENT *OSSemCreate(INT16U value);
INT8U OSSemPost(OS_EVENT *pevent);
void OSSemPend(OS_EVENT *pevent,INT16U timeout, INT8U* perr);










#endif /* APPLICATION_USER_UOSIISIS_H_ */

