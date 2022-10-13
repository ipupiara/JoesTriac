#ifndef EDITCALIBVALUESSCREENVIEW_HPP
#define EDITCALIBVALUESSCREENVIEW_HPP

#include <gui_generated/editcalibvaluesscreen_screen/editCalibValuesScreenViewBase.hpp>
#include <gui/editcalibvaluesscreen_screen/editCalibValuesScreenPresenter.hpp>

class editCalibValuesScreenView : public editCalibValuesScreenViewBase
{
public:
    editCalibValuesScreenView();
    virtual ~editCalibValuesScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
    uint32_t CLow;
    uint32_t CHigh;
};

#endif // EDITCALIBVALUESSCREENVIEW_HPP
