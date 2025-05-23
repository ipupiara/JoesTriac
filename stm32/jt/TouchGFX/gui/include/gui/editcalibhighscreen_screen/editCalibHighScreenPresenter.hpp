#ifndef EDITCALIBHIGHSCREENPRESENTER_HPP
#define EDITCALIBHIGHSCREENPRESENTER_HPP

//#include <gui/model/ModelListener.hpp>
//#include <mvp/Presenter.hpp>
#include <gui/JtPresenter.hpp>

using namespace touchgfx;

class editCalibHighScreenView;

class editCalibHighScreenPresenter : public JtPresenter  //, public ModelListener
{
public:
    editCalibHighScreenPresenter(editCalibHighScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    virtual void tick();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~editCalibHighScreenPresenter() {};


private:
    editCalibHighScreenPresenter();
    uint8_t tickCnt;

    editCalibHighScreenView& view;
};

#endif // EDITCALIBHIGHSCREENPRESENTER_HPP
