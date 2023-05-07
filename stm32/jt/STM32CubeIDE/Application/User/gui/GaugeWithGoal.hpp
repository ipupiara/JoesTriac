/*
 * GaugeWithGoal.hpp
 *
 *  Created on: May 3, 2023
 *      Author: diego
 */

#ifndef APPLICATION_USER_GUI_GAUGEWITHGOAL_HPP_
#define APPLICATION_USER_GUI_GAUGEWITHGOAL_HPP_

#include <math.h>

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/widgets/Gauge.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>

#define pi 3.1415926536   // last digit rounded up

class GaugeWithGoal  {

public:
	GaugeWithGoal();
	GaugeWithGoal(Gauge aGauge,TextureMapper* apTexture );
    virtual ~GaugeWithGoal();
    void calcCache();

    void setGauge(Gauge aGauge,TextureMapper* apTexture );
    void setGoalValue(float aValue);
    void paintGoalNeedle(float val);

protected:
    void setGoalAngle(float angle);
    static int32_t cacheInt;
    Gauge gauge;
    TextureMapper* pTexture;
};

#endif /* APPLICATION_USER_GUI_GAUGEWITHGOAL_HPP_ */
