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

void runScreenPresenter::stopButtonPressed()
{
	CMainJtEventT msg;
	msg.evType = stopButtonClicked;
	sendEventToMainJtMessageQ(&msg, isNotFromIsr);
}

void runScreenPresenter::tick()
{
	CJoesPresenterEventT  presenterMessage;

	if (presenterActive) {
		while ( osMessageQueueGetCount ( presenterMessageQ))
		{
			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
				if (presenterMessage.messageType ==  runScreenUpdate) {
					view.update(&presenterMessage);
				}
				if (presenterMessage.messageType ==  paintPidGraph) {
					view.updateGraph(&presenterMessage);
				}
			}
		}
	}
}



