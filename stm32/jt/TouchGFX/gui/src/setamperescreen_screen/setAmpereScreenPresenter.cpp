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
