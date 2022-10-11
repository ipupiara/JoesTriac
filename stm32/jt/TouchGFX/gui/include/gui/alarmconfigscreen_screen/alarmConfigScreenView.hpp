#ifndef ALARMCONFIGSCREENVIEW_HPP
#define ALARMCONFIGSCREENVIEW_HPP

#include <gui_generated/alarmconfigscreen_screen/alarmConfigScreenViewBase.hpp>
#include <gui/alarmconfigscreen_screen/alarmConfigScreenPresenter.hpp>

class alarmConfigScreenView : public alarmConfigScreenViewBase
{
public:
    alarmConfigScreenView();
    virtual ~alarmConfigScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void backNSaveButtonPressed();

    virtual void cancelButtonPressed();

protected:
};

#endif // ALARMCONFIGSCREENVIEW_HPP
