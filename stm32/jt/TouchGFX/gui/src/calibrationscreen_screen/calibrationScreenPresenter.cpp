#include <gui/calibrationscreen_screen/calibrationScreenView.hpp>
#include <gui/calibrationscreen_screen/calibrationScreenPresenter.hpp>
#include <gui/Model/Model.hpp>
#include <mainJt.h>

calibrationScreenPresenter::calibrationScreenPresenter(calibrationScreenView& v)
    : view(v)
{
	presenterActive = 0;
}

void calibrationScreenPresenter::activate()
{
	setPresenterQActive();
	presenterActive = 1;
}

void calibrationScreenPresenter::deactivate()
{
	setPresenterQInactive();
	presenterActive = 0;
}

void calibrationScreenPresenter::tick()
{
	CJoesPresenterEventT  presenterMessage;

//	if (this->    presenterActive) {
//		while ( osMessageQueueGetCount ( presenterMessageQ))
//		{	//  todo check performance if while is done without any delay (blocking behaviour)
//			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
//			if (presenterMessage.messageType ==  runScreenSecondUpdate) {
//	//			view.updateScreen(presenterMessage.evData.secondRunData.secondsRemaining, presenterMessage.evData.secondRunData.amps);
//	//			}
//	//
//	//			if (presenterMessage.messageType ==  changeToRequesStopScreen) {
//	//				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
//				}
//			}
//		}
//	}
}
