#include <gui/configdebugscreen_screen/configDebugScreenView.hpp>
#include <gui/configdebugscreen_screen/configDebugScreenPresenter.hpp>

configDebugScreenPresenter::configDebugScreenPresenter(configDebugScreenView& v)
    : view(v)
{

}

void configDebugScreenPresenter::activate()
{

}

void configDebugScreenPresenter::deactivate()
{

}


void configDebugScreenPresenter::getDebugData(uint8_t* pidP, uint8_t* infoP, uint8_t* exti)
{
	model->getDebugData(pidP, infoP, exti);
}


void configDebugScreenPresenter::setDebugData(uint8_t  pidP, uint8_t infoP, uint8_t exti)
{
	model->setDebugData(pidP, infoP, exti);
}
