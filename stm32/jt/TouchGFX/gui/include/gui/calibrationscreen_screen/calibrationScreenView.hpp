#ifndef CALIBRATIONSCREENVIEW_HPP
#define CALIBRATIONSCREENVIEW_HPP

#include <gui_generated/calibrationscreen_screen/calibrationScreenViewBase.hpp>
#include <gui/calibrationscreen_screen/calibrationScreenPresenter.hpp>

class calibrationScreenView : public calibrationScreenViewBase
{
public:
    calibrationScreenView();
    virtual ~calibrationScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void update(float amps, uint32_t secondsRemaining);
protected:
};

#endif // CALIBRATIONSCREENVIEW_HPP
