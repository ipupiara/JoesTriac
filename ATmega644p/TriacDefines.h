

// TriacDefines.h contains global and some local defines, but not all of the local defines
// also it contains many of the global data definition and their persistent storage on eeprom

// Yoar Attention please, yoar attention: set Brown out level on 2.7V at least
// otherwise errors on the eeprom could be observed at startup

// PN, oct 2016, todo: the whole defines component should be refactored and all the data access on eeprom should be simplified 
// by definitions of the data in a table and generalized methods what would drastically reduce the amount of methods.
// also the edit methods should be refactored to string edit methods instead of the current character by character mode,
// which was grown out of the beginning, where just desiredAmps and deriredDuration had to be edited on the running screen.

#ifndef TriacDefines_H
	#define TriacDefines_H


#include <inttypes.h>
#include "miniString.h"

//#define jtDebugMode 

#ifdef jtDebugMode
	#define noI2C					// for debugging without I2C
	#define noScreen				// for debugging without screen
	#define jtagDebugKeyboardMode	// tobe set if jtag debug is used, it will divert the serial input (terminal)									//  to the keyboard and leave the keyboard port (port C) free for jtag
#endif					
				
//#define jtagDebugKeyboardMode   // for debugging using the jtag interface which needs the same port as keyboard.
								//  keyboard input can then be done via the pc keyboard in the terminal connected to uart0						

	#define maxSecsPossible  0x7FFF


	#define JT_F_CPU 11095200	
	
	#define amps100EEPROMpos    4   // 8-bit
	#define amps10EEPROMpos    5   // 8-bit
	#define ampsEEPROMpos    6   // 8-bit
	#define min10EEPROMpos    7   // 8-bit
	#define minEEPROMpos    8   // 8-bit
	#define sec10EEPROMpos    9   // 8-bit
	#define secEEPROMpos    10   // 8-bit
	#define calibLowAdcEEPROMpos   11
	#define calibHighAdcEEPROMpos  13   // both uint16
	
	#define ampsInputPinEEPROMpos         20     // uint8	

	#define calibLowAmps        15
	#define calibHighAmps       60

	#define   amtMiniStrings  8
	miniString  miniStringArray [amtMiniStrings];
	
	#define shortCircuitAlarmSecs10ArrPos   0
	#define shortCircuitAlarmAmpsArrPos  1
	#define completionAlarmMinsArrPos     2
	#define completionAlarmOnArrPos    3	
	#define dValueAlarmLowArrPos       4
	#define dValueAlarmHighArrPos      5
	#define dValueAlarmSec10ArrPos     6
	#define dValueAlarmFatalArrPos     7
	

	#define triggerDelayMaxTcnt2 810     // works properly with 50 Hz Ac and 11.0592 Mhz
	
	#define inductiveLoad   1      // to be set to 0 or 1
	#define triacTriggerLength   0      // delay approx ( n * 2.5 us ) + 5.5    at 11.0592  mhz
	#define delayBetweenTriacTriggers  12  //  in timer 2 tcnt values , prescaler at   128
	#define measuredRepetitionIntervalus  200;      //  approx   ( 1s  /(11.0592 E+6  /128) ) * delayBetweenTriacTriggers    [us]

// takes approx  2 us at 10 Mhz with -o0 (no opts. !) ,with all  call/ret, push N Pop
void delayEmptyProc (); 


void delay6pnt2d5us(unsigned int n);   // description in TriacDefines.c


// for simplicity reasons some of the "Database logic" and "Model logic" are implemented on the 
// Defines component. If ever the application should come to a certain complexity, refactoring on this should
// be done for clearer overview, but so far the current simplification seems acceptable

float desiredAmps;
int16_t desiredTimeS;



int16_t  calibLowADC;
int16_t  calibHighADC;  

uint16_t  calibLowTriacFireDuration;
uint16_t  calibHighTriacFireDuration;

int8_t   completionAlarmOn;
int8_t   completionAlarmMinutes;

int16_t  shortCircuitAlarmSecond10;
int16_t  shortCircuitAlarmAmps;

int16_t dValueAlarmHigh;
int16_t dValueAlarmLow;
int16_t  dValueAlarmSec10;
int8_t  dValueAlarmFatal;

int8_t amps100, amps10, amps, min10, min, sec10, sec;

uint8_t  zeroPotiPos;

uint8_t  ampsInputPin;

int8_t debugEvent1Triggered;    // signals that a debug event was triggered somewhere in the code
								// to inform the main-loop do do a specific action. This is used for
								// debugging without keypad (JTAG) to go into a specific state automatically.

int8_t fatalErrorOccurred;

char lastFatalErrorString [20];

void EEPROM_write(uint16_t uiAddress, unsigned char ucData);

unsigned char EEPROM_read(uint16_t uiAddress);

void calcDesiredAmps();

void storeAmps100(int8_t val);

//void checkEEPOROM();


void storeAmps10(int8_t val);


void storeAmps(int8_t val);

void calcDesiredTime();

void storeMin10(int8_t val);

void storeMin(int8_t val);

void storeSec10(int8_t val);

void storeSec(int8_t val);

void storeCalibLowADC();

void storeCalibHighADC();   

void saveCalibLowADC();

void saveCalibHighADC();

void storeCalibLowTriacFireDuration();

void storeCalibHighFireDuration();

//void storeZeroPotiPos(int8_t val);

void storeAmpsInputPin(int8_t val);


void restorePersistentData();

#define zeroAdjustATtinyID  0x10

enum zeroAdjustJobStates 
{
	jobIdle = 1,
	persistentZeroAdjust,
	volatileZeroAdjust,
	up10,
	up1,
	down10,
	down1,
	fatalError	
};


enum ampsLine
{  rms = 0,
	avg
};


#endif
