#ifndef CALIBRATEZEROSCREENPRESENTER_HPP
#define CALIBRATEZEROSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class calibrateZeroScreenView;

class calibrateZeroScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    calibrateZeroScreenPresenter(calibrateZeroScreenView& v);

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

    virtual ~calibrateZeroScreenPresenter() {};

private:
    calibrateZeroScreenPresenter();

    calibrateZeroScreenView& view;
};

#endif // CALIBRATEZEROSCREENPRESENTER_HPP
