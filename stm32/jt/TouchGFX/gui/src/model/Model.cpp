#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <stdint.h>
#include <defines.h>

 osMessageQueueId_t Model::modelMessageQ =  osMessageQueueNew(5,sizeof(CJoesModelEventT)*4, NULL);

Model::Model() : modelListener(0)
{
	char  modelm []= " modelMessageQ ";
	char  meth   []= "Model::Model";

	if (modelMessageQ  == NULL)   {
		errorHandler((uint32_t)NULL, stop,(char*) &modelm, (char*)&meth);
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
