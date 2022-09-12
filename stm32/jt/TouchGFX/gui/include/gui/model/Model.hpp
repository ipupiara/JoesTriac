#ifndef MODEL_HPP
#define MODEL_HPP
#include <stdint.h>

class ModelListener;

class Model
{
public:
    Model();

   uint16_t   debugWeldingTime = 742;


    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

////   interface get/set values from Triac
    uint16_t getWeldingTimeSec()
    {
    	return debugWeldingTime;
    }

    void setWeldingTimeSec(uint16_t  tm)
    {
    	debugWeldingTime = tm;
    }


    int16_t stringToSecValue();

////   basic methods

protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
