

// TriacDefines.h contains global and some local defines, but not all of the local defines
// also it contains many of the global data definition and their persistent storage on eeprom

// Yoar Attention please, yoar attention: set Brown out level on 2.7V at least
// otherwise errors on the eeprom could be observed at startup

#ifndef TriacDefines_H
	#define TriacDefines_H


#include <inttypes.h>


	#define maxSecsPossible  0x7FFF


	#define JT_F_CPU 11095200	
	
//	#define calibLowCmsEEPROMpos   0
//	#define calibHighCmsEEPROMpos  2   // both int16
	#define amps100EEPROMpos    4   // 8-bit
	#define amps10EEPROMpos    5   // 8-bit
	#define ampsEEPROMpos    6   // 8-bit
	#define min10EEPROMpos    7   // 8-bit
	#define minEEPROMpos    8   // 8-bit
	#define sec10EEPROMpos    9   // 8-bit
	#define secEEPROMpos    10   // 8-bit
	#define calibLowAdcEEPROMpos   11
	#define calibHighAdcEEPROMpos  13   // both uint16
	#define calibLowTriggerDelayEEPROMpos   15
	#define calibHighTriggerDelayEEPROMpos   17   // both uint16
//	#define zeroPotiPosEEPROMpos                19   // unit8  (obsolete, leaf place free )
	#define ampsInputPinEEPROMpos         20     // uint8
	#define completionAlarmOnEEPROMpos    21     // uint8
	#define completionAlarmMinsEEPROMpos  22
	#define completionAlarmMins10EEPROMpos  23


	#define calibLowAmps        15
	#define calibHighAmps       60

	#define triggerDelayMax 810     // works properly with 50 Hz Ac 

// takes approx  2 us at 10 Mhz with -o0 (no opts. !) ,with all  call/ret, push N Pop
void delayEmptyProc (); 


void delay6pnt2d5us(unsigned int n);   // description in TriacDefines.c


// for simplicity reasons some of the "Database logic" and "Model logic" are implemented on the 
// Defines component. If ever the application should come to a certain complexity, refactoring on this should
// be done for clearer overview, but so far the current simplification seems acceptable

float desiredAmps;
int16_t desiredTimeS;

// for right now, for simplicity reasons
// some of the data model stuff is left in the Defines component, 
//  later refactoring into model component should be considered
//  if application should grow over certain limits


int16_t  calibLowADC;
int16_t  calibHighADC;  

uint16_t  calibLowTriggerDelay;
uint16_t  calibHighTriggerDelay;

uint8_t   completionAlarmOn;
uint8_t   completionAlarmMins;
uint8_t   completionAlarmMins10;
uint8_t   completionAlarmMinutes;

int8_t amps100, amps10, amps, min10, min, sec10, sec;

uint8_t  zeroPotiPos;

uint8_t  ampsInputPin;

int8_t debugEvent1Triggered;    // signals that a debug event was triggered somewhere in the code
								// to inform the main-loop do do a specific action. This is used for
								// debugging without keypad (JTAG) to go into a specific state automatically.

int8_t fatalErrorOccurred;

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

void storeCalibLowTriggerDelay();

void storeCalibHighTriggerDelay();

//void storeZeroPotiPos(int8_t val);

void storeAmpsInputPin(int8_t val);

void storeCompletionAlarmOn(int8_t val);

void storeCompletionAlarmMins(int8_t val);

void storeCompletionAlarmMins10(int8_t val);

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
