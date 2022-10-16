#include <gui/mainscreen_screen/mainScreenView.hpp>
#include <mainJt.h>

mainScreenView::mainScreenView()
{

}

void mainScreenView::setupScreen()
{
    mainScreenViewBase::setupScreen();
    uint16_t timeValueSec = presenter->getWeldingTimeSec();
	uint8_t  minVal = uint8_t( timeValueSec / 60);
	uint8_t  secVal = (uint8_t) ( timeValueSec % 60);
	Unicode::snprintf(timeValueTextBuffer, 6, "%02d:%02d", minVal, secVal);
	timeValueText.setWildcard(timeValueTextBuffer);
	timeValueText.invalidate();

	uint32_t ampsValue = presenter->getWeldingAmps();
	uint16_t  sVal = uint8_t( ampsValue / 100);
	uint8_t  rVal = (uint8_t) ( ampsValue % 100);
	Unicode::snprintf(ampereTextBuffer, 7, "%03d.%02d", sVal, rVal);
	ampereText.setWildcard(ampereTextBuffer);
	ampereText.invalidate();
}

void mainScreenView::tearDownScreen()
{
    mainScreenViewBase::tearDownScreen();
}

void mainScreenView::configButtonPressed()
{
	CMainJtEventT evt;
	evt.evType = configPressed;
	sendEventToMainJtMessageQ(&evt,0);
}
