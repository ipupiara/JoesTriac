#ifndef ALARMCONFIGSCREENPRESENTER_HPP
#define ALARMCONFIGSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class alarmConfigScreenView;

class alarmConfigScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    alarmConfigScreenPresenter(alarmConfigScreenView& v);

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

    virtual ~alarmConfigScreenPresenter() {};

private:
    alarmConfigScreenPresenter();

    alarmConfigScreenView& view;
};

#endif // ALARMCONFIGSCREENPRESENTER_HPP
