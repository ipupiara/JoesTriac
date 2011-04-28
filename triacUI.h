#ifndef triacUIh
#define triacUIh

#include <inttypes.h>

void displayRunningValues();

void displayCalibrationPrompt();

int8_t amps100, amps10, amps, min10, min, sec10, sec;

void setAmps100(int8_t val);

void setAmps10(int8_t val);

void setAmps(int8_t val);

void setMin10(int8_t val);

void setMin(int8_t val);

void setSec10(int8_t val);

void setSec(int8_t val);

void initUI();

void startDurationEntry();

void abortDurationEntry();

void startAmpsEntry();

void abortAmpsEntry();



#endif