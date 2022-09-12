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

uint16_t setTimeScreenPresenter::getWeldingTime()
{
	return model->getWeldingTime();
}

void setTimeScreenPresenter::setWeldingTime(uint16_t  tm)
{
	model->setWeldingTime(tm);
}

void setTimeScreenPresenter::tick()
{
	if (tickCnt > 30) {
		tickCnt = 0;
		view.toggleCursor();
	}
	++tickCnt;
}

