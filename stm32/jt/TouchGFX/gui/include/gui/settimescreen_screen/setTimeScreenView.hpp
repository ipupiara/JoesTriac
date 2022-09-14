#ifndef SETTIMESCREENVIEW_HPP
#define SETTIMESCREENVIEW_HPP

#include <gui_generated/settimescreen_screen/setTimeScreenViewBase.hpp>
#include <gui/settimescreen_screen/setTimeScreenPresenter.hpp>

class setTimeScreenView : public setTimeScreenViewBase
{
public:
    setTimeScreenView();
    virtual ~setTimeScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void numPressed(char ch);
    void toggleCursor();
    virtual void backSaveButtonPressed();
    void buttonPressed(uint8_t val);
    virtual void numButtonPressed(uint8_t value);

protected:
    void goBack();

    uint16_t  timeValueSec;
    void      setValArray(uint16_t val);
    void      recalcTimeSec();
    void      printCurrentValueTimeOnScreen();
   uint8_t   valPos = 0;
   uint8_t   valArray[4];
};

#endif // SETTIMESCREENVIEW_HPP
