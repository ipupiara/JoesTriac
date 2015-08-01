
#include <stdio.h>
#include "TriacDefines.h"
#include "TriacIntr.h"
#define ownEepromMethods 

#ifndef ownEepromMethods
#include <avr/eeprom.h>
#else

// ATTENTION: use of EEPROM needs BOD Level of at least 2.7 V, otherwise EEPROM memory
// is likely to crash on restore when done at mcu startup

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	// Wait for completion of previous write 
	while(EECR & (1<<EEPE));
	// Set up address and Data Registers 
	EEAR = uiAddress;
	EEDR = ucData;
	cli();
	/*
	// Write logical one to EEMPE 
	EECR |= (1<<EEMPE);
	// Start eeprom write by setting EEPE 
	EECR |= (1<<EEPE);
	*/

	// standard EEPROM code does not work under all optimization levels (-o directive of compiler)
    // for timing reasons (max. 4 cpu cycles between the two sbi commands for safety reasons)
	 asm volatile (
     "sbi 0x1f,0x02" "\r\n"
     "sbi 0x1f,0x01" "\r\n"
       );
	sei();
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	// Wait for completion of previous write 
	while(EECR & (1<<EEPE))
	;
	// Set up address register 
	EEAR = uiAddress;
	// Start eeprom read by writing EERE 
	EECR |= (1<<EERE);
	// Return data from Data Register 
	return EEDR;
}

void eeprom_write_byte (uint8_t *adr, uint8_t val)
{
	uint8_t checkRes = 0;
	uint16_t adre = (uint16_t) adr;
	EEPROM_write(adre,*(&val));
	checkRes = EEPROM_read(adre); 
	if (val != checkRes) {
		printf("eeprom stored %X, but check returned %X\n",val,checkRes);
	} else {
		printf("reread ok returned %X\n",checkRes);
	}
}

uint8_t eeprom_read_byte (const uint8_t *adr)
{
	unsigned char bu;
	uint16_t adre = (uint16_t) adr;
	bu =  EEPROM_read(adre); 
	return bu;
}

uint16_t eeprom_read_word (uint16_t* adr)
{	
	uint16_t res;
	unsigned char bu;
	uint16_t adre;
	adre = (uint16_t) adr; 

	res = 0;
	bu = EEPROM_read(adre); 
	*(&res) = bu;
	bu = EEPROM_read(adre + 1); 
	*((unsigned char*)&res+1) = bu;

	return res;
}

void eeprom_write_word(uint16_t* adr, uint16_t val)
{
	uint16_t adre;
	adre = (uint16_t) adr; 
	EEPROM_write(adre,*(&val));
	EEPROM_write(adre + 1,*((unsigned char*)&val + 1));
}


#endif

void delayEmptyProc ()
{
}



void delay6pnt2d5us(unsigned int enn)
{
//delay approx ( n * 2.5 us ) + 6    at 11.0592  mhz  
//    6 Plus N Times  2 Dot 5  (  "6pnt2d5"  )

//n        t    tested on Simulator 2  (PN 28. May 2011)

//0		6
//1		8 - 9
//2		11
//4		16
//8		25 -26
//16	45   approx 500 cycles

//  accurate enough for our keyboard delays

uint8_t ix;  
	ix= 0;

  while(enn--){
  }
} 

/*
void checkEEPOROM()
{
	int8_t by1, by2, by3;
	by1 = eeprom_read_byte((uint8_t*)amps100EEPROMpos);	
	printf("restore check amps100 %X\n",by1);
	by2 = eeprom_read_byte((uint8_t*)amps10EEPROMpos);
	printf("restore check amps10 %X\n",by2);
	by3 = eeprom_read_byte((uint8_t*)ampsEEPROMpos);
	printf("restore che k amps %X\n",by3);
}
*/



void calcDesiredAmps()
{
	desiredAmps = ((amps100 - 0x30) * 100 ) + ((amps10 -0x30) *10) + (amps - 0x30);
}

