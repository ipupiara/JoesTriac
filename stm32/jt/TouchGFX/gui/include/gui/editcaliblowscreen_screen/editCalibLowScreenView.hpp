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

#endif // EDITCALIBLOWSCREENVIEW_HPP
