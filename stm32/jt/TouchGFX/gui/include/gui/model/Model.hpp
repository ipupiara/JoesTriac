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

    typedef enum {
    	changeToRunScreen = 0,
		changeToMainScreen,
		secondUpdate

    }  modelMessageType;



    typedef struct  {
    	modelMessageType messageType;
    	union {
    		uint32_t keyCode;
    		struct {
    			float   voltage;
    			uint16_t  potiPos;
    		} secondRunData;
    	}  evData;
    } CJoesModelEventT ;

    typedef CJoesModelEventT*  pJoesModelEventT;

    osMessageQueueId_t    modelMessageQ;


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

    osStatus_t sendModelMessage(pJoesModelEventT  pMsg);

////   basic methods

protected:
    ModelListener* modelListener;
    uint16_t   debugWeldingTime = 754;
};

#endif // MODEL_HPP
