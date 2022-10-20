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

	if (presenterActive) {
		while ( osMessageQueueGetCount ( presenterMessageQ))
		{
			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
				if (presenterMessage.messageType ==  calibrateZeroPPScreenUpdate) {
					view.update(presenterMessage.evData.calibZeroPPScreenData.potiPos,
							presenterMessage.evData.calibZeroPPScreenData.devV);
				}
			}
		}
	}
}
