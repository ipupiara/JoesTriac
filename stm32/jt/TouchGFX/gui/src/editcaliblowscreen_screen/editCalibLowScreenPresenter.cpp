#include <gui/editcaliblowscreen_screen/editCalibLowScreenView.hpp>
#include <gui/editcaliblowscreen_screen/editCalibLowScreenPresenter.hpp>

editCalibLowScreenPresenter::editCalibLowScreenPresenter(editCalibLowScreenView& v)
    : view(v)
{

}

void editCalibLowScreenPresenter::activate()
{

}

void editCalibLowScreenPresenter::tick()
{
	// -fno-inline-functions  and no optimization -O0   ! if not set, sends the code to  booby hatch,  dancing wild on the stack :-)
	if (tickCnt > 30) {
		tickCnt = 0;
		view.toggleCursor();
	}
	++tickCnt;
}

void editCalibLowScreenPresenter::deactivate()
{

}
