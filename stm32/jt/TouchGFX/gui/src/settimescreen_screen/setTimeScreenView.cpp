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

void setTimeScreenView::backSaveButtonPressed()
{
	Application::getInstance()->changeToStartScreen();
}



void setTimeScreenView::toggleCursor()
{
	if (cursor.isVisible()) {
		cursor.setVisible(false);
	}  else {
		cursor.setVisible(true);
	}

}
