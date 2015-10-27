

#ifndef TriacKeyPad_H
	#define TriacKeyPad_H

#include <avr/io.h>
#include <stdint.h>

enum  kpKey {
	kpStart = 1,
	kpStop,
	kpF1,
	kpF2,   
	kpAst,
	kpNum,
	kp0 = 0x30 ,
	kp1,
	kp2,
	kp3,
	kp4,
	kp5,
	kp6,
	kp7,
	kp8,
	kp9
};

//int8_t keyPressed();

int8_t keyEntered();

void initKeyPad();

int16_t  getKeybIntCnt();

void onTriacIdleSecondTick_Keyb();

#endif
