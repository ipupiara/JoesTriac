#include <stdio.h>
#include "TStatechart.h"
#include "StateClass.h"
#include <TriacIntr.h>
#include <uart-comms.h>
#include <mainJt.h>
#include <defines.h>
#include "triacPID.h"
#include <i2c.h>



// This defines and names the states the class has.
//  todo  attention: sequence must be the same as in xaStates (below)
enum eStates
{
	eStateJoesTriac,
		eStartState = eStateJoesTriac,
		eStateTriacOperating,
			eStateStartupCheck,
			eStateSetup,
				eStateSetupIdle,
				eStateAutoCalibrating,
//					eStateCalibrateZeroSignal,
					eStateCalibratingScale,
						eStateCalibrateLow,
						eStateCalibrateHigh,
			eStateTriacIdle,
			eStateTriacActive,
				eStateTriacRunning,
				eStateRequestStop,
			eStateJobOkDisplay,
		eStateFatalError,
	eNumberOfStates
};

TStatechart SJoesTriacStateChart;
TStatechart* PJoesTriacStateChart;
extern const uStInt uStIntHandlingDone;
extern const uStInt uStIntNoMatch;

fsmTriacEvent* currentEvent;
uint16_t secondsBeforeReturn;
uint32_t timeCnt;

void entryJoesTriac()
{
	printf("entryJoesTriac\n");
}

void exitJoesTriac()
{

}

uStInt evJoesTriacChecker(void)
{
	return (uStIntNoMatch);
}


void entryTriacOperatingChecker()
{
	printf("entryTriacOperatingChecker\n");
}

void exitTriacOperatingChecker()
{

}

uStInt evTriacOperatingChecker(void)   // during Start Screen
{
	uStInt res = uStIntNoMatch;

	if (currentEvent->evType == evFatalError)
	{
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateFatalError);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}

	return (res);
}

void entryStartupCheck()
{
	startDurationTimer(5);
}

void exitStartupCheck()
{
	stopDurationTimer();
}

uStInt evStartupCheckChecker(void)   // during Start Screen
{
	uStInt res = uStIntNoMatch;
	
	if (currentEvent->evType == evSecondsTick) {
		if (getSecondsInDurationTimer() == 3) {
			osStatus_t status;
			CJoesModelEventT ev;
			ev.messageType = restoreModelData;
			 status = sendModelMessage(&ev);
			 if (status != osOK) {
				 errorHandler(status, goOn, "restoreModelData","evStartupCheckChecker");
			 }
		}
	}

	if (currentEvent->evType == evTimeOutDurationTimer)
	{
		if (isCalibrationReady() == tOk) {
 			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}  else {
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetup);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
	}

	return (res); 
}



void entrySetupState(void)
{
//	CJoesModelEventT  msg;
//	osStatus_t status;
//	info_printf("entryTriacIdleState\n");
//	msg.messageType = changeToConfigScreen;
//	status = sendModelMessage(&msg);
//	if(status != osOK)  {
//		errorHandler(status,goOn," status ","entryTriacIdleState");
//	}
}

void exitSetupState(void)
{
//	printf("exit I\n");
}

uStInt evSetupChecker(void)
{
	uStInt res = uStIntNoMatch;
	printf("\ncheck for event in State evStateIdle");

//	if (currentEvent->evType == evConfigOkPressed)  {
//		if (isCalibrationReady() == tOk) {
//			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
//				// No event action.
//			END_EVENT_HANDLER(PJoesTriacStateChart);
//			res =  uStIntHandlingDone;
//		}
//	}
//	if (currentEvent->evType == evConfigOkPressed)  { for abort
//		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
//			// No event action.
//		END_EVENT_HANDLER(PJoesTriacStateChart);
//		res =  uStIntHandlingDone;
//	}
	return res;
}



