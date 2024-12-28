#ifndef CANDEBUGSELECTCONTA_HPP
#define CANDEBUGSELECTCONTA_HPP

#include <gui_generated/containers/canDebugSelectContainerBase.hpp>
#include <canComms.h>

class canDebugSelectContainer : public canDebugSelectContainerBase
{
public:
    canDebugSelectContainer();
    virtual ~canDebugSelectContainer() {}

    virtual void initialize();


protected:
};

#endif // CANDEBUGSELECTCONTA_HPP
