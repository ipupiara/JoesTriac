#include <gui/mainscreen_screen/mainScreenView.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>
#include <mainJt.h>

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

void mainScreenPresenter::configButtonPressed()
{
	CMainJtEventT evt;
	evt.evType = configPressed;
	sendEventToMainJtMessageQ(&evt,0);
}

void mainScreenPresenter::startButtonPressed()
{
	CMainJtEventT evt;
	evt.evType = runButtonClicked;
	sendEventToMainJtMessageQ(&evt,0);
}
