#include <gui/containers/pidDataGraphContainer.hpp>
#include <touchgfx/Color.hpp>
#include <triacPid.h>
#include <mainJt.h>


static Screen originalScreen;

void  pidDataGraphContainer::setOriginalScreen(Screen scr)
{
	originalScreen = scr;
}


pidDataGraphContainer::pidDataGraphContainer()
{
}

void pidDataGraphContainer::updateGraph(pJoesPresenterEventT  pMsg )
{
	graphDataRec*  pRec = pMsg->evData.pidDataArrayPtr;
	if (graphInitialized == 0)  {
		goalGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xFA, 0x14, 0x2B));
		for (uint32_t cnt = 0; cnt < pRec->amtValidGoalPoints  ;  ++ cnt) {
			goalGraph.addDataPoint(pRec->goalValue);
		}
		graphInitialized = 1;
		goalGraph.invalidate();
	}
	if (pRec->amtValidDataPoints < (uint16_t) pidGraph.getMaxCapacity())  {
		pidGraph.addDataPoint(pRec->dataValues[pRec->amtValidDataPoints -1]);
		triacGraph.addDataPoint(pRec->triacValues[pRec->amtValidDataPoints -1]);
	}
	pidGraph.invalidate();
}

void pidDataGraphContainer::initialize()
{
    pidDataGraphContainerBase::initialize();
    graphInitialized = 0;
    goalGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xFA, 0x14, 0x2B));
    triacGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xAA, 0xB2, 0x04));
}


void pidDataGraphContainer::initFromData(pJoesPresenterEventT  pMsg )
{
	if (graphInitialized == 0) {
		graphDataRec*  pRec = pMsg->evData.pidDataArrayPtr;
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
	}
}


void pidDataGraphContainer::backButtonPressed()
 {
     setVisible(false);
     invalidate();
 }
