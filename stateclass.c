
#include <avr/io.h>
#include <stdio.h>
//#include <iostream.h>
//#include <assert.h>

#include "TStatechart.h"
#include "StateClass.h"
#include "TriacDefines.h"


extern const uStInt uStIntHandlingDone;
extern const uStInt uStIntNoMatch;

CJoesTriacEvent* currentEvent;

#include "TriacIntr.h"
#include "triacPID.h"
#include "triacUI.h"
#include "TriacKeyPad.h"
#include "twi_master.h"

// This defines and names the states the class has.
// attention: sequence must be the same as in xaStates (below)  !!!
enum eStates
{
	eStateJoesTriac,
	eStartState = eStateJoesTriac,
	eStateTriacOperating,
	eStateAskForCalibration,
	eStateCalibrating,
	eStateAskingRmsAvg,
	eStateCalibrateZeroSignal,
	eStateCalibrateScale,
	eStateCalibrateLow,
	eStateCalibrateHigh,
	eStateTriacIdle,
	eStateEditIdle,
	eStateEditAmps,
	eStateEditDuration,
	eStateSetup,
	eStateSetupIdle,
	eStateSetupAlarmYesNo,
	eStateSetupAlarmMinutes,
	eStateTriacRunning,
	eStateJobOkDisplay,
	eStateFatalError,
	eNumberOfStates
};

uStInt evJoesTriacChecker(void)
{
	return (uStIntNoMatch);
}

uStInt evTriacOperatingChecker(void)
{
	uStInt res = uStIntNoMatch;
//	printf("check for event in State evTriacOperating\n");

	if (currentEvent->evType == evFatalError) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateFatalError);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	
	return (res); 
}

void entryAskForCalibrationState(void)
{
	printf("entry AskForCalibration\n");
	displayCalibrationPrompt();
	startDurationTimer(6);

//	startDurationTimer(maxSecsPossible);
}

void exitAskForCalibrationState(void)
{
	printf("exit ask calib\n");
	stopDurationTimer();
	clr_scr();
}

uStInt evAskForCalibrationChecker(void)
{
	uStInt res = uStIntNoMatch;
//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evTimeOutDurationTimer) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	if ((currentEvent->evType == evAstPressed) || (currentEvent->evType == evStartPressed))
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrating);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evSecondsTick) 
	{	
		displayCountDown();		
		res =  uStIntHandlingDone;
//		debugEvent1Triggered = 1;
	}
	return (res); 
}

void entryCalibratingState(void)
{
//	printf("\nentry Calib");
	startDurationTimer(maxSecsPossible);   // enable secondsTick
}

void exitCalibratingState(void)
{
//	printf("exit calib\n");
	stopDurationTimer();
}

uStInt evCalibratingChecker(void)
{
	int8_t res;
	
	res = uStIntNoMatch;

//	printf("inside evCalibratingChecker\n");
	if (currentEvent->evType == evStopPressed)  {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}	
	return res;
}

void entryAskingRmsAvgState(void)
{
//	printf("entry I\n");
	displayRmsAvgQuery();
}

void exitAskingRmsAvgState(void)
{
//	printf("exit I\n");
}

uStInt evAskingRmsAvgChecker(void)
{
//	printf("check for event in State evStateIdle\n");
	uStInt res = uStIntNoMatch;

	if (currentEvent->evType == evNumPressed) 
	{	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateZeroSignal);
		// No event action.
		
		storeAmpsInputPin(avg);

		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;		
	}

	if (currentEvent->evType == evAstPressed) 
	{	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateZeroSignal);
		// No event action.

		storeAmpsInputPin(rms);

		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;		
	}
	if (currentEvent->evType == evSecondsTick) 
	{	
//		debugEvent1Triggered = 1;	
		res =  uStIntHandlingDone;		
	}
	return (res);
}

void entryCalibrateZeroSignalState(void)
{
//	printf("entry I\n");
	displayCalibrateZeroPotiPos();

	stableZeroAdjReached = 0;
    if (!setAdjustJob(persistentZeroAdjust)) {
		sprintf((char *) &lastFatalErrorString,"i2c comms err");
		fatalErrorOccurred = 1;
	}
}

void exitCalibrateZeroSignalState(void)
{
//	printf("exit exitCalibrateZeroSignalState\n");
}

