/*
 * joesTriac.c
 *
 * Created: 18.04.2011 14:34:59
 *  Author: duenda
 */ 

#include <avr/io.h>
#include "st7565r.h"
#include "triacPID.h"
#include "triacUI.h"
#include "StateClass.h"
#include "TriacIntr.h"
#include "TriacKeyPad.h"

extern const uStInt u32HandlingDone;


extern TStatechart SHumidityStateChart;




void calibrate()
{
	
}


int main(void)
{
	CGrowBoxEvent ev;
	bool res;
	
	
	lcd_init();
	initExtInterrupts();
	createPID();
	displayCalibrationPrompt();
	//startCalibrationTimer();
	
	startStateCharts();

	ev.evType = eValueAssignement;
	ev.humidity = 85.0;

	res= processGrowBoxEvent(&SHumidityStateChart, &ev);
	
	
	while ((!charPressed()) && (!calibTimerReachead))  {	
	}
	if (!calibTimerReachead)  {
		//stopCalibrationTimer();
		calibrate();
	}
    while(1)
    {
        //TODO:: Please write your application code 
		
		
		
		
    }
}