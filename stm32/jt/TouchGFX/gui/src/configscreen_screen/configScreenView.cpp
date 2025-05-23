#include <gui/configscreen_screen/configScreenView.hpp>
#include <mainJt.h>

configScreenView::configScreenView()
{

}

void configScreenView::setupScreen()
{
    configScreenViewBase::setupScreen();
    if (isCalibrationReady() != tOk) {
    	alarmText.setVisible(true);
    	backButton.setVisible(false);
    }  else {
    	alarmText.setVisible(false);
    }
    alarmText.invalidate();
}

void configScreenView::tearDownScreen()
{
    configScreenViewBase::tearDownScreen();
}

void configScreenView::backPressed()
{
	presenter->backPressed();
}

void configScreenView::autoCalibrationClicked()
{
	presenter->autoCalibrationClicked();
}

