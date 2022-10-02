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
protected:
};

#endif // SETAMPERESCREENVIEW_HPP
