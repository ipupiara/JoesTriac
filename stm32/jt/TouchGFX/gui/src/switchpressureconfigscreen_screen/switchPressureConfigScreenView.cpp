#include <gui/switchpressureconfigscreen_screen/switchPressureConfigScreenView.hpp>

switchPressureConfigScreenView::switchPressureConfigScreenView()
{

}

void switchPressureConfigScreenView::setupScreen()
{
    switchPressureConfigScreenViewBase::setupScreen();

    switchPressureTimeMin = presenter->getSwitchPressureTime();
    switchPressureNeeded = presenter->getSwitchPressureNeeded();
    if (switchPressureNeeded != 0)  {
    	switchPressureOnRadiobutton.setSelected(true);
    }  else {
    	switchPressureOffRadiobutton.setSelected(true);
    }
    setValArray(switchPressureTimeMin);
    printCurrentTimeValueOnScreen();
}

void switchPressureConfigScreenView::tearDownScreen()
{
    switchPressureConfigScreenViewBase::tearDownScreen();
}

void switchPressureConfigScreenView::toggleCursor()
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

void switchPressureConfigScreenView::cancelButtonPressed()
{
	static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotobehaviourConfigScreenScreenNoTransition();
}

void switchPressureConfigScreenView::backNSaveButtonPressed()
{
	uint8_t spNeeded;
	uint16_t spTime = switchPressureTimeMin;
	spNeeded = switchPressureOnRadiobutton.getSelected();
	presenter->storeSwitchPressureData(spTime, spNeeded);
	cancelButtonPressed();
}

void switchPressureConfigScreenView::buttonPressed(uint8_t val)
{
	valArray[valPos] = val;
	recalcPressureSwitchTime();
	printCurrentTimeValueOnScreen();

	if (valPos  == 0)  {
		cursor.moveRelative(24, 0);
		valPos = 1;
	} else if (valPos == 1)  {
		cursor.moveRelative(-24, 0);
		valPos = 0;
	}
}

void switchPressureConfigScreenView::numPressed(uint8_t value)
{
	buttonPressed(value);  // todo  place buttonpressed code here
}

void switchPressureConfigScreenView::setValArray(uint16_t val)
{
	valArray[0] =  (uint8_t) (val/10);
	valArray[1] =  (uint8_t) (val % 10) ;
}
 void switchPressureConfigScreenView::recalcPressureSwitchTime()
 {
	 switchPressureTimeMin = (valArray[0] * 10) + valArray[1]    ;

 }
 void switchPressureConfigScreenView::printCurrentTimeValueOnScreen()
 {
	 uint16_t  minVal =  switchPressureTimeMin;
	 Unicode::snprintf(switchPressureTimeTextBuffer, 3, "%02d", minVal);
	 switchPressureTimeText.setWildcard(switchPressureTimeTextBuffer);
	 switchPressureTimeText.invalidate();
 }






