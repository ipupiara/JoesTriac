// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
*
* Atmel Corporation
*
* File              : USI_TWI_Slave.c
* Compiler          : IAR EWAAVR 4.11A
* Revision          : $Revision: 1.14 $
* Date              : $Date: Friday, December 09, 2005 17:25:38 UTC $
* Updated by        : $Author: jtyssoe $
*
* Support mail      : avr@atmel.com
*
* Supported devices : All device with USI module can be used.
*
* AppNote           : AVR312 - Using the USI module as a I2C slave
*
* Description       : Functions for USI_TWI_receiver and USI_TWI_transmitter.
*
*
****************************************************************************/

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "USI_TWI_Slave.h"
#include "ATtinyjt.h"

/*! Static Variables
 */

static unsigned char TWI_slaveAddress;
static volatile unsigned char USI_TWI_Overflow_State;


#define START_TWI_TIMER()                    \
{											\
	t0Cnt = 0x00;							\
	TCNT0 = 0x00;									\
	TCCR0B = 0x00 | (1<<CS02) | (1<<CS00);     /* prescaler 1024 and started   */  \
}   


#define STOP_TWI_TIMER()			\
{									\
	TCCR0B = 0x00;					\
	t0Cnt = 0;						\
}





void initTWITimer()
{
	      TCCR0A = 0x00;  //  Normal mode

	      TCNT0 = 0x00 ;  
	  	  
		TCCR0B = 0x00  ; // Normal mode , not yet started	  
		TIMSK0  = 0x00 | (1<< TOIE0);

}


ISR(TIM0_OVF_vect)
{
//PINA |= 0x08;
	++t0Cnt;
	if (t0Cnt == t0MaxCnt) {
		STOP_TWI_TIMER();
		USISR |= (1<<USI_START_COND_INT);   // clear flag 
		SET_USI_TO_TWI_START_CONDITION_MODE();
		
//PORTA &= ~0x08;
	}
}



//********** USI_TWI functions **********//

/*! \brief
 * Initialise USI for TWI Slave mode.
 */
void USI_TWI_Slave_Initialise( unsigned char TWI_ownAddress )
{
	
//	DDRA |= 0x8f;
//	PORTA |= 0x8f;  // pn debug lines a0..a3 switched to leds


	 initTWITimer();   


  TWI_slaveAddress = TWI_ownAddress;

  DDR_USI |= ( 1 << PORT_USI_SCL ) | ( 1 << PORT_USI_SDA );	  // Set SCL and SDA as output
  PORT_USI |= ( 1 << PORT_USI_SCL );  // Set SCL high
  PORT_USI |= ( 1 << PORT_USI_SDA );  // Set SDA high
  DDR_USI &= ~( 1 << PORT_USI_SDA );  // Set SDA as input

  USICR    =  (1<<USISIE)|(0<<USIOIE)|                            // Enable Start Condition Interrupt. Disable Overflow Interrupt.
              (1<<USIWM1)|(0<<USIWM0)|                            // Set USI in Two-wire mode. No USI Counter overflow prior
                                                                  // to first Start Condition (potentail failure)
              (1<<USICS1)|(0<<USICS0)|(0<<USICLK)|                // Shift Register Clock Source = External, positive edge
              (0<<USITC);
  USISR    = 0xF0; 
                                                // Clear all flags and reset overflow counter
	sei();
//	PORTA &= 0xF0;
//	PORTA |= 0x0F;
}



/*! \brief Usi start condition ISR
 * Detects the USI_TWI Start Condition and intialises the USI
 * for reception of the "TWI Address" packet.
 */

