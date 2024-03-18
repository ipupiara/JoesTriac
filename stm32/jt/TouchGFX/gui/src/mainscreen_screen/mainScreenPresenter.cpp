#include <gui/mainscreen_screen/mainScreenView.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>
#include <mainJt.h>

mainScreenPresenter::mainScreenPresenter(mainScreenView& v)
    : view(v)
{

}

void mainScreenPresenter::redrawPressed()
{
	CMainJtEventT evnt;
	evnt.evType = redrawButtonPressed;
	sendEventToMainJtMessageQ( &evnt, isNotFromIsr);
}
void mainScreenPresenter::activate()
{
	setPresenterQActive();
	presenterActive = 1;
}

void mainScreenPresenter::deactivate()
{
	setPresenterQInactive();
	presenterActive = 0;
}

uint16_t mainScreenPresenter::getWeldingTimeSec()
{
	return model->getWeldingTimeSec();
}

float mainScreenPresenter::getWeldingAmps()
{
	return model->getWeldingAmps();
}

void mainScreenPresenter::configButtonPressed()
{
	CMainJtEventT evt;
	evt.evType = configPressed;
	sendEventToMainJtMessageQ(&evt,isNotFromIsr);
}

void mainScreenPresenter::startButtonPressed()
{
	CMainJtEventT evt;
	evt.evType = runButtonClicked;
	sendEventToMainJtMessageQ(&evt,isNotFromIsr);
}

void mainScreenPresenter::tick()
{
	CJoesPresenterEventT  presenterMessage;

	if (presenterActive) {
		while ( osMessageQueueGetCount ( presenterMessageQ))
		{
			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
				if (presenterMessage.messageType ==  pidGraphFromData) {  // never should happen here
					view.showPidGraphFromData(&presenterMessage);
				}
			}
		}
	}

}

