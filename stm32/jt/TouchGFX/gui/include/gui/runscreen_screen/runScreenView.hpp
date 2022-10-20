#ifndef RUNSCREENVIEW_HPP
#define RUNSCREENVIEW_HPP

#include <gui_generated/runscreen_screen/runScreenViewBase.hpp>
#include <gui/runscreen_screen/runScreenPresenter.hpp>

class runScreenView : public runScreenViewBase
{
public:
    runScreenView();
    virtual ~runScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void update(float amps,uint32_t secRemain,uint32_t pPos );
protected:
};

#endif // RUNSCREENVIEW_HPP
