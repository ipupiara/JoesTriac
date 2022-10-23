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
    virtual void buttonPressed(uint8_t val);
protected:
    void      printCurrentCalibValuesOnScreen(bool first);
    uint8_t   valPos;
    uint8_t   valArray[4];
    uint16_t  calibVal;
    void      setValArray(uint16_t val);
    void      recalcVal();
    void      printCurrentValuesTimeOnScreen();
    void	toggleCursor();
};

#endif // EDITCALIBHIGHSCREENVIEW_HPP
