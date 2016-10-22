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

typedef void(*calcMenthodType)(void);

typedef void(*displayMethodType)(int8_t);


void editMiniString(int16_t miniStringArrayPos, calcMenthodType calcMeth, displayMethodType dispMeth);

void endEditMiniString();

void initMiniStringComponent();

// int16_t calcMiniString(int16_t miniStringArrPos);




#endif /* MINISTRING_H_ */