#ifndef EDITCALIBLOWSCREENVIEW_HPP
#define EDITCALIBLOWSCREENVIEW_HPP

#include <gui_generated/editcaliblowscreen_screen/editCalibLowScreenViewBase.hpp>
#include <gui/editcaliblowscreen_screen/editCalibLowScreenPresenter.hpp>

class editCalibLowScreenView : public editCalibLowScreenViewBase
{
public:
    editCalibLowScreenView();
    virtual ~editCalibLowScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // EDITCALIBLOWSCREENVIEW_HPP
