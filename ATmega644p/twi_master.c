/* =============================================================================

    Copyright (c) 2006 Pieter Conradie [www.piconomic.co.za]
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    
    * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
    
    * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
    
    * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    Title:          Interrupt-driven TWI (I2C) Master
    Author(s):      Pieter Conradie
    Creation Date:  2007-03-31
    Revision Info:  $Id: twi_master.c 117 2010-06-24 20:21:28Z pieterconradie $

============================================================================= */

/* _____STANDARD INCLUDES____________________________________________________ */
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <compat/twi.h>
#include <avr/interrupt.h>

#include "TriacDefines.h"

/* _____PROJECT INCLUDES_____________________________________________________ */
#include "twi_master.h"

/* _____LOCAL DEFINITIONS____________________________________________________ */
/// Size TWI clock prescaler according to desired TWI clock frequency
#define TWI_PRESCALER    ((1<<TWPS1)|(0<<TWPS0))

/// Map TWI clock prescaler to prescaler value
#if   (TWI_PRESCALER == ((0<<TWPS1)|(0<<TWPS0)))
#define TWI_PRESCALER_VALUE 1
#elif (TWI_PRESCALER == ((0<<TWPS1)|(1<<TWPS0)))
#define TWI_PRESCALER_VALUE 4
#elif (TWI_PRESCALER == ((1<<TWPS1)|(0<<TWPS0)))
#define TWI_PRESCALER_VALUE 16
#elif (TWI_PRESCALER == ((1<<TWPS1)|(1<<TWPS0)))
#define TWI_PRESCALER_VALUE 64
#else
#error "Undefined TWI_PRESCALER value!"
#endif

/// Calculate TWI Baud rate value according to selected frequency and prescaler
#define TWI_BR_VALUE ((DIV_ROUND(F_CPU,TWI_FREQUENCY_HZ)-16ul)/(2ul*TWI_PRESCALER_VALUE))

/// Make sure that TWI Baud rate value is an 8-bit value
#ifndef __DOXYGEN__
#if (TWI_BR_VALUE > 255)
#error "Make TWI_PRESCALER_VALUE larger!"
#endif
#endif

/* _____LOCAL VARIABLES______________________________________________________ */
static u8_t twi_adr;
static u8_t *twi_data;
static u8_t twi_data_counter;
static u8_t twi_status;

/*
int16_t succeededTxAmt;
int16_t failedTxAmt;

int16_t succeededRxAmt;
int16_t failedRxAmt;


char tmpBuf [debugBuffLen];

void appendDebugChars(char* ch)
{
	strlcat(debugBuffer,ch,sizeof(debugBuffer));
}

void appendDebugSmallNum(char num1)
{
	debugBuffer[strlen(debugBuffer)] = (num1 + 0x30);
}

void checkDebugBuffer()
{
	
	cli();
//	memset(tmpBuf,0,sizeof(tmpBuf));
	memcpy(tmpBuf,debugBuffer,sizeof(tmpBuf));
	memset(debugBuffer,0,sizeof(debugBuffer));
	sei();
	if (strlen(tmpBuf) > 0) {
		printf("\n%s\n",tmpBuf);
	}
}
*/

