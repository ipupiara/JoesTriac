#ifndef PIDDATAGRAPHCONTAINER_HPP
#define PIDDATAGRAPHCONTAINER_HPP

#include <gui_generated/containers/pidDataGraphContainerBase.hpp>

class pidDataGraphContainer : public pidDataGraphContainerBase
{
public:
    pidDataGraphContainer();
    virtual ~pidDataGraphContainer() {}
    virtual void backButtonPressed();
    virtual void initialize();
    void updateGraph(pJoesPresenterEventT  pMsg );
    void setGoalLine(float goalAmps);
protected:


};

#endif // PIDDATAGRAPHCONTAINER_HPP
