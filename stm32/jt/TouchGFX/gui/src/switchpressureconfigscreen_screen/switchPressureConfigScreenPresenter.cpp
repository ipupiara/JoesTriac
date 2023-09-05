#include <gui/switchpressureconfigscreen_screen/switchPressureConfigScreenView.hpp>
#include <gui/switchpressureconfigscreen_screen/switchPressureConfigScreenPresenter.hpp>

switchPressureConfigScreenPresenter::switchPressureConfigScreenPresenter(switchPressureConfigScreenView& v)
    : view(v)
{

}

void switchPressureConfigScreenPresenter::activate()
{

}

void switchPressureConfigScreenPresenter::deactivate()
{

}

uint8_t switchPressureConfigScreenPresenter::getSwitchPressureNeeded()
{
	return model->getSwitchPressureNeeded();
}

uint16_t switchPressureConfigScreenPresenter::getSwitchPressureTime()
{
	return model->getSwitchPressureTime();
}

void  switchPressureConfigScreenPresenter::storeSwitchPressureData(uint32_t spTime, uint8_t spNeeded)
{
	model->storeSwitchPressureData(spTime, spNeeded);
}

void switchPressureConfigScreenPresenter::tick()
{
	// -fno-inline-functions  and no optimization -O0   ! if not set, sends the code to  booby hatch,  dancing wild on the stack :-)
	if (tickCnt > 30) {
		tickCnt = 0;
		view.toggleCursor();
	}
	++tickCnt;
}
