#include <mvp/View.hpp>
#include <gui/setamperescreen_screen/setAmpereScreenView.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned long uint32_t;

setAmpereScreenView::setAmpereScreenView()
{

}

void setAmpereScreenView::setupScreen()
{
    setAmpereScreenViewBase::setupScreen();

     ampsValue = presenter->getWeldingAmps();
     setValArray(ampsValue);
     printCurrentValueTimeOnScreen();
}

void setAmpereScreenView::tearDownScreen()
{
    setAmpereScreenViewBase::tearDownScreen();
}

void setAmpereScreenView::backSaveButtonPressed()
{
	presenter->storeWeldingAmps(ampsValue);

	static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotomainScreenScreenNoTransition();
}

void setAmpereScreenView::toggleCursor()
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


void setAmpereScreenView::buttonPressed(uint8_t val)
{
	valArray[valPos] = val;
	recalcAmps();
	printCurrentValueTimeOnScreen();

	++valPos;

	if (valPos > 4)  {
		backSaveButtonPressed();
	}  else {
		if (valPos== 3)  {
			cursor.moveRelative(48, 0);

		} else  {
			cursor.moveRelative(24, 0);
		}
	}
}

void setAmpereScreenView::numButtonPressed(uint8_t value)
{
	buttonPressed(value);
}

void setAmpereScreenView::setValArray(float ampsF)
{
	uint32_t ampsI = (uint32_t) (ampsF * 100);  // evtl + 0.1 or so... to prevent rounding loss
	valArray[0] =  (uint8_t) (ampsI/10000);
	valArray[1] =  (uint8_t) ((ampsI % 10000)/1000);
	valArray[2] =  (uint8_t) ((ampsI % 1000)/100);
	valArray[3] =  (uint8_t) ((ampsI  % 100) /10);
	valArray[4] =  (uint8_t)  (ampsI  % 10);
}
 void setAmpereScreenView::recalcAmps()
 {
	 uint32_t ampsIntValue = (valArray[0] * 10000) + (valArray[1] * 1000) +
			 	 	 	 (valArray[2] * 100) + (valArray[3] *10 ) + valArray[4];
	 ampsValue = (float) (ampsIntValue);
	 ampsValue = ampsValue / 100;
 }
 void setAmpereScreenView::printCurrentValueTimeOnScreen()
 {
	 uint32_t  ampsI = (uint32_t) (ampsValue *100 ); // evtl + 0.1 or so... to prevent rounding loss
	 uint32_t uVal = ampsI/100;
	 uint32_t lVal = ampsI % 100;
	 Unicode::snprintf(setAmpereTextBuffer, 7, "%03d.%02d", uVal,  lVal);
	 setAmpereText.setWildcard(setAmpereTextBuffer);
	 setAmpereText.invalidate();
 }