void storeAmps100(int8_t val)
{
	printf("storeAmps100 %X\n",val);
	eeprom_write_byte((uint8_t*)amps100EEPROMpos,val);
	amps100 = val;
	calcDesiredAmps();
}

void storeAmps10(int8_t val)
{
	printf("storeAmps10 %X\n",val);
	eeprom_write_byte((uint8_t*)amps10EEPROMpos,val);
	amps10 = val;	
	calcDesiredAmps();
}

void storeAmps(int8_t val)
{
	printf("storeAmps %X\n",val);
	eeprom_write_byte((uint8_t*)ampsEEPROMpos,val);
	amps = val;	
	calcDesiredAmps();
}

void calcDesiredTime()
{
	desiredTimeS = ((min10 - 0x30) * 600 ) + ((min - 0x30) *60) + ((sec10 - 0x30) * 10) + (sec - 0x30);
}

void storeMin10(int8_t val)
{
	eeprom_write_byte((uint8_t*)min10EEPROMpos,val);
	min10 = val;	
	calcDesiredTime();
}

void storeMin(int8_t val)
{
	eeprom_write_byte((uint8_t*)minEEPROMpos,val);
	min = val;	
	calcDesiredTime();
}

void storeSec10(int8_t val)
{
	eeprom_write_byte((uint8_t*)sec10EEPROMpos,val);
	sec10 = val;	
	calcDesiredTime();
}

void storeSec(int8_t val)
{
	eeprom_write_byte((uint8_t*)secEEPROMpos,val);
	sec = val;
	calcDesiredTime();		
}

void storeCalibLowADC()
{
	calibLowADC = ampsADCValue();
	eeprom_write_word((uint16_t *) calibLowAdcEEPROMpos, calibLowADC);

}

void storeCalibHighADC()
{
	calibHighADC = ampsADCValue();
	eeprom_write_word((uint16_t *) calibHighAdcEEPROMpos, calibHighADC);
}    

void saveCalibLowADC()
{
	eeprom_write_word((uint16_t *) calibLowAdcEEPROMpos, calibLowADC);
}

void saveCalibHighADC()
{
	eeprom_write_word((uint16_t *) calibHighAdcEEPROMpos, calibHighADC);
}    


void storeCalibLowTriacFireDuration()
{
	calibLowTriacFireDuration = triacFireDurationTcnt2;
	eeprom_write_word((uint16_t *) calibLowTriacFireDurationEEPROMpos, calibLowTriacFireDuration);
}

void storeCalibHighFireDuration()
{
	calibHighTriacFireDuration = triacFireDurationTcnt2;
	eeprom_write_word((uint16_t *) calibHighTriacFireDurationEEPROMpos, calibHighTriacFireDuration);
}
/*
void storeZeroPotiPos(int8_t val)
{
	zeroPotiPos = val;
	eeprom_write_byte((uint8_t *) zeroPotiPosEEPROMpos, zeroPotiPos);
}
*/

void storeAmpsInputPin(int8_t val)
{
	ampsInputPin = val;
	eeprom_write_byte((uint8_t *) ampsInputPinEEPROMpos, ampsInputPin);
}

void calcCompletionAlarmMinutes()
{
	completionAlarmMinutes = ((completionAlarmMins10 -0x30) *10) + (completionAlarmMins - 0x30);
}

void storeCompletionAlarmOn(int8_t val)
{
	completionAlarmOn = val;
	eeprom_write_byte((uint8_t *) completionAlarmOnEEPROMpos, val);
}

void storeCompletionAlarmMins(int8_t val)
{
	completionAlarmMins = val;
	eeprom_write_byte((uint8_t *) completionAlarmMinsEEPROMpos, val);
	calcCompletionAlarmMinutes();
}

void storeCompletionAlarmMins10(int8_t val)
{
	completionAlarmMins10 = val;
	eeprom_write_byte((uint8_t *) completionAlarmMins10EEPROMpos, val);
	calcCompletionAlarmMinutes();
}


