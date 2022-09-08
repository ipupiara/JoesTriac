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
protected:
};

#endif // SETTIMESCREENVIEW_HPP
