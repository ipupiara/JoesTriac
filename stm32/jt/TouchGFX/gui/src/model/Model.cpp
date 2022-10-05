#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <stdint.h>
#include <defines.h>

Model::Model() : modelListener(0)
{
	char  modelm []= " modelMessageQ ";
	char  meth   []= "Model::Model";
	modelMessageQ =  osMessageQueueNew(1,sizeof(CJoesModelEventT)*4, NULL);
	if (modelMessageQ  == NULL)   {
		errorHandler((uint32_t)modelMessageQ, stop,(char*) &modelm, (char*)&meth);
	}
}

int16_t Model::stringToSecValue()
{
	return 0;
}


void Model::tick()
{
	modelListener->tick();
}

void Model::storeWeldingTimeSec(uint16_t  tm)
{

}

osStatus_t  Model::sendModelMessage(pJoesModelEventT  pMsg)
{
	osStatus_t  status = osError;
	char  desc [] = " status ";
	char  meth [] = "Model::sendModelMessage";
	status = osMessageQueuePut (modelMessageQ, (void *) pMsg, 0, 10);
	if (status != osOK) {
		errorHandler(status,goOn,desc,meth);
	}
	return status;
}
