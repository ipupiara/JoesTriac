#include <gui/containers/pidDataGraphContainer.hpp>
#include <touchgfx/Color.hpp>
#include <triacPid.h>

pidDataGraphContainer::pidDataGraphContainer()
{

}

void pidDataGraphContainer::updateGraph(pJoesPresenterEventT  pMsg )
{
//	goalGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xFA, 0x14, 0x2B));
//	goalGraph.invalidate();
	pidGraph.addDataPoint(pMsg->evData.pidGraphData.ampsF);
	pidGraph.invalidate();
}

void pidDataGraphContainer::initialize()
{
    pidDataGraphContainerBase::initialize();

	goalGraphLine1Painter.setColor(touchgfx::Color::getColorFromRGB(0xFA, 0x14, 0x2B));
    float goalValue = getDefinesWeldingAmps();
    for (uint16_t cnt = 0; cnt < goalGraph.getMaxCapacity();  ++ cnt) {
    	goalGraph.addDataPoint(goalValue);
    }
    for (uint16_t cnt = 0; cnt < triacPidGraphData.amtValidPoints ;  ++ cnt) {
     	goalGraph.addDataPoint(triacPidGraphData.dataValue[cnt]);
     }

//    pidGraph.addDataPoint(triacPidGraphData.dataValue[cnt]);

	goalGraph.invalidate();
}

void pidDataGraphContainer::setData(graphDataRec* pData)
{
	for (uint16_t cnt = 0; cnt < pidGraph.getMaxCapacity(); ++ cnt) {
		pidGraph.addDataPoint(pData->dataValue[cnt]);
		goalGraph.addDataPoint(pData->goalValue);
	}
	goalGraph.invalidate();
	pidGraph.invalidate();
}

void pidDataGraphContainer::backButtonPressed()
 {
     setVisible(false);
     invalidate();
 }
