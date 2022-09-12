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
protected:
    void goBack();
    uint8_t   valPos = 0;
    uint8_t   valArray[4];
    uint16_t  currentTimeValue;
    void      setValArrayTime(uint16_t val);
    void      charPressedRecalcDataTime(uint8_t val, uint8_t pos);
    void      printCurrentValueTimeOn(touchgfx::TextAreaWithOneWildcard* textA);
};

#endif // SETTIMESCREENVIEW_HPP
