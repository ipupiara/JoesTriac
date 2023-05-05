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
		uint32_t zeroPotiPosCache;
		bool  calibHighCacheValid;
		bool  calibLowCacheValid;
		bool  calibHighCacheChanged;
		bool  calibLowCacheChanged;
		bool zeroPotiPosCacheValid;
		bool zeroPotiPosCacheChanged;

	public:

		void storeCalibHigh();
		void storeCalibLow();
		void storeZeroPotiPos();

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
				calibHighCache = res = getDefinesCalibHighAdc();
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
				calibLowCache = res = getDefinesCalibLowAdc();
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

		uint32_t getZeroPotiPos()
		{
			uint32_t res;
			if (zeroPotiPosCacheValid)  {
				res = zeroPotiPosCache;
			}  else {
				zeroPotiPosCache = res = getDefinesZeroPotiPos();
				calibHighCacheValid = true;
			}
			return res;		}

		void setZeroPotiPos(uint32_t val)
		{
			if (zeroPotiPosCacheValid)  {
				if (zeroPotiPosCache != val)  {
					zeroPotiPosCache = val;
					zeroPotiPosCacheChanged = true;
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
			if ((zeroPotiPosCacheValid) && (zeroPotiPosCacheChanged)) {
				storeZeroPotiPos();
			}

			resetCalibValues();
		}



		void resetCalibValues()
		{
			calibHighCacheValid = false;
			calibLowCacheValid  = false;
			calibHighCacheChanged = false;
			calibLowCacheChanged = false;
			zeroPotiPosCacheChanged = false;
			zeroPotiPosCacheChanged = false;
			calibLowCache = 0;
			calibHighCache = 0;
			zeroPotiPosCache = 0;
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
    void storeWeldingTimeSec(uint16_t  tm);

    uint16_t getAlarmTime();
    uint8_t  getAlarmNeeded();
    uint32_t getZCalibOn();
    void storeAlarm(uint8_t alNeeded, uint16_t alTime, uint32_t zCalibOn);
    float getWeldingAmps();
    void storeWeldingAmps(float amps);
    uint32_t getCalibHigh();
    uint32_t getCalibLow();
    uint32_t getZeroPotiPos();
    void setZeroPotiPos(uint32_t val);
    void setCalibHigh(uint32_t cH);
    void setCalibLow(uint32_t cL);
    void   saveCalibValues();
    void   resetCalibValues();


    uint8_t getSwitchPressureNeeded();
    uint16_t getSwitchPressureTime();
    void storeSwitchPressureData(uint32_t spTime, uint8_t spNeeded);
protected:
//    int16_t weldingTime;
//    uint32_t weldingAmps;
//    uint16_t alarmTime;
//    uint8_t alarmNeeded;
//    uint32_t zCalibOn;


////   basic methods

protected:
    ModelListener* modelListener;
    uint16_t   debugWeldingTime = 754;
    CalibCache  calibCache;
};

#endif // MODEL_HPP
