#include <gui/calibrationscreen_screen/calibrationScreenView.hpp>
#include <uart-Comms.h>

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

void calibrationScreenView::updateAmpsValues(float adcV, uint32_t adc, uint16_t triacDelay)
{
	Unicode::snprintfFloat(adcVoltageTextBuffer, 6, "%01.3f", adcV);
	adcVoltageText.setWildcard(adcVoltageTextBuffer);
	adcVoltageText.invalidate();

	Unicode::snprintf(adcValueTextBuffer, 5, "%4d", adc);
	adcValueText.setWildcard(adcValueTextBuffer);
	adcValueText.invalidate();

	Unicode::snprintf(delayTextBuffer,5, "%4d", triacDelay);
	delayText.setWildcard(delayTextBuffer);
	delayText.invalidate();
}

//void p_printf(const char *emsg, ...)
//{
//	va_list ap;
//	va_start(ap, emsg);
//	char buffer[100];
//
//	vsnprintf((char *)&buffer, 100,  emsg, ap);
//	info_printf(buffer);
//
//	va_end(ap);
//}
//// causes hardcrash
//void calibrationScreenView::cppvsnprintf(char* buffer,uint32_t maxLen,const char *emsg, ...)
//{
//	va_list ap;
//	va_start(ap, emsg);
//
//
//	vsnprintf((char *)buffer, maxLen,  emsg, ap);
//
//
//	va_end(ap);
//}



void calibrationScreenView::updateTriacDelay(uint32_t val)
{
	Unicode::snprintf(delayTextBuffer,5, "%4d", val);
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
