#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <stdint.h>
#include <defines.h>
#include <mainJt.h>


Model::Model() : modelListener(0)
{

}

int16_t Model::stringToSecValue()
{
	return 0;
}


void Model::tick()
{
	modelListener->tick();

	CJoesModelEventT  modelMessage;
	uint8_t  prio = 0;

	while ( osMessageQueueGet ( modelMessageQ, &modelMessage, &prio, 0) == osOK)
	{
		if (modelMessage.messageType ==  changeToRequesStopScreen) {
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
		}
		if (modelMessage.messageType ==  changeToRunScreen) {
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorunScreenScreenNoTransition();
		}
		if (modelMessage.messageType ==  changeToMainScreen) {
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotomainScreenScreenNoTransition();
		}
		if (modelMessage.messageType ==  changeToCalibratingScreen) {
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoconfigScreenScreenNoTransition();
		}
	}
}

void Model::storeWeldingTimeSec(uint16_t  tm)
{

}

