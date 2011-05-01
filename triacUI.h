#ifndef triacUIh
#define triacUIh

#include <inttypes.h>

void displayRunningValues();

void displayCalibrationPrompt();

void displayCalibratingExplanation();

void displayCalibrateLow();

void displayCalibrateHigh();

void displayCurrentAmps();

void displayCountDown();

float desiredAmps;
int16_t desiredTimeS;

int8_t amps100, amps10, amps, min10, min, sec10, sec;

void setAmps100(int8_t val);

void setAmps10(int8_t val);

void setAmps(int8_t val);

void setMin10(int8_t val);

void setMin(int8_t val);

void setSec10(int8_t val);

void setSec(int8_t val);

void initUI();


#endif