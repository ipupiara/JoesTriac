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

	uint8_t  minVal = uint8_t( weldingTimeSec / 60);
	uint8_t  secVal = (uint8_t) ( weldingTimeSec % 60);
	Unicode::snprintf(currentTimeTextBuffer, 6, "%02d:%02d", minVal, secVal);
	currentTimeText.setWildcard(currentTimeTextBuffer);
	currentTimeText.invalidate();

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

	 int ampValue = ((int)( amps  ));
	 ampGauge.setValue(ampValue);
	 ampGauge.invalidate();

	 uint8_t  minVal = uint8_t( secRemain / 60);
	 uint8_t  secVal = (uint8_t) ( secRemain % 60);
	 Unicode::snprintf(currentTimeTextBuffer, 6, "%02d:%02d", minVal, secVal);
	 currentTimeText.setWildcard(currentTimeTextBuffer);
	 currentTimeText.invalidate();

	 int boxPro = 100 * ((weldingTimeSec - secRemain)/ weldingTimeSec);
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
