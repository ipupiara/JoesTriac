#ifndef triacUIh
#define triacUIh

#include <inttypes.h>

//#define shortCircuitAlarmSupported  1

typedef void(*setupPageDisplayMethodType)();

void clr_scr();

void displayCalibrationPrompt();


void displayCalibrateLow();

void displayCalibrateHigh();

void displayCurrentAmps();

void displayCountDown();

void displayInDurationTimerSince();

void displayTriacRunning();

void displayEditAmpsDuration();

void displayAmps(int8_t kInd);

void displayTime(int8_t kInd);

void displayEditAmps();

void displayEditDuration();

void displayJobOk();

void displayRmsAvgQuery();

void showCompletionAlarmSetup();

void writeCompletionAlarmMinutes(int8_t kInd);

void writeCompletionAlarmOn(int8_t kInd);

void showShortCircuitAlarmSetup();

void writeShortCircuitAlarmAmps(int8_t kInd);

void writeShortCircuitAlarmSec10(int8_t kInd);

void showDValueAlarmLowHighSetup();

void writeDValueLow(int8_t kInd);

void writeDValueHigh(int8_t kInd);

void showDValueAlarmSec10FatalSetup();

void writeDValueAlarmSec10(int8_t kInd);

void writeDValueAlarmFatal(int8_t kInd);

void clearSetupInputHint();

void inputHintNumeric(int8_t upperLimit);

void initUI();

void displayDebugVoltageNTriggerDelay();

void displayPotiPersistent();

void displayPotiVolatile();

void displayCalibrateZeroPotiPos();

void displayVoltage();

void displayFatalError();

int16_t currentVarVal;
char*   currentTitle;

void displayCurrentVar();
void currentVarChanged();


#endif
