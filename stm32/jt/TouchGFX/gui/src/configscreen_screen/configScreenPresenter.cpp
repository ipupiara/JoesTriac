#include <gui/configscreen_screen/configScreenView.hpp>
#include <gui/configscreen_screen/configScreenPresenter.hpp>
#include <mainJt.h>

configScreenPresenter::configScreenPresenter(configScreenView& v)
    : view(v)
{

}

void configScreenPresenter::activate()
{

}

void configScreenPresenter::deactivate()
{

}

void configScreenPresenter::backPressed()
{
	CMainJtEventT evnt;
	evnt.evType = configBackPressed;
	sendEventToMainJtMessageQ( &evnt, 0);
}

void configScreenPresenter::autoCalibrationClicked()
{
	CMainJtEventT evnt;
	evnt.evType = autoConfigPressed;
	sendEventToMainJtMessageQ( &evnt, 0);
}

