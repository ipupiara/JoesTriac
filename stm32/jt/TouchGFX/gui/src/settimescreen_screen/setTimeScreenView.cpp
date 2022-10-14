#include <mvp/View.hpp>
#include <gui/settimescreen_screen/setTimeScreenView.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdio.h>

////  ATTENTION  urgently set    -fno-inline-functions  in C++ and G++   plus -O0   no optimization, else code goes to loony bin and booby hatch


setTimeScreenView::setTimeScreenView()
{

}

void setTimeScreenView::setupScreen()
{
    setTimeScreenViewBase::setupScreen();

    timeValueSec = presenter->getWeldingTimeSec();
    setValArray(timeValueSec);
    printCurrentValueTimeOnScreen();
}

void setTimeScreenView::tearDownScreen()
{
    setTimeScreenViewBase::tearDownScreen();
}

void setTimeScreenView::backSaveButtonPressed()
{
	presenter->storeWeldingTimeSec(timeValueSec);

	static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotomainScreenScreenNoTransition();
}

void setTimeScreenView::toggleCursor()
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


void setTimeScreenView::buttonPressed(uint8_t val)
{
	valArray[valPos] = val;
	recalcTimeSec();
	printCurrentValueTimeOnScreen();

	++valPos;

	if (valPos > 3)  {
		backSaveButtonPressed();
	}  else {
		if (valPos== 2)  {
			numericKeyPad1.setEnable7to9forInput(false);
			cursor.moveRelative(48, 0);

		} else  {
			if (valPos == 3)  {
				numericKeyPad1.setEnable7to9forInput(true);
			}
			cursor.moveRelative(24, 0);
		}
	}
}

void setTimeScreenView::numButtonPressed(uint8_t value)
{
	buttonPressed(value);
}

//void      setTimeScreenView::setValArray(uint16_t val)
//{
//	valArray[0] = getWeldingTime();
//}
//
//void      setTimeScreenView::charPressedRecalcData(uint8_t val, uint8_t pos)
//{
//
//}

void setTimeScreenView::setValArray(uint16_t val)
{
	valArray[0] =  (uint8_t) (val/600);
	valArray[1] =  (uint8_t) ((val % 600)/60);
	valArray[2] =  (uint8_t) ((val % 60)/10);
	valArray[3] =  (uint8_t) (val  % 10);
}
 void setTimeScreenView::recalcTimeSec()
 {
	 timeValueSec = (valArray[0] * 600) + (valArray[1] * 60) +
			 	 	 	 (valArray[2] * 10) + valArray[3]  ;

 }
 void setTimeScreenView::printCurrentValueTimeOnScreen()
 {
	 uint8_t  minVal = uint8_t( timeValueSec / 60);
	 uint8_t  secVal = (uint8_t) ( timeValueSec % 60);
	 Unicode::snprintf(timeValueTextBuffer, 6, "%02d:%02d", minVal, secVal);
	 timeValueText.setWildcard(timeValueTextBuffer);
	 timeValueText.invalidate();
 }


