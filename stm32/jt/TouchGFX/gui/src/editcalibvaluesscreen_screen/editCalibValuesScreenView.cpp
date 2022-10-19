#include <gui/editcalibvaluesscreen_screen/editCalibValuesScreenView.hpp>

editCalibValuesScreenView::editCalibValuesScreenView()
{

}

void editCalibValuesScreenView::setupScreen()
{
    editCalibValuesScreenViewBase::setupScreen();
    printCurrentCalibValuesOnScreen();
}

void editCalibValuesScreenView::tearDownScreen()
{
    editCalibValuesScreenViewBase::tearDownScreen();
}

void editCalibValuesScreenView::printCurrentCalibValuesOnScreen()
{
	 uint32_t  cHigh = presenter->getCalibHigh();
	 uint32_t  cLow  = presenter->getCalibLow();

	 Unicode::snprintf(calibLowTextBuffer, 5, "%04d", cLow);
	 calibLowText.setWildcard(calibLowTextBuffer);
	 calibLowText.invalidate();

	 Unicode::snprintf(calibHighTextBuffer, 5, "%04d", cHigh);
	 calibHighText.setWildcard(calibHighTextBuffer);
	 calibHighText.invalidate();
}

void editCalibValuesScreenView::backNSaveButtonPressed()
{
	presenter->saveCalibValues();
	static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoconfigScreenScreenNoTransition();
}

 void editCalibValuesScreenView::cancelPressed()
 {
	 presenter->resetCache();
	 static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoconfigScreenScreenNoTransition();
 }
