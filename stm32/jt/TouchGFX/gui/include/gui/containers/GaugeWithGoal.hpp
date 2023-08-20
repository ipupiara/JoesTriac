#ifndef GAUGEWITHGOAL_HPP
#define GAUGEWITHGOAL_HPP

#include <gui_generated/containers/GaugeWithGoalBase.hpp>

class GaugeWithGoal : public GaugeWithGoalBase
{
public:
    GaugeWithGoal();
    virtual ~GaugeWithGoal() {}

    void setValue(float val);
    void setGoalValue(float val);

    virtual void initialize();
protected:
};

#endif // GAUGEWITHGOAL_HPP
