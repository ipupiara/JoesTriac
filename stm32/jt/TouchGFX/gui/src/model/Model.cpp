#include <defines.h>
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <stdint.h>
#include <uart-Comms.h>


Model::Model() : modelListener(0)
{

}

//void p_printf(const char *emsg, ...)
//{
//	va_list ap;
//	va_start(ap, emsg);
//	char buffer[100];
//
//	vsnprintf((char *)&buffer, 100,  emsg, ap);
//

//uint32_t amt =osMessageQueueGetCount(serialMessageQ);
//		calc(amt);
//		status = osMessageQueuePut(serialMessageQ,&transmitBuffer,0,0);
//		if (status != osOK)  {
//			errorHandler(status ,goOn," osMessageQueuePut ","info_printf");
//		}
//
//	va_end(ap);
//}

//void calibrationScreenView::updateTriacDelay(uint32_t val)
//{
//	Unicode::snprintf(delayTextBuffer,5, "%4d", val);
//	char const * stringConstant1 = "delay change to %4d \n";
//	char buffer [100];
//	memset(buffer,0x5a,sizeof(buffer));
//	char * pBuffer = (char*)buffer;
//
////	cppvsnprintf(pBuffer,100,stringConstant1, val);
//
//	delayText.setWildcard(delayTextBuffer);
//	delayText.invalidate();
//}

void Model::cppvsnprintf(char* buffer,uint32_t maxLen,const char *emsg, ...)
{
	va_list ap;
	va_start(ap, emsg);

	vsnprintf((char *)buffer, maxLen,  emsg, ap);
	private_printf((char*)buffer);

	va_end(ap);
}

void Model::printPid (CJoesModelEventT* mEv)
{
	Unicode::UnicodeChar  floatBufferU [50];
	uint8_t  floatBuffer8 [50];
	memset(floatBufferU,0,sizeof(floatBufferU));
	memset(floatBuffer8,0,sizeof(floatBuffer8));
	memset(uartInputBuffer,0,serialBufferSize);

	if (mEv->evData.pidPrintData.pidAndPrintBool == pidAndPrint) {
		float fParams [4] =  {mEv->evData.pidPrintData.ampsV, mEv->evData.pidPrintData.Vpa
								 , mEv->evData.pidPrintData.Vin, mEv->evData.pidPrintData.Vde};
		Unicode::snprintfFloats(( short unsigned int*)floatBufferU, 50 ," amps %6.2f, Vp %4.1f, Vi  %4.1f, Vd  %4.1f", fParams );
		Unicode::toUTF8(( short unsigned int*)floatBufferU,  floatBuffer8, 50);

		cppvsnprintf((char *)uartInputBuffer, serialBufferSize,  "pidstep: adc %4d, delay %4d, corr %4d %s\n", mEv->evData.pidPrintData.triAdc,
								mEv->evData.pidPrintData.triDelay, mEv->evData.pidPrintData.triCorrInt, floatBuffer8 );
	}  else {
		cppvsnprintf((char *)uartInputBuffer, serialBufferSize,  "pidstep: adc %4d, delay %4d", mEv->evData.pidPrintData.triAdc,
										mEv->evData.pidPrintData.triDelay);
	}
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
			if (modelMessage.messageType ==  changeToRequestStopScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotorequestStopScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToJobOkScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotojobCompleteScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  changeToCalibrationScreen) {
				static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotocalibrationScreenScreenNoTransition();
			}
			if (modelMessage.messageType ==  restoreModelData) {
				restoreData();
			}
			if (modelMessage.messageType == pidPrint) {
				printPid(&modelMessage);
			}
		} else {
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
	sendEventToMainJtMessageQ(&evt,isNotFromIsr);
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
	sendEventToMainJtMessageQ(&evt,isNotFromIsr);
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

void Model::storeAlarm(uint8_t alNeeded, uint16_t alTime)
{
	CMainJtEventT evt;
	evt.evType = storeAlarmData;
	evt.mainUnion.alarmData.alarmTime = alTime;
	evt.mainUnion.alarmData.alarmNeeded = alNeeded;
	sendEventToMainJtMessageQ(&evt,isNotFromIsr);
}

void Model::restoreData()  // check if this method is in use somewhere
{
//	weldingTime = (uint16_t) getDefinesWeldingTime();
//	weldingAmps=  getDefinesWeldingAmps();
//
//	alarmNeeded =  getDefinesAlarmNeeded();
//
//	alarmTime =  (uint16_t) getDefinesAlarmTime();
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

void Model::CalibCache::storeCalibHigh()
{
	CMainJtEventT evt;
	evt.evType = saveCalibHi;
	evt.mainUnion.calibHigh = calibHighCache;
	sendEventToMainJtMessageQ(&evt,isNotFromIsr);
}

void Model::CalibCache::storeCalibLow()
{
	CMainJtEventT evt;
	evt.evType = saveCalibLo;
	evt.mainUnion.calibLow = calibLowCache;
	sendEventToMainJtMessageQ(&evt,isNotFromIsr);
}

uint8_t Model::getSwitchPressureNeeded()
{
	return getDefinesSwitchPressureNeeded();
}

uint16_t Model::getSwitchPressureTime()
{
	return getDefinesSwitchPressureTime();
}

void  Model::storeSwitchPressureData(uint32_t spTime, uint8_t spNeeded)
{
	storeDefinesSwitchPressureData(spTime,  spNeeded);
}

void  Model::getDebugData(uint8_t*  pidP, uint8_t* infoP, uint8_t* exti)
{
	getDefinesDebugData(pidP, infoP, exti);
}


void  Model::setDebugData(uint8_t  pidP, uint8_t infoP, uint8_t exti)
{
	CMainJtEventT evt;
	evt.evType = storeDebugData;
	evt.mainUnion.debugData.pidPri = pidP;
	evt.mainUnion.debugData.infoPri = infoP;
	evt.mainUnion.debugData.extiDo = exti;
	sendEventToMainJtMessageQ(&evt,isNotFromIsr);
}
