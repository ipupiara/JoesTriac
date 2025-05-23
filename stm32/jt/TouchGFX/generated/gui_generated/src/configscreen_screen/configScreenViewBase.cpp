/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/configscreen_screen/configScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

configScreenViewBase::configScreenViewBase() :
    buttonCallback(this, &configScreenViewBase::buttonCallbackHandler)
{
    __background.setPosition(0, 0, 800, 480);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    bgBox.setPosition(0, 0, 800, 480);
    bgBox.setColor(touchgfx::Color::getColorFromRGB(176, 247, 191));
    add(bgBox);

    calibrationButton.setXY(230, 53);
    calibrationButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    calibrationButton.setLabelText(touchgfx::TypedText(T___SINGLEUSE_VXN9));
    calibrationButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    calibrationButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    calibrationButton.setAction(buttonCallback);
    add(calibrationButton);

    backButton.setXY(15, 389);
    backButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_BACK_ARROW_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_BACK_ARROW_32_ID));
    backButton.setIconXY(77, 15);
    backButton.setAction(buttonCallback);
    add(backButton);

    editCalibButton.setXY(230, 147);
    editCalibButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    editCalibButton.setLabelText(touchgfx::TypedText(T___SINGLEUSE_GX36));
    editCalibButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    editCalibButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    editCalibButton.setAction(buttonCallback);
    add(editCalibButton);

    configButton.setXY(230, 240);
    configButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    configButton.setLabelText(touchgfx::TypedText(T___SINGLEUSE_AOKD));
    configButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    configButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    configButton.setAction(buttonCallback);
    add(configButton);

    textArea1.setXY(428, 72);
    textArea1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_ON4B));
    add(textArea1);

    textArea2.setXY(428, 166);
    textArea2.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea2.setLinespacing(0);
    textArea2.setTypedText(touchgfx::TypedText(T___SINGLEUSE_8CTH));
    add(textArea2);

    textArea3.setXY(428, 259);
    textArea3.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea3.setLinespacing(0);
    textArea3.setTypedText(touchgfx::TypedText(T___SINGLEUSE_1T1P));
    add(textArea3);

    alarmText.setXY(241, 396);
    alarmText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    alarmText.setLinespacing(0);
    alarmText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_TOZG));
    add(alarmText);
}

configScreenViewBase::~configScreenViewBase()
{

}

void configScreenViewBase::setupScreen()
{

}

void configScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &calibrationButton)
    {
        //calibrationInteraction
        //When calibrationButton clicked call virtual function
        //Call autoCalibrationClicked
        autoCalibrationClicked();
    }
    if (&src == &backButton)
    {
        //backInteraction
        //When backButton clicked call virtual function
        //Call backPressed
        backPressed();
    }
    if (&src == &editCalibButton)
    {
        //editCalibPressedInteraction
        //When editCalibButton clicked change screen to editCalibValuesScreen
        //Go to editCalibValuesScreen with no screen transition
        application().gotoeditCalibValuesScreenScreenNoTransition();
    }
    if (&src == &configButton)
    {
        //behaviourPressedInteraction
        //When configButton clicked change screen to behaviourConfigScreen
        //Go to behaviourConfigScreen with no screen transition
        application().gotobehaviourConfigScreenScreenNoTransition();
    }
}
