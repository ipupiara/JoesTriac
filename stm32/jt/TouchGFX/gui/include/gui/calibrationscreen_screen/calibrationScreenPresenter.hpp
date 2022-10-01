#ifndef CALIBRATIONSCREENPRESENTER_HPP
#define CALIBRATIONSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class calibrationScreenView;

class calibrationScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    calibrationScreenPresenter(calibrationScreenView& v);

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

    virtual ~calibrationScreenPresenter() {};

private:
    calibrationScreenPresenter();

    calibrationScreenView& view;
};

#endif // CALIBRATIONSCREENPRESENTER_HPP
