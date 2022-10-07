#include <gui/startupscreen_screen/startupScreenView.hpp>
#include <gui/startupscreen_screen/startupScreenPresenter.hpp>

startupScreenPresenter::startupScreenPresenter(startupScreenView& v)
    : view(v)
{

}

void startupScreenPresenter::activate()
{

}

void startupScreenPresenter::deactivate()
{

}


void startupScreenPresenter::tick()
{
	Model::CJoesModelEventT  modelMessage;

	while ( osMessageQueueGet ( Model::modelMessageQ, &modelMessage, NULL, 0) == osOK)
	{
		if (modelMessage.messageType ==  Model::changeToMainScreen) {
//			Application::getInstance()->switchScreen();
		}
	}
}
