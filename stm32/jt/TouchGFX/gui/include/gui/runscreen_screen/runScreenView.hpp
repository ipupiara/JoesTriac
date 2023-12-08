#ifndef RUNSCREENVIEW_HPP
#define RUNSCREENVIEW_HPP

#include <gui_generated/runscreen_screen/runScreenViewBase.hpp>
#include <gui/runscreen_screen/runScreenPresenter.hpp>
#include <stdint.h>

class runScreenView : public runScreenViewBase
{
public:
    runScreenView();
    virtual ~runScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void update(float amps,uint32_t secRemain, int32_t adcVal, int32_t triacDelay, float adcVolts  );
    virtual void stopButtonPressed();
protected:
    uint32_t weldingTimeSec;
    float     weldingAmps;
    void setAstroVisible(bool vis);
    void hideAstro();
    void showAstro();
    void toggleAstro();
    void doAstroInvalidate();
    bool astroVisible;
    virtual void astroButtonPressed();
    int32_t triacDelay;
    int32_t adcValue;
    float adcVoltage;
};

#endif // RUNSCREENVIEW_HPP
