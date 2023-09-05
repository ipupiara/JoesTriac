#ifndef SWITCHPRESSURECONFIGSCREENPRESENTER_HPP
#define SWITCHPRESSURECONFIGSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class switchPressureConfigScreenView;

class switchPressureConfigScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    switchPressureConfigScreenPresenter(switchPressureConfigScreenView& v);

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

    virtual ~switchPressureConfigScreenPresenter() {};

    uint8_t getSwitchPressureNeeded();
    uint16_t getSwitchPressureTime();
    void  storeSwitchPressureData(uint32_t spTime, uint8_t spNeeded);
    virtual void tick();


private:
    switchPressureConfigScreenPresenter();
    uint8_t tickCnt;
    switchPressureConfigScreenView& view;
};

#endif // SWITCHPRESSURECONFIGSCREENPRESENTER_HPP
