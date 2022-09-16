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

void numericKeyPad::setEnable7to9forInput(bool ena)
{
	button6.setVisible(ena);
	button6.invalidate();
	button7.setVisible(ena);
	button7.invalidate();
	button8.setVisible(ena);
	button8.invalidate();
	button9.setVisible(ena);
	button9.invalidate();

}