ISR(USI_START_VECTOR)
{
	START_TWI_TIMER();
//	PORTA |= 0x0f;
                                              // Not necessary, but prevents warnings
// Set default starting conditions for new TWI package
    USI_TWI_Overflow_State = USI_SLAVE_CHECK_ADDRESS;
    DDR_USI  &= ~(1<<PORT_USI_SDA);     // Set SDA as input

	// Wait for SCL to go low to ensure the Start Condition has completed (the
	// Start detector will hold SCL low ) - if a Stop Condition arises then leave
	// The interrupt to prevent waiting forever - don't use USISR to test for Stop
	// Condition as in Application Note AVR312 because the Stop Condition Flag is
	// going to be set from the last TWI sequence
	
	while ((PIN_USI & (1<<PIN_USI_SCL)) &&	!((PIN_USI & (1<<PIN_USI_SDA))));// SCL his high and SDA is low

	if ( !(PIN_USI & (1<<PIN_USI_SDA))) {	// A Stop Condition did not occur

	    USICR   =   (1<<USISIE)|(1<<USIOIE)|                            // Enable Overflow and Start Condition Interrupt. (Keep StartCondInt to detect RESTART)
	                (1<<USIWM1)|(1<<USIWM0)|                            // Set USI in Two-wire mode.
	                (1<<USICS1)|(0<<USICS0)|(0<<USICLK)|                // Shift Register Clock Source = External, positive edge
	                (0<<USITC);
	    USISR  =    (1<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Clear flags
	                (0x0<<USICNT0);                                     // Set USI to sample 8 bits i.e. count 16 external pin toggles.

	}  else {
		USISR |= (1<<USI_START_COND_INT);   // clear flag and release scl (wm 10)    
		SET_USI_TO_TWI_START_CONDITION_MODE();

	}
}


/*! \brief USI counter overflow ISR
 * Handels all the comunication. Is disabled only when waiting
 * for new Start Condition.
 */

ISR(USI_OVERFLOW_VECTOR)
{

	
  switch (USI_TWI_Overflow_State)
  {

    // ---------- Address mode ----------
    // Check address and send ACK (and next USI_SLAVE_SEND_DATA) if OK, else reset USI.
    case USI_SLAVE_CHECK_ADDRESS:
      if ((USIDR == 0) || (( USIDR & ~1 ) == TWI_slaveAddress))
      {

//PORTA   &= ~0x01;

        if ( USIDR & 0x01 )  {
          	USI_TWI_Overflow_State = USI_SLAVE_SEND_DATA;

			i2c_rdlen = 6;  // pn 30.aug 11  set this here, always the same message sent
			i2c_rdptr = 0;
	    } else {

          	USI_TWI_Overflow_State = USI_SLAVE_REQUEST_DATA;

			i2c_wrlen = 0;  //PN. 30aug11 it is always the same message received 
		}
          SET_USI_TO_SEND_ACK();
      }
      else
      {
        SET_USI_TO_TWI_START_CONDITION_MODE();
      }
      break;

    // ----- Master write data mode ------
    // Check reply and goto USI_SLAVE_SEND_DATA if OK, else reset USI.
    case USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA:
      if ( USIDR ) // If NACK, the master does not want more data.
      {
//PORTA |= 0x07;
        SET_USI_TO_TWI_START_CONDITION_MODE();
        return;
      }
      // From here we just drop straight into USI_SLAVE_SEND_DATA if the master sent an ACK
//PORTA &= ~0x04;
    // Copy data from buffer to USIDR and set USI to shift byte. Next USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA
    case USI_SLAVE_SEND_DATA:

      // Get data from Buffer

		USIDR = i2c_rdbuf[i2c_rdptr];
		if (++i2c_rdptr >= i2c_rdlen)			// PN 4.SEPT11, replaced original buffer handling
			i2c_rdptr = 0;


		USI_TWI_Overflow_State = USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA;
		SET_USI_TO_SEND_DATA();
		break;

    // Set USI to sample reply from master. Next USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA
    case USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA:

//PORTA &= ~0x02;

      USI_TWI_Overflow_State = USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA;
      SET_USI_TO_READ_ACK();
      break;

    // ----- Master read data mode ------
    // Set USI to sample data from master. Next USI_SLAVE_GET_DATA_AND_SEND_ACK.
    case USI_SLAVE_REQUEST_DATA:
      USI_TWI_Overflow_State = USI_SLAVE_GET_DATA_AND_SEND_ACK;
		if (i2c_wrlen >= 1) {        // pn 4.sept 11, in this app,  messages to slave contain olny 1 byte  

//PORTA |= 0x07;	


			SET_USI_TO_TWI_START_CONDITION_MODE();
		} else {
      		SET_USI_TO_READ_DATA();

//PORTA &= ~0x02;
	  	}
      break;

    // Copy data from USIDR and send ACK. Next USI_SLAVE_REQUEST_DATA
    case USI_SLAVE_GET_DATA_AND_SEND_ACK:
      // Put data into Buffer

//PORTA &= ~0x04;

		// PN 4.sept11 changed buffer code
		if (i2c_wrlen < I2C_WRSIZE)	{		
			i2c_wrbuf[i2c_wrlen++] = USIDR;
		}

		jobBuffer = USIDR;

      USI_TWI_Overflow_State = USI_SLAVE_REQUEST_DATA;
      SET_USI_TO_SEND_ACK();
      break;
  }
}
