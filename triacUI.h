#ifndef triacUIh
#define triacUIh

#include <inttypes.h>

void clr_scr();

void displayRunningValues();

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


// for simplicity reasons some of the "Database logic" and "Model logic" are implemented on the 
// UI component. If ever the application should come to a certain complexity, refactoring on this should
// be done for clearer overview, but so far the current simplification seems acceptable

float desiredAmps;
int16_t desiredTimeS;

int16_t  calibLowADC;
int16_t  calibHighADC;  // for right now left in the UI component, later refactoring into model component should be considered


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
