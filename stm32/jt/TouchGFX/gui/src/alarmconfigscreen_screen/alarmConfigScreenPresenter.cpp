#include <gui/alarmconfigscreen_screen/alarmConfigScreenView.hpp>
#include <gui/alarmconfigscreen_screen/alarmConfigScreenPresenter.hpp>

alarmConfigScreenPresenter::alarmConfigScreenPresenter(alarmConfigScreenView& v)
    : view(v)
{

}

void alarmConfigScreenPresenter::activate()
{

}

void alarmConfigScreenPresenter::deactivate()
{

}

uint8_t alarmConfigScreenPresenter::getAlarmNeeded()
{

	return model->getAlarmNeeded();
}

uint16_t alarmConfigScreenPresenter::getAlarmTime()
{
	return model->getAlarmTime();
}

//uint32_t alarmConfigScreenPresenter::getZCalibOn()
//{
//	return model->getZCalibOn();
//}

void  alarmConfigScreenPresenter::storeAlarmData(uint32_t aTime, uint8_t aNeeded)
{
	model->storeAlarm(aNeeded, aTime);
}

void alarmConfigScreenPresenter::tick()
{
	// -fno-inline-functions  and no optimization -O0   ! if not set, sends the code to  booby hatch,  dancing wild on the stack :-)
	if (tickCnt > 30) {
		tickCnt = 0;
		view.toggleCursor();
	}
	++tickCnt;
}
