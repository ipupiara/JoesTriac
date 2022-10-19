#ifndef CALIBRATEZEROSCREENPRESENTER_HPP
#define CALIBRATEZEROSCREENPRESENTER_HPP

//#include <gui/model/ModelListener.hpp>
//#include <mvp/Presenter.hpp>
#include <gui/JtPresenter.hpp>

using namespace touchgfx;

class calibrateZeroScreenView;

class calibrateZeroScreenPresenter : public JtPresenter
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

    virtual void tick();

private:
    calibrateZeroScreenPresenter();

    calibrateZeroScreenView& view;
    uint8_t  presenterActive;
};

#endif // CALIBRATEZEROSCREENPRESENTER_HPP
