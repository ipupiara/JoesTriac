#ifndef SWITCHPRESSURECONFIGSCREENVIEW_HPP
#define SWITCHPRESSURECONFIGSCREENVIEW_HPP

#include <gui_generated/switchpressureconfigscreen_screen/switchPressureConfigScreenViewBase.hpp>
#include <gui/switchpressureconfigscreen_screen/switchPressureConfigScreenPresenter.hpp>

class switchPressureConfigScreenView : public switchPressureConfigScreenViewBase
{
public:
    switchPressureConfigScreenView();
    virtual ~switchPressureConfigScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void cancelButtonPressed();
    virtual void backNSaveButtonPressed();
    virtual void numPressed(uint8_t value);
    void toggleCursor();
protected:
    void buttonPressed(uint8_t val);

    uint16_t     switchPressureTimeMin;
      uint8_t    switchPressureNeeded;
      void      setValArray(uint16_t val);
      void      recalcPressureSwitchTime();
      void      printCurrentTimeValueOnScreen();
     uint8_t   valPos = 0;
     uint8_t   valArray[2];
};

#endif // SWITCHPRESSURECONFIGSCREENVIEW_HPP
