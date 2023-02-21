#ifndef canComms_h
#define canComms_h
#include <stdint.h>
//#include <uosii-includes.h>
#include <canRelated.h>

void initCanComms();
void sendCanTestMessage();;


uint8_t syncSendTempixSimpleCommand( TempixSimpleCommand* scmd);

#endif
