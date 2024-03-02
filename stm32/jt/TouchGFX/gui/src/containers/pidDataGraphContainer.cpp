#include <gui/containers/pidDataGraphContainer.hpp>
#include <touchgfx/Color.hpp>
#include <triacPid.h>
#include <mainJt.h>

uint32_t getCPPGraphSize()
{
	return 900;
}


pidDataGraphContainer::pidDataGraphContainer()
{
}

void pidDataGraphContainer::updateGraph(pJoesPresenterEventT  pMsg )
{
	pidGraph.addDataPoint(pMsg->evData.pidGraphData.ampsF);
	pidGraph.invalidate();
}

void pidDataGraphContainer::initialize()
{
    pidDataGraphContainerBase::initialize();
	CMainJtEventT  ev;
	memset(&ev, 0x0, sizeof(ev));
	ev.evType = pidGraphInitializing;
	osStatus_t status =  sendEventToMainJtMessageQ( &ev, isNotFromIsr);
	if (status != osOK) {
		errorHandler(status,goOn," status ","pidDataGraphContainer::initialize");
	}
}


void pidDataGraphContainer::initFromData()
{
	graphDataRec* pData = &triacPidGraphData;
	goalGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xFA, 0x14, 0x2B));
	for (uint16_t cnt = 0; cnt < goalGraph.getMaxCapacity();  ++ cnt) {
		goalGraph.addDataPoint(pData->goalValue);
		if (cnt < pData->amtValidPoints) {
			pidGraph.addDataPoint(pData->dataValue[cnt]);
		}
	}
	goalGraph.invalidate();
	pidGraph.invalidate();
}


void pidDataGraphContainer::backButtonPressed()
 {
     setVisible(false);
     invalidate();
 }
