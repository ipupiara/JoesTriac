#include <gui/setamperescreen_screen/setAmpereScreenView.hpp>
#include <gui/setamperescreen_screen/setAmpereScreenPresenter.hpp>

setAmpereScreenPresenter::setAmpereScreenPresenter(setAmpereScreenView& v)
    : view(v)
{

}

void setAmpereScreenPresenter::activate()
{

}

void setAmpereScreenPresenter::deactivate()
{

}

void setAmpereScreenPresenter::tick()
{
	// -fno-inline-functions  and no optimization -O0   ! if not set, sends the code to  booby hatch,  dancing wild on the stack :-)
	if (tickCnt > 30) {
		tickCnt = 0;
		view.toggleCursor();
	}
	++tickCnt;
}

void setAmpereScreenPresenter::storeWeldingAmps(float ampsV)
{
	model->storeWeldingAmps(ampsV);
}

float setAmpereScreenPresenter::getWeldingAmps()
{
	float res = 0;
	res = model->getWeldingAmps();
	return res;
}
