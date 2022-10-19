#include <gui/runscreen_screen/runScreenView.hpp>
#include <gui/runscreen_screen/runScreenPresenter.hpp>
#include <gui/Model/Model.hpp>
#include <mainJt.h>

runScreenPresenter::runScreenPresenter(runScreenView& v)
    : view(v)
{
	presenterActive = 0;
}

void runScreenPresenter::activate()
{
	setPresenterQActive();
	presenterActive = 1;
}

void runScreenPresenter::deactivate()
{
	setPresenterQInactive();
	presenterActive = 0;
}

void runScreenPresenter::tick()
{

	CJoesPresenterEventT  presenterMessage;

	if (presenterActive) {
		while ( osMessageQueueGetCount ( presenterMessageQ))
		{	//  todo check performance if while is done without any delay (blocking behaviour)
			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
				if (presenterMessage.messageType ==  runScreenSecondUpdate) {
	//				view.updateScreen(presenterMessage.evData.secondRunData.secondsRemaining, presenterMessage.evData.secondRunData.amps);
	//			}
	//
	//			if (presenterMessage.messageType ==  changeToRequesStopScreen) {
	//				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
				}
			}
		}
	}
}



