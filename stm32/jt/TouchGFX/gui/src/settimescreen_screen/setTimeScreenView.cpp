#include <mvp/View.hpp>
#include <gui/settimescreen_screen/setTimeScreenView.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Unicode.hpp>

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

void  setTimeScreenView::goBack()
{
	// save value
	Application::getInstance()->changeToStartScreen();
}

void setTimeScreenView::backSaveButtonPressed()
{
	goBack();
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

	++valPos;
	if (valPos > 3)  {
		goBack();
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
	valArray[1] =  (uint8_t) (val/60);
	valArray[2] =  (uint8_t) (val/10);
	valArray[3] =  (uint8_t) (val % 10);
}
 void setTimeScreenView::recalcTimeSec()
 {
	 timeValueSec = (valArray[0] * 600) + (valArray[1] * 60) +
			 	 	 	 (valArray[2] * 10) + valArray[0]  ;
	 presenter->setWeldingTimeSec(timeValueSec);

 }
 void setTimeScreenView::printCurrentValueTimeOnScreen()
 {
	 uint8_t  minVal = timeValueSec % 60;
	 uint8_t  secVal = (uint8_t) ( timeValueSec / 60);
	 Unicode::snprintf(timeValueTextBuffer, 5, "%2d:%2d", minVal, secVal);
	 timeValueText.setWildcard(timeValueTextBuffer);
	 timeValueText.invalidate();
 }


 /*
   SOME SOURCE SAMPLES FOR TEXT MODIFICATION
 void MainScreenView::changeTextAreaValue(int slidervalue)
 {
     Unicode::snprintf(textArea1Buffer, 3, "%d", slidervalue);
     textArea1.resizeToCurrentText();
     textArea1.invalidate();
 }
 ***************************************************
 HPP

 class SomeView : public View<SomePresenter>
 {
   TextAreaWithOneWildcard txt;
   Unicode::UnicodeChar txtBuffer[10];
 }

 CPP:

 #include <touchgfx/widgets/TextAreaWithWildcard.hpp>
 ...
 class SomeView : public View<SomePresenter>
 {
   TextAreaWithOneWildcard txt;
   Unicode::UnicodeChar txtBuffer[10];
 }

 gui/src/some_screen/SomeView.cpp:
 #include <texts/TextKeysAndLanguages.hpp>

 void SomeView::setupScreen()
 {
   txt.setTypedText(TypedText(T_TEMPERATURE_READOUT));
   txt.setXY(10, 20);
   txt.setWildcard(txtBuffer);
   add(txt);

   updateTxt(5);
 }

 void SomeView::updateTxt(int newValue)
 {
   Unicode::snprintf(txtBuffer, 10, "%d", newValue);
   txt.invalidate();
 }

 *************************************

  counter++;
   Unicode::snprintf(textCounterBuffer, TEXTCOUNTER_SIZE, "%d", counter);
   // Invalidate text area, which will result in it being redrawn in next tick.
   textCounter.invalidate();
 // -fno-inline-functions
 */
