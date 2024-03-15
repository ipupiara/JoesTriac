#include <gui/configdebugscreen_screen/configDebugScreenView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <images/BitmapDatabase.hpp>


configDebugScreenView::configDebugScreenView()
{
    printPidOnButton.setXY(200, 272);
    printPidOnButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_ACTION_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_PRESSED_ID));
    printPidOnButton.setSelected(false);
    printPidOnButton.setDeselectionEnabled(false);
    pidRadioButtonGroup.add(printPidOnButton);
    add(printPidOnButton);

    printPidOffButton.setXY(330, 272);
    printPidOffButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_ACTION_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_PRESSED_ID));
    printPidOffButton.setSelected(false);
    printPidOffButton.setDeselectionEnabled(false);
    pidRadioButtonGroup.add(printPidOffButton);
    add(printPidOffButton);

    extiCheckOnButton.setXY(200, 109);
    extiCheckOnButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_ACTION_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_PRESSED_ID));
    extiCheckOnButton.setSelected(false);
    extiCheckOnButton.setDeselectionEnabled(false);
    extiCheckOnButton.setTouchable(false);
    extiCheckOnButton.setAlpha(80);
    extiRadioButtonGroup.add(extiCheckOnButton);
    add(extiCheckOnButton);

    extiCheckOffButton.setXY(330, 109);
    extiCheckOffButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_ACTION_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_PRESSED_ID));
    extiCheckOffButton.setSelected(false);
    extiCheckOffButton.setDeselectionEnabled(false);
    extiCheckOffButton.setTouchable(false);
    extiCheckOffButton.setAlpha(80);
    extiRadioButtonGroup.add(extiCheckOffButton);
    add(extiCheckOffButton);

    infoPrintfOnButton.setXY(200, 191);
    infoPrintfOnButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_ACTION_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_PRESSED_ID));
    infoPrintfOnButton.setSelected(false);
    infoPrintfOnButton.setDeselectionEnabled(false);
    infoRadioButtonGroup.add(infoPrintfOnButton);
    add(infoPrintfOnButton);

    infoPrintfOffButton.setXY(330, 191);
    infoPrintfOffButton.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_OFF_PRESSED_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_ACTION_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_RADIOBUTTON_RADIO_LARGE_ROUND_ON_PRESSED_ID));
    infoPrintfOffButton.setSelected(false);
    infoPrintfOffButton.setDeselectionEnabled(false);
    infoRadioButtonGroup.add(infoPrintfOffButton);
    add(infoPrintfOffButton);
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
    	extiCheckOffButton.setSelected(true);
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
	pidPrintB = printPidOnButton.getSelected();
	infoPrintB = infoPrintfOnButton.getSelected();
	extiCheckB = extiCheckOnButton.getSelected();
	presenter->setDebugData(pidPrintB, infoPrintB, extiCheckB);
	cancelButtonPressed();
}
