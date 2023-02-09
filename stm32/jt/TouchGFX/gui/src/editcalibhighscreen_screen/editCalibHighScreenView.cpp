#include <gui/editcalibhighscreen_screen/editCalibHighScreenView.hpp>

editCalibHighScreenView::editCalibHighScreenView()
{
	memset(valArray,0,sizeof(valArray));
}

void editCalibHighScreenView::setupScreen()
{
    editCalibHighScreenViewBase::setupScreen();

    calibVal =  presenter->getCalibHigh();
    setValArray(calibVal);
    printCurrentCalibValuesOnScreen(true);
    valPos = 0;
}

void editCalibHighScreenView::tearDownScreen()
{
    editCalibHighScreenViewBase::tearDownScreen();
}

void editCalibHighScreenView::toggleCursor()
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

void editCalibHighScreenView::printCurrentCalibValuesOnScreen(bool first)
{
	if (first)  {
		uint32_t  cLow = presenter->getCalibLow();
		Unicode::snprintf(calibLowTextBuffer, 5, "%04d", cLow);
		calibLowText.setWildcard(calibLowTextBuffer);
		calibLowText.invalidate();
	}
	Unicode::snprintf(calibHighTextBuffer, 5, "%04d", calibVal);
	calibHighText.setWildcard(calibHighTextBuffer);
	calibHighText.invalidate();
}

void editCalibHighScreenView::setValArray(uint16_t val)
{
	valArray[0] =  (uint8_t)  (val / 1000);
	valArray[1] =  (uint8_t) ((val % 1000) /100);
	valArray[2] =  (uint8_t) ((val % 100) / 10 );
	valArray[3] =  (uint8_t)  (val % 10) ;
}
 void editCalibHighScreenView::recalcVal()
 {
	 calibVal = (valArray[0] * 1000) + (valArray[1] * 100) + (valArray[2] * 10) + valArray[3];
 }

 void editCalibHighScreenView::buttonPressed(uint8_t val)
 {
 	valArray[valPos] = val;
 	recalcVal();
 	presenter->setCalibHigh(calibVal);
 	printCurrentCalibValuesOnScreen(false);

 	if (valPos  <3)  {
 		cursor.moveRelative(24, 0);
 		++valPos;
 	} else   {
 		static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoeditCalibValuesScreenScreenNoTransition();
 	}
 }


