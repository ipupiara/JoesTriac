#include <gui/requeststopscreen_screen/requestStopScreenView.hpp>
#include <mainJt.h>

requestStopScreenView::requestStopScreenView()
{

}



void requestStopScreenView::setupScreen()
{
    requestStopScreenViewBase::setupScreen();
    weldingTimeSec = presenter->getWeldingTimeSec();
    weldingAmps    = presenter->getWeldingAmps();

	ampGauge.setValue(0);
	ampGauge.setGoalValue(weldingAmps);
	ampGauge.invalidate();

	uint8_t  minVal = uint8_t( weldingTimeSec / 60);
	uint8_t  secVal = (uint8_t) ( weldingTimeSec % 60);
	Unicode::snprintf(timeValueTextBuffer, 6, "%02d:%02d", minVal, secVal);
	timeValueText.setWildcard(timeValueTextBuffer);
	timeValueText.invalidate();

	Unicode::snprintfFloat(setAmpereTextBuffer, 7, "%6.2f", weldingAmps);
	setAmpereText.setWildcard(setAmpereTextBuffer);
	setAmpereText.invalidate();

}

void requestStopScreenView::tearDownScreen()
{
    requestStopScreenViewBase::tearDownScreen();
}

void requestStopScreenView::update(float amps,uint32_t secRemain, uint16_t secondsBeforeReturn )
{
	Unicode::snprintfFloat(currentAmpereTextBuffer, 7, "%6.2f",amps);
	currentAmpereText.setWildcard(currentAmpereTextBuffer);
	currentAmpereText.invalidate();

	ampGauge.setValue(amps );
	ampGauge.setGoalValue(weldingAmps);
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

	 secVal = (uint8_t) (secondsBeforeReturn);
	 Unicode::snprintf(secondsb4ReturnTextBuffer, 6, "%02d",  secVal);
	 secondsb4ReturnText.setWildcard(secondsb4ReturnTextBuffer);
	 secondsb4ReturnText.invalidate();

}

void requestStopScreenView::continueButtonPressed()
{
	presenter->continueButtonPressed();
}

void requestStopScreenView::abortButtonPressed()
{
	presenter->abortButtonPressed();
}
