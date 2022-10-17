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
		bool  calibHighCacheChanged;
		bool  calibLowCacheChanged;

		uint32_t zeroPotiPos;
		uint32_t calibHigh;
		uint32_t calibLow;
		bool zeroPotiPosValid;
		bool zeroPotiPosChanged;
		bool calibHighValid;
		bool calibHighChanged;
		bool calibLowValid;
		bool calibLowChanged;
	public:
		CalibCache()
		{
			resetCalibValues();
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
			return res;
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
			return res;
		}

		void setCalibHigh(uint32_t cHi)
		{
			if (calibHighCacheValid)  {
				if (calibHighCache != cHi)  {
					calibHighCache = cHi;
					calibHighCacheChanged = true;
				}
			}
		}

		void setCalibLow(uint32_t cLo)
		{
			if (calibLowCacheValid)  {
				if (calibLowCache != cLo)  {
					calibLowCache = cLo;
					calibLowCacheChanged = true;
				}
			}
		}

		void storeCalibValues()
		{
			if ((calibLowCacheValid) && (calibLowCacheChanged))  {
				storeCalibLow();
			}
			if ((calibHighCacheValid) && (calibHighCacheChanged)) {
				storeCalibHigh();
			}
			resetCalibValues();
		}

		void storeCalibHigh();
		void storeCalibLow();
		void storeZeroPotiPos();

		void resetCalibValues()
		{
			calibHighCacheValid = false;
			calibLowCacheValid  = false;
			calibHighCacheChanged = false;
			calibLowCacheChanged = false;
			calibLowCache = 0;
			calibHighCache = 0;
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
    uint32_t getWeldingAmps();
    void storeWeldingAmps(uint32_t amps);
    uint32_t getCalibHigh();
    uint32_t getCalibLow();
    void setCalibHigh(uint32_t cH);
    void setCalibLow(uint32_t cL);
    void   saveCalibValues();
    void   resetCalibValues();

    int16_t weldingTime;
    uint32_t weldingAmps;
    uint16_t alarmTime;
    uint8_t alarmNeeded;



////   basic methods

protected:
    ModelListener* modelListener;
    uint16_t   debugWeldingTime = 754;
    CalibCache  calibCache;
};

#endif // MODEL_HPP
