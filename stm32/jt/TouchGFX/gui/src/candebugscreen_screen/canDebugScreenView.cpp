#include <gui/candebugscreen_screen/canDebugScreenView.hpp>

canDebugScreenView::canDebugScreenView()
{

}

void canDebugScreenView::setupScreen()
{
    canDebugScreenViewBase::setupScreen();

    triacDebugSelection.setHostType(triacHost);
    ammeterDebugSelection.setHostType(ammeterHost);

    triacDebugSelection.setSelection(triacHost ,noCanTest);
    ammeterDebugSelection.setSelection(ammeterHost ,noCanTest);

}

void canDebugScreenView::tearDownScreen()
{
    canDebugScreenViewBase::tearDownScreen();
}
