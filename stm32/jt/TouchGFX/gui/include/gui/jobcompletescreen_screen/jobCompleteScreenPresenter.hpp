#ifndef JOBCOMPLETESCREENPRESENTER_HPP
#define JOBCOMPLETESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class jobCompleteScreenView;

class jobCompleteScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    jobCompleteScreenPresenter(jobCompleteScreenView& v);

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

    virtual ~jobCompleteScreenPresenter() {};

private:
    jobCompleteScreenPresenter();

    jobCompleteScreenView& view;
};

#endif // JOBCOMPLETESCREENPRESENTER_HPP
