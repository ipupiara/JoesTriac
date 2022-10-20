#include <gui/calibratezeroscreen_screen/calibrateZeroScreenView.hpp>

calibrateZeroScreenView::calibrateZeroScreenView()
{

}

void calibrateZeroScreenView::setupScreen()
{
    calibrateZeroScreenViewBase::setupScreen();
}

void calibrateZeroScreenView::tearDownScreen()
{
    calibrateZeroScreenViewBase::tearDownScreen();
}

void calibrateZeroScreenView::abortPressed()
{
	presenter->resetCache();
}

void calibrateZeroScreenView::update(uint32_t pPos, float devV)
{
	 Unicode::snprintf(zeroPotiPosTextBuffer, 4, "%03d", pPos);
	 zeroPotiPosText.setWildcard(zeroPotiPosTextBuffer);
	 zeroPotiPosText.invalidate();

	 Unicode::snprintfFloat(devVoltageTextBuffer, 5, "%+01.2d", pPos);
	 devVoltageText.setWildcard(devVoltageTextBuffer);
	 devVoltageText.invalidate();
}

void calibrateZeroScreenView::updateState(char* txt)
{
	Unicode::snprintf(stateTextBuffer, 19, "%c", txt);
	stateText.setWildcard(stateTextBuffer);
	stateText.invalidate();
}
