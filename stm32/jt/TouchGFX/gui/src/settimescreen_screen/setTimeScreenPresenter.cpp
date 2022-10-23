#include <gui/settimescreen_screen/setTimeScreenView.hpp>
#include <gui/settimescreen_screen/setTimeScreenPresenter.hpp>

setTimeScreenPresenter::setTimeScreenPresenter(setTimeScreenView& v)
    : view(v)
{

}

void setTimeScreenPresenter::activate()
{

}

void setTimeScreenPresenter::deactivate()
{

}

uint16_t setTimeScreenPresenter::getWeldingTimeSec()
{
	return model->getWeldingTimeSec();
}

void setTimeScreenPresenter::setTimeScreenPresenter::tick()
{
	// -fno-inline-functions  and no optimization -O0   ! if not set, sends the code to  booby hatch,  dancing wild on the stack :-)
	if (tickCnt > 30) {
		tickCnt = 0;
		view.toggleCursor();
	}
	++tickCnt;
}

void setTimeScreenPresenter::storeWeldingTimeSec(uint16_t  tm)
{
	model->storeWeldingTimeSec(tm);
}


