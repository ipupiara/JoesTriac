#ifndef CONFIGSCREENVIEW_HPP
#define CONFIGSCREENVIEW_HPP

#include <gui_generated/configscreen_screen/configScreenViewBase.hpp>
#include <gui/configscreen_screen/configScreenPresenter.hpp>

class configScreenView : public configScreenViewBase
{
public:
    configScreenView();
    virtual ~configScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void backPressed();
    virtual void autoCalibrationClicked();

protected:
};

#endif // CONFIGSCREENVIEW_HPP
