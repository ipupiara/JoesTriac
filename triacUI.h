#ifndef triacUIh
#define triacUIh

#include <inttypes.h>

void clr_scr();

void displayCalibrationPrompt();

void displayCalibratingExplanation();

void displayCalibrateLow();

void displayCalibrateHigh();

void displayCurrentAmps();

void displayCountDown();

void displayTriacRunning();

void displayEditAmpsDuration();

void displayAmps(int8_t kInd);

void displayTime(int8_t kInd);

void displayEditAmps();

void displayEditDuration();




void setAmps100(int8_t val);

void setAmps10(int8_t val);

void setAmps(int8_t val);

void setMin10(int8_t val);

void setMin(int8_t val);

void setSec10(int8_t val);

void setSec(int8_t val);

void initUI();

void displayDebugVoltageNTriggerDelay();

void displayVoltage();

#endif
