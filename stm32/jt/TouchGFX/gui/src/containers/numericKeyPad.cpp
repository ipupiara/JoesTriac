#include <gui/containers/numericKeyPad.hpp>

numericKeyPad::numericKeyPad()
{

}

void numericKeyPad::initialize()
{
    numericKeyPadBase::initialize();
}

void numericKeyPad::buttonPressed(uint8_t  btn)
{
	emitNumPressedCallback(btn);
}

void numericKeyPad::setEnable7to0forInput(bool ena)
{
	button7.setTouchable(ena);
	button8.setTouchable(ena);
	button9.setTouchable(ena);
	button10.setTouchable(ena);
}
