#include <gui/containers/GaugeWithGoal.hpp>

GaugeWithGoal::GaugeWithGoal()
{

}

void GaugeWithGoal::initialize()
{
    GaugeWithGoalBase::initialize();
}

void GaugeWithGoal::setValue(float val)
{
	valGauge.setValue(val);
}

void GaugeWithGoal::setGoalValue(float val)
{
	goalGauge.setValue(val);
}
