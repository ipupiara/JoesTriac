/*
 * miniString.h
 *
 * Created: 22.10.2016 13:46:16
 *  Author: mira
 */ 


#ifndef MINISTRING_H_
#define MINISTRING_H_

typedef struct  {
	int16_t   eepromPos;
	int8_t    length;
} miniString ;


int16_t calcMiniString(int16_t eepromPos);




#endif /* MINISTRING_H_ */