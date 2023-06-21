/*
 * GaugeWithGoal.cpp
 *
 *  Created on: May 7, 2023
 *      Author: diego
 */
#include <gui/GaugeWithGoal.hpp>

#include <gui/mainscreen_screen/mainScreenPresenter.hpp>

	int32_t GaugeWithGoal::cacheInt = 0;

	GaugeWithGoal::GaugeWithGoal()
	{

	}

	GaugeWithGoal::GaugeWithGoal(Gauge aGauge,TextureMapper* apTexture )
	{
		setGauge(aGauge, apTexture);
	}

	GaugeWithGoal::~GaugeWithGoal()
	{

	}

	void GaugeWithGoal::setGauge(Gauge aGauge,TextureMapper* apTexture )
	{
		gauge = aGauge;
		pTexture = apTexture;
	}

	void GaugeWithGoal::setGoalValue(float aValue)
	{
		pTexture->setAngles(0, 0, aValue);
	}

	void GaugeWithGoal::setGoalAngle(float angle)
	{

	}

	void GaugeWithGoal::paintGoalNeedle(float val)   //  todo tobe tested
	{
	//	uint16_t oldVal = ampGauge.getValue();
	//	ampGauge.setNeedle(BITMAP_RED_NEEDLES_GOAL_GAUGE_NEEDLE_STYLE_01_ID, 11, 115);
	//    ampGauge.setValue(val);
	//    ampGauge.setNeedle(BITMAP_BLUE_NEEDLES_ORIGINAL_GAUGE_NEEDLE_STYLE_01_ID, 11, 55);
	//    ampGauge.setValue(oldVal);
	//    //   ampGauge.invalidateContent();
	//        ampGauge.invalidate();
	}


	void GaugeWithGoal::calcCache()
	{
		int32_t startA = 0; // gauge.
		int32_t stopA  = 0;//    ;

	}

