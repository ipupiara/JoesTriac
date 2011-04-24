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
#include "StateClass.h"
#include <avr/interrupt.h>


extern const uStInt u32HandlingDone;


extern TStatechart SHumidityStateChart;


int calibTimerReachead;

ISR(PCINT0_vect)
{
      
}   

ISR(PCINT1_vect)   
{   
            
}
  

void initExtInterrupts()
{
	//Port_initialize();    
	  DDRB = 0xFF;   
      DDRD = 0xf0;   
      PORTB = 0xff;   
	  
	  // INT_initialize(); 
	  EICRA = 0xaa;   //fall edge trigger   
   //   EICRB = 0x00;   //no influence   
      EIMSK = 0x0f;   
      
   //    SEI();   
}

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
	CGrowBoxEvent ev;
	bool res;
	
	
	lcd_init();
	initExtInterrupts();
	createPID();
	displayCalibrationPrompt();
	startCalibrationTimer();
	
	startStateCharts();

	ev.evType = eValueAssignement;
	ev.humidity = 85.0;

	res= processGrowBoxEvent(&SHumidityStateChart, &ev);
	
	
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