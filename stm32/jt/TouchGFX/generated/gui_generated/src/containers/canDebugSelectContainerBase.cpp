/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/canDebugSelectContainerBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

canDebugSelectContainerBase::canDebugSelectContainerBase() :
    radioButtonSelectedCallback(this, &canDebugSelectContainerBase::radioButtonSelectedCallbackHandler)
{
    setWidth(315);
    setHeight(85);
    boxWithBorder1.setPosition(0, 0, 315, 85);
    boxWithBorder1.setColor(touchgfx::Color::getColorFromRGB(247, 218, 218));
    boxWithBorder1.setBorderColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    boxWithBorder1.setBorderSize(1);
    add(boxWithBorder1);

    radioButtonGroup1.setRadioButtonSelectedHandler(radioButtonSelectedCallback);
    
    nonButton.setXY(6, 6);
    nonButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_DARK_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_PRESSED_ID));
    nonButton.setSelected(false);
    nonButton.setDeselectionEnabled(false);
    radioButtonGroup1.add(nonButton);
    add(nonButton);

    singleButton.setXY(116, 6);
    singleButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_DARK_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_PRESSED_ID));
    singleButton.setSelected(false);
    singleButton.setDeselectionEnabled(false);
    radioButtonGroup1.add(singleButton);
    add(singleButton);

    continuousButton.setXY(270, 6);
    continuousButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_DARK_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_PRESSED_ID));
    continuousButton.setSelected(false);
    continuousButton.setDeselectionEnabled(false);
    radioButtonGroup1.add(continuousButton);
    add(continuousButton);

    textArea1.setXY(6, 49);
    textArea1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_INT2));
    add(textArea1);

    textArea2.setXY(80, 49);
    textArea2.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea2.setLinespacing(0);
    textArea2.setTypedText(touchgfx::TypedText(T___SINGLEUSE_94WC));
    add(textArea2);

    textArea7.setXY(186, 49);
    textArea7.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea7.setLinespacing(0);
    textArea7.setTypedText(touchgfx::TypedText(T___SINGLEUSE_Z6KK));
    add(textArea7);
}

canDebugSelectContainerBase::~canDebugSelectContainerBase()
{

}

void canDebugSelectContainerBase::initialize()
{

}

void canDebugSelectContainerBase::radioButtonSelectedCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &nonButton)
    {
        //noneButtonSelectedInteraction
        //When nonButton selected call virtual function
        //Call nonButtonSelected
        nonButtonSelected();
    }
    if (&src == &singleButton)
    {
        //singleButtonSelectedInteraction
        //When singleButton selected call virtual function
        //Call singleButtonSelected
        singleButtonSelected();
    }
    if (&src == &continuousButton)
    {
        //continuousButtonSelectedInteraction
        //When continuousButton selected call virtual function
        //Call continuousButtonSelected
        continuousButtonSelected();
    }
}