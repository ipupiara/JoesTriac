/*
 * JtPresenter.cpp
 *
 *  Created on: Oct 13, 2022
 *      Author: diego
 */

#include <gui/JtPresenter.hpp>

JtPresenter::JtPresenter() {


}

JtPresenter::~JtPresenter() {

}

uint32_t JtPresenter::getCalibLow()
{
	return model->getCalibLow();
}

uint32_t JtPresenter::getCalibHigh()
{
	return model->getCalibHigh();
}

void JtPresenter::setCalibHigh(uint32_t cH)
{
	model->setCalibHigh(cH);
}
void JtPresenter::setCalibLow(uint32_t cL)
{
	model->setCalibLow(cL);
}

void   JtPresenter::saveCalibValues()
{
	model->saveCalibValues();
}

void   JtPresenter::resetCalibValues()
{
	model->resetCalibValues();
}



