// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
*
* Atmel Corporation
*
* File              : USI_TWI_Slave.h
* Compiler          : IAR EWAAVR 4.11A
* Revision          : $Revision: 1.14 $
* Date              : $Date: Friday, December 09, 2005 17:25:38 UTC $
* Updated by        : $Author: jtyssoe $
*
* Support mail      : avr@atmel.com
*
* Supported devices : All device with USI module can be used.
*                     The example is written for the ATmega169, ATtiny26 & ATtiny2313
*
* AppNote           : AVR312 - Using the USI module as a TWI slave
*
* Description       : Header file for USI_TWI driver
*
*
*
****************************************************************************/
//! Prototypes
void          USI_TWI_Slave_Initialise( unsigned char );

// pn 4sept11 removed some original methods that were not needed

void          Timer_Init(void);

#define TRUE                1
#define FALSE               0

typedef     unsigned char       uint8_t;

//////////////////////////////////////////////////////////////////
///////////////// Driver Buffer Definitions //////////////////////
//////////////////////////////////////////////////////////////////

// PN 4.SEPT 11 Buffer definitions changed , because original buffer didnt match
// requirements of our app  



#define I2C_RDSIZE	8	// slave => master buffer size
#define I2C_WRSIZE	4	// master => slave buffer size

 uint8_t	i2c_rdbuf[I2C_RDSIZE];
 uint8_t	i2c_rdlen;
 uint8_t	i2c_wrbuf[I2C_WRSIZE];
 uint8_t	i2c_wrlen;
 uint8_t	i2c_rdptr;






// end changes PN


#define USI_SLAVE_CHECK_ADDRESS                (0x00)
#define USI_SLAVE_SEND_DATA                    (0x01)
#define USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA (0x02)
#define USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA   (0x03)
#define USI_SLAVE_REQUEST_DATA                 (0x04)
#define USI_SLAVE_GET_DATA_AND_SEND_ACK        (0x05)


//! Device dependent defines
#if defined(__AT90tiny26__) | defined(__ATtiny26__)
    #define DDR_USI             DDRB
    #define PORT_USI            PORTB
    #define PIN_USI             PINB
    #define PORT_USI_SDA        PORTB0
    #define PORT_USI_SCL        PORTB2
    #define PIN_USI_SDA         PINB0
    #define PIN_USI_SCL         PINB2
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_STRT_vect
    #define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

#if defined(__AT90Tiny2313__) | defined(__ATtiny2313__)
    #define DDR_USI             DDRB
    #define PORT_USI            PORTB
    #define PIN_USI             PINB
    #define PORT_USI_SDA        PORTB5
    #define PORT_USI_SCL        PORTB7
    #define PIN_USI_SDA         PINB5
    #define PIN_USI_SCL         PINB7
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_STRT_vect
    #define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

#if defined(__ATtiny25__) | defined(__ATtiny45__) | defined(__ATtiny85__)
    #define DDR_USI             DDRB
    #define PORT_USI            PORTB
    #define PIN_USI             PINB
    #define PORT_USI_SDA        PORTB0
    #define PORT_USI_SCL        PORTB2
    #define PIN_USI_SDA         PINB0
    #define PIN_USI_SCL         PINB2
    #define USI_START_COND_INT  USICIF
    #define USI_START_VECTOR    USI_START_vect
    #define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif


#if defined(__AVR_ATtiny44A__)
    #define DDR_USI             DDRA
    #define PORT_USI            PORTA
    #define PIN_USI             PINA
    #define PORT_USI_SDA        PORTA6
    #define PORT_USI_SCL        PORTA4
    #define PIN_USI_SDA         PINA6
    #define PIN_USI_SCL         PINA4

    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_STR_vect
    #define USI_OVERFLOW_VECTOR USI_OVF_vect
 
#endif


#if defined(__AT90Mega165__) | defined(__ATmega165__) | \
    defined(__ATmega325__) | defined(__ATmega3250__) | \
    defined(__ATmega645__) | defined(__ATmega6450__) | \
    defined(__ATmega329__) | defined(__ATmega3290__) | \
    defined(__ATmega649__) | defined(__ATmega6490__)
    #define DDR_USI             DDRE
    #define PORT_USI            PORTE
    #define PIN_USI             PINE
    #define PORT_USI_SDA        PORTE5
    #define PORT_USI_SCL        PORTE4
    #define PIN_USI_SDA         PINE5
    #define PIN_USI_SCL         PINE4
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_START_vect
    #define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect
#endif

#if defined(__AT90Mega169__) | defined(__ATmega169__)
    #define DDR_USI             DDRE
    #define PORT_USI            PORTE
    #define PIN_USI             PINE
    #define PORT_USI_SDA        PORTE5
    #define PORT_USI_SCL        PORTE4
    #define PIN_USI_SDA         PINE5
    #define PIN_USI_SCL         PINE4
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_STRT_vect
    #define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

//! Functions implemented as macros
#define SET_USI_TO_SEND_ACK()                                                                                 \
{                                                                                                             \
        USIDR    =  0;                                              /* Prepare ACK                         */ \
        DDR_USI |=  (1<<PORT_USI_SDA);                              /* Set SDA as output                   */ \
        USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|  /* Clear all flags, except Start Cond  */ \
                    (0x0E<<USICNT0);                                /* set USI counter to shift 1 bit. */ \
}

#define SET_USI_TO_READ_ACK()                                                                                 \
{                                                                                                             \
        DDR_USI &=  ~(1<<PORT_USI_SDA);                             /* Set SDA as intput */                   \
        USIDR    =  0;                                              /* Prepare ACK        */                  \
        USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|  /* Clear all flags, except Start Cond  */ \
                    (0x0E<<USICNT0);                                /* set USI counter to shift 1 bit. */ \
}

#define SET_USI_TO_TWI_START_CONDITION_MODE()                                                                                     \
{             																														\
  STOP_TWI_TIMER();                             										                   \
  DDR_USI &=  ~(1<<PORT_USI_SDA);                             /*Set SDA as intput  */                   \
  USICR    =  (1<<USISIE)|(0<<USIOIE)|                        /* Enable Start Condition Interrupt. Disable Overflow Interrupt.*/  \
              (1<<USIWM1)|(0<<USIWM0)|                        /* Set USI in Two-wire mode. No USI Counter overflow hold.      */  \
              (1<<USICS1)|(0<<USICS0)|(0<<USICLK)|            /* Shift Register Clock Source = External, positive edge        */  \
              (0<<USITC);                                                                                                         \
  USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|  /* Clear all flags, except Start Cond                            */ \
              (0x0<<USICNT0);                                                                                                     \
}

#define SET_USI_TO_SEND_DATA()                                                                               \
{                                                                                                            \
    DDR_USI |=  (1<<PORT_USI_SDA);                                  /* Set SDA as output                  */ \
    USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      /* Clear all flags, except Start Cond */ \
                (0x0<<USICNT0);                                     /* set USI to shift out 8 bits        */ \
}

#define SET_USI_TO_READ_DATA()                                                                               \
{                                                                                                            \
    DDR_USI &= ~(1<<PORT_USI_SDA);                                  /* Set SDA as input                   */ \
    USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      /* Clear all flags, except Start Cond */ \
                (0x0<<USICNT0);                                     /* set USI to shift out 8 bits        */ \
}

int8_t t0Cnt;
#define t0MaxCnt  50   // approx 30 per second


