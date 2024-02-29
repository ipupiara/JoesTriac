#ifndef RUNSCREENPRESENTER_HPP
#define RUNSCREENPRESENTER_HPP

//#include <gui/model/ModelListener.hpp>
//#include <mvp/Presenter.hpp>
#include <gui/JtPresenter.hpp>

using namespace touchgfx;

class runScreenView;

class runScreenPresenter : public JtPresenter
{
public:

    runScreenPresenter(runScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~runScreenPresenter() {};

    virtual void tick();

    void stopButtonPressed();
    void continueButtonPressed();
    void abortButtonPressed();


private:
    runScreenPresenter();
    bool presenterActive;

    runScreenView& view;
};

#endif // RUNSCREENPRESENTER_HPP
