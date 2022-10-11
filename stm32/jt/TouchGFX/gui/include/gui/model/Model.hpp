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

    void restoreData();

    void tick();


    uint16_t getWeldingTimeSec();

    void setWeldingTimeSec(uint16_t  tm)
    {
    	weldingTime = tm;
    }

    void storeWeldingTimeSec(uint16_t  tm);


    int16_t weldingTime;
    float weldingAmps;
    uint16_t alarmTime;
    uint8_t alarmNeeded;



////   basic methods

protected:
    ModelListener* modelListener;
    uint16_t   debugWeldingTime = 754;
};

#endif // MODEL_HPP
