#ifndef CANDEBUGSCREENVIEW_HPP
#define CANDEBUGSCREENVIEW_HPP

#include <gui_generated/candebugscreen_screen/canDebugScreenViewBase.hpp>
#include <gui/candebugscreen_screen/canDebugScreenPresenter.hpp>

class canDebugScreenView : public canDebugScreenViewBase
{
public:
    canDebugScreenView();
    virtual ~canDebugScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CANDEBUGSCREENVIEW_HPP
