#include <gui/mainscreen_screen/mainScreenView.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>

mainScreenPresenter::mainScreenPresenter(mainScreenView& v)
    : view(v)
{

}

void mainScreenPresenter::activate()
{

}

void mainScreenPresenter::deactivate()
{

}

uint16_t mainScreenPresenter::getWeldingTimeSec()
{
	return model->getWeldingTimeSec();
}

float mainScreenPresenter::getWeldingAmps()
{
	return model->getWeldingAmps();
}