void entrySetupIdleState(void)
{
	printf("entrySetupIdleState\n");
	CJoesModelEventT  msg;
	osStatus_t status;
	msg.messageType = changeToConfigScreen;
	status = sendModelMessage(&msg);
	if(status != osOK)  {
		errorHandler(status,goOn," status ","entrySetupIdleState");
	}
//	calibVarInd = 0;
//	currentVarVal = calibLowADC;
//	currentTitle = "calibLowADC";
//	displayCurrentVar();

}



void exitSetupIdleState(void)
{
	info_printf("exitSetupIdleState\n");
//	updateGradAmps();
}

uStInt evSetupIdleChecker(void)
{
	uStInt res = uStIntNoMatch;

	if (currentEvent->evType == evAutoConfigPressed)
	{
//		if (getDefinesZCalibOn() == 1)  {
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateAutoCalibrating);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evConfigOkPressed)  {
		if (isCalibrationReady() == tOk) {
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
	}
	return res;
}

void entryAutoCalibratingState(void)
{
	info_printf("entryAutoCalibratingState\n");
}

void exitAutoCalibratingState(void)
{
	info_printf("exitAutoCalibratingState\n");
}

uStInt evAutoCalibratingChecker(void)
{
	int8_t res;
	
	res = uStIntNoMatch;

//	printf("inside evCalibratingChecker\n");
	if (currentEvent->evType == evCalibAbortClick)  {
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetup);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}	
	return res;
}


void entryCalibrateZeroSignalState(void)
{
	CJoesModelEventT  msg;
	osStatus_t status;
	info_printf("entryCalibrateZeroSignalState\n");
	msg.messageType = changeToCalibrateZeroScreen;
	status = sendModelMessage(&msg);
	if(status != osOK)  {
		errorHandler(status,goOn," status ","entryCalibrateZeroSignalState");
	}
}

void exitCalibrateZeroSignalState(void)
{
	info_printf("exitCalibrateZeroSignalState\n");
}



uStInt evCalibrateZeroSignalChecker(void)
{
	int8_t res = 0;
	if (currentEvent->evType == evZeroSignalOK) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibratingScale);
			// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	
	if (currentEvent->evType == evSecondsTick) 
	{	
		persistentZeroAdjustSecondTickJob();
//		displayPotiPersistent();
		res =  uStIntHandlingDone;
	}
	return (res);
}


void entryCalibrateScaleState(void)
{
	info_printf("entryCalibrateScaleState\n");
	CJoesModelEventT  msg;
	osStatus_t status;
	info_printf("entryCalibrateZeroSignalState\n");
	msg.messageType = changeToCalibrationScreen;
	status = sendModelMessage(&msg);
	if(status != osOK)  {
		errorHandler(status,goOn," status ","entryCalibrateScaleState");
	}
	startTriacRun();
}

void exitCalibrateScaleState(void)
{
//	info_printf("exit calib\n");
	stopTriacRun();
}

uStInt evCalibrateScaleChecker(void)
{
	uStInt res = uStIntNoMatch;
	if (currentEvent->evType == evSecondsTick)  {
		sendActualValuesToCalibScreen();
		res =  uStIntHandlingDone;
	}
	return res;
}


void entryCalibrateLowState(void)
{
	info_printf("entryCalibrateLowState\n");
	setTriacTriggerDelay(stmTriggerDelayMax);
//	setTriacTriggerDelay(100);
}

void exitCalibrateLowState(void)
{
//	printf("exit calib Low\n");
//	clr_scr();
}

uStInt evCalibrateLowChecker(void)
{
	printf("check for event in State evStateIdle\n");
	uStInt res = uStIntNoMatch;
	if (currentEvent->evType == evCalibScreenReady)
	{
		CJoesPresenterEventT msg;
		msg.messageType = calibDesiredAmps;
		msg.evData.desiredAmps = calibLowAmps;
		sendPresenterMessage(&msg);
	}
	if (currentEvent->evType == evCalibContinueClick)
	{
		setCurrentAdcValAsCalibLow();
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateHigh);


		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evCalibSkipClick)
	{
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateHigh);

		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	return res;
}


