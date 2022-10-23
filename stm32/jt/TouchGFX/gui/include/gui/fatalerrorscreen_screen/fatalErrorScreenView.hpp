#ifndef FATALERRORSCREENVIEW_HPP
#define FATALERRORSCREENVIEW_HPP

#include <gui_generated/fatalerrorscreen_screen/fatalErrorScreenViewBase.hpp>
#include <gui/fatalerrorscreen_screen/fatalErrorScreenPresenter.hpp>

class fatalErrorScreenView : public fatalErrorScreenViewBase
{
public:
    fatalErrorScreenView();
    virtual ~fatalErrorScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // FATALERRORSCREENVIEW_HPP
