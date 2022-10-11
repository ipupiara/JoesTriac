#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <stdint.h>
#include <defines.h>
#include <mainJt.h>


Model::Model() : modelListener(0)
{

}


void Model::tick()
{
	modelListener->tick();

	CJoesModelEventT  modelMessage;
	uint8_t  prio = 0;
	osStatus_t  status;

	while (osMessageQueueGetCount(modelMessageQ) )
	{
		if ((status =osMessageQueueGet ( modelMessageQ, &modelMessage, &prio, 0)) == osOK) {
			if (modelMessage.messageType ==  changeToRequesStopScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToRunScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorunScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToMainScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotomainScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToConfigScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoconfigScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  restoreModelData) {
				restoreData();
			}
		} else {
//			char* mm = "modelMessageQ";
//			char* met = "Model::tick";
//			errorHandler(status, goOn,mm,met);
		}

	}
}

uint16_t Model::getWeldingTimeSec()
{
	return (uint16_t) getDefinesWeldingTime();
}

void Model::storeWeldingTimeSec(uint16_t  tm)
{

}

void Model::restoreData()
{
	weldingTime = (uint16_t) getDefinesWeldingTime();
	weldingAmps=  getDefinesWeldingAmps();

	alarmNeeded =  getDefinesAlarmNeeded();

	alarmTime =  (uint16_t) getDefinesAlarmTime();
}

