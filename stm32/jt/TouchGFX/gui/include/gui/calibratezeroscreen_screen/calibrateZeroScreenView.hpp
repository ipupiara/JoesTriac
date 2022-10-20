#ifndef CALIBRATEZEROSCREENVIEW_HPP
#define CALIBRATEZEROSCREENVIEW_HPP

#include <gui_generated/calibratezeroscreen_screen/calibrateZeroScreenViewBase.hpp>
#include <gui/calibratezeroscreen_screen/calibrateZeroScreenPresenter.hpp>

class calibrateZeroScreenView : public calibrateZeroScreenViewBase
{
public:
    calibrateZeroScreenView();
    virtual ~calibrateZeroScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void abortPressed();
    void update(uint32_t pPos, float devV);
    void updateState(char* txt);
    virtual void autoButtonOn();
    virtual void autoButtonOff();

protected:
};

#endif // CALIBRATEZEROSCREENVIEW_HPP
