/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/configdebugscreen_screen/configDebugScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <images/BitmapDatabase.hpp>

configDebugScreenViewBase::configDebugScreenViewBase() :
    buttonCallback(this, &configDebugScreenViewBase::buttonCallbackHandler)
{
    __background.setPosition(0, 0, 800, 480);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    box1.setPosition(0, 0, 800, 480);
    box1.setColor(touchgfx::Color::getColorFromRGB(176, 247, 191));
    add(box1);

    textArea1.setPosition(54, 21, 120, 51);
    textArea1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_Y55L));
    add(textArea1);

    textArea2.setXY(54, 122);
    textArea2.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea2.setLinespacing(0);
    textArea2.setTypedText(touchgfx::TypedText(T___SINGLEUSE_EA7H));
    add(textArea2);

    textArea3.setXY(54, 204);
    textArea3.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea3.setLinespacing(0);
    textArea3.setTypedText(touchgfx::TypedText(T___SINGLEUSE_4FRP));
    add(textArea3);

    printpidText.setXY(54, 285);
    printpidText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    printpidText.setLinespacing(0);
    printpidText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_AQ60));
    add(printpidText);

    textArea5.setXY(213, 72);
    textArea5.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea5.setLinespacing(0);
    textArea5.setTypedText(touchgfx::TypedText(T___SINGLEUSE_5DBZ));
    add(textArea5);

    textArea6.setXY(330, 72);
    textArea6.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea6.setLinespacing(0);
    textArea6.setTypedText(touchgfx::TypedText(T___SINGLEUSE_HPZE));
    add(textArea6);

    radioButton1.setXY(0, 0);
    radioButton1.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_DARK_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_PRESSED_ID));
    radioButton1.setSelected(false);
    radioButton1.setDeselectionEnabled(false);
    radioButton1.setVisible(false);
    radioButtonGroup1.add(radioButton1);
    add(radioButton1);

    cancelButton.setXY(54, 371);
    cancelButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_BUTTON_REGULAR_HEIGHT_50_MEDIUM_ROUNDED_NORMAL_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_BUTTON_REGULAR_HEIGHT_50_MEDIUM_ROUNDED_PRESSED_ID));
    cancelButton.setLabelText(touchgfx::TypedText(T___SINGLEUSE_OONN));
    cancelButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    cancelButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    cancelButton.setAction(buttonCallback);
    add(cancelButton);

    saveButton.setXY(400, 371);
    saveButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_BUTTON_REGULAR_HEIGHT_50_MEDIUM_ROUNDED_NORMAL_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_BUTTON_REGULAR_HEIGHT_50_MEDIUM_ROUNDED_PRESSED_ID));
    saveButton.setLabelText(touchgfx::TypedText(T___SINGLEUSE_JP7J));
    saveButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    saveButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    saveButton.setAction(buttonCallback);
    add(saveButton);
}

configDebugScreenViewBase::~configDebugScreenViewBase()
{

}

void configDebugScreenViewBase::setupScreen()
{

}

void configDebugScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &cancelButton)
    {
        //cancelButtonInteraction
        //When cancelButton clicked call virtual function
        //Call cancelButtonPressed
        cancelButtonPressed();
    }
    if (&src == &saveButton)
    {
        //saveButtonInteraction
        //When saveButton clicked call virtual function
        //Call saveButtonPressed
        saveButtonPressed();
    }
}