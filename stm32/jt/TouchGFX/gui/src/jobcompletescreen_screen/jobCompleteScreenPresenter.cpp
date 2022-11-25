#include <gui/jobcompletescreen_screen/jobCompleteScreenView.hpp>
#include <gui/jobcompletescreen_screen/jobCompleteScreenPresenter.hpp>
#include <mainJt.h>

jobCompleteScreenPresenter::jobCompleteScreenPresenter(jobCompleteScreenView& v)
    : view(v)
{
	presenterActive = 0;
}

void jobCompleteScreenPresenter::activate()
{
	setPresenterQActive();
	presenterActive = 1;
}

void jobCompleteScreenPresenter::deactivate()
{
	setPresenterQInactive();
	presenterActive = 0;
}

void jobCompleteScreenPresenter::returnButtonPressed()
 {
	CMainJtEventT  msg;
	msg.evType = okPressed;
	sendEventToMainJtMessageQ(&msg,isNotFromIsr);
 }

void jobCompleteScreenPresenter::tick()
{
	CJoesPresenterEventT  presenterMessage;
	if (presenterActive) {
		while ( osMessageQueueGetCount ( presenterMessageQ))
		{
			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
				if (presenterMessage.messageType ==  jobOkScreenUpdate) {
					view.updateCompletionTime(presenterMessage.evData.alarmTimeSince);
				}
			}
		}
	}
}
