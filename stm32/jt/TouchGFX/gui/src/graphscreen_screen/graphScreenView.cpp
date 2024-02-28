#include <gui/graphscreen_screen/graphScreenView.hpp>

graphScreenView* graphScreenView::pInstance= nullptr;

graphScreenView::graphScreenView()
{
	graphScreenView::pInstance = this;
}

void graphScreenView::setupScreen()
{
    graphScreenViewBase::setupScreen();

}

void graphScreenView::tearDownScreen()
{
    graphScreenViewBase::tearDownScreen();
}

void graphScreenView::addData(float ampi, float goalF)
{
	pidAmpsGraph.addDataPoint(ampi);
	pidGoalGraph.addDataPoint(goalF);
}
