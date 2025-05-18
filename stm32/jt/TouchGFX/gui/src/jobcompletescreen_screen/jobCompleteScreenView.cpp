#include <gui/jobcompletescreen_screen/jobCompleteScreenView.hpp>

jobCompleteScreenView::jobCompleteScreenView()
{
	//  todo add coolingtime on screen
}

void jobCompleteScreenView::setupScreen()
{
    jobCompleteScreenViewBase::setupScreen();
    uint8_t alarmTime = getDefinesAlarmTime();

	Unicode::snprintf(totalTimeTextBuffer, 6, "%02d", alarmTime);
	totalTimeText.setWildcard(totalTimeTextBuffer);
	totalTimeText.invalidate();
	uint8_t alarmState = getDefinesAlarmNeeded();
	if (alarmState == 0) {
		Unicode::snprintf(alarmStateTextBuffer, 4, "off");
	} else {
		Unicode::snprintf(alarmStateTextBuffer, 4, "on");
	}
	alarmStateText.setWildcard(alarmStateTextBuffer);
	alarmStateText.invalidate();
}

void jobCompleteScreenView::tearDownScreen()
{
    jobCompleteScreenViewBase::tearDownScreen();
}

void jobCompleteScreenView::returnButtonPressed()
 {
	presenter->returnButtonPressed();
 }

void jobCompleteScreenView::updateCompletionTime(uint32_t secsRemain)
{
	 uint8_t  minVal = uint8_t( secsRemain / 60);
	 uint8_t  secVal = (uint8_t) ( secsRemain % 60);
	 Unicode::snprintf(timeTextBuffer, 6, "%02d:%02d", minVal, secVal);
	 timeText.setWildcard(timeTextBuffer);
	 timeText.invalidate();
}
