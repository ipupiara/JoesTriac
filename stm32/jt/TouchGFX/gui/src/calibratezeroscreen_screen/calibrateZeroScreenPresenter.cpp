#include <gui/calibratezeroscreen_screen/calibrateZeroScreenView.hpp>
#include <gui/calibratezeroscreen_screen/calibrateZeroScreenPresenter.hpp>

calibrateZeroScreenPresenter::calibrateZeroScreenPresenter(calibrateZeroScreenView& v)
    : view(v)
{
	presenterActive = 0;
}

void calibrateZeroScreenPresenter::activate()
{
	setPresenterQActive();
	presenterActive = 1;
}

void calibrateZeroScreenPresenter::deactivate()
{
	setPresenterQInactive();
	presenterActive = 0;
}

void calibrateZeroScreenPresenter::tick()
{
	CJoesPresenterEventT  presenterMessage;

	if (this->    presenterActive) {
		while ( osMessageQueueGetCount ( presenterMessageQ))
		{	//  todo check performance if while is done without any delay (blocking behaviour)
			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
			if (presenterMessage.messageType ==  runScreenSecondUpdate) {
	//			view.updateScreen(presenterMessage.evData.secondRunData.secondsRemaining, presenterMessage.evData.secondRunData.amps);
	//			}
	//
	//			if (presenterMessage.messageType ==  changeToRequesStopScreen) {
	//				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
				}
			}
		}
	}
}
