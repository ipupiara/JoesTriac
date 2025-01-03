#include <gui/containers/canDebugSelectContainer.hpp>
#include <canComms.h>

canDebugSelectContainer::canDebugSelectContainer()
{

}

void canDebugSelectContainer::initialize()
{
    canDebugSelectContainerBase::initialize();
}

void canDebugSelectContainer::setHostType(canHosts cHost)
{
	canHost=cHost;
}

void canDebugSelectContainer::setSelection(canHosts cHost ,canTestTypes canTestType)
{
	if (canTestType == noCanTest)
	{
		nonButton.setSelected(true);
		singleButton.setSelected(false);
		continuousButton.setSelected(false);
		singleButton.setVisible(true);
		continuousButton.setSelected(true);
		sendSelection(cHost ,noCanTest);
	}
	if (canTestType == singleCanTest)
	{
		nonButton.setSelected(false);
		singleButton.setSelected(true);
		continuousButton.setSelected(false);
		sendSelection(cHost ,singleCanTest);
	}	if (canTestType == continuousCanTest)
	{
		nonButton.setSelected(false);
		singleButton.setSelected(false);
		continuousButton.setSelected(true);
		singleButton.setVisible(false);
		continuousButton.setSelected(false);
		sendSelection(cHost ,continuousCanTest);
	}
}

void canDebugSelectContainer::nonButtonSelected()
{
	setSelection(this->canHost, noCanTest);
}

void canDebugSelectContainer::singleButtonSelected()
{
	setSelection(this->canHost, singleCanTest);
}

void canDebugSelectContainer::continuousButtonSelected()
{
	setSelection(this->canHost, continuousCanTest);
}

void canDebugSelectContainer::sendSelection(canHosts cHost ,canTestTypes canTestType)
{
	setCurrentCanSelection(cHost, canTestType);
}
