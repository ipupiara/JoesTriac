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

void setTimeScreenPresenter::setWeldingTimeSec(uint16_t  tm)
{
	model->setWeldingTimeSec(tm);
}

void setTimeScreenPresenter::tick()
{
	// -fno-inline-functions
	if (tickCnt > 30) {
		tickCnt = 0;
		view.toggleCursor();
//		view.toggleTouchable();
	}
	++tickCnt;
}

