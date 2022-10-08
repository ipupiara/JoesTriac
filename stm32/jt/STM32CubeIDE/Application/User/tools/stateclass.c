#include <stdio.h>
#include <defines.h>
#include "TStatechart.h"
#include "StateClass.h"
#include <TriacIntr.h>
#include <uart-comms.h>


extern const uStInt uStIntHandlingDone;
extern const uStInt uStIntNoMatch;

fsmTriacEvent* currentEvent;


#include "triacPID.h"

TStatechart SJoesTriacStateChart;
TStatechart* PJoesTriacStateChart;

// This defines and names the states the class has.
#warning  attention: sequence must be the same as in xaStates (below)  !!!
enum eStates
{
	eStateJoesTriac,
		eStartState = eStateJoesTriac,  // decide calib or idle
		eStateTriacOperating,
			eStateCalibrating,  // calibration/setup
				eStateManualCalibrating,   // manual
				eStateAutoCalibrating,		// autoCalibrate
					eStateCalibrateZeroSignal,
					eStateCalibratingScale,
						eStateCalibrateLow,
						eStateCalibrateHigh,
			eStateTriacIdle,
			eStateTriacRunning,
			eStateJobOkDisplay,
		eStateFatalError,
	eNumberOfStates
};


void entryJoesTriac()
{

}

void exitJoesTriac()
{

}

uStInt evJoesTriacChecker(void)
{
	return (uStIntNoMatch);
}


void entryTiacOperatingChecker()
{
	startDurationTimer(3);
}

void exitTiacOperatingChecker()
{
	stopDurationTimer();
}

uStInt evTriacOperatingChecker(void)   // during Start Screen
{
	uStInt res = uStIntNoMatch;
	info_printf("check for event in State evTriacOperating\n");

	if (currentEvent->evType == evFatalError) 
	{	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateFatalError);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
	}
	
	if (currentEvent->evType == evTimeOutDurationTimer)
	{
		if (isCalibrationReady() == tOk) {
 			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}  else {
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrating);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
	}

	return (res); 
}

void entryCalibratingState(void)
{
	printf("entry AskForCalibration\n");
//	displayCalibrationPrompt();
//	startDurationTimer(6);

//	startDurationTimer(maxSecsPossible);
}

void exitCalibratingState(void)
{
	printf("exit ask calib\n");
//	stopDurationTimer();
//	clr_scr();
}

uStInt evStateCalibratingChecker(void)
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
////			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrating);
//				// No event action.
//			END_EVENT_HANDLER(PJoesTriacStateChart);
//			res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evF1Pressed) 
	{	
////			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateChangeCalibVars);
//				// No event action.
//			END_EVENT_HANDLER(PJoesTriacStateChart);
//			res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evSecondsTick) 
	{	
//		displayCountDown();
		res =  uStIntHandlingDone;
//		debugEvent1Triggered = 1;
	}
	return (res); 
}

int8_t  calibVarInd;


void entryChangeCalibVarsState(void)
{
//	printf("entry AskForCalibration\n");
//	calibVarInd = 0;
//	currentVarVal = calibLowADC;
//	currentTitle = "calibLowADC";
//	displayCurrentVar();
}



void exitChangeCalibVarsState(void)
{
//	printf("exit ask calib\n");
//	updateGradAmps();
}

uStInt evChangeCalibVarsChecker(void)
{
	uStInt res = uStIntNoMatch;
//	printf("check for event in State evStateIdle\n");


	if ((currentEvent->evType == evAstPressed) || (currentEvent->evType == evNumPressed)){
		if (currentEvent->evType == evNumPressed)  {
//			if (calibVarInd == 0) {
////				calibLowADC = currentVarVal;
////				saveCalibLowADC();
//			}
//			if (calibVarInd == 1) {
//				calibHighADC = currentVarVal;
//				saveCalibHighADC();
//			}
		}

		if (calibVarInd == 1)   {	
			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateTriacIdle);
				// No event action.
			END_EVENT_HANDLER(PJoesTriacStateChart);
			res =  uStIntHandlingDone;
		}
		if (calibVarInd == 0) {
//			currentVarVal = calibHighADC;
//			currentTitle = "calibHighADC";
//			calibVarInd = 1;
//			displayCurrentVar();
		}
/*		if (calibVarInd == 1) {
			currentVarVal = zeroPotiPos;
			currentTitle = "zeroPotiPos";
			calibVarInd = 2;
			displayCurrentVar();
		}    // stored on assistant cpu atTiny.  needs message to tiny (i2c) and changes on tiny as well  */
		res =  uStIntHandlingDone;
	}

	if (currentEvent->evType == evCharEntered) {
//		switch (currentEvent->evData.keyCode) {
//			case kp1 :
//				currentVarVal++;
//				break;
//			case kp2 :
//				currentVarVal += 10;
//				break ;
//			case kp3 :
//				currentVarVal += 100;
//				break ;
//			case kp7 :
//				currentVarVal--;
//				break;
//			case kp8 :
//				currentVarVal -= 10;
//				break ;
//			case kp9 :
//				currentVarVal -= 100;
//				break ;
//		}
//		if (currentVarVal < 0) currentVarVal = 0;
//		if (currentVarVal > 1023) currentVarVal = 1023;
//		currentVarChanged();
//		res =  uStIntHandlingDone;
	}
	return (res); 
}

