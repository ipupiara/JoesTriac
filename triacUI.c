
#include <avr/eeprom.h>
#include "triacUI.h"
#include "st7565r.h"
#include "TriacDefines.h"

void initUI()
{
	amps100 = eeprom_read_byte((uint8_t*)amps100EEPROMpos);	
	amps10 = eeprom_read_byte((uint8_t*)amps10EEPROMpos);
	amps = eeprom_read_byte((uint8_t*)ampsEEPROMpos);
	min10 = eeprom_read_byte((uint8_t*)min10EEPROMpos);
	min = eeprom_read_byte((uint8_t*)minEEPROMpos);
	sec10 = eeprom_read_byte((uint8_t*)sec10EEPROMpos);
	sec = eeprom_read_byte((uint8_t*)secEEPROMpos);
}

void displayRunningValues()
{
	
}


void displayCalibrationPrompt()
{
	
}

void calcDesiredAmps()
{
	desiredAmps = ((amps100 - 0x30) * 100 ) + ((amps10 -0x30) *10) + (amps - 0x30);
}

void calcDesiredTime()
{
	
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

void storeMin10(int8_t val)
{
	eeprom_write_byte((uint8_t*)min10EEPROMpos,val);
	min10 = val;	
}

void storeMin(int8_t val)
{
	eeprom_write_byte((uint8_t*)minEEPROMpos,val);
	min = val;	
}

void storeSec10(int8_t val)
{
	eeprom_write_byte((uint8_t*)sec10EEPROMpos,val);
	sec10 = val;	
}

void storeSec(int8_t val)
{
	eeprom_write_byte((uint8_t*)secEEPROMpos,val);
	sec = val;	
}


void setAmps100(int8_t val)
{
	storeAmps100(val);
	// set cursor to amps 10
}

void setAmps10(int8_t val)
{
	storeAmps10(val);
	// as method before
}

void setAmps(int8_t val)
{
	storeAmps(val);
	// amps is the last of this entry, so set cursor off, no abortEntry will be sent
}

void setMin10(int8_t val)
{
	storeMin10(val);
}

void setMin(int8_t val)
{
	storeMin(val);
}

void setSec10(int8_t val)
{
	storeSec10(val);
}

void setSec(int8_t val)
{
	storeSec(val);
}

void startDurationEntry()
{
	// set cursor at correct pos
}

void abortDurationEntry()
{
	// clear cursor
}

void startAmpsEntry()
{
	
}

void abortAmpsEntry()
{
	
}
