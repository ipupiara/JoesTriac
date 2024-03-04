#ifndef CONFIGDEBUGSCREENPRESENTER_HPP
#define CONFIGDEBUGSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class configDebugScreenView;

class configDebugScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    configDebugScreenPresenter(configDebugScreenView& v);

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

    virtual ~configDebugScreenPresenter() {}

private:
    configDebugScreenPresenter();

    configDebugScreenView& view;
};

#endif // CONFIGDEBUGSCREENPRESENTER_HPP