void entryAutoCalibratingState(void)
{
//	printf("\nentry Calib");
//	startDurationTimer(maxSecsPossible);   // enable secondsTick
}

void exitAutoCalibratingState(void)
{
//	printf("exit calib\n");
//	stopDurationTimer();
}

uStInt evAutoCalibratingChecker(void)
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
//	displayRmsAvgQuery();
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
		
//		storeAmpsInputPin(avg);

		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;		
	}

	if (currentEvent->evType == evAstPressed) 
	{	
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateZeroSignal);
		// No event action.

//		storeAmpsInputPin(rms);

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
//	printf("entryCalibrateZeroSignalState\n");
//	displayCalibrateZeroPotiPos();
//
//	stableZeroAdjReached = 0;
//    if (!setAdjustJob(persistentZeroAdjust)) {
//		sprintf((char *) &lastFatalErrorString,"i2c comms err");
//		fatalErrorOccurred = 1;
//	}
}

void exitCalibrateZeroSignalState(void)
{
//	printf("exitCalibrateZeroSignalState\n");
//   if (!fatalErrorOccurred) {
//	   if (!setAdjustJob(jobIdle)) {
//		   sprintf((char *) &lastFatalErrorString,"i2c comms err");
//		   fatalErrorOccurred = 1;
//	   }
//   }
}

uStInt checkCalibZeroInner(uStInt res)
// AVR Studio's produced code will not crash anymore
// when this is outplaced into inner method (method size problem ??)
// "silly problems, silly solutions"
{
	if (currentEvent->evType == evNumPressed) 
	{	
//		debugEvent1Triggered = 1;			
		
		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibratingScale);
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

//	res = checkCalibZeroInner(res);  // method only works if one part is placed out
									// no idea why (lenght of code ? )
	return (res);
}


void entryCalibrateScaleState(void)
{
//	printf("\nentry Calib");
//	startTriacRun();
}

void exitCalibrateScaleState(void)
{
//	printf("exit calib\n");
//	stopTriacRun();
}

uStInt evCalibrateScaleChecker(void)
{
////	int16_t triggerDelay;
	int8_t res = 0;
////
//	res = uStIntNoMatch;
//	if (currentEvent->evType == evCharEntered) {
////		triggerDelay = triacFireDurationTcnt2;
////		switch (currentEvent->evData.keyCode) {
////			case kp1 :
////				triggerDelay++;
////				break;
////			case kp2 :
////				triggerDelay += 10;
////				break ;
////			case kp3 :
////				triggerDelay += 100;
////				break ;
////			case kp7 :
////				triggerDelay--;
////				break;
////			case kp8 :
////				triggerDelay -= 10;
////				break ;
////			case kp9 :
////				triggerDelay -= 100;
////				break ;
////		}
////		if (triggerDelay < 0) triggerDelay = 0;
////		if (triggerDelay > triggerDelayMaxTcnt2) triggerDelay = triggerDelayMaxTcnt2;
////		setTriacFireDuration(triggerDelay);
////		displayDebugVoltageNTriggerDelay();
////		res =  uStIntHandlingDone;
//	}
	if (currentEvent->evType == evSecondsTick) 
	{	
//		displayDebugVoltageNTriggerDelay();
//		res =  uStIntHandlingDone;
	}
	return res;
}


void entryCalibrateLowState(void)
{
//	printf("entry calib Low\n");
//	displayCalibrateLow();
//	setTriacFireDuration(0);
}

void exitCalibrateLowState(void)
{
//	printf("exit calib Low\n");
//	clr_scr();
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
//			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateCalibrateHigh);
//
//			storeCalibLowADC();
//
//			END_EVENT_HANDLER(PJoesTriacStateChart);
//			res =  uStIntHandlingDone;
	}
	return res;
}


void entryCalibrateHighState(void)
{
//	printf("entry I\n");
//	displayCalibrateHigh();
}

