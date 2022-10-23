#ifndef DUMMYSCREENVIEW_HPP
#define DUMMYSCREENVIEW_HPP

#include <gui_generated/dummyscreen_screen/dummyScreenViewBase.hpp>
#include <gui/dummyscreen_screen/dummyScreenPresenter.hpp>

class dummyScreenView : public dummyScreenViewBase
{
public:
    dummyScreenView();
    virtual ~dummyScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // DUMMYSCREENVIEW_HPP
