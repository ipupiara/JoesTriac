/*
 * uosIIsis.c
 *
 *  Created on: Sep 21, 2022
 *      Author: diego
 */

#include <uosIIsis.h>

uint8_t*   perr;

/*
#define MUTEX_CREATE() osMutexNew(0)
#define MUTEX_LOCK(m) osMutexAcquire(m, osWaitForever)
#define MUTEX_TYPE osMutexId_t
#define MUTEX_UNLOCK(m) osMutexRelease(m)

#define MUTEX_UNLOCK(m) OSMutexDel(m,  (OS_DEL_ALWAYS, &perr)

*/

//////////////  interface  to touchgfx  //////////////////

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

////////////////////////  quick hack dummy methods for wrapper development ////////////

void OSMutexPend(OS_EVENT *pevent, uint16_t    timeout, uint8_t    *perr)
{

}

OS_EVENT *OSMutexCreate(uint8_t  prio, uint8_t *perr)
{
	OS_EVENT* res = 0;
	return res;
}


uint8_t OSMutexPost(OS_EVENT *pevent)
{
	return 1;
}

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

