#include <gui/alarmconfigscreen_screen/alarmConfigScreenView.hpp>

alarmConfigScreenView::alarmConfigScreenView()
{

}

void alarmConfigScreenView::setupScreen()
{
    alarmConfigScreenViewBase::setupScreen();

    timeValueMin = presenter->getAlarmTime();
    alarmNeeded = presenter->getAlarmNeeded();
    zCalibOn = presenter->getZCalibOn();
    if (alarmNeeded != 0)  {
    	radioButtonOn.setSelected(true);
    }  else {
    	radioButtonOff.setSelected(true);
    }
    if (zCalibOn != 0)  {
    	ZCalibOnButton.setSelected(true);
    }  else {
    	ZCalibOffButton.setSelected(true);
    }

    setValArray(timeValueMin);
    printCurrentValueTimeOnScreen();
}

void alarmConfigScreenView::tearDownScreen()
{
    alarmConfigScreenViewBase::tearDownScreen();
}

void alarmConfigScreenView::backNSaveButtonPressed()
{
	uint8_t aNeeded;
	uint16_t aTime = timeValueMin;
	uint32_t zCalibOn;
	aNeeded = radioButtonOn.getSelected();
	zCalibOn= ZCalibOnButton.getSelected();
	presenter->storeAlarmData(aTime,aNeeded, zCalibOn);
	static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoconfigScreenScreenNoTransition();
}

void alarmConfigScreenView::toggleCursor()
{
	//-fno-inline-functions
	bool vis = cursor.isVisible();
	if (vis == true) {
		cursor.setVisible(false);
	}  else {
		cursor.setVisible(true);
	}
	cursor.invalidate();
}

void alarmConfigScreenView::buttonPressed(uint8_t val)
{
	valArray[valPos] = val;
	recalcTimeMin();
	printCurrentValueTimeOnScreen();

	if (valPos  == 0)  {
		cursor.moveRelative(24, 0);
		valPos = 1;
	} else if (valPos == 1)  {
		cursor.moveRelative(-24, 0);
		valPos = 0;
	}
}

void alarmConfigScreenView::cancelButtonPressed()
{
	static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoconfigScreenScreenNoTransition();
}

void alarmConfigScreenView::numPressed(uint8_t value)
{
	buttonPressed(value);
}

void alarmConfigScreenView::setValArray(uint16_t val)
{
	valArray[0] =  (uint8_t) (val/10);
	valArray[1] =  (uint8_t) (val % 10) ;
}
 void alarmConfigScreenView::recalcTimeMin()
 {
	 timeValueMin = (valArray[0] * 10) + valArray[1]    ;

 }
 void alarmConfigScreenView::printCurrentValueTimeOnScreen()
 {
	 uint16_t  minVal =  timeValueMin;

	 Unicode::snprintf(timeValueTextBuffer, 3, "%02d", minVal);
	 alarmTimeText.setWildcard(timeValueTextBuffer);
	 alarmTimeText.invalidate();
 }
