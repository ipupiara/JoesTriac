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

#define pi 3.1415926536   // last number rounded up

class GaugeWithGoal  {

public:
	GaugeWithGoal();
	GaugeWithGoal(Gauge aGauge);
    virtual ~GaugeWithGoal();
    void calcCache();

    void setGauge(Gauge aGauge);
    void setGoalValue(float aValue);
    void paintGoalNeedle(float val);

protected:
    Gauge gauge;
};

#endif /* APPLICATION_USER_GUI_GAUGEWITHGOAL_HPP_ */
