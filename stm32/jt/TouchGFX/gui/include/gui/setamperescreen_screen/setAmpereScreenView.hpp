#ifndef SETAMPERESCREENVIEW_HPP
#define SETAMPERESCREENVIEW_HPP

#include <gui_generated/setamperescreen_screen/setAmpereScreenViewBase.hpp>
#include <gui/setamperescreen_screen/setAmpereScreenPresenter.hpp>

class setAmpereScreenView : public setAmpereScreenViewBase
{
public:
    setAmpereScreenView();
    virtual ~setAmpereScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void numPressed(char ch);
    void toggleCursor();
    virtual void backSaveButtonPressed();
    void buttonPressed(uint8_t val);
    virtual void numButtonPressed(uint8_t value);

    void textClickHandler(const TextAreaWithOneWildcard& txt, const ClickEvent& ev);

protected:

    float     ampsValue;
    void      setValArray(float ampsF);
    void      recalcAmps();
    void      printCurrentValueTimeOnScreen();
    int16_t xOffset;
   uint8_t   valPos;
   uint8_t   valArray[5];

   Callback<setAmpereScreenView, const TextAreaWithOneWildcard&, const ClickEvent&> textClickedCallback;
};

#endif // SETAMPERESCREENVIEW_HPP
