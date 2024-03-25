#include <gui/mainscreen_screen/mainScreenView.hpp>
#include <images/BitmapDatabase.hpp>
#include <mainJt.h>


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
	gaugeWithGoal.setValue(0);
	gaugeWithGoal.setGoalValue(ampsValue);
	gaugeWithGoal.invalidate();
//	pidDataGraphContainer1.setVisible(false);
//	pidDataGraphContainer1.invalidate();

	pidDataGraphContainer1.graphInitialized = 0; //  todo tobe tested when debugging problem that graph disappears sometimes after screen changes
	// complicated call mechanism left so far, idea was to be able to send different graphs from repository to screen, just a brainstorm scratch paper idea
	pidDataGraphContainer1.setRedrawButtonVisible(true);
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

void mainScreenView::showPidGraphFromData(pJoesPresenterEventT  pMsg)
//  todo for timing reason print in graph own Q-method instead of within touchgfx tick with low prio(pJoesPresenterEventT  pMsg)
{
	pidDataGraphContainer1.showPidGraphFromData(pMsg);
}

void mainScreenView::graphButtonPressed()
{
	if (! pidDataGraphContainer1.isVisible()) {
		pidDataGraphContainer1.setVisible(true);
	}
	pidDataGraphContainer1.invalidate();
}
