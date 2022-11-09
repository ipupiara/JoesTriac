#ifndef CALIBRATIONSCREENPRESENTER_HPP
#define CALIBRATIONSCREENPRESENTER_HPP

//#include <gui/model/ModelListener.hpp>
//#include <mvp/Presenter.hpp>
#include <gui/JtPresenter.hpp>

using namespace touchgfx;

class calibrationScreenView;

class calibrationScreenPresenter : public JtPresenter
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

    virtual void tick();
    void sendTriacDelayCalibCorrection(int32_t val);
    void abortButtonPressed();
    void continueButtonPressed();
    void skipButtonPressed();
    void calibrationScreenReady();
 private:
    calibrationScreenPresenter();

    calibrationScreenView& view;
    uint8_t  presenterActive;
};

#endif // CALIBRATIONSCREENPRESENTER_HPP
