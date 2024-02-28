#ifndef GRAPHSCREENPRESENTER_HPP
#define GRAPHSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class graphScreenView;

class graphScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    graphScreenPresenter(graphScreenView& v);

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

    virtual ~graphScreenPresenter() {}

private:
    graphScreenPresenter();

    graphScreenView& view;
};

#endif // GRAPHSCREENPRESENTER_HPP
