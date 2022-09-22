/*
 * uosIIsis.c
 *
 *  Created on: Sep 21, 2022
 *      Author: diego
 */
#define debuggingUosII
#include <uosIIsis.h>



uint8_t*   perr;

/*
#define MUTEX_CREATE() osMutexNew(0)
#define MUTEX_LOCK(m) osMutexAcquire(m, osWaitForever)
#define MUTEX_TYPE osMutexId_t
#define MUTEX_UNLOCK(m) osMutexRelease(m)

#define MUTEX_UNLOCK(m) OSMutexDel(m,  (OS_DEL_ALWAYS, &perr)

*/

#ifdef  debuggingUosII
////////////////////////  quick hack dummy methods for wrapper development ////////////

//OS_EVENT *OSSemCreate(uint32_t value)
//{
//	OS_EVENT*  ev= (OS_EVENT*) value;
//	return ev;
//}



#endif

//////////////  interface  to touchgfx  ///////////////


