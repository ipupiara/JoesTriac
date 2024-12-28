#ifndef CANDEBUGSELECTCONTAINER_HPP
#define CANDEBUGSELECTCONTAINER_HPP

#include <gui_generated/containers/canDebugSelectContainerBase.hpp>
#include <mainJt.h>

class canDebugSelectContainer : public canDebugSelectContainerBase
{
public:
    canDebugSelectContainer();
    virtual ~canDebugSelectContainer() {}

    virtual void initialize();

    canHosts canHost;
    void setHostType(canHosts cHost);

    void setSelection(canHosts cHost ,canTestTypes canTestType);
    virtual void nonButtonSelected();
    virtual void singleButtonSelected();
    virtual void continuousButtonSelected();
    void sendSelection(canHosts cHost ,canTestTypes canTestType);
protected:
};

#endif // CANDEBUGSELECTCONTAINER_HPP
