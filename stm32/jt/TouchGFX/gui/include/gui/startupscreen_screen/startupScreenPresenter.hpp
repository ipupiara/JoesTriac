#ifndef STARTUPSCREENPRESENTER_HPP
#define STARTUPSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class startupScreenView;

class startupScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    startupScreenPresenter(startupScreenView& v);

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

    virtual ~startupScreenPresenter() {};

    virtual void tick();

private:
    startupScreenPresenter();

    startupScreenView& view;
};

#endif // STARTUPSCREENPRESENTER_HPP
