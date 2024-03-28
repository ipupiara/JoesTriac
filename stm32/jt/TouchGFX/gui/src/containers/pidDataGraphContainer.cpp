#include <gui/containers/pidDataGraphContainer.hpp>
#include <touchgfx/Color.hpp>
#include <mainJt.h>


static Screen originalScreen;

void  pidDataGraphContainer::setOriginalScreen(Screen scr)
{
	originalScreen = scr;
	pGraphRec = NULL;
	zoomState = zoomNormal;
}


pidDataGraphContainer::pidDataGraphContainer()
{
	pidDataGraphContainerBase();
}

void pidDataGraphContainer::updateGraph(pJoesPresenterEventT  pMsg )
{
	pGraphRec =  pMsg->evData.pidGraphDataArrayPtr;
	if (graphInitialized == 0)  {
		goalGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xFA, 0x14, 0x2B));
		for (uint32_t cnt = 0; cnt < pGraphRec->amtValidGoalPoints  ;  ++ cnt) {
			goalGraph.addDataPoint(pGraphRec->goalValue);
		}
//		goalGraph.invalidate();  // todo test if invalidates are needed here and below or if invalidate of the whole containter is preferable
	}
	if (pGraphRec->amtValidDataPoints < (uint16_t) pidGraph.getMaxCapacity() )  {
		pidGraph.addDataPoint(pGraphRec->dataValues[pGraphRec->amtValidDataPoints -1]);
		triacGraph.addDataPoint(pGraphRec->triacValues[pGraphRec->amtValidDataPoints -1]);
	}
	if (graphInitialized == 0) {
		origYMax = pidGraph.getGraphRangeYMaxAsFloat();
//		pidGraph.invalidate();
//		triacGraph.invalidate();  maybe not needed
		graphInitialized = 1;
	}
}

void pidDataGraphContainer::initialize()
{
    pidDataGraphContainerBase::initialize();
    graphInitialized = 0;
    goalGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xFA, 0x14, 0x2B));
    triacGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xAA, 0xB2, 0x04));
}

void pidDataGraphContainer::showPidGraphFromData(pJoesPresenterEventT  pMsg)
{
	initFromGraphDataRec(pMsg->evData.pidGraphDataArrayPtr);
	setVisible(true);
//	invalidate();  maybe not needed
}

void pidDataGraphContainer::initFromGraphDataRec(graphDataRec* pRec)
{
	pGraphRec = pRec;
	goalGraph.clear();
	pidGraph.clear();
	triacGraph.clear();
	goalGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xFA, 0x14, 0x2B));
	triacGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xDF, 0xEB, 0x02));
	for (uint16_t cnt = 0; cnt < pRec->amtValidGoalPoints;  ++ cnt) {
		goalGraph.addDataPoint(pRec->goalValue);
	}
	for (uint16_t cnt = 0; cnt < pRec->amtValidDataPoints;  ++ cnt) {
		pidGraph.addDataPoint(pRec->dataValues[cnt]);
		triacGraph.addDataPoint(pRec->triacValues[cnt]);
	}
	graphInitialized = 1;
	origYMax = pidGraph.getGraphRangeYMaxAsFloat();
	setVisible(true);
//	invalidate();  maybe not needed
}

void pidDataGraphContainer::backButtonPressed()
 {
     setVisible(false);
     invalidate();
 }

void pidDataGraphContainer::redrawPressed()
 {
	//  todo for timing reason print in graph own Q-method instead of within touchgfx tick with low prio
    printExistingGraph();
 }

void pidDataGraphContainer::setRedrawButtonVisible(bool vis)
{
	redrawButton.setVisible(vis);
}

void pidDataGraphContainer::zoomButttonPressed()
{
	if (pGraphRec != NULL ) {
		if (zoomState == zoomNormal)  {
			goalGraph.setGraphRangeY((float)(pGraphRec->goalValue - 10.0), (float) (pGraphRec->goalValue + 10.0));
			pidGraph.setGraphRangeY((float)(pGraphRec->goalValue - 10.0), (float) (pGraphRec->goalValue + 10.0));
			zoomState = zoomPressed;
		}  else  {
			goalGraph.setGraphRangeY(0.0, origYMax);
			pidGraph.setGraphRangeY(0.0, origYMax);
			zoomState = zoomNormal;
		}
	}
}

