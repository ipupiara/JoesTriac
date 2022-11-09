#include <gui/calibrationscreen_screen/calibrationScreenView.hpp>

calibrationScreenView::calibrationScreenView()
{

}

void calibrationScreenView::setupScreen()
{
    calibrationScreenViewBase::setupScreen();
    presenter->calibrationScreenReady();
}

void calibrationScreenView::tearDownScreen()
{
    calibrationScreenViewBase::tearDownScreen();
}

void calibrationScreenView::updateAmpsValues(float adcV, uint32_t adc)
{
	Unicode::snprintfFloat(adcVoltageTextBuffer, 6, "%01.3f", adcV);
	adcVoltageText.setWildcard(adcVoltageTextBuffer);
	adcVoltageText.invalidate();

	Unicode::snprintf(adcValueTextBuffer, 5, "%04d", adc);
	adcValueText.setWildcard(adcValueTextBuffer);
	adcValueText.invalidate();
}

void calibrationScreenView::updateTriacDelay(uint32_t val)
{
	Unicode::snprintf(delayTextBuffer,5, "%04d", val);
	delayText.setWildcard(delayTextBuffer);
	delayText.invalidate();

}

void calibrationScreenView::updateDesiredAmps(float amps)
{
	Unicode::snprintfFloat(ampereTextBuffer, 4, "%03.f", amps);
	ampereText.setWildcard(ampereTextBuffer);
	ampereText.invalidate();
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
