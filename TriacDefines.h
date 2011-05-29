

// TriacDefines.h contains global and some local defines, but not all of the local defines


#ifndef TriacDefines_H
	#define TriacDefines_H

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
	#define calibHighAdcEEPROMpos  13   // both int16

// takes approx  2 us at 10 Mhz with -o0 (no opts. !) ,with all  call/ret, push N Pop
void delayEmptyProc (); 


void delay6pnt2d5us(unsigned int n);   // description in TriacDefines.c


#endif
