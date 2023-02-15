#include <mvp/View.hpp>
#include <gui/settimescreen_screen/setTimeScreenView.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdio.h>

////  ATTENTION  urgently set    -fno-inline-functions  in C++ and G++   plus -O0   no optimization, else code goes to loony bin and booby hatch


setTimeScreenView::setTimeScreenView():
	textClickedCallback(this, &setTimeScreenView::textClickHandler)
{
	xOffset = 3;
	valPos = 0;
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
		valPos = 0;
		cursor.setX(timeValueText.getX() + xOffset);
//		backSaveButtonPressed();
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

//  todo if more such string input is needed -> implement textClickHandeled Text into one or more components with a textfield
void setTimeScreenView::textClickHandler(const TextAreaWithOneWildcard& txt, const ClickEvent& evt )
{
   if (&txt == &timeValueText)
	{
		uint8_t fieldPattern [] = {0,0,0xFF,0,0};
		uint8_t valPosPattern []  = {0,1,0xFF,2,3};
		int16_t xPos = evt.getX();
		int16_t fieldOffset = txt.getX();
		int16_t fieldWidth = 24;
		int16_t clickedField =  (xPos / fieldWidth);

		if (fieldPattern[clickedField] == 0xFF)  {
			uint16_t remXPos = (xPos % fieldWidth);
			if (remXPos >  (fieldWidth / 2) ) {
				clickedField ++;
			}
			else  {
				clickedField --;
			}
		}

		int16_t newPos = (clickedField * fieldWidth);
		cursor.invalidate();
		cursor.setX(newPos + fieldOffset + xOffset);
		valPos = valPosPattern[clickedField];
		cursor.invalidate();

		if (valPos== 2)  {
			numericKeyPad1.setEnable7to9forInput(false);
		} else  {
			numericKeyPad1.setEnable7to9forInput(true);
		}
	}
}


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


