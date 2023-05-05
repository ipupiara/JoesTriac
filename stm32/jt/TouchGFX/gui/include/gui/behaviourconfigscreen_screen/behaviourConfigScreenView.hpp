#ifndef BEHAVIOURCONFIGSCREENVIEW_HPP
#define BEHAVIOURCONFIGSCREENVIEW_HPP

#include <gui_generated/behaviourconfigscreen_screen/behaviourConfigScreenViewBase.hpp>
#include <gui/behaviourconfigscreen_screen/behaviourConfigScreenPresenter.hpp>

class behaviourConfigScreenView : public behaviourConfigScreenViewBase
{
public:
    behaviourConfigScreenView();
    virtual ~behaviourConfigScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // BEHAVIOURCONFIGSCREENVIEW_HPP
