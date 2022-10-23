#ifndef EDITCALIBLOWSCREENPRESENTER_HPP
#define EDITCALIBLOWSCREENPRESENTER_HPP

//#include <gui/model/ModelListener.hpp>
//#include <mvp/Presenter.hpp>
#include <gui/JtPresenter.hpp>

using namespace touchgfx;

class editCalibLowScreenView;

class editCalibLowScreenPresenter : public JtPresenter //, public ModelListener
{
public:
    editCalibLowScreenPresenter(editCalibLowScreenView& v);

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

    virtual ~editCalibLowScreenPresenter() {};


private:
    editCalibLowScreenPresenter();

    editCalibLowScreenView& view;
};

#endif // EDITCALIBLOWSCREENPRESENTER_HPP
