#ifndef EDITCALIBHIGHSCREENVIEW_HPP
#define EDITCALIBHIGHSCREENVIEW_HPP

#include <gui_generated/editcalibhighscreen_screen/editCalibHighScreenViewBase.hpp>
#include <gui/editcalibhighscreen_screen/editCalibHighScreenPresenter.hpp>

class editCalibHighScreenView : public editCalibHighScreenViewBase
{
public:
    editCalibHighScreenView();
    virtual ~editCalibHighScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // EDITCALIBHIGHSCREENVIEW_HPP
