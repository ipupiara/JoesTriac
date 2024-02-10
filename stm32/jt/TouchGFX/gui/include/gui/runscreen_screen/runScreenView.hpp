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
    void update(pJoesPresenterEventT  pMsg);
    virtual void stopButtonPressed();
    virtual void astroButtonPressed();
protected:
    void showAstrolabium(bool vis);
    uint32_t weldingTimeSec;
    float     weldingAmps;
};

#endif // RUNSCREENVIEW_HPP
