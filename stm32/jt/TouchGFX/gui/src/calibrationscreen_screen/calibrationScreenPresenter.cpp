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

void calibrationScreenPresenter::sendTriacDelayCalibCorrection(int32_t val)
{
	CMainJtEventT msg;
	msg.evType = calibTriacDelayDelta;
	msg.mainUnion.calibTriDelayCorrection = val;
	sendEventToMainJtMessageQ(&msg, isNotFromIsr);
}

void calibrationScreenPresenter::abortButtonPressed()
{
	CMainJtEventT msg;
	msg.evType = calibAbortClick;
	sendEventToMainJtMessageQ(&msg, isNotFromIsr);
}
void calibrationScreenPresenter::continueButtonPressed()
{
	CMainJtEventT msg;
	msg.evType = calibContinueClick;
	sendEventToMainJtMessageQ(&msg, isNotFromIsr);
}

void calibrationScreenPresenter::skipButtonPressed()
{
	CMainJtEventT msg;
	msg.evType = calibSkipClick;
	sendEventToMainJtMessageQ(&msg, isNotFromIsr);
}

void calibrationScreenPresenter::calibrationScreenReady()
{
	CMainJtEventT msg;
	msg.evType = calibScreenReady;
	sendEventToMainJtMessageQ(&msg, isNotFromIsr);
}

void calibrationScreenPresenter::tick()
{
	CJoesPresenterEventT  presenterMessage;

	if (presenterActive) {
		while ( osMessageQueueGetCount ( presenterMessageQ))
		{
			if ( osMessageQueueGet ( presenterMessageQ, &presenterMessage, NULL, 0) == osOK)  {
				if (presenterMessage.messageType ==  calibrationScreenUpdate) {
					view.updateAmpsValues(presenterMessage.evData.calibrationScreenData.adcVolts,
							presenterMessage.evData.calibrationScreenData.adcValue,
							presenterMessage.evData.calibTriacDelay);
				}
				if (presenterMessage.messageType ==  calibTriacDelay) {
					view.updateTriacDelay(presenterMessage.evData.calibTriacDelay);
				}
				if (presenterMessage.messageType ==  calibDesiredAmps) {
					view.updateDesiredAmps(presenterMessage.evData.desiredAmps);
				}
			}
		}
	}
}