void exitCalibrateHighState(void)
{
//	printf("exit I\n");
//	clr_scr();
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

//		storeCalibHighADC();
//		updateGradAmps();

		END_EVENT_HANDLER(PJoesTriacStateChart);
		res =  uStIntHandlingDone;
	}
	return res;
}

int8_t keyInd;


void entryTriacIdleState(void)
{
	info_printf("entryTriacIdleState\n");
//	Model::sendModelMessage(Model::)
////	debugEvent1Triggered = 1;
//	onEntryIdlePID();
//	startDurationTimer(maxSecsPossible);   // enable secondsTick
//    if (!setAdjustJob(volatileZeroAdjust)) {
//		sprintf((char *) &lastFatalErrorString,"i2c comms err");
//		fatalErrorOccurred = 1;
//	}
}

void exitTriacIdleState(void)
{
//	printf("exitTriacIdleState\n");
//	stopDurationTimer();
//    if (!fatalErrorOccurred) {
//		if (!setAdjustJob(jobIdle)) {
//			sprintf((char *) &lastFatalErrorString,"i2c comms err");
//			fatalErrorOccurred = 1;
//		}
//	}
//	clr_scr();
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
//		if (resetMiniStringComponent()) {
//			BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateSetupMiniString);
//
//			END_EVENT_HANDLER(PJoesTriacStateChart);
//			res =  uStIntHandlingDone;
//		}
	} 
	if (currentEvent->evType == evTimeOutDurationTimer) 
	{	
//		startDurationTimer(maxSecsPossible);   // enable secondsTick
		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evSecondsTick) 
	{	
//		onTriacIdleSecondTick_PID();
//		onTriacIdleSecondTick_Keyb();
		res =  uStIntHandlingDone;
	}
	return res;
}


void entryEditIdleState(void)
{
//	displayEditAmpsDuration();
//	displayAmps(-1);
//	displayTime(-1);
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
	
////		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditAmps);
//			// No event action.
//		END_EVENT_HANDLER(PJoesTriacStateChart);
//		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType==evNumPressed) {	
//		printf("\ncheck for event in State evStateIdle dur");

////		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditDuration);
//			// No event action.
//		END_EVENT_HANDLER(PJoesTriacStateChart);
//		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evTWIDataReceived) 
	// do writing  only in edit idle, not to interfere with the edit-cursor position
	// could be done also by laying edit-cursor pos "on stack" and set again after write, 
	 //  but lets first try with this simpler version ... should be ok as well
	{				
//		checkTWIZeroAdjustMsg();
//		displayPotiVolatile();
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
//	displayEditAmps();
//	keyInd = 0;
//	displayAmps(keyInd);
}

void exitEditAmpsState(void)
{
//	printf("exit I\n");
//	displayAmps(-1);
}

uStInt evEditAmpsChecker(void)
{
	uStInt res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evNumPressed)  {	
////		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditDuration);
//			// No event action.
//		END_EVENT_HANDLER(PJoesTriacStateChart);
//		res =  uStIntHandlingDone;
	}
			if (currentEvent->evType == evAstPressed)  {	
////		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
//			// No event action.
//		END_EVENT_HANDLER(PJoesTriacStateChart);
//		res =  uStIntHandlingDone;
	}

	if (currentEvent->evType == evCharEntered) {
		
	//  TODO:	editChar(currentEvent,storeAmps100,keyInd,3,Amps);  //( , , amps)  struct editLine (char,amtChar,zeroPos,storeMethod[])
	//

//		if ((currentEvent->evData.keyCode <= kp9) && (currentEvent->evData.keyCode >= kp0)) {
//			switch (keyInd)
//			{
//				case 0:
//					storeAmps100(currentEvent->evData.keyCode);
//					break;
//				case 1:
//					storeAmps10(currentEvent->evData.keyCode);
//					break;
//				case 2:
//					storeAmps(currentEvent->evData.keyCode);
//					BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
//
//					END_EVENT_HANDLER(PJoesTriacStateChart);
//			}
//			keyInd ++;
//			displayAmps(keyInd);  // if keyInd = 3, dispAmps can be done again in next State, no matter
//
//		res =  uStIntHandlingDone;
//		}
	}		
	return res;
}


void entryEditDurationState(void)
{
//	printf("entry I\n");
//	displayEditDuration();
//	keyInd = 0;
//	displayTime(keyInd);
}

void exitEditDurationState(void)
{
//	printf("exit I\n");
//	displayTime(-1);
}

