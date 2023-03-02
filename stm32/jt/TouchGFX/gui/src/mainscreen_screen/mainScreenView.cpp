#include <gui/mainscreen_screen/mainScreenView.hpp>
#include <mainJt.h>

mainScreenView::mainScreenView()
{

}

void mainScreenView::paintGoalNeedle(int16_t val)   //  todo tobe tested
{
//	uint16_t oldVal = ampGauge.getValue();
//	ampGauge.setNeedle(BITMAP_RED_NEEDLES_GOAL_GAUGE_NEEDLE_STYLE_01_ID, 11, 115);
//    ampGauge.setValue(val);
//    ampGauge.invalidateContent();
// //   ampGauge.invalidate();
//    ampGauge.setNeedle(BITMAP_BLUE_NEEDLES_ORIGINAL_GAUGE_NEEDLE_STYLE_01_ID, 11, 55);
//    ampGauge.setValue(oldVal);
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

	float ampsValue = presenter->getWeldingAmps();
//	 uint32_t  ampsI =  (ampsValue *100 ); // evtl + 0.1 or so... to prevent rounding loss
//	 uint32_t sVal = ampsI/100;
//	 uint32_t rVal = ampsI % 100;
//	Unicode::snprintf(ampereTextBuffer, 7, "%03d.%02d", sVal, rVal);
	Unicode::snprintfFloat(ampereTextBuffer, 7, "%06.2f", ampsValue );
	ampereText.setWildcard(ampereTextBuffer);
	ampereText.invalidate();

//	int16_t ampsValue16 = ampsValue;
//	paintGoalNeedle(ampsValue16);
}

void mainScreenView::tearDownScreen()
{
    mainScreenViewBase::tearDownScreen();
}

void mainScreenView::configButtonPressed()
{
	presenter->configButtonPressed();
}

void mainScreenView::startButtonPressed()
{
	presenter->startButtonPressed();
}
