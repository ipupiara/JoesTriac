#ifndef MAINSCREENVIEW_HPP
#define MAINSCREENVIEW_HPP

#include <gui_generated/mainscreen_screen/mainScreenViewBase.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>
#include <gui/GaugeWithGoal.hpp>

class mainScreenView : public mainScreenViewBase
{
public:
    mainScreenView();
    virtual ~mainScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void configButtonPressed();
    virtual void startButtonPressed();
protected:
    void paintGoalNeedle(int16_t val);
    GaugeWithGoal gaugeWithGoal;
};

#endif // MAINSCREENVIEW_HPP
