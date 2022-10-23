#include <gui/calibrationscreen_screen/calibrationScreenView.hpp>

calibrationScreenView::calibrationScreenView()
{

}

void calibrationScreenView::setupScreen()
{
    calibrationScreenViewBase::setupScreen();
}

void calibrationScreenView::tearDownScreen()
{
    calibrationScreenViewBase::tearDownScreen();
}

void calibrationScreenView::updateAmpsValues(float amps, uint32_t adc)
{

}

void calibrationScreenView::updateTriacDelay(uint32_t val)
{

}

void calibrationScreenView::updateDesiredAmps(float amps)
{

}

void calibrationScreenView::sendTriacDelayCalibCorrection(int32_t val)
{
	presenter->sendTriacDelayCalibCorrection(val);
}

void calibrationScreenView::abortButtonPressed()
{
	presenter->abortButtonPressed();
}

void calibrationScreenView::continueButtonPressed()
{
	presenter->continueButtonPressed();
}

void calibrationScreenView::skipButtonPressed()
{
     presenter->skipButtonPressed();
}
