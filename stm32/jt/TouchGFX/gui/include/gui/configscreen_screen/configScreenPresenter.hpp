#ifndef CONFIGSCREENPRESENTER_HPP
#define CONFIGSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class configScreenView;

class configScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    configScreenPresenter(configScreenView& v);

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

    virtual ~configScreenPresenter() {};

private:
    configScreenPresenter();

    configScreenView& view;
};

#endif // CONFIGSCREENPRESENTER_HPP
