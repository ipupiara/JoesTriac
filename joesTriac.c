/*
 * joesTriac.c
 *
 * Created: 18.04.2011 14:34:59
 *  Author: duenda
 */ 

#include <avr/io.h>


// The last character is a lower case ‘L’ not a 1 (one)
//#define F_CPU 1000000l     //  //   F_CPU 16000000
//#include <util/delay.h>

#include "st7565r.h"
#include "triacPID.h"
#include "triacUI.h"


int calibTimerReachead;

void startCalibrationTimer()
{
	calibTimerReachead = 0;
	
	
}

void stopCalibrationTimer()
{
	
}

void calibrate()
{
	
}


int main(void)
{
	lcd_init();
	createPID();
	displayCalibrationPrompt();
	startCalibrationTimer();
	while ((!charPressed()) && (!calibTimerReachead))  {	
	}
	if (!calibTimerReachead)  {
		stopCalibrationTimer();
		calibrate();
	}
	

    while(1)
    {
        //TODO:: Please write your application code 
    }
}