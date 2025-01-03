#ifndef REQUESTSTOPSCREENPRESENTER_HPP
#define REQUESTSTOPSCREENPRESENTER_HPP

//#include <gui/model/ModelListener.hpp>
//#include <mvp/Presenter.hpp>
#include <gui/JtPresenter.hpp>

using namespace touchgfx;

class requestStopScreenView;

class requestStopScreenPresenter : public JtPresenter
{
public:
    requestStopScreenPresenter(requestStopScreenView& v);

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

    virtual ~requestStopScreenPresenter() {};

    virtual void tick();

    void continueButtonPressed();
    void abortButtonPressed();

private:
    requestStopScreenPresenter();
    bool presenterActive;

    requestStopScreenView& view;
};

#endif // REQUESTSTOPSCREENPRESENTER_HPP
