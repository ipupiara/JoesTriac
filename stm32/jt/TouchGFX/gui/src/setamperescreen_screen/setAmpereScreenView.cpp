#include <mvp/View.hpp>
#include <gui/setamperescreen_screen/setAmpereScreenView.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned long uint32_t;

setAmpereScreenView::setAmpereScreenView():
	textClickedCallback(this, &setAmpereScreenView::textClickHandler)
{
	xOffset = 3;
	valPos = 0;
}

void setAmpereScreenView::setupScreen()
{
    setAmpereScreenViewBase::setupScreen();

     ampsValue = presenter->getWeldingAmps() + 0.005;
     setValArray(ampsValue);
     printCurrentValueTimeOnScreen();
     setAmpereText.setClickAction(textClickedCallback);
}

//uint8_t amtFields;
//
//void initClickValues()
//{
//	amtFields = 5;
//}



void setAmpereScreenView::textClickHandler(const TextAreaWithOneWildcard& txt, const ClickEvent& evt )
{
	uint8_t fieldPattern [] = {0,0,0,0xFF,0,0};
	uint8_t valPosPattern []  = {0,1,2,0xFF,3,4};
	int16_t xPos = evt.getX();
	int16_t fieldOffset = txt.getX();
	uint8_t amtFields = 5;
	int16_t fieldWidth = 24;
	int16_t clickedField =  (xPos / fieldWidth);


	//	uint16_t txtWidth = txt.getWidth();
	//	int16_t fieldWidth = (txtWidth / amtFields);



    if (&txt == &setAmpereText)
    {
    	if (fieldPattern[clickedField] == 0xFF)  {
    		uint16_t remXPos = (xPos % fieldWidth);
    		if (remXPos >  (fieldWidth / 2) ) {
    			clickedField ++;
    		}
    		else  {
    			clickedField --;
    		}
    	}
    }
    int16_t newPos = (clickedField * fieldWidth);
    int16_t ps0 = cursor.getX();
    cursor.invalidate();
    cursor.setX(newPos + fieldOffset + xOffset);
    int16_t ps1 = cursor.getX();
    valPos = valPosPattern[clickedField];
    cursor.invalidate();
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
		valPos = 0;
		cursor.setX(setAmpereText.getX() + xOffset);
//		backSaveButtonPressed();
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
	uint32_t ampsI =  ((ampsF * 100.0) + 0.1);  // todo evtl + 0.1 or so... to prevent rounding loss
//	double ampsI = (ampsF * 100.0) + 0.5;
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
	 double ampsV =  ampsIntValue +0.1;
	 ampsValue = ampsV / 100.0;
 }
 void setAmpereScreenView::printCurrentValueTimeOnScreen()
 {
	 uint32_t  ampsI =  (ampsValue *100.0 ); // evtl + 0.1 or so... to prevent rounding loss
	 uint32_t uVal = ampsI/100;
	 uint32_t lVal = ampsI % 100;
	 Unicode::snprintf(setAmpereTextBuffer, 7, "%03d.%02d", uVal,  lVal);
	 setAmpereText.setWildcard(setAmpereTextBuffer);
	 setAmpereText.invalidate();
 }

