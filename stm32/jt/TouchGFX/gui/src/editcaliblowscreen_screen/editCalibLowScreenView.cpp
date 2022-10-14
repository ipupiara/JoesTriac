#include <gui/editcaliblowscreen_screen/editCalibLowScreenView.hpp>
#include <string.h>

editCalibLowScreenView::editCalibLowScreenView()
{
	memset(valArray,0,sizeof(valArray));
}

void editCalibLowScreenView::setupScreen()
{
    editCalibLowScreenViewBase::setupScreen();

    calibVal =  presenter->getCalibLow();
    setValArray(calibVal);
    printCurrentCalibValuesOnScreen(true);
    valPos = 0;
}

void editCalibLowScreenView::tearDownScreen()
{
    editCalibLowScreenViewBase::tearDownScreen();
}

void editCalibLowScreenView::printCurrentCalibValuesOnScreen(bool first)
{

	 Unicode::snprintf(calibLowTextBuffer, 5, "%04d", calibVal);
	 calibLowText.setWildcard(calibLowTextBuffer);
	 calibLowText.invalidate();

	 if (first)  {
		 uint32_t  cHigh = presenter->getCalibHigh();
		 Unicode::snprintf(calibHighTextBuffer, 5, "%04d", cHigh);
		 calibHighText.setWildcard(calibHighTextBuffer);
		 calibHighText.invalidate();
	 }
}

void editCalibLowScreenView::setValArray(uint16_t val)
{
	valArray[0] =  (uint8_t)  (val / 1000);
	valArray[1] =  (uint8_t) ((val % 1000) /100);
	valArray[2] =  (uint8_t) ((val % 100) / 10 );
	valArray[3] =  (uint8_t)  (val % 10) ;
}
 void editCalibLowScreenView::recalcVal()
 {
	 calibVal = (valArray[0] * 1000) + (valArray[1] * 100) + (valArray[2] * 10) + valArray[3];

 }

 void editCalibLowScreenView::buttonPressed(uint8_t val)
 {
 	valArray[valPos] = val;
 	recalcVal();
 	presenter->setCalibLow(calibVal);
 	printCurrentCalibValuesOnScreen(false);

 	if (valPos  <3)  {
 		cursor.moveRelative(24, 0);
 		++valPos;
 	} else   {
 		static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoeditCalibValuesScreenScreenNoTransition();
 	}
 }

 void editCalibLowScreenView::toggleCursor()
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