void restorePersistentData()
{
	amps100 = eeprom_read_byte((uint8_t*)amps100EEPROMpos);	
	printf("restore amps100 %X\n",amps100);
	if ((amps100 < 0x30) || (amps100 > 0x39)) { 
		printf("amps100 not valid restored %X\n", amps100);
		storeAmps100(0x30);
	}   // use lazy initialization
	amps10 = eeprom_read_byte((uint8_t*)amps10EEPROMpos);
	printf("restore amps10 %X\n",amps10);
	if ((amps10 < 0x30) || (amps10 > 0x39)) { 
		printf("amps10 not valid restored %X\n", amps10);
		storeAmps10(0x30);
	}
	amps = eeprom_read_byte((uint8_t*)ampsEEPROMpos);
	printf("restore amps %X\n",amps);
	if ((amps < 0x30) || (amps > 0x39)) { 
		printf("amps not valid restored %X\n", amps);
		storeAmps(0x30);
	}
	printf("amps done\n");
	min10 = eeprom_read_byte((uint8_t*)min10EEPROMpos);
	if ((min10 < 0x30) || (min10 > 0x39)) { storeMin10(0x30);}
	min = eeprom_read_byte((uint8_t*)minEEPROMpos);
	if ((min < 0x30) || (min > 0x39)) { storeMin(0x30);}
	sec10 = eeprom_read_byte((uint8_t*)sec10EEPROMpos);
	if ((sec10 < 0x30) || (sec10 > 0x35)) { storeSec10(0x30);}
	sec = eeprom_read_byte((uint8_t*)secEEPROMpos);
	if ((sec < 0x30) || (sec > 0x39)) { storeSec(0x30);}
	calcDesiredTime();
	calcDesiredAmps();

	completionAlarmMins = eeprom_read_byte((uint8_t*)completionAlarmMinsEEPROMpos);
	if ((completionAlarmMins < 0x30) || (completionAlarmMins > 0x39)) { storeCompletionAlarmMins(0x30);}
	completionAlarmMins10 = eeprom_read_byte((uint8_t*)completionAlarmMins10EEPROMpos);
	if ((completionAlarmMins10 < 0x30) || (completionAlarmMins10 > 0x39)) { storeCompletionAlarmMins10(0x30);}
	calcCompletionAlarmMinutes();
	completionAlarmOn = eeprom_read_byte((uint8_t*)completionAlarmOnEEPROMpos);
	if ((completionAlarmOn < 0x00) || (completionAlarmOn > 0x01)) { storeCompletionAlarmOn(0x00);}


	calibLowADC = eeprom_read_word((uint16_t*) calibLowAdcEEPROMpos);
	if (calibLowADC == 0xFFFF) calibLowADC = 0x0000;
	calibHighADC = eeprom_read_word((uint16_t*) calibHighAdcEEPROMpos);
	if (calibHighADC == 0xFFFF) calibHighADC = 0x0000;   
	calibLowTriacFireDuration = eeprom_read_word((uint16_t*) calibLowTriacFireDurationEEPROMpos);
//	printf("\nrestore calibLowTD %x \n",calibLowTriggerDelay);
	if (calibLowTriacFireDuration == 0xFFFF) calibLowTriacFireDuration = 0x0000;
	calibHighTriacFireDuration = eeprom_read_word((uint16_t*) calibHighTriacFireDurationEEPROMpos);
	if (calibHighTriacFireDuration == 0xFFFF) calibHighTriacFireDuration = 0x0000;
//	zeroPotiPos = eeprom_read_byte((uint8_t*)zeroPotiPosEEPROMpos);	
//	if ((zeroPotiPos < 0x00) || (zeroPotiPos > 100)) { storeZeroPotiPos(0x00);}   
	ampsInputPin = eeprom_read_byte((uint8_t*)ampsInputPinEEPROMpos);	
	if ( (ampsInputPin < 0x00) || (ampsInputPin > 0x01)) { storeAmpsInputPin(0x00);}   

	debugEvent1Triggered = 0;
    fatalErrorOccurred = 0;
}


