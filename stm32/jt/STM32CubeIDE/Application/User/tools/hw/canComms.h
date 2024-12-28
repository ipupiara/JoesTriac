#ifndef canComms_h
#define canComms_h
#include <stdint.h>
//#include <uosii-includes.h>
#include <canRelated.h>


#ifdef  __cplusplus
extern "C"
{
#endif

typedef enum {
	triacHost,
	ammeterHost,
	amtCanHosts
} canHosts;

typedef enum {
	noCanTest,
	singleCanTest,
	continuousCanTest,
	amtCanTestTypes
} canTestTypes;




void initCanComms();
void sendCanTestMessage(canHosts cHost ,canTestTypes canTestType);
void setCurrentCanSelection(canHosts cHost ,canTestTypes canTestType);

uint8_t syncSendTempixSimpleCommand( TempixSimpleCommand* scmd);

#ifdef  __cplusplus
}
#endif

#endif

