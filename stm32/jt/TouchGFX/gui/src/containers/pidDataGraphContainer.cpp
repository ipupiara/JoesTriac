#include <gui/containers/pidDataGraphContainer.hpp>

pidDataGraphContainer::pidDataGraphContainer()
{

}

void pidDataGraphContainer::updateGraph(pJoesPresenterEventT  pMsg )
{
	pidGraph.addDataPoint(pMsg->evData.pidGraphData.ampsF);
	pidGraph.invalidate();
	uint16_t graphHeight =  pidGraph.getGraphAreaHeight();
	uint16_t goalHeight  = graphHeight * ((120 -  pMsg->evData.pidGraphData.goalF) / 120);
	goalLine.setLine((uint16_t) 0,goalHeight ,(uint16_t) 767, goalHeight);
	goalLine.invalidate();


}

void pidDataGraphContainer::initialize()
{
    pidDataGraphContainerBase::initialize();
}

void pidDataGraphContainer::setGoalLine(float goalAmps)
{
	uint16_t graphHeight =  pidGraph.getGraphAreaHeight();
	uint16_t goalHeight  = graphHeight * ((120 - goalAmps) / 120);
	goalLine.setLine((uint16_t) 0,goalHeight ,(uint16_t) 767, goalHeight);
	goalLine.invalidate();
//	    {
//	        setStart(startX, startY);
//	        setEnd(endX, endY);
//	    }
}


void pidDataGraphContainer::backButtonPressed()
 {
     setVisible(false);
     invalidate();
 }
