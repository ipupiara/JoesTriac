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



#define SEM_TYPE  OS_EVENT
#define MUTEX_TYPE OS_EVENT

/////////////   uosII debugging during development

#define debuggingUosII

#ifdef  debuggingUosII
OS_EVENT *OSSemCreate(uint32_t value)
{
	OS_EVENT*  ev= (OS_EVENT*) value;
	return ev;
}

void OSSemPend(OS_EVENT *pevent,uint16_t timeout, uint8_t* perr)
{

}

uint8_t OSSemPost(OS_EVENT *pevent)
{
	return 1;
}

#endif

////  user

OS_EVENT  SEM_CREATE()
{
	OS_EVENT*  ev=  OSSemCreate(1);
	OS_EVENT   evnt = *ev;
	return evnt;
}


uint8_t SEM_POST(OS_EVENT pevent)
{
	OSSemPost(&pevent);
	return 1;
}

uint8_t SEM_WAIT(OS_EVENT pevent)
{
	OSSemPend(&pevent,WaitForever, perr);
	return 1;
}



OS_EVENT  MUTEX_CREATE()
{
	OS_EVENT*  ev=  OSSemCreate(1);
	OS_EVENT   evnt = *ev;
	return evnt;
}

uint8_t MUTEX_UNLOCK(OS_EVENT pevent)
{
	OSSemPost(&pevent);
	return 1;
}

uint8_t MUTEX_LOCK(OS_EVENT pevent)
{
	OSSemPend(&pevent,WaitForever, perr);
	return 1;
}









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

