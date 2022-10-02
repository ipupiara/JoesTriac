#ifndef SETAMPERESCREENPRESENTER_HPP
#define SETAMPERESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class setAmpereScreenView;

class setAmpereScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    setAmpereScreenPresenter(setAmpereScreenView& v);

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

    virtual ~setAmpereScreenPresenter() {};

private:
    setAmpereScreenPresenter();

    setAmpereScreenView& view;
};

#endif // SETAMPERESCREENPRESENTER_HPP
