

#ifndef TriacKeyPad_H
	#define TriacKeyPad_H

#include <avr/io.h>
#include <stdint.h>

enum  kpKey {
	kpStart,
	kpStop,
	kpFunction,
	kpRed,
	kpWhite,
	kp0,
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





#endif
