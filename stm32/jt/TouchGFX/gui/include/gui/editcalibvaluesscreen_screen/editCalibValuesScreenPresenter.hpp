#ifndef EDITCALIBVALUESSCREENPRESENTER_HPP
#define EDITCALIBVALUESSCREENPRESENTER_HPP

//#include <gui/model/ModelListener.hpp>
//#include <mvp/Presenter.hpp>
#include <gui/JtPresenter.hpp>

using namespace touchgfx;

class editCalibValuesScreenView;

class editCalibValuesScreenPresenter : public JtPresenter
{
public:
    editCalibValuesScreenPresenter(editCalibValuesScreenView& v);

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

    virtual ~editCalibValuesScreenPresenter() {};

private:
    editCalibValuesScreenPresenter();

    editCalibValuesScreenView& view;
};

#endif // EDITCALIBVALUESSCREENPRESENTER_HPP
