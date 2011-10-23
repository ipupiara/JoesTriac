
#include <stdio.h>
#include "TriacDefines.h"
#include "TriacIntr.h"
#define ownEepromMethods 

#ifndef ownEepromMethods
#include <avr/eeprom.h>
#else

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
	uint16_t adre = (uint16_t) adr;
	EEPROM_write(adre,*(&val));
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



void delay6pnt2d5us(unsigned int n)
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

uint8_t x;  
	x= 0;

  while(n--){
  }
} 




void calcDesiredAmps()
{
	desiredAmps = ((amps100 - 0x30) * 100 ) + ((amps10 -0x30) *10) + (amps - 0x30);
}

void storeAmps100(int8_t val)
{
	eeprom_write_byte((uint8_t*)amps100EEPROMpos,val);
	amps100 = val;
	calcDesiredAmps();
}

void storeAmps10(int8_t val)
{
	eeprom_write_byte((uint8_t*)amps10EEPROMpos,val);
	amps10 = val;	
	calcDesiredAmps();
}

void storeAmps(int8_t val)
{
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

void storeCalibLowTriggerDelay()
{
	calibLowTriggerDelay = triacTriggerDelayCms;
	eeprom_write_word((uint16_t *) calibLowTriggerDelayEEPROMpos, calibLowTriggerDelay);
}

void storeCalibHighTriggerDelay()
{
	calibHighTriggerDelay = triacTriggerDelayCms;
	eeprom_write_word((uint16_t *) calibHighTriggerDelayEEPROMpos, calibHighTriggerDelay);
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

void restorePersistentData()
{
	amps100 = eeprom_read_byte((uint8_t*)amps100EEPROMpos);	
	if ((amps100 < 0x30) || (amps100 > 0x39)) { storeAmps100(0x30);}   // use lazy initialization
	amps10 = eeprom_read_byte((uint8_t*)amps10EEPROMpos);
	if ((amps10 < 0x30) || (amps10 > 0x39)) { storeAmps10(0x30);}
	amps = eeprom_read_byte((uint8_t*)ampsEEPROMpos);
	if ((amps < 0x30) || (amps > 0x39)) { storeAmps(0x30);}
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
	calibLowADC = eeprom_read_word((uint16_t*) calibLowAdcEEPROMpos);
	if (calibLowADC == 0xFFFF) calibLowADC = 0x0000;
	calibHighADC = eeprom_read_word((uint16_t*) calibHighAdcEEPROMpos);
	if (calibHighADC == 0xFFFF) calibHighADC = 0x0000;   
	calibLowTriggerDelay = eeprom_read_word((uint16_t*) calibLowTriggerDelayEEPROMpos);
//	printf("\nrestore calibLowTD %x \n",calibLowTriggerDelay);
	if (calibLowTriggerDelay == 0xFFFF) calibLowTriggerDelay = 0x0000;
	calibHighTriggerDelay = eeprom_read_word((uint16_t*) calibHighTriggerDelayEEPROMpos);
	if (calibHighTriggerDelay == 0xFFFF) calibHighTriggerDelay = 0x0000;
//	zeroPotiPos = eeprom_read_byte((uint8_t*)zeroPotiPosEEPROMpos);	
//	if ((zeroPotiPos < 0x00) || (zeroPotiPos > 100)) { storeZeroPotiPos(0x00);}   
	ampsInputPin = eeprom_read_byte((uint8_t*)ampsInputPinEEPROMpos);	
	if ( (ampsInputPin < 0x00) || (ampsInputPin > 0x01)) { storeAmpsInputPin(0x00);}   

	debugEvent1Triggered = 0;
    fatalErrorOccurred = 0;
}


