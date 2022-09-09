#include <gui/settimescreen_screen/setTimeScreenView.hpp>

setTimeScreenView::setTimeScreenView()
{

}

void setTimeScreenView::setupScreen()
{
    setTimeScreenViewBase::setupScreen();
}

void setTimeScreenView::tearDownScreen()
{
    setTimeScreenViewBase::tearDownScreen();
}

virtual void saveButtonPressed()
{
    // Override and implement this function in setTimeScreen
}

virtual void cancelButtonPressed()
{
    // Override and implement this function in setTimeScreen
}
