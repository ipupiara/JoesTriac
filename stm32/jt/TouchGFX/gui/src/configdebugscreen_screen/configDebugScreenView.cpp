#include <gui/configdebugscreen_screen/configDebugScreenView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <images/BitmapDatabase.hpp>


configDebugScreenView::configDebugScreenView()
{
    printPidOnButton.setXY(207, 222);
    printPidOnButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_DARK_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_PRESSED_ID));
    printPidOnButton.setSelected(false);
    printPidOnButton.setDeselectionEnabled(false);
    pidRadioButtonGroup.add(printPidOnButton);
    add(printPidOnButton);

    printPidOffButton.setXY(291, 222);
    printPidOffButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_DARK_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_PRESSED_ID));
    printPidOffButton.setSelected(false);
    printPidOffButton.setDeselectionEnabled(false);
    pidRadioButtonGroup.add(printPidOffButton);
    add(printPidOffButton);

    extiCheckOnButton.setXY(207, 116);
    extiCheckOnButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_DARK_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_PRESSED_ID));
    extiCheckOnButton.setSelected(false);
    extiCheckOnButton.setDeselectionEnabled(false);
    extiRadioButtonGroup.add(extiCheckOnButton);
    add(extiCheckOnButton);

    extiCheckOffButton.setXY(291, 116);
    extiCheckOffButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_DARK_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_PRESSED_ID));
    extiCheckOffButton.setSelected(false);
    extiCheckOffButton.setDeselectionEnabled(false);
    extiRadioButtonGroup.add(extiCheckOffButton);
    add(extiCheckOffButton);

    infoPrintfOffButton.setXY(291, 169);
    infoPrintfOffButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_DARK_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_PRESSED_ID));
    infoPrintfOffButton.setSelected(false);
    infoPrintfOffButton.setDeselectionEnabled(false);
    infoRadioButtonGroup.add(infoPrintfOffButton);
    add(infoPrintfOffButton);

    infoPrintfOnButton.setXY(207, 169);
    infoPrintfOnButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_DARK_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_MEDIUM_ROUNDED_ON_PRESSED_ID));
    infoPrintfOnButton.setSelected(false);
    infoPrintfOnButton.setDeselectionEnabled(false);
    infoRadioButtonGroup.add(infoPrintfOnButton);
    add(infoPrintfOnButton);
}


void configDebugScreenView::setupScreen()
{
    configDebugScreenViewBase::setupScreen();

    presenter->getDebugData(&pidPrintB, &infoPrintB, &extiCheckB);
    if (pidPrintB != 0)  {
    	printPidOnButton.setSelected(true);
    }  else {
    	printPidOffButton.setSelected(true);
    }
    if (infoPrintB != 0)  {
    	infoPrintfOnButton.setSelected(true);
    }  else {
    	infoPrintfOffButton.setSelected(true);
    }
    if (extiCheckB != 0)  {
    	extiCheckOnButton.setSelected(true);
    }  else {
    	infoPrintfOffButton.setSelected(true);
    }
}

void configDebugScreenView::tearDownScreen()
{
    configDebugScreenViewBase::tearDownScreen();
}

void configDebugScreenView::cancelButtonPressed()
{
	static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotobehaviourConfigScreenScreenNoTransition();
}
void configDebugScreenView::saveButtonPressed()
{
	presenter->setDebugData(pidPrintB, infoPrintB, extiCheckB);
	cancelButtonPressed();
}
