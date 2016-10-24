#ifndef triacUIh
#define triacUIh

#include <inttypes.h>

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

void displaySetup();

void displayAlarmMinutes(int8_t kInd);

void displayAlarmYesNo(int8_t kInd);

void displaySetupAlarmShortCircuit();

void displaySetupAlarmShortCircuitAmps(int8_t kInd);

void displaySetupAlarmShortCircuitSecs(int8_t kInd);

void clearSetupInputHint();

void toggleSetupInputHint();

void numericSetupInputHint();

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
