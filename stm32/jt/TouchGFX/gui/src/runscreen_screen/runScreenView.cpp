#include <gui/runscreen_screen/runScreenView.hpp>

runScreenView::runScreenView()
{
}

void runScreenView::setupScreen()
{
    runScreenViewBase::setupScreen();

    hideAstro();

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
}

void runScreenView::tearDownScreen()
{
    runScreenViewBase::tearDownScreen();
}

void runScreenView::update(float amps,uint32_t secRemain, int32_t adcVal, int32_t triacDelay, float adcVolts  )
{

	// todo bug: amps does not show / calculate delay decimal places in setamperetext

	Unicode::snprintfFloat(currentAmpereTextBuffer, 7, "%6.2f",amps);
	currentAmpereText.setWildcard(currentAmpereTextBuffer);
	currentAmpereText.invalidate();

//	 int ampValue = ((int)( amps  ));

		ampGauge.setValue(amps);
		ampGauge.invalidate();

	 uint8_t  minVal = uint8_t( secRemain / 60);
	 uint8_t  secVal = (uint8_t) ( secRemain % 60);
	 Unicode::snprintf(currentTimeTextBuffer, 6, "%02d:%02d", minVal, secVal);
	 currentTimeText.setWildcard(currentTimeTextBuffer);
	 currentTimeText.invalidate();

	 float wTime = weldingTimeSec;
	 float rTime = secRemain;
	 float  remain = (wTime - rTime)/ wTime;
	 uint32_t boxPro = 100 * remain;
	 boxProgress1.setValue(boxPro);
	 boxProgress1.invalidate();

	 Unicode::snprintfFloat(adcVoltageTextBuffer, 6, "%01.3f", adcVolts);
	adcVoltageText.setWildcard(adcVoltageTextBuffer);
	adcVoltageText.invalidate();

	Unicode::snprintf(adcValueTextBuffer, 5, "%4d", adcVal);
	adcValueText.setWildcard(adcValueTextBuffer);
	adcValueText.invalidate();

	Unicode::snprintf(delayTextBuffer,5, "%4d", triacDelay);
	delayText.setWildcard(delayTextBuffer);
	delayText.invalidate();
}

void runScreenView::stopButtonPressed()
{
	presenter->stopButtonPressed();
}

void runScreenView::setAstroVisible(bool vis)
{
    astroBorder.setVisible(vis);
    textArea3.setVisible(vis);
    textArea4.setVisible(vis);
    textArea5.setVisible(vis);
    adcVoltageText.setVisible(vis);
    adcValueText.setVisible(vis);
    delayText.setVisible(vis);

}

void runScreenView::toggleAstro()
{
	if (astroVisible == true) {
		hideAstro();
	} else {
		showAstro();
	}
}

void runScreenView::hideAstro()
{
	setAstroVisible(false);
	astroVisible = false;
}

void runScreenView::showAstro()
{
	setAstroVisible(true);
	astroVisible = true;
}

void runScreenView::astroButtonPressed()
{
	toggleAstro();
}