void entryCalibrateHighState(void)
{
	info_printf("entryCalibrateHighState\n");
	CJoesPresenterEventT msg;
	msg.messageType = calibDesiredAmps;
	msg.evData.desiredAmps = calibHighAmps;
	sendPresenterMessage(&msg);
}

void exitCalibrateHighState(void)
{
//	printf("exit I\n");
//	clr_scr();
}

uStInt evCalibrateHighChecker(void)
{
	uStInt res = uStIntNoMatch;

	if (currentEvent->evType == evCalibSkipClick)
	{
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetupIdle);

		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evCalibContinueClick)
	{
		setCurrentAdcValAsCalibHigh();
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetupIdle);

		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	return res;
}

int8_t keyInd;


void entryTriacIdleState(void)
{
	CJoesModelEventT  msg;
	osStatus_t status;
	info_printf("entryTriacIdleState\n");
	msg.messageType = changeToMainScreen;
	status = sendModelMessage(&msg);
	if(status != osOK)  {
		errorHandler(status,goOn," status ","entryTriacIdleState");
	}
}

void exitTriacIdleState(void)
{

}

uStInt evTriacIdleChecker(void)
{
//	printf("check for event in State evStateIdle\n");
	uStInt res = uStIntNoMatch;

	if (currentEvent->evType == evStartPressed) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacRunning);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evConfigPressed)
	{
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetup);

		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}

	if (currentEvent->evType == evSecondsTick) 
	{	
//		onTriacIdleSecondTick_PID();
//		onTriacIdleSecondTick_Keyb();
//		res =  uStIntHandlingDone;
	}
	return res;
}
void entryTriacActiveState(void)
{
	info_printf("entryTriacActiveState\n");
	startDurationTimer(getDefinesWeldingTime());
	setTriacTriggerDelay(stmTriggerDelayMax);
	startTriacRun();
}

void exitTriacActiveState(void)
{
	stopTriacRun();
	stopDurationTimer();
}

uStInt evTriacActiveChecker(void)
{
	uStInt res = uStIntNoMatch;

	if (currentEvent->evType == evTimeOutDurationTimer) {
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateJobOkDisplay);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evAdcTick)
	{
		calcNextTriacDelay();
		res =  uStIntHandlingDone;
	}
	return res;
}

void entryTriacRunningState(void)
{
	info_printf("entryTriacRunningState\n");

	CJoesModelEventT  msg;
	msg.messageType = changeToRunScreen;
	sendModelMessage(&msg);
}

void exitTriacRunningState(void)
{

}

uStInt evTriacRunningChecker(void)
{
	uStInt res = uStIntNoMatch;

	if (currentEvent->evType == evStopPressed)  {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateRequestStop);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}		

	if (currentEvent->evType == evSecondsTick) {
		sendActualValuesToRunNStopScreen(secondsDurationTimerRemaining, secondsBeforeReturn);
		res =  uStIntHandlingDone;
	}	

	return res;
}


void entryRequestStopState(void)
{
	info_printf("entryRequestStopState\n");

	CJoesModelEventT  msg;
	msg.messageType = changeToRequestStopScreen;
	sendModelMessage(&msg);
	void setBuzzerOn();
	secondsBeforeReturn = 11;
}

void exitRequestStopState(void)
{
//	info_printf("exit Running\n");
	void setBuzzerOff();
	secondsBeforeReturn = 0;
}

uStInt evRequestStopChecker(void)
{
	uStInt res = uStIntNoMatch;

	if ((secondsBeforeReturn == 0) || (currentEvent->evType == evContinuePressed)){
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacRunning);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}

	if (currentEvent->evType == evStopPressed)  {
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}

	if (currentEvent->evType == evSecondsTick) {
		if (timeCnt < 2 )  {
			setBuzzerOn();
			++timeCnt;
		} else {
			setBuzzerOff();
			timeCnt = 0;
		}
		--secondsBeforeReturn;
		sendActualValuesToRunNStopScreen(secondsDurationTimerRemaining, secondsBeforeReturn);

		res =  uStIntHandlingDone;
	}
	return res;
}


