#ifndef CANDEBUGSCREENPRESENTER_HPP
#define CANDEBUGSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class canDebugScreenView;

class canDebugScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    canDebugScreenPresenter(canDebugScreenView& v);

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

    virtual ~canDebugScreenPresenter() {}

private:
    canDebugScreenPresenter();

    canDebugScreenView& view;
};

#endif // CANDEBUGSCREENPRESENTER_HPP
