#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <stdint.h>
#include <defines.h>


Model::Model() : modelListener(0)
{

}

int16_t Model::stringToSecValue()
{
	return 0;
}


void Model::tick()
{
	modelListener->tick();
}

void Model::storeWeldingTimeSec(uint16_t  tm)
{

}