void entryJobOkDisplayState(void)
{
	printf("entryJobOkDisplayState\n");

	CJoesModelEventT  msg;
	msg.messageType = changeToJobOkScreen;
	sendModelMessage(&msg);
	startDurationTimer(maxSecsPossible);
}

void exitJobOkDisplayState(void)
{
//	printf("exit I\n");
	setCompletionAlarmOff();
	stopDurationTimer();
}



uStInt evJobOkDisplayChecker(void)
{
	uStInt res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");


	if  (currentEvent->evType == evOkPressed)  {
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}

	if (currentEvent->evType == evSecondsTick) 
	{	
		sendActualValuesToJobOkScreen();
		if (getDefinesAlarmNeeded() != 0) {
			int32_t secondsRel = getSecondsInDurationTimer();
			if (secondsRel >= (getDefinesAlarmTime() * 60)) {
				toggleCompletionAlarm();
			}
		}
		res =  uStIntHandlingDone;
	}
	return (res);
}



void entryFatalErrorState(void)
{
//	printf("entry FatalError\n");
//	printf("**************fatal Error: %s *************************\n",lastFatalErrorString);
//	displayFatalError();
}

void exitFatalErrorState(void)
{
	printf("exit FatalError\n");
}

uStInt evFatalErrorChecker(void)
{
	uStInt res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	return (res);
}



#ifndef  sdccNULL 

#define tfNull 0

#else

t_fvoid  tfNull;

#endif 

// attention: sequence must be the same as above enum eStates


xStateType xaStates[eNumberOfStates] = {
 	{eStateJoesTriac,    // name
 	-1,									//parent
 	eStateTriacOperating,    // default substate
 	0,    // keep history
 	evJoesTriacChecker,    // event checking fu
	tfNull,       // def state entry function
	entryJoesTriac,     //    entering state
	exitJoesTriac},     // exiting state

		{eStateTriacOperating,
		eStateJoesTriac,
		eStateStartupCheck,
		0,
		evTriacOperatingChecker,
		tfNull,
		entryTriacOperatingChecker,
		exitTriacOperatingChecker},

			{eStateStartupCheck,
			eStateTriacOperating,
			-1,
			0,
			evStartupCheckChecker,
			tfNull,
			entryStartupCheck,
			exitStartupCheck},


			{eStateSetup,
			eStateTriacOperating,
			eStateSetupIdle,
			0,
			evSetupChecker,
			tfNull,
			entrySetupState,
			exitSetupState},

				{eStateSetupIdle,
				eStateSetup,
				-1,
				0,
				evSetupIdleChecker,
				tfNull,
				entrySetupIdleState,
				exitSetupIdleState},

				{eStateAutoCalibrating,
				eStateSetup,
				eStateCalibratingScale,
				0,
				evAutoCalibratingChecker,
				tfNull,
				entryAutoCalibratingState,
				exitAutoCalibratingState},

//					{eStateCalibrateZeroSignal,
//					eStateAutoCalibrating,
//					eStateCalibrateLow,
//					0,
//					evCalibrateZeroSignalChecker,
//					tfNull,
//					entryCalibrateZeroSignalState,
//					exitCalibrateZeroSignalState},

					{eStateCalibratingScale,
					eStateAutoCalibrating,
					eStateCalibrateLow,
					0,
					evCalibrateScaleChecker,
					tfNull,
					entryCalibrateScaleState,
					exitCalibrateScaleState},

						 {eStateCalibrateLow,
						eStateCalibratingScale,
						-1,
						0,
						evCalibrateLowChecker,
						tfNull,
						entryCalibrateLowState,
						exitCalibrateLowState},

						{eStateCalibrateHigh,
						eStateCalibratingScale,
						-1,
						0,
						evCalibrateHighChecker,
						tfNull,
						entryCalibrateHighState,
						exitCalibrateHighState},

			{eStateTriacIdle,
			eStateTriacOperating,
			-1,
			0,
			evTriacIdleChecker,
			tfNull,
			entryTriacIdleState,
			exitTriacIdleState},

			{eStateTriacActive,
			eStateTriacOperating,
			eStateTriacRunning,
			0,
			evTriacActiveChecker,
			tfNull,
			entryTriacActiveState,
			exitTriacActiveState},

				{eStateTriacRunning,
				eStateTriacActive,
				-1,
				0,
				evTriacRunningChecker,
				tfNull,
				entryTriacRunningState,
				exitTriacRunningState},

				{eStateRequestStop,
				eStateTriacActive,
				-1,
				0,
				evRequestStopChecker,
				tfNull,
				entryRequestStopState,
				exitRequestStopState},

			{eStateJobOkDisplay,
			eStateTriacOperating,
			-1,
			0,
			evJobOkDisplayChecker,
			tfNull,
			entryJobOkDisplayState,
			exitJobOkDisplayState},

		{eStateFatalError,
		eStateJoesTriac,
		-1,
		0,
		evFatalErrorChecker,
		tfNull,
		entryFatalErrorState,
		exitFatalErrorState}
};



