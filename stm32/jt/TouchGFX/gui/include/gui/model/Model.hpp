#ifndef MODEL_HPP
#define MODEL_HPP
#include <stdint.h>
#include <cmsis_os.h>
#include <defines.h>

class ModelListener;

class Model
{
public:
    Model();


    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();


    uint16_t getWeldingTimeSec()
    {
    	return debugWeldingTime;
    }

    void setWeldingTimeSec(uint16_t  tm)
    {
    	debugWeldingTime = tm;
    }

    void storeWeldingTimeSec(uint16_t  tm);


    int16_t stringToSecValue();


    static osMessageQueueId_t    modelMessageQ;

////   basic methods

protected:
    ModelListener* modelListener;
    uint16_t   debugWeldingTime = 754;
};

#endif // MODEL_HPP
