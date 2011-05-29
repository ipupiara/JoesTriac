

#include "TriacDefines.h"


void delayEmptyProc ()
{
}



void delay6pnt2d5us(unsigned int n)
{
//delay approx ( n * 2.5 us ) + 6    at 11.0592  mhz  
// 2 Plus N Times  2 Dot 5

//n        t    tested on Simulator 2  (PN 28. May 2011)

//0		6
//1		8 - 9
//2		11
//4		16
//8		25 -26
//6		45   approx 500 cycles

//  accurate enough for our keyboard delays

unsigned int x;   // left this unused variable because method was tested like this 

  while(n--){
  }
} 

