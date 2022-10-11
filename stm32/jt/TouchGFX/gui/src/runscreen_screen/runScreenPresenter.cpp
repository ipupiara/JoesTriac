#include <gui/runscreen_screen/runScreenView.hpp>
#include <gui/runscreen_screen/runScreenPresenter.hpp>
#include <gui/Model/Model.hpp>
#include <mainJt.h>

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

//	CJoesPresenterEventT  presenterMessage;
//
//	while ( osMessageQueueGetCount ( presenterMessageQ))
//	{
//		if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
////			if (presenterMessage.messageType ==  runScreenSecondUpdate) {
////				view.updateScreen(presenterMessage.evData.secondRunData.secondsRemaining, presenterMessage.evData.secondRunData.amps);
////			}
////
////			if (presenterMessage.messageType ==  changeToRequesStopScreen) {
////				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
////			}
//		}
//	}
}



