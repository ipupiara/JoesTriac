#ifndef RUNSCREENPRESENTER_HPP
#define RUNSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>


using namespace touchgfx;

class runScreenView;

class runScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:

//	   typedef enum {
//	    	changeToRunScreen = 0,
//			changeToMainScreen,
//			secondUpdate
//
//	    }  runScreenPresenterMessageType;
//
//	    typedef struct  {
//	    	runScreenPresenterMessageType messageType;
//	    	union {
//	    		uint32_t keyCode;
//	    		struct {
//	    			float   voltage;
//	    			uint16_t  potiPos;
//	    		} secondRunData;
//	    	}  evData;
//	    } CJoesRunScreenPresenterEventT ;
//
//	    typedef CJoesRunScreenPresenterEventT*  pJoesRunScreenPresenterEventT;
//
//	    osMessageQueueId_t    presenterScreenMessageQ;

    runScreenPresenter(runScreenView& v);

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

    virtual ~runScreenPresenter() {};

    virtual void tick();


private:
    runScreenPresenter();
    bool presenterActive;

    runScreenView& view;
};

#endif // RUNSCREENPRESENTER_HPP
