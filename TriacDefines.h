

// TriacDefines.h contains global and some local defines, but not all of the local defines
// also it contains many of the global data definition and their persistent storage on eeprom


#ifndef TriacDefines_H
	#define TriacDefines_H


#include <inttypes.h>


	#define maxSecsPossible  0x7FFF


	#define JT_F_CPU 11095200	
	
	#define calibLowCmsEEPROMpos   0
	#define calibHighCmsEEPROMpos  2   // both int16
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
	#define zeroPotiPosEEPROMpos                19   // unit8
	#define ampsInputPinEEPROMpos         20     // uint8

	#define calibLowAmps        20
	#define calibHighAmps       55

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

int16_t  calibLowTriggerDelay;
int16_t  calibHighTriggerDelay;

int8_t amps100, amps10, amps, min10, min, sec10, sec;

uint8_t  zeroPotiPos;

uint8_t  ampsInputPin;

int8_t debugEvent1Triggered;    // signals that a debug event was triggered somewhere in the code
								// to inform the main-loop do do a specific action. This is used for
								// debugging without keypad (JTAG) to go into a specific state automatically.

void calcDesiredAmps();

void storeAmps100(int8_t val);


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

void storeZeroPotiPos(int8_t val);

void restorePersistentData();



#endif