uStInt evEditDurationChecker(void)
{
	uStInt res = uStIntNoMatch;
	//	printf("check for event in State evStateIdle\n");

	if (currentEvent->evType == evAstPressed)  {	
////		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditAmps);
//			// No event action.
//		END_EVENT_HANDLER(PJoesTriacStateChart);
//		res =  uStIntHandlingDone;
	}
	if (currentEvent->evType == evNumPressed)  {	
////		BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
//			// No event action.
//		END_EVENT_HANDLER(PJoesTriacStateChart);
//		res =  uStIntHandlingDone;
	}

	if (currentEvent->evType == evCharEntered) {

//		if ((currentEvent->evData.keyCode <= kp9) && (currentEvent->evData.keyCode >= kp0)) {
//			switch (keyInd)
//			{
//				case 0:
//					storeMin10(currentEvent->evData.keyCode);
//					break;
//				case 1:
//					storeMin(currentEvent->evData.keyCode);
//					keyInd++;
//					break;
//				case 3:
//					if (currentEvent->evData.keyCode <= kp5) {
//						storeSec10(currentEvent->evData.keyCode);
//					} else keyInd --;
//					break;
//				case 4:
//					storeSec(currentEvent->evData.keyCode);
//					BEGIN_EVENT_HANDLER(PJoesTriacStateChart, eStateEditIdle);
//
//					END_EVENT_HANDLER(PJoesTriacStateChart);
//			}
//			keyInd ++;
//			displayTime(keyInd);
//
//			res =  uStIntHandlingDone;
//		}
	}		
	return res;
}


void entryTriacRunningState(void)
{
//	printf("entry Running\n");
//	displayTriacRunning();
//	startDurationTimer(desiredTimeS);
////	startDurationTimer(maxSecsPossible);   // sometimes used for debugging
//	setTriacFireDuration(calibLowTriacFireDuration);  // start defined,  not just somewhere
//												// because of 220 V fuse ejects
//												// lowCalib seems better choice than 0
//	startTriacRun();
}

void exitTriacRunningState(void)
{
//	printf("exit Running\n");
//	stopDurationTimer();
//	stopTriacRun();
//	clr_scr();
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
//		displayCurrentAmps();
//		displayCountDown();
//		displayVoltage();
////		printDValueVars();
		res =  uStIntHandlingDone;
	}	
	if (currentEvent->evType == evAdcTick)
	{
//		calcNextTriacDelay();
//		res =  uStIntHandlingDone;
	}		
	return res;
}


void entryJobOkDisplayState(void)
{
//	printf("entry I\n");
//	displayJobOk();
//	startDurationTimer(maxSecsPossible);
}

void exitJobOkDisplayState(void)
{
//	printf("exit I\n");
//	setCompletionAlarmOff();
//	stopDurationTimer();
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
//		displayInDurationTimerSince();
//		if (completionAlarmOn) {
//			int16_t secondsRel = getSecondsInDurationTimer();
//			if (secondsRel >= completionAlarmMinutes * 60) {
//				toggleCompletionAlarm();
//			}
//		}
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

/*
eStateJoesTriac,
		eStartState = eStateJoesTriac,  // decide calib or idle
		eStateTriacOperating,
			eStateCalibration,  // calibration/setup
				eStateManualCalibrating,   // manual
				eStateAutoCalibrating,		// autoCalibrate
					eStateCalibrateZeroSignal,
					eStateCalibrateScale,
						eStateCalibrateLow,
						eStateCalibrateHigh,
			eStateTriacIdle,
			eStateTriacRunning,
			eStateJobOkDisplay,
		eStateFatalError,
	eNumberOfStates
*/

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
		-1,
		0,
		evTriacOperatingChecker,
		tfNull,
		entryTiacOperatingChecker,
		exitTiacOperatingChecker},

			{eStateCalibrating,
			eStateTriacOperating,
			eStateManualCalibrating,
			0,
			evStateCalibratingChecker,
			tfNull,
			entryCalibratingState,
			exitCalibratingState},

				{eStateManualCalibrating,
				eStateCalibrating,
				-1,
				0,
				evChangeCalibVarsChecker,
				tfNull,
				entryChangeCalibVarsState,
				exitChangeCalibVarsState},

				{eStateAutoCalibrating,
				eStateCalibrating,
				eStateCalibrateZeroSignal,
				0,
				evAutoCalibratingChecker,
				tfNull,
				entryAutoCalibratingState,
				exitAutoCalibratingState},

					{eStateCalibrateZeroSignal,
					eStateAutoCalibrating,
					-1,
					0,
					evCalibrateZeroSignalChecker,
					tfNull,
					entryCalibrateZeroSignalState,
					exitCalibrateZeroSignalState},

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


bool processTriacFsmEvent(TStatechart* t,fsmTriacEvent* ev)
{
	currentEvent = ev;
	return ProcessEvent(t);
}