uStInt checkCalibZeroInner(uStInt res)
// AVR Studio's produced code will not crash anymore
// when this is outplaced into inner method (method size problem ??)
// "silly problems, silly solutions"
{
	if (currentEvent->evType == evNumPressed) 
	{	
//		debugEvent1Triggered = 1;			
		
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateScale);
		// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;	
	}

/*
	if (currentEvent->evType == evCharEntered) {
		switch (currentEvent->evData.keyCode) {
			case kp1 : 
				sendZeroAdjustMsg(up1);
				break;
			case kp2 :
				sendZeroAdjustMsg(up10);
				break ;			
			case kp7 : 
				sendZeroAdjustMsg(down1);	
				break;
			case kp8 :
				sendZeroAdjustMsg(down10);
				break ;									
		}
	}
*/
	return res;
}

uStInt evCalibrateZeroSignalChecker(void)
{
//	printf("check for event in State evStateIdle\n");
	uStInt res = uStIntNoMatch;

	if (currentEvent->evType == evZeroSignalOK) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateScale);
			// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	
	if (currentEvent->evType == evSecondsTick) 
	{	
		persistentZeroAdjStep();
		displayPotiPersistent();
	
		res =  uStIntHandlingDone;
	}

	res = checkCalibZeroInner(res);  // method only works if one part is placed out
									// no idea why (lenght of code ? )
	return (res);
}


void entryCalibrateScaleState(void)
{
//	printf("\nentry Calib");
	setAmpsADC();
	startTriacRun();
}

void exitCalibrateScaleState(void)
{
//	printf("exit calib\n");
	stopTriacRun();
	closeAmpsADC();
}

uStInt evCalibrateScaleChecker(void)
{
	int16_t triggerDelay;
	int8_t res;
	
	res = uStIntNoMatch;
	if (currentEvent->evType == evCharEntered) {
		triggerDelay = triacTriggerDelayCms;
		switch (currentEvent->evData.keyCode) {
			case kp1 : 
				triggerDelay++;
				break;
			case kp2 :
				triggerDelay += 10;
				break ;	
			case kp3 :
				triggerDelay += 100;
				break ;		
			case kp7 : 
				triggerDelay--;
				break;
			case kp8 :
				triggerDelay -= 10;
				break ;	
			case kp9 :
				triggerDelay -= 100;
				break ;									
		}
		if (triggerDelay < 0) triggerDelay = 0;
		if (triggerDelay > triggerDelayMax) triggerDelay = triggerDelayMax;
		setTriacTriggerDelay(triggerDelay);
		displayDebugVoltageNTriggerDelay();
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evSecondsTick) 
	{	
		displayDebugVoltageNTriggerDelay();
		res =  uStIntHandlingDone;
	}
	return res;
}


void entryCalibrateLowState(void)
{
//	printf("entry calib Low\n");
	displayCalibrateLow();
	setTriacTriggerDelay(0);
}

void exitCalibrateLowState(void)
{
//	printf("exit calib Low\n");
	clr_scr();
}

uStInt evCalibrateLowChecker(void)
{
//	printf("check for event in State evStateIdle\n");
	uStInt res = uStIntNoMatch;

	if (currentEvent->evType == evAstPressed) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateHigh);
			// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evNumPressed) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateHigh);
			
			storeCalibLowTriggerDelay();
			storeCalibLowADC();			

			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	return res;
}


void entryCalibrateHighState(void)
{
//	printf("entry I\n");
	displayCalibrateHigh();
}

void exitCalibrateHighState(void)
{
//	printf("exit I\n");
	clr_scr();
}

uStInt evCalibrateHighChecker(void)
{
	uStInt res = uStIntNoMatch;

	if (currentEvent->evType == evAstPressed) 
	{	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);

		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evNumPressed) 
	{	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);

		storeCalibHighTriggerDelay();
		storeCalibHighADC();
		updateGradAmps();			

		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	return res;
}

int8_t keyInd;


void entryTriacIdleState(void)
{
//	printf("entry I\n");
//	debugEvent1Triggered = 1;
	onEntryIdlePID();
	startDurationTimer(maxSecsPossible);   // enable secondsTick
    if (!setAdjustJob(volatileZeroAdjust)) {
		sprintf((char *) &lastFatalErrorString,"i2c comms err");
		fatalErrorOccurred = 1;
	}
}

