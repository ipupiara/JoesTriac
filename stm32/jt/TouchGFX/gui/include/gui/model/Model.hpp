#ifndef MODEL_HPP
#define MODEL_HPP
#include <stdint.h>
#include <cmsis_os.h>
#include <defines.h>

class ModelListener;

class Model
{

protected:

	class CalibCache
	{
	protected:
		uint32_t calibHighCache;
		uint32_t calibLowCache;
		bool  calibHighCacheValid;
		bool  calibLowCacheValid;
	public:
		CalibCache()
		{
			calibHighCacheValid = false;
			calibLowCacheValid  = false;
		}

		uint32_t getCalibHigh()
		{
			uint32_t res;
			if (calibHighCacheValid)  {
				res = calibHighCache;
			}  else {
				calibHighCache = res = getDefinesCalibHigh();
				calibHighCacheValid = true;
			}
		}

		uint32_t getCalibLow()
		{
			uint32_t res;
			if (calibLowCacheValid)  {
				res = calibLowCache;
			}  else {
				calibLowCache = res = getDefinesCalibLow();
				calibLowCacheValid = true;
			}
		}

		void store()
		{
			if (calibLowCacheValid)  {
				saveCalibLow(calibLowCache);
			}
			if (calibHighCacheValid)  {
				saveCalibHigh(calibHighCache);
			}
		}

		void reset()
		{
			calibHighCacheValid = false;
			calibLowCacheValid  = false;
		}

	};


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

    uint16_t getAlarmTime();
    uint8_t  getAlarmNeeded();
    void storeAlarmTime(uint16_t alTime);
    void storeAlarmNeeded(uint8_t alNeeded);
    void storeAlarm(uint8_t alNeeded, uint16_t alTime);
    float getWeldingAmps();
    void storeWeldingAmps(float amps);
    uint32_t getCalibHigh();
    uint32_t getCalibLow();
    void   saveCalibValues();
    void   resetCalibValues();

    int16_t weldingTime;
    float weldingAmps;
    uint16_t alarmTime;
    uint8_t alarmNeeded;



////   basic methods

protected:
    ModelListener* modelListener;
    uint16_t   debugWeldingTime = 754;
    CalibCache  calibCache;
};

#endif // MODEL_HPP
