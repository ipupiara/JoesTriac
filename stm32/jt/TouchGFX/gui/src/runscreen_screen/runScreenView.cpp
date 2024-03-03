#include <gui/runscreen_screen/runScreenView.hpp>
#include <stdlib.h>
#include <math.h>
//#include <touchgfx/containers/buttons/Buttons.hpp>
#include <mainJt.h>


runScreenView::runScreenView()
{

}

void runScreenView::setupScreen()
{

    runScreenViewBase::setupScreen();

    weldingTimeSec = presenter->getWeldingTimeSec();
    weldingAmps    = presenter->getWeldingAmps();

	ampGauge.setValue(0);
	ampGauge.setGoalValue(weldingAmps);
	ampGauge.invalidate();

	Unicode::snprintfFloat(setAmpereTextBuffer, 7, "%06.2f", weldingAmps);
	setAmpereText.setWildcard(setAmpereTextBuffer);
	setAmpereText.invalidate();

	Unicode::snprintfFloat(currentAmpereTextBuffer, 7, "%06.2f",0.0);
	currentAmpereText.setWildcard(currentAmpereTextBuffer);
	currentAmpereText.invalidate();

	uint8_t  minVal = uint8_t( weldingTimeSec / 60);
	uint8_t  secVal = (uint8_t) ( weldingTimeSec % 60);
	Unicode::snprintf(currentTimeTextBuffer, 6, "%02d:%02d", minVal, secVal);
	currentTimeText.setWildcard(currentTimeTextBuffer);
	currentTimeText.invalidate();

	Unicode::snprintf(timeValueTextBuffer, 6, "%02d:%02d", minVal, secVal);
	timeValueText.setWildcard(timeValueTextBuffer);
	timeValueText.invalidate();

	 boxProgress1.setValue(0);
	 boxProgress1.invalidate();

	 astrolabiumContainer1.setVisible(false);
	 astrolabiumContainer1.invalidate();

	 pidDataGraphContainer1.setVisible(false);
	 pidDataGraphContainer1.invalidate();

	 raiseDoRun();
}

void runScreenView::tearDownScreen()
{
    runScreenViewBase::tearDownScreen();
}

void runScreenView::update(pJoesPresenterEventT  pMsg )
{

	// todo bug: amps does not show / calculate delay decimal places in setamperetext

	Unicode::snprintfFloat(currentAmpereTextBuffer, 7, "%6.2f",pMsg->evData.runScreenData.amps);
	currentAmpereText.setWildcard(currentAmpereTextBuffer);
	currentAmpereText.invalidate();

//	 int ampValue = ((int)( amps  ));

		ampGauge.setValue(pMsg->evData.runScreenData.amps);
		ampGauge.invalidate();

	 float wTime = weldingTimeSec;
	 float rTime = pMsg->evData.runScreenData.secondsRemaining;
	 float  remain = (wTime - rTime)/ wTime;
	 uint32_t boxPro = 100 * remain;
	 boxProgress1.setValue(boxPro);
	 boxProgress1.invalidate();
	 astrolabiumContainer1.update(pMsg);


	 if (aborting == 1)  {
		 uint8_t  minVal = uint8_t( pMsg->evData.runScreenData.secondsRemaining  / 60);
		 uint8_t  secVal = (uint8_t) ( pMsg->evData.runScreenData.secondsRemaining % 60);
		 Unicode::snprintf(currentTimeTextBuffer, 6, "%02d:%02d", minVal, secVal);
		 currentTimeText.setWildcard(currentTimeTextBuffer);
		 currentTimeText.invalidate();

		 secVal = (uint8_t) (pMsg->evData.runScreenData.secondsBeforeReturn);
		 Unicode::snprintf(secondsb4ReturnTextBuffer, 6, "%02d",  secVal);
		 secondsb4ReturnText.setWildcard(secondsb4ReturnTextBuffer);
		 secondsb4ReturnText.invalidate();
	 }
}

void runScreenView::graphButtonClicked()
{
	if (! pidDataGraphContainer1.isVisible()) {
		pidDataGraphContainer1.setVisible(true);
	}
	pidDataGraphContainer1.invalidate();
}


void runScreenView::updatePidGraph(pJoesPresenterEventT  pMsg )
{
	pidDataGraphContainer1.updateGraph(pMsg);
}


void runScreenView::stopButtonPressed()
{
	presenter->stopButtonPressed();
}

void runScreenView::astroButtonPressed()
{
	if (astrolabiumContainer1.isVisible()) {
		astrolabiumContainer1.setVisible(false);
	} else {
		astrolabiumContainer1.setVisible(true);
	}
	astrolabiumContainer1.invalidate();
}

void runScreenView::continueButtonClicked()
{
	presenter->continueButtonPressed();
}

void runScreenView::abortButtonPressed()
{
	presenter->abortButtonPressed();
}


void runScreenView::raiseRequestStop()
{
	aborting = 1;
	setActive(&abortButton, true);
	setActive(&continueButton, true);
	setActive(&secondsb4ReturnText, true);
	setActive(&seconb4Title, true);

	setActive(&startButton, false);
	setActive(&stopButton, false);
	setActive(&astrolabiumContainer1, false);
	setActive(&pidDataGraphContainer1, false);
	setActive(&graphButton, false);
	setActive(&astroButton, false);
}

void runScreenView::raiseDoRun()
{
	aborting = 0;
	setActive(&startButton, true);
	setActive(&stopButton, true);
	setActive(&graphButton, true);
	setActive(&astroButton, true);

	setActive(&abortButton, false);
	setActive(&secondsb4ReturnText, false);
	setActive(&seconb4Title, false);
	setActive(&continueButton, false);
	setActive(&astrolabiumContainer1, false);
	setActive(&pidDataGraphContainer1, false);
}

void runScreenView::setActive(Drawable* btn,bool act )
{
	btn->setVisible(act);
	btn->invalidate();
}

