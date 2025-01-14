/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/numericKeyPadBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>

numericKeyPadBase::numericKeyPadBase() :
    numPressedCallback(0),
    buttonCallback(this, &numericKeyPadBase::buttonCallbackHandler)
{
    setWidth(346);
    setHeight(457);
    box1.setPosition(0, 0, 346, 457);
    box1.setColor(touchgfx::Color::getColorFromRGB(85, 135, 135));
    add(box1);

    button1.setXY(11, 10);
    button1.setBitmaps(touchgfx::Bitmap(BITMAP_BLUENORMAL_1_ID), touchgfx::Bitmap(BITMAP_BLUEPRESSED_X_ID));
    button1.setAction(buttonCallback);
    add(button1);

    button2.setXY(123, 10);
    button2.setBitmaps(touchgfx::Bitmap(BITMAP_BLUENORMAL_2_ID), touchgfx::Bitmap(BITMAP_BLUEPRESSED_X_ID));
    button2.setAction(buttonCallback);
    add(button2);

    button3.setXY(234, 10);
    button3.setBitmaps(touchgfx::Bitmap(BITMAP_BLUENORMAL_3_ID), touchgfx::Bitmap(BITMAP_BLUEPRESSED_X_ID));
    button3.setAction(buttonCallback);
    add(button3);

    button4.setXY(11, 121);
    button4.setBitmaps(touchgfx::Bitmap(BITMAP_BLUENORMAL_4_ID), touchgfx::Bitmap(BITMAP_BLUEPRESSED_X_ID));
    button4.setAction(buttonCallback);
    add(button4);

    button5.setXY(123, 121);
    button5.setBitmaps(touchgfx::Bitmap(BITMAP_BLUENORMAL_5_ID), touchgfx::Bitmap(BITMAP_BLUEPRESSED_X_ID));
    button5.setAction(buttonCallback);
    add(button5);

    button6.setXY(234, 121);
    button6.setBitmaps(touchgfx::Bitmap(BITMAP_BLUENORMAL_6_ID), touchgfx::Bitmap(BITMAP_BLUEPRESSED_X_ID));
    button6.setAction(buttonCallback);
    add(button6);

    button7.setXY(11, 233);
    button7.setBitmaps(touchgfx::Bitmap(BITMAP_BLUENORMAL_7_ID), touchgfx::Bitmap(BITMAP_BLUEPRESSED_X_ID));
    button7.setAction(buttonCallback);
    add(button7);

    button8.setXY(123, 233);
    button8.setBitmaps(touchgfx::Bitmap(BITMAP_BLUENORMAL_8_ID), touchgfx::Bitmap(BITMAP_BLUEPRESSED_X_ID));
    button8.setAction(buttonCallback);
    add(button8);

    button9.setXY(234, 233);
    button9.setBitmaps(touchgfx::Bitmap(BITMAP_BLUENORMAL_9_ID), touchgfx::Bitmap(BITMAP_BLUEPRESSED_X_ID));
    button9.setAction(buttonCallback);
    add(button9);

    button10.setXY(123, 346);
    button10.setBitmaps(touchgfx::Bitmap(BITMAP_BLUENORMAL_0_ID), touchgfx::Bitmap(BITMAP_BLUEPRESSED_X_ID));
    button10.setAction(buttonCallback);
    add(button10);
}

numericKeyPadBase::~numericKeyPadBase()
{

}

void numericKeyPadBase::initialize()
{

}

void numericKeyPadBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &button10)
    {
        //button0Pessed
        //When button10 clicked call virtual function
        //Call button0Pressed
        button0Pressed();
    }
    if (&src == &button1)
    {
        //button1Pressed
        //When button1 clicked call virtual function
        //Call button1Pressed
        button1Pressed();
    }
    if (&src == &button2)
    {
        //button2Pressed
        //When button2 clicked call virtual function
        //Call button2Pressed
        button2Pressed();
    }
    if (&src == &button3)
    {
        //button3Pressed
        //When button3 clicked call virtual function
        //Call button3Pressed
        button3Pressed();
    }
    if (&src == &button4)
    {
        //button4Pressed
        //When button4 clicked call virtual function
        //Call button4Pressed
        button4Pressed();
    }
    if (&src == &button5)
    {
        //button5Pressed
        //When button5 clicked call virtual function
        //Call button5Pressed
        button5Pressed();
    }
    if (&src == &button6)
    {
        //button6Pressed
        //When button6 clicked call virtual function
        //Call button6Pressed
        button6Pressed();
    }
    if (&src == &button7)
    {
        //button7Pressed
        //When button7 clicked call virtual function
        //Call button7Pressed
        button7Pressed();
    }
    if (&src == &button8)
    {
        //button8Pressed
        //When button8 clicked call virtual function
        //Call button8Pressed
        button8Pressed();
    }
    if (&src == &button9)
    {
        //button9Pressed
        //When button9 clicked call virtual function
        //Call button9Pressed
        button9Pressed();
    }
}