void exitTriacIdleState(void)
{
//	printf("exit I\n");
	stopDurationTimer();
    if (!fatalErrorOccurred) {
		if (!setAdjustJob(jobIdle)) {
			sprintf((char *) &lastFatalErrorString,"i2c comms err");
			fatalErrorOccurred = 1;
		}
	}
	clr_scr();
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
	if (currentEvent->evType == evF1Pressed) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetup);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evTimeOutDurationTimer) 
	{	
		startDurationTimer(maxSecsPossible);   // enable secondsTick
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evSecondsTick) 
	{	
		onIdleSecondTickPID();
		res =  uStIntHandlingDone;
	}
	return res;
}


void entryEditIdleState(void)
{
	displayEditAmpsDuration();
	displayAmps(-1);
	displayTime(-1);
//	printf("entry I\n");
}	

void exitEditIdleState(void)
{
//	printf("exit I\n");
}

uStInt evEditIdleChecker(void)
{
	uStInt res = uStIntNoMatch;
//	printf("\ncheck for event in State evStateIdle");

	if (currentEvent->evType==evAstPressed) {
//		printf("\ncheck for event in State evStateIdle amps");
	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditAmps);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType==evNumPressed) {	
//		printf("\ncheck for event in State evStateIdle dur");

		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditDuration);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evTWIDataReceived) 
	// do writing  only in edit idle, not to interfere with the edit-cursor position
	// could be done also by laying edit-cursor pos "on stack" and set again after write, 
	 //  but lets first try with this simpler version ... should be ok as well
	{				
		checkTWIZeroAdjustMsg();
		displayPotiVolatile();
	}
/*
	if (currentEvent->evType == evCharEntered) {
		switch (currentEvent->evData.keyCode) {
			case kp1 : 
				sendZeroAdjustMsg(up1);
				break;
			case kp2 :
				sendZeroAdjustMsg(up10);
				break ;			
			case kp7 : 
				sendZeroAdjustMsg(down1);	
				break;
			case kp8 :
				sendZeroAdjustMsg(down10);
				break ;									
		}
		res =  uStIntHandlingDone;
	}
*/
	return res;
}

void entryEditAmpsState(void)
{
//	printf("entry I\n");
	displayEditAmps();
	keyInd = 0;
	displayAmps(keyInd);
}

void exitEditAmpsState(void)
{
//	printf("exit I\n");
	displayAmps(-1);
}

uStInt evEditAmpsChecker(void)
{
	uStInt res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evNumPressed)  {	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditDuration);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
			if (currentEvent->evType == evAstPressed)  {	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}

	if (currentEvent->evType == evCharEntered) {

		if ((currentEvent->evData.keyCode <= kp9) && (currentEvent->evData.keyCode >= kp0)) {
			switch (keyInd)
			{
				case 0: 
					storeAmps100(currentEvent->evData.keyCode);
					break;
				case 1:
					storeAmps10(currentEvent->evData.keyCode);
					break;
				case 2:
					storeAmps(currentEvent->evData.keyCode);
					BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
				
					END_EVENT_HANDLER(PJoesTriacStateChart);				
			}
			keyInd ++;
			displayAmps(keyInd);  // if keyInd = 3, dispAmps can be done again in next State, no matter
			
			res =  uStIntHandlingDone;
		}
	}		
	return res;
}


void entryEditDurationState(void)
{
//	printf("entry I\n");
	displayEditDuration();
	keyInd = 0;
	displayTime(keyInd);
}

void exitEditDurationState(void)
{
//	printf("exit I\n");
	displayTime(-1);
}

uStInt evEditDurationChecker(void)
{
	uStInt res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evAstPressed)  {	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditAmps);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evNumPressed)  {	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}

	if (currentEvent->evType == evCharEntered) {

		if ((currentEvent->evData.keyCode <= kp9) && (currentEvent->evData.keyCode >= kp0)) {
			switch (keyInd)
			{
				case 0: 
					storeMin10(currentEvent->evData.keyCode);
					break;
				case 1:
					storeMin(currentEvent->evData.keyCode);
					keyInd++;
					break;
				case 3:	
					if (currentEvent->evData.keyCode <= kp5) {
						storeSec10(currentEvent->evData.keyCode);
					} else keyInd --;
					break;
				case 4:
					storeSec(currentEvent->evData.keyCode);
					BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
				
					END_EVENT_HANDLER(PJoesTriacStateChart);				
			}
			keyInd ++;
			displayTime(keyInd);
			
			res =  uStIntHandlingDone;
		}
	}		
	return res;
}



