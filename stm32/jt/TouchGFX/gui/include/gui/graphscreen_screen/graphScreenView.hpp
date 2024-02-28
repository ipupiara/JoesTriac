#ifndef GRAPHSCREENVIEW_HPP
#define GRAPHSCREENVIEW_HPP

#include <gui_generated/graphscreen_screen/graphScreenViewBase.hpp>
#include <gui/graphscreen_screen/graphScreenPresenter.hpp>

class graphScreenView : public graphScreenViewBase
{
public:
    graphScreenView();
    virtual ~graphScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    static graphScreenView* pInstance;
    void addData(float ampi, float goalF );
protected:
};

#endif // GRAPHSCREENVIEW_HPP
