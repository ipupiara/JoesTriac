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
    void updatePidGraph(pJoesPresenterEventT  pMsg);
    virtual void stopButtonPressed();
    virtual void astroButtonPressed();
    virtual void graphButtonClicked();
    virtual void continueButtonClicked();
    virtual void abortButtonPressed();

    void raiseRequestStop();
    void raiseDoRun();
    void initPidGraphFromData();
protected:
    void showAstrolabium(bool vis);
    uint32_t weldingTimeSec;
    float     weldingAmps;
    void setActive(touchgfx::Drawable* btn,bool act );
    uint8_t aborting;
};

#endif // RUNSCREENVIEW_HPP
