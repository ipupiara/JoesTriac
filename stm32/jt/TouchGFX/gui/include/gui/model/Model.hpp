#ifndef MODEL_HPP
#define MODEL_HPP
#include <stdint.h>
#include <cmsis_os.h>
#include <defines.h>
#include <uart-comms.h>
#include <mainJt.h>


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

	public:

		void storeCalibHigh();
		void storeCalibLow();

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

		void resetCalibValues()
		{
			calibHighCacheValid = false;
			calibLowCacheValid  = false;
			calibHighCacheChanged = false;
			calibLowCacheChanged = false;
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
    void storeAlarm(uint8_t alNeeded, uint16_t alTime, uint32_t zCalibOn);
    float getWeldingAmps();
    void storeWeldingAmps(float amps);
    uint32_t getCalibHigh();
    uint32_t getCalibLow();
    void setCalibHigh(uint32_t cH);
    void setCalibLow(uint32_t cL);
    void   saveCalibValues();
    void   resetCalibValues();


    uint8_t getSwitchPressureNeeded();
    uint16_t getSwitchPressureTime();
    void storeSwitchPressureData(uint32_t spTime, uint8_t spNeeded);
    void getDebugData(uint8_t*  pidP, uint8_t* infoP, uint8_t* exti);
    void setDebugData(uint8_t  pidP, uint8_t infoP, uint8_t exti);

protected:
    static void cppvsnprintf(char* buffer,uint32_t maxLen,const char *emsg, ...);
    void printPid (CJoesModelEventT* mEv);
    void drawPidLine (CJoesModelEventT* mEv);

protected:
    ModelListener* modelListener;
    CalibCache  calibCache;
    uint8_t uartInputBuffer [serialBufferSize];
};

#endif // MODEL_HPP
