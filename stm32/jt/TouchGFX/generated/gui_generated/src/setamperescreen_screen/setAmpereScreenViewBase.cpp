/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/setamperescreen_screen/setAmpereScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>


setAmpereScreenViewBase::setAmpereScreenViewBase() :
    buttonCallback(this, &setAmpereScreenViewBase::buttonCallbackHandler)
{

    touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

    __background.setPosition(0, 0, 800, 480);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));

    box1.setPosition(0, 0, 800, 480);
    box1.setColor(touchgfx::Color::getColorFromRGB(190, 226, 247));

    setAmpereText.setXY(465, 12);
    setAmpereText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    setAmpereText.setLinespacing(0);
    setAmpereText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_EKBE));

    numericKeyPad1.setXY(0, 23);

    backNSaveButton.setXY(596, 391);
    backNSaveButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    backNSaveButton.setLabelText(touchgfx::TypedText(T___SINGLEUSE_NX94));
    backNSaveButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    backNSaveButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    backNSaveButton.setAction(buttonCallback);

    cursor.setPosition(471, 19, 23, 39);
    cursorPainter.setColor(touchgfx::Color::getColorFromRGB(247, 212, 15));
    cursor.setPainter(cursorPainter);
    cursor.setStart(5, 5);
    cursor.setEnd(5, 20);
    cursor.setLineWidth(20);
    cursor.setLineEndingStyle(touchgfx::Line::ROUND_CAP_ENDING);
    cursor.setAlpha(138);

    add(__background);
    add(box1);
    add(setAmpereText);
    add(numericKeyPad1);
    add(backNSaveButton);
    add(cursor);
}

void setAmpereScreenViewBase::setupScreen()
{
    numericKeyPad1.initialize();
}

void setAmpereScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &backNSaveButton)
    {
        //Interaction1
        //When backNSaveButton clicked call virtual function
        //Call backNSaveButtonPressed
        backNSaveButtonPressed();
    }
}