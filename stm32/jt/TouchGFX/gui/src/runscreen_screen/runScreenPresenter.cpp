#include <gui/runscreen_screen/runScreenView.hpp>
#include <gui/runscreen_screen/runScreenPresenter.hpp>
#include <gui/Model/Model.hpp>

runScreenPresenter::runScreenPresenter(runScreenView& v)
    : view(v)
{

}

void runScreenPresenter::activate()
{

}

void runScreenPresenter::deactivate()
{

}

void runScreenPresenter::tick()
{
	Model::CJoesModelEventT  modelMessage;

	while ( osMessageQueueGet ( model->modelMessageQ, &modelMessage, NULL, 0) == osOK)
	{
		if (modelMessage.messageType == Model::secondUpdate) {

		}
		if (modelMessage.messageType == Model::changeToMainScreen) {
//			view.application().gotomainScreenScreenNoTransition();
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotomainScreenScreenNoTransition();
		}
		if (modelMessage.messageType == Model::changeToRequesStopScreen) {
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
		}
	}
}
