#include <gui/mainscreen_screen/mainScreenView.hpp>

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
}

void mainScreenView::tearDownScreen()
{
    mainScreenViewBase::tearDownScreen();
}
