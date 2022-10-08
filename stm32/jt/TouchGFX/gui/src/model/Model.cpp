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

	while ( osMessageQueueGet ( modelMessageQ, &modelMessage, NULL, 0) == osOK)
	{
		if (modelMessage.messageType ==  changeToRequesStopScreen) {
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
		}
		if (modelMessage.messageType ==  changeToRunScreen) {
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorunScreenScreenNoTransition();
		}
		if (modelMessage.messageType ==  changeToMainScreen) {
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
		}
		if (modelMessage.messageType ==  changeToCalibratingScreen) {
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
		}
	}
}

void Model::storeWeldingTimeSec(uint16_t  tm)
{

}

