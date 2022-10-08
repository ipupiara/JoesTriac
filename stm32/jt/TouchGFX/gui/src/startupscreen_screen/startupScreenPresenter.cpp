#include <gui/startupscreen_screen/startupScreenView.hpp>
#include <gui/startupscreen_screen/startupScreenPresenter.hpp>
#include <mainJt.h>

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
	CJoesPresenterEventT  presenterMessage;

	while ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)
	{
		if (presenterMessage.messageType ==  changeToMainScreen) {
			static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotomainScreenScreenNoTransition();
		}
	}
}
