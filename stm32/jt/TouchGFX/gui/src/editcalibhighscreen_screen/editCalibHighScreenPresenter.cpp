#include <gui/editcalibhighscreen_screen/editCalibHighScreenView.hpp>
#include <gui/editcalibhighscreen_screen/editCalibHighScreenPresenter.hpp>

editCalibHighScreenPresenter::editCalibHighScreenPresenter(editCalibHighScreenView& v)
    : view(v)
{

}

void editCalibHighScreenPresenter::activate()
{

}

void editCalibHighScreenPresenter::tick()
{
	// -fno-inline-functions  and no optimization -O0   ! if not set, sends the code to  booby hatch,  dancing wild on the stack :-)
	if (tickCnt > 30) {
		tickCnt = 0;
		view.toggleCursor();
	}
	++tickCnt;
}

void editCalibHighScreenPresenter::deactivate()
{

}
