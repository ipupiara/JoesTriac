#ifndef CONFIGDEBUGSCREENVIEW_HPP
#define CONFIGDEBUGSCREENVIEW_HPP

#include <gui_generated/configdebugscreen_screen/configDebugScreenViewBase.hpp>
#include <gui/configdebugscreen_screen/configDebugScreenPresenter.hpp>

class configDebugScreenView : public configDebugScreenViewBase
{
public:
    configDebugScreenView();
    virtual ~configDebugScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CONFIGDEBUGSCREENVIEW_HPP
