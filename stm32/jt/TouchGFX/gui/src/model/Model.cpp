#include <defines.h>
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <stdint.h>
#include <mainJt.h>


Model::Model() : modelListener(0)
{

}


void Model::tick()
{
	modelListener->tick();

	CJoesModelEventT  modelMessage;
	uint8_t  prio = 0;
	osStatus_t  status;

	while (osMessageQueueGetCount(modelMessageQ) )
	{
		if ((status =osMessageQueueGet ( modelMessageQ, &modelMessage, &prio, 0)) == osOK) {
			if (modelMessage.messageType ==  changeToRequesStopScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToRunScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorunScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToMainScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotomainScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToConfigScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoconfigScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToCalibrateZeroScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotocalibrateZeroScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToRequestStopScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToJobOkScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotojobCompleteScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  restoreModelData) {
				restoreData();
			}
		} else {
//			char* mm = "modelMessageQ";
//			char* met = "Model::tick";
//			errorHandler(status, goOn,mm,met);
		}

	}
}

uint16_t Model::getWeldingTimeSec()
{
	return (uint16_t) getDefinesWeldingTime();
}

void Model::storeWeldingTimeSec(uint16_t  tm)
{
	CMainJtEventT evt;
	evt.evType = storeWeldingTime;
	evt.mainUnion.weldingTime = tm;
	sendEventToMainJtMessageQ(&evt,0);
}

float Model::getWeldingAmps()
{
	return getDefinesWeldingAmps();
}

void Model::storeWeldingAmps(float amps)
{
	CMainJtEventT evt;
	evt.evType = storeWeldingAmpere;
	evt.mainUnion.weldingAmps = amps;
	sendEventToMainJtMessageQ(&evt,0);
}


uint16_t Model::getAlarmTime()
{
	uint16_t res;
	res = (uint16_t) getDefinesAlarmTime();
	return res;
}

uint8_t  Model::getAlarmNeeded()
{
	uint8_t res;
	res = getDefinesAlarmNeeded();
	return res;
}

uint32_t Model::getZCalibOn()
{
	uint32_t res;
	res = getDefinesZCalibOn();
	return res;
}

void Model::storeAlarm(uint8_t alNeeded, uint16_t alTime, uint32_t zCalibOn)
{
	CMainJtEventT evt;
	evt.evType = storeAlarmData;
	evt.mainUnion.alarmData.alarmTime = alTime;
	evt.mainUnion.alarmData.alarmNeeded = alNeeded;
	evt.mainUnion.alarmData.zCalibOn = zCalibOn;
	sendEventToMainJtMessageQ(&evt,0);
}

void Model::restoreData()
{
//	weldingTime = (uint16_t) getDefinesWeldingTime();
//	weldingAmps=  getDefinesWeldingAmps();
//
//	alarmNeeded =  getDefinesAlarmNeeded();
//
//	alarmTime =  (uint16_t) getDefinesAlarmTime();
//	zCalibOn  =  getDefinesZCalibOn();
}

uint32_t Model::getCalibHigh()
{
	return calibCache.getCalibHigh();
}

uint32_t Model::getCalibLow()
{
	return calibCache.getCalibLow();
}

void Model::setCalibHigh(uint32_t cH)
{
	calibCache.setCalibHigh(cH);
}

void Model::setCalibLow(uint32_t cL)
{
	calibCache.setCalibLow(cL);
}

void   Model::saveCalibValues()
{
	calibCache.storeCalibValues();
}

void   Model::resetCalibValues()
{
	calibCache.resetCalibValues();
}

uint32_t Model::getZeroPotiPos()
{
	return calibCache.getZeroPotiPos();
}

void Model::setZeroPotiPos(uint32_t val)
{
	calibCache.setZeroPotiPos(val);
}

void Model::CalibCache::storeCalibHigh()
{
	CMainJtEventT evt;
	evt.evType = saveCalibHi;
	evt.mainUnion.calibHigh = calibHighCache;
	sendEventToMainJtMessageQ(&evt,0);
}

void Model::CalibCache::storeCalibLow()
{
	CMainJtEventT evt;
	evt.evType = saveCalibLo;
	evt.mainUnion.calibLow = calibLowCache;
	sendEventToMainJtMessageQ(&evt,0);
}

void Model::CalibCache::storeZeroPotiPos()
{
	CMainJtEventT evt;
	evt.evType = saveZPotiPos;
	evt.mainUnion.zPotiPos = zeroPotiPosCache;
	sendEventToMainJtMessageQ(&evt,0);
}


