#include <gui/requeststopscreen_screen/requestStopScreenView.hpp>
#include <gui/requeststopscreen_screen/requestStopScreenPresenter.hpp>


requestStopScreenPresenter::requestStopScreenPresenter(requestStopScreenView& v)
    : view(v)
{
	presenterActive = 0;
}

void requestStopScreenPresenter::activate()
{
	setPresenterQActive();
	presenterActive = 1;
}

void requestStopScreenPresenter::deactivate()
{
	setPresenterQInactive();
	presenterActive = 0;
}

void requestStopScreenPresenter::tick()
{
	CJoesPresenterEventT  presenterMessage;

	if (presenterActive) {
		while ( osMessageQueueGetCount ( presenterMessageQ))
		{
			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
				if (presenterMessage.messageType ==  runScreenUpdate) {
					view.update(presenterMessage.evData.runScreenData.amps, presenterMessage.evData.runScreenData.secondsRemaining,
							presenterMessage.evData.runScreenData.potiPos);
				}
			}
		}
	}
}