/* _____PRIVATE FUNCTIONS____________________________________________________ */
/// TWI state machine interrupt handler
ISR(TWI_vect)
{
    switch(TW_STATUS)
    {
    case TW_START:
        // START has been transmitted

    case TW_REP_START:
        // REPEATED START has been transmitted
//appendDebugChars("strt");
        // Load data register with TWI slave address
        TWDR = twi_adr;
        // TWI Interrupt enabled and clear flag to send next byte
        TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
        break;

    case TW_MT_SLA_ACK:
        // SLA+W has been tramsmitted and ACK received

    case TW_MT_DATA_ACK:
        // Data byte has been tramsmitted and ACK received
//appendDebugChars("mtAk ");
        if(twi_data_counter != 0)
        {
//appendDebugChars("cgz");
            // Decrement counter
            twi_data_counter--;
            // Load data register with next byte
            TWDR = *twi_data++;
            // TWI Interrupt enabled and clear flag to send next byte
            TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
        }
        else
        {
#if TWI_REPEATED_START_SUPPORT
            // Disable TWI Interrupt
            TWCR = (0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);
#else
            // Initiate STOP condition after last byte; TWI Interrupt disabled
            TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);
#endif
            // Transfer finished
            twi_status = TWI_STATUS_DONE;

			twiDataSent = 1;        // added by pn 30aug11 to send event to main loop
//appendDebugChars("cz");
        }
        break;

    case TW_MR_DATA_ACK:
        // Data byte has been received and ACK tramsmitted
        // Buffer received byte
        *twi_data++ = TWDR;

        // Decrement counter
        twi_data_counter--;
//appendDebugSmallNum(twi_data_counter);
//appendDebugChars("DR ");

    case TW_MR_SLA_ACK:
        // SLA+R has been transmitted and ACK received
        // See if last expected byte will be received ...
//appendDebugChars("mrak ");

        if(twi_data_counter > 1)    
        {
            // Send ACK after reception
            TWCR = (1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
//appendDebugChars("cgz");
        }
        else
        {
			// Send NACK after next reception
            TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
//appendDebugChars("cz ");						
        }
        break;

    case TW_MR_DATA_NACK:
        // Data byte has been received and NACK tramsmitted
        // Buffer received byte
        *twi_data++ = TWDR;
        // Decrement counter

        twi_data_counter--;
//appendDebugSmallNum(twi_data_counter);
//appendDebugChars("mrnkDR ");

#if TWI_REPEATED_START_SUPPORT
        // Disable TWI Interrupt
        TWCR = (0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);
#else
        // Initiate STOP condition after last byte; TWI Interrupt disabled
        TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);
#endif
        // Transfer finished
        twi_status = TWI_STATUS_DONE;

		twiDataReceived = 1;    // added by pn 30aug11 to send event to main loop

        break;

    case TW_MT_ARB_LOST:
//appendDebugChars("arbLst");
        // Arbitration lost...
        // Initiate a (REPEATED) START condition; Interrupt enabled and flag cleared
        TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
        break;

    default:
//appendDebugChars("errDef");
        // Error condition; save status
        twi_status = TWSR;
        // Reset TWI Interface; disable interrupt
        TWCR = (0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);
    }
}

/* _____FUNCTIONS_____________________________________________________ */
void twi_init(void)
{

	twiDataSent = 0;
	twiDataReceived	= 0;  // both added by PN 30. Aug 2011 


/*	memset(debugBuffer,0,sizeof(debugBuffer));
	succeededTxAmt = 0;
	failedTxAmt  = 0;
	succeededRxAmt = 0;
	failedRxAmt = 0;
*/
	printf("TWI init\n");

#ifndef noI2C
    // Initialise variable
    twi_data_counter = 0;

    // Initialize TWI clock
    TWSR = TWI_PRESCALER;
    TWBR = TWI_BR_VALUE;

    // Load data register with default content; release SDA
    TWDR = 0xff;

    // Enable TWI peripheral with interrupt disabled
    TWCR = (0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);
#else
	twi_status = TWI_STATUS_DONE;
	printf("twi_init  no I2C started\n");
#endif		
}

