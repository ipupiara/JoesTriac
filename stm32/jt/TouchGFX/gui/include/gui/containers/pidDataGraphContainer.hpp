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
//    void initNew(graphDataRec* pData);
    void initFromData(pJoesPresenterEventT  pMsg );
protected:
    void setOriginalScreen(Screen scr);
    static Screen originalScreen;

};

#endif // PIDDATAGRAPHCONTAINER_HPP