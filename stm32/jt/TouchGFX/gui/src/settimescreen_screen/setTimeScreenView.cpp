#include <gui/settimescreen_screen/setTimeScreenView.hpp>
#include <stdint.h>

setTimeScreenView::setTimeScreenView()
{

}

void setTimeScreenView::setupScreen()
{
    setTimeScreenViewBase::setupScreen();


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

*/

void setTimeScreenView::toggleCursor()
{
	if (cursor.isVisible()) {
		cursor.setVisible(false);
	}  else {
		cursor.setVisible(true);
	}
}

void setTimeScreenView::buttonPressed(uint8_t val)
{
//	valBuffer[valPos] = val;
	++valPos;
	if (valPos > 3)  {
		goBack();
	}
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

void      setTimeScreenView::printCurrentValueTimeOn(touchgfx::TextAreaWithOneWildcard* textA)
{

}