uStInt evSetupChecker(void)
{
	uStInt res;
	res = uStIntNoMatch;
	if (currentEvent->evType == evF2Pressed) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	return (res);
}

void entrySetupState(void)
{
//	printf("entry I\n");
	displaySetup();
}

void exitSetupState(void)
{
//	printf("exit I\n");
}

uStInt evSetupIdleChecker(void)
{
	uStInt res;
	res = uStIntNoMatch;
	if (currentEvent->evType==evAstPressed) {
//		printf("\ncheck for event in State evStateIdle amps");
	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetupAlarmMinutes);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType==evNumPressed) {	
//		printf("\ncheck for event in State evStateIdle dur");

		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetupAlarmYesNo);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	return (res);
}

void entrySetupIdleState(void)
{
//	printf("entry I\n");
	displayAlarmMinutes(-1);
	displayAlarmYesNo(-1);
	clearSetupInputHint();
}

void exitSetupIdleState(void)
{
//	printf("exit I\n");
}

uStInt evSetupAlarmYesNoChecker(void)
{
	uStInt res;
	res = uStIntNoMatch;

	if (currentEvent->evType == evAstPressed)  {	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetupAlarmMinutes);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evNumPressed)  {	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetupIdle);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evCharEntered) {

		if (currentEvent->evData.keyCode == kp1) {
			storeCompletionAlarmOn(!completionAlarmOn);
			displayAlarmYesNo(keyInd);
			res =  uStIntHandlingDone;
		}
	}


	return (res);
}

void entrySetupAlarmYesNoState(void)
{
//	printf("entry I\n");
	keyInd = 0;
	displayAlarmYesNo(keyInd);
	toggleSetupInputHint();
}

void exitSetupAlarmYesNoState(void)
{
//	printf("exit I\n");
}

uStInt evSetupAlarmMinutesChecker(void)
{
	uStInt res;
	res = uStIntNoMatch;

	if (currentEvent->evType == evAstPressed)  {	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetupIdle);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evNumPressed)  {	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetupAlarmYesNo);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evCharEntered) {

		if ((currentEvent->evData.keyCode <= kp9) && (currentEvent->evData.keyCode >= kp0)) {
			switch (keyInd)
			{
				case 0: 
					storeCompletionAlarmMins10(currentEvent->evData.keyCode);
					break;
				case 1:
					storeCompletionAlarmMins(currentEvent->evData.keyCode);
					BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
				
					END_EVENT_HANDLER(PJoesTriacStateChart);				
			}
			keyInd ++;
			displayAlarmMinutes(keyInd);  // if keyInd = 3, dispAmps can be done again in next State, no matter
			
			res =  uStIntHandlingDone;
		}
	}

	return (res);
}

void entrySetupAlarmMinutesState(void)
{
	keyInd = 0;
	displayAlarmMinutes(keyInd);
	numericSetupInputHint();
}

void exitSetupAlarmMinutesState(void)
{
//	printf("exit I\n");
	displayAlarmMinutes(-1);
}

void entryTriacRunningState(void)
{
	printf("entry Running\n");
	displayTriacRunning();
	startDurationTimer(desiredTimeS);
//	startDurationTimer(maxSecsPossible);   // sometimes used for debugging	
	setTriacTriggerDelay(calibLowTriggerDelay);  // start defined,  not just somewhere
												// because of 220 V fuse ejects
												// lowCalib seems better joice than 0
	setAmpsADC();
	startTriacRun();
}

void exitTriacRunningState(void)
{
	printf("exit Running\n");
	stopDurationTimer();
	stopTriacRun();
	closeAmpsADC();
	clr_scr();
}

uStInt evTriacRunningChecker(void)
{
	uStInt res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evStopPressed)  {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}		
	if (currentEvent->evType == evTimeOutDurationTimer) {
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateJobOkDisplay);
			// No event action.
		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}			
	if (currentEvent->evType == evSecondsTick) {
		displayCurrentAmps();
		displayCountDown();
		displayVoltage();	
		res =  uStIntHandlingDone;
	}	
	if (currentEvent->evType == evAdcTick)
	{
		calcNextTriacDelay();
		res =  uStIntHandlingDone;
	}		
	return res;
}


void entryJobOkDisplayState(void)
{
//	printf("entry I\n");
	displayJobOk();
	startDurationTimer(maxSecsPossible);
}