void twi_resetAfterCrash()
{
	printf("twi reset after crash\n");
	//  TWI  disabled; 
#ifdef noI2C	
    TWCR = (0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(0<<TWEN)|(0<<TWIE);

	twiDataSent = 0;
	twiDataReceived	= 0;  // both added by PN 30. Aug 2011 

    // Initialise variable
    twi_data_counter = 0;

    // Load data register with default content; release SDA
    TWDR = 0xff;

    // Enable TWI peripheral with interrupt disabled
    TWCR = (0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);
#endif	
}



void twi_start_tx(u8_t adr, u8_t *data, u8_t bytes_to_send)

{
    // Wait for previous transaction to finish
//printf("start_tx bef while busy .. ");
    while(twi_busy())
    {
//        checkDebugBuffer();
    }
//printf("aft while busy\n");
#ifndef noI2C
	twiDataSent = 0;     // added by pn 30. aug 11

    // Copy address; clear R/~W bit in SLA+R/W address field
    twi_adr = adr & ~TW_READ;

    // Save pointer to data and number of bytes to send
    twi_data        = data;
    twi_data_counter = bytes_to_send;

    // Initiate a START condition; Interrupt enabled and flag cleared
    TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
#else
	twiDataSent = 1;	
	twi_status = TWI_STATUS_DONE;
#endif	
}

int8_t twi_synch_tx(u8_t adr, u8_t *data, u8_t bytes_to_send)
{
//	printf("twi_sync_tx entered\n");
//	while (res != TWI_STATUS_DONE ) {
		twi_start_tx(adr, data, bytes_to_send);
		while ((! twiDataSent ) &&  twi_busy()) {
//			checkDebugBuffer();
		}
		if (twi_status != TWI_STATUS_DONE) {
//			checkDebugBuffer();
//			failedTxAmt ++;
//			printf("stat not ok: %x restart aft crash..\n",twi_status);
			twi_resetAfterCrash();
		} else {
//			succeededTxAmt ++;
//			checkDebugBuffer();
//			printf("SUCCESS: sent ok\n"); 
		}
//	}
//	printf("twi_sync_tx leave res %x s/f %i / %i\n",twi_status, succeededTxAmt,failedTxAmt);
	return  twi_status;
}

void twi_start_rx(u8_t adr, u8_t *data, u8_t bytes_to_receive)
{
//printf("start_rx bef while busy .. ");
    // Wait for previous transaction to finish
    while(twi_busy())
    {
//        checkDebugBuffer();
    }
//printf("aft while busy\n");

	twiDataReceived = 0;   // added by PN 30 aug 11

#ifndef noI2C
    // Copy address; set R/~W bit in SLA+R/W address field
    twi_adr = adr | TW_READ;

    // Save pointer to data and number of bytes to receive
    twi_data         = data;
    twi_data_counter = bytes_to_receive;

    // Initiate a START condition; Interrupt enabled and flag cleared
    TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
#else
	twiDataReceived = 1;
	twi_status = TWI_STATUS_DONE;
	for (int8_t i1= 0; i1 < bytes_to_receive; ++ i1)
	{
		twi_data[i1] = i1;   // pn, 16. oct 2016 ok for our purpose
	}
#endif		
}


uint8_t twi_synch_rx(u8_t adr, u8_t *data, u8_t bytes_to_receive)
{
//	printf("twi_sync_rx entered\n");
//	while (res != TWI_STATUS_DONE ) {
		twi_start_rx(adr, data, bytes_to_receive);
		while ((! twiDataReceived ) &&  twi_busy()) {
//			checkDebugBuffer();
		}
		if (twi_status != TWI_STATUS_DONE) {
//			checkDebugBuffer();
//			failedRxAmt ++;
//			printf("stat not ok: %x restart aft crash..\n",twi_status);
			twi_resetAfterCrash();
		} else {
//			succeededRxAmt ++;
//			checkDebugBuffer();
//		+	printf("SUCCESS: synch_rx ok\n"); 
		}
//	}
//	printf("twi_sync_rx leave res %x s/f %i / %i\n",twi_status, succeededRxAmt,failedRxAmt);
	return  twi_status;
}

bool_t twi_busy(void)
{
#ifndef noI2C	
    // IF TWI Interrupt is enabled then the peripheral is busy
	return ( TWCR & (1<<TWIE) );  
#else
	return 0;
#endif
}

bool_t twi_success(void)
{
    if(twi_status == TWI_STATUS_DONE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

u8_t twi_get_status(void)
{
    return twi_status;
}

#if TWI_REPEATED_START_SUPPORT
void twi_stop(void)
{
    // Wait for transaction to finish
    while(twi_busy())
    {
        ;
    }

    // Make sure transaction was succesful
    if(twi_status != TWI_STATUS_DONE)
    {
        return;
    }
#ifndef noI2C
    // Initiate a STOP condition
    TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);

    // Wait until STOP has finished
    LOOP_UNTIL_BIT_IS_LO(TWCR, TWSTO);
#endif
}
#endif

/**
 *  @}
 */

/* _____LOG__________________________________________________________________ */
/*

 2007-03-31 : Pieter Conradie
 - First release
   
*/
