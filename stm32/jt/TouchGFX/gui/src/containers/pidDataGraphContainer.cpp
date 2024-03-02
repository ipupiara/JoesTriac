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

	if (pMsg->evData.pidDataArrayPtr->amtValidPoints == 0)  {
		goalGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xFA, 0x14, 0x2B));
		for (uint16_t cnt = 0; cnt < goalGraph.getMaxCapacity();  ++ cnt) {
			goalGraph.addDataPoint(pMsg->evData.pidDataArrayPtr->goalValue);
		}
	}
	pidGraph.addDataPoint(pMsg->evData.pidGraphData.ampsF);
	pidGraph.invalidate();
}

void pidDataGraphContainer::initialize()
{
    pidDataGraphContainerBase::initialize();
}


void pidDataGraphContainer::initFromData(pJoesPresenterEventT  pMsg )
{
	goalGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xFA, 0x14, 0x2B));
	for (uint16_t cnt = 0; cnt < goalGraph.getMaxCapacity();  ++ cnt) {
		goalGraph.addDataPoint(pMsg->evData.pidDataArrayPtr->goalValue);
		if (cnt < pMsg->evData.pidDataArrayPtr->amtValidPoints) {
			pidGraph.addDataPoint(pMsg->evData.pidDataArrayPtr->dataValue[cnt]);
		}
	}
	goalGraph.invalidate();
	pidGraph.invalidate();
	setVisible(true);
	invalidate();
}


void pidDataGraphContainer::backButtonPressed()
 {
     setVisible(false);
     invalidate();
 }
