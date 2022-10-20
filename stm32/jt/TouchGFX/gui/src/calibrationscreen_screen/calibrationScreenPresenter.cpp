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

	if (presenterActive) {
		while ( osMessageQueueGetCount ( presenterMessageQ))
		{
			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
				if (presenterMessage.messageType ==  calibrationScreenUpdate) {
					view.update(presenterMessage.evData.calibrationScreenData.amps,
							presenterMessage.evData.calibrationScreenData.secsRemain);
				}
				if (presenterMessage.messageType ==  calibPotiPos) {
					view.updateState(presenterMessage.evData.calibState);
				}
			}
		}
	}
}
