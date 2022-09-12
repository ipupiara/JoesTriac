#ifndef SETTIMESCREENPRESENTER_HPP
#define SETTIMESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class setTimeScreenView;

class setTimeScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    setTimeScreenPresenter(setTimeScreenView& v);

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

    virtual ~setTimeScreenPresenter() {};

    virtual void tick();

    uint16_t getWeldingTimeSec();
    void setWeldingTimeSec(uint16_t  tm);


private:
    setTimeScreenPresenter();
    uint8_t tickCnt;
//    uint16_t sampleCnt;
    uint16_t   debugWeldingTime = 742;

    setTimeScreenView& view;
};

#endif // SETTIMESCREENPRESENTER_HPP
