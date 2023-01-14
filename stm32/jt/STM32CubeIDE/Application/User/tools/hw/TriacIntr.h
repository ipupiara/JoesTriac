

#ifndef TriacIntr_H
	#define TriacIntr_H

#ifdef  __cplusplus
extern "C"
{
#endif
	
#include <stdio.h>

#define maxSecsPossible  0xFFFFFFFF
	
//int8_t durationTimerReachead;
//int8_t runningSecondsTick;

//extern int64_t  secondCount;

#define stmTriggerDelayMax1000  1000    //  measures 10 ms, +- crystal tolerance
//#define stmTriggerDelayMax2000  2000  // not yet available
//#define stmTriggerDelayMax5000  5000   // not yet available

#ifdef stmTriggerDelayMax1000

#define kStepUnits  stmTriggerDelayMax1000
#define stmTriggerDelayMax  1000
#define kStepUnitsFactor  1
#define defaultTriacDelayPsc   940   // measured max delay psc,
									    //lays within ZX-detection range
										//  ie. max returned delay
#define triacDelayPsc  940   // (defaultTriacDelayPsc / kStepUnitsFactor)
										// effectively used max psc

#else

	#ifdef  stmTriggerDelayMax5000
		#define stmTriggerDelayMax  5000
		#define kStepUnits  stmTriggerDelayMax5000
		#define kStepUnitsFactor  5
		#define defaultTriacDelayPsc   940   // measured max delay psc,
												//lays within ZX-detection range
												//  ie. max returned delay
		#define triacDelayPsc  188   // (defaultTriacDelayPsc / kStepUnitsFactor)
												// effectively used max psc

	#else
		#ifdef  stmTriggerDelayMax2000

			#define stmTriggerDelayMax  2000
			#define kStepUnits  stmTriggerDelayMax2000
			#define kStepUnitsFactor  2
			#define defaultTriacDelayPsc   940   // measured max delay psc,
											//lays within ZX-detection range
											//  ie. max returned delay
			#define triacDelayPsc  470   // (defaultTriacDelayPsc / kStepUnitsFactor)

		#endif
	#endif
#endif



extern uint32_t secondsDurationTimerRemaining;  // todo check this memory direct access

//int8_t adcTick;
void startAmpsADC();
void stopAmpsADC();
void adcValueReceived(uint16_t adcVal);
uint32_t getCurrentAmpsADCValue();
void setCurrentAmpsADCValueNonIsr(uint32_t adcV );
float adcVoltage();

////void setDiffADC();
////void closeDiffADC();
//void startSingleADC();
//int16_t ampsADCValue();
//int16_t diffADCValue();
//double adcVoltage();


void startDurationTimer(uint32_t secs);
uint32_t getSecondsDurationTimerRemaining();
uint32_t getSecondsInDurationTimer();
void resumeDurationTimer();
void haltDurationTimer();
void stopDurationTimer();
void durationTimerTick();

void setBuzzerOn();
void setBuzzerOff();
void toggleBuzzer();

extern uint16_t triacTriggerDelay;
void setTriacTriggerDelay(int32_t cmsecs);
uint16_t getTriacTriggerDelay();

void startTriacRun();
void stopTriacRun();

void setCompletionAlarmOn();
void setCompletionAlarmOff();
void toggleCompletionAlarm();

void initTriacIntr();
//
//void printDValueVars();
//
///*
//opto coupler based 0x (zerocross) handling. optocoupler based diode opens at
//*/

#ifdef  __cplusplus
}
#endif


#endif
