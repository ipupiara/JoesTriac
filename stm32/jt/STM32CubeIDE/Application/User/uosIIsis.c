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


void OSMutexPend(OS_EVENT *pevent, INT16U    timeout, INT8U    *perr)
{

}

OS_EVENT *OSMutexCreate(INT8U  prio, INT8U *perr)
{
	OS_EVENT* res = 0;
	return res;
}


INT8U OSMutexPost(OS_EVENT *pevent)
{
	return 1;
}

OS_EVENT *OSSemCreate(INT16U value)
{
	OS_EVENT*  ev=0;
	return ev;
}

void OSSemPend(OS_EVENT *pevent,INT16U timeout, INT8U* perr)
{

}

void OSSemPend(OS_EVENT *pevent,INT16U timeout, INT8U* perr)
{

}
