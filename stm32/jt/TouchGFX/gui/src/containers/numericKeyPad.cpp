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

