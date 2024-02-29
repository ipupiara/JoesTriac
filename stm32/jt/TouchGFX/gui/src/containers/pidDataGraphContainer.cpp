#include <gui/containers/pidDataGraphContainer.hpp>

pidDataGraphContainer::pidDataGraphContainer()
{

}

void pidDataGraphContainer::updateGraph(pJoesPresenterEventT  pMsg )
{
	pidGraph.addDataPoint(pMsg->evData.pidGraphData.ampsF);
	invalidate();
}

void pidDataGraphContainer::initialize()
{
    pidDataGraphContainerBase::initialize();
}


void pidDataGraphContainer::backButtonPressed()
 {
     setVisible(false);
     invalidate();
 }
