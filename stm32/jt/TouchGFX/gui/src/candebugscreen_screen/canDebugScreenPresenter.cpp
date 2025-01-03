#include <gui/candebugscreen_screen/canDebugScreenView.hpp>
#include <gui/candebugscreen_screen/canDebugScreenPresenter.hpp>

canDebugScreenPresenter::canDebugScreenPresenter(canDebugScreenView& v)
    : view(v)
{

}

void canDebugScreenPresenter::activate()
{
	setPresenterQActive();
	presenterActive = 1;
}

void canDebugScreenPresenter::deactivate()
{
	setPresenterQInactive();
	presenterActive = 0;
}


void canDebugScreenPresenter::tick()
{
	CJoesPresenterEventT  presenterMessage;

	if (presenterActive) {
		while ( osMessageQueueGetCount ( presenterMessageQ))
		{
			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
				if (presenterMessage.messageType ==  canDebugData) {  // never should happen here
//					view.showPidGraphFromData(&presenterMessage);
				}
			}
		}
	}

}