/*
void entry...State(void)
{
//	printf("entry I\n");
}

void exit...State(void)
{
//	printf("exit I\n");
}

uStInt ev...Checker(void)
{
//	printf("check for event in State evStateIdle\n");

//	if (currentEvent->evType == eValueAssignement) 
//	{	if (HumidifyingLowerLimit > currentEvent->humidity)
//		{
//			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateHumidifying);
//				// No event action.
//			END_EVENT_HANDLER(PJoesTriacStateChart);
//			return (u32HandlingDone);
//		}
//	}
	return (u32NoMatch);
}

*/

/*

enum eStates
{
	eStateJoesTriac,
	eStartState = eStateJoesTriac,
	eStateAskForCalibration,
	eStateCalibrating,
	eNumberOfStates
};


uStInt evJoesTriacChecker(void)
{
	return (uStIntNoMatch);
}

uStInt evAskForCalibrationChecker(void)
{
	return (uStIntNoMatch);
}

uStInt evCalibratingChecker(void)
{
	return (uStIntNoMatch);
}


#ifndef  sdccNULL 

#define tfNull 0

#else

t_fvoid  tfNull;

#endif 

xStateType xaStates[eNumberOfStates] = {
 	{eStateJoesTriac,    // name
 	-1,									//parent
 	eStateAskForCalibration,    // default substate
 	0,    // keep history
 	evJoesTriacChecker,    // event checking fu
	tfNull,       // def state entry function
 	tfNull,     //    entering state
 	tfNull},     // exiting state

 	{eStateAskForCalibration,
 	eStateJoesTriac,
 	-1,
 	0,									
 	evAskForCalibrationChecker,
 	tfNull,
 	tfNull,
 	tfNull},

 	{eStateCalibrating,
 	eStateJoesTriac,
 	-1,
 	0,
 	evCalibratingChecker,
 	tfNull,
 	tfNull,
 	tfNull},
	 

};

*/


void startStateCharts()
{
	timeCnt = 0;

#ifdef  sdccNULL 

	tfNull = (t_fvoid ) NULL;

#endif 

 	PJoesTriacStateChart = & SJoesTriacStateChart; 
	createTStatechart (& SJoesTriacStateChart, xaStates, eNumberOfStates, eStartState);
	
}


void stopStateCharts()
{
	destructTStatechart(&SJoesTriacStateChart);
}


bool processTriacFsmEvent(TStatechart* t,fsmTriacEvent* ev)
{
	currentEvent = ev;
	return ProcessEvent(t);
}