void exitJobOkDisplayState(void)
{
//	printf("exit I\n");
	stopDurationTimer();
}

uStInt evJobOkDisplayChecker(void)
{
	uStInt res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	if ((currentEvent->evType == evAstPressed) || (currentEvent->evType == evTimeOutDurationTimer))  {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evSecondsTick) 
	{	
		displayInDurationTimerSince();		
		res =  uStIntHandlingDone;
	}
	return (res);
}



void entryFatalErrorState(void)
{
//	printf("entry FatalError\n");
	displayFatalError();
}

void exitFatalErrorState(void)
{
//	printf("exit FatalErro\n");
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
 	tfNull,     //    entering state
 	tfNull},     // exiting state

 	{eStateTriacOperating,
 	eStateJoesTriac,
 	eStateAskForCalibration,
 	0,									
 	evTriacOperatingChecker,
 	tfNull,
 	tfNull,
 	tfNull},


 	{eStateAskForCalibration,
 	eStateTriacOperating,
 	-1,
 	0,									
 	evAskForCalibrationChecker,
 	tfNull,
 	entryAskForCalibrationState,
 	exitAskForCalibrationState},

 	{eStateCalibrating,
 	eStateTriacOperating,
 	eStateAskingRmsAvg,
 	0,
 	evCalibratingChecker,
 	tfNull,
 	entryCalibratingState,
 	exitCalibratingState},

 	{eStateAskingRmsAvg,
 	eStateCalibrating,
 	-1,
 	0,
 	evAskingRmsAvgChecker,
 	tfNull,
 	entryAskingRmsAvgState,
 	exitAskingRmsAvgState},

	{eStateCalibrateZeroSignal,
 	eStateCalibrating,
 	-1,
 	0,
 	evCalibrateZeroSignalChecker,
 	tfNull,
 	entryCalibrateZeroSignalState,
 	exitCalibrateZeroSignalState},

 	{eStateCalibrateScale,
 	eStateCalibrating,
 	eStateCalibrateLow,
 	0,
 	evCalibrateScaleChecker,
 	tfNull,
 	entryCalibrateScaleState,
 	exitCalibrateScaleState},

	 {eStateCalibrateLow,
 	eStateCalibrateScale,
 	-1,
 	0,
 	evCalibrateLowChecker,
 	tfNull,
 	entryCalibrateLowState,
 	exitCalibrateLowState},
	 
 	{eStateCalibrateHigh,
 	eStateCalibrateScale,
 	-1,
 	0,
 	evCalibrateHighChecker,
 	tfNull,
 	entryCalibrateHighState,
 	exitCalibrateHighState},

 	{eStateTriacIdle,
 	eStateTriacOperating,
 	eStateEditIdle,
 	0,
 	evTriacIdleChecker,
 	tfNull,
 	entryTriacIdleState,
 	exitTriacIdleState},

 	{eStateEditIdle,
 	eStateTriacIdle,
 	-1,
 	0,
 	evEditIdleChecker,
 	tfNull,
 	entryEditIdleState,
 	exitEditIdleState},

 	{eStateEditAmps,
 	eStateTriacIdle,
 	-1,
 	0,
 	evEditAmpsChecker,
 	tfNull,
 	entryEditAmpsState,
 	exitEditAmpsState},
	 
	{eStateEditDuration,
 	eStateTriacIdle,
 	-1,
 	0,
 	evEditDurationChecker,
 	tfNull,
 	entryEditDurationState,
 	exitEditDurationState},

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

 	{eStateSetupAlarmYesNo,
 	eStateSetup,
 	-1,
 	0,
 	evSetupAlarmYesNoChecker,
 	tfNull,
 	entrySetupAlarmYesNoState,
 	exitSetupAlarmYesNoState},
	 
	{eStateSetupAlarmMinutes,
 	eStateSetup,
 	-1,
 	0,
 	evSetupAlarmMinutesChecker,
 	tfNull,
 	entrySetupAlarmMinutesState,
 	exitSetupAlarmMinutesState},

	{eStateTriacRunning,
 	eStateTriacOperating,
 	-1,
 	0,
 	evTriacRunningChecker,
 	tfNull,
 	entryTriacRunningState,
 	exitTriacRunningState},
	 
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


bool processTriacEvent(TStatechart* t,CJoesTriacEvent* ev)
{
	currentEvent = ev;
	return ProcessEvent(t);
}

