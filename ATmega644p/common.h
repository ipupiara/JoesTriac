#ifndef __COMMON_H__
#define __COMMON_H__
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
    
    Title:          Common definitions
    Author(s):      Pieter Conradie
    Creation Date:  2008-02-05
    Revision Info:  $Id: common.h 116 2010-06-20 21:52:14Z pieterconradie $

============================================================================= */
/** 
 *  @ingroup GENERAL
 *  @defgroup COMMON common.h : Common definitions
 *
 *  Definition of standard types, boolean, scope and utility
 *  macros.
 *
 *  File: common.h
 *  
 *  @{
 */

/* _____STANDARD INCLUDES____________________________________________________ */
#ifndef STDINT_H_ABSENT
#include <stdint.h>
#endif

/* _____DEFINITIONS _________________________________________________________ */
#ifndef NULL
/// NULL pointer
#define NULL 0
#endif

/* _____TYPE DEFINITIONS_____________________________________________________ */
#ifdef STDINT_H_ABSENT
typedef unsigned char   u8_t;     ///< unsigned 8-bit value (0 to 255)
typedef signed char     s8_t;     ///< signed 8-bit value (-128 to +127)
typedef unsigned int    u16_t;    ///< unsigned 16-bit value (0 to 65535)
typedef signed int      s16_t;    ///< signed 16-bit value (-32768 to 32767)
typedef unsigned long   u32_t;    ///< unsigned 32-bit value (0 to 4294967296)
typedef signed long     s32_t;    ///< signed 32-bit value (-2147483648 to +2147483647)
#else
/// @name Standard types
//@{
typedef uint8_t  u8_t;     ///< unsigned 8-bit value (0 to 255)
typedef int8_t   s8_t;     ///< signed 8-bit value (-128 to +127)
typedef uint16_t u16_t;    ///< unsigned 16-bit value (0 to 65535)
typedef int16_t  s16_t;    ///< signed 16-bit value (-32768 to 32767)
typedef uint32_t u32_t;    ///< unsigned 32-bit value (0 to 4294967296)
typedef int32_t  s32_t;    ///< signed 32-bit value (-2147483648 to +2147483647)
//@}
#endif

/// @name Boolean type
//@{
#ifndef __cplusplus

#define bool_t _Bool
#ifndef FALSE
#define FALSE  0
#endif
#ifndef TRUE
#define TRUE   (!FALSE)
#endif

#else

#define bool_t bool
#ifndef FALSE
#define FALSE  false
#endif
#ifndef TRUE
#define TRUE   true
#endif

#endif


//@}

/* _____MACROS_______________________________________________________________ */
/**
 * @name Type size and sign macros
 * 
 * Source: <a href="http://www.fefe.de/intof.html">"Catching Integer Overflows in C"</a>
 */
//@{
///@cond
#define __HALF_MAX_SIGNED(type) ((type)1 << (sizeof(type)*8-2))
#define __MAX_SIGNED(type) (__HALF_MAX_SIGNED(type) - 1 + __HALF_MAX_SIGNED(type))
#define __MIN_SIGNED(type) (-1 - __MAX_SIGNED(type))
///@endcond

/// Test if a type is signed or unsigned
#define TYPE_IS_SIGNED(type)((type)-1 < 1)

/**
 *  Return the minimum of a type.
 * 
 *  For an unsigned type it is zero.
 *  For a signed type this is the largest negative value
 */
#define MIN_OF_TYPE(type) (TYPE_IS_SIGNED(type)?__MIN_SIGNED(type):(type)0)

///  Return the maximum of a type.
#define MAX_OF_TYPE(type) ((type)~MIN_OF_TYPE(type))
//@}

/// @name Concatenation macros
//@{
///@cond
#define _CONCAT(x, y) x ## y
///@endcond
/** 
 *  Recursive token concatenation macro.
 * 
 * @par Example:
 * @code
 * #define XY       123456
 * #define TOKEN1   X
 * #define TOKEN2   Y
 * #define TOKEN1_2 CONCAT(X,Y)
 * @endcode
 * 
 * CONCAT(X,Y) is expanded by the C preprocessor to the macro XY, which is
 * expanded to 123456.
 * 
 */
#define CONCAT(x, y) _CONCAT(x, y)
//@}

/// @name Bit manipulation macros useful to manipulate Port IO pins
//@{
/// Macro to set a bit (1)
#define BIT_SET_HI(var,bit)           {var |=(1<<bit);}

/// Macro to clear a bit (0)
#define BIT_SET_LO(var,bit)           {var &= ~(1<<bit);}

/// Macro to toggle a bit
#define BIT_TOGGLE(var,bit)           {if(var&(1<<bit)) {var &= ~(1<<bit);} else {var |=(1<<bit);}}

/// Macro to test if a bit is set (1?)
#define BIT_IS_HI(var,bit)            ((var&(1<<bit)) != 0)

/// Macro to test if a bit is cleared (0?)
#define BIT_IS_LO(var,bit)            ((var&(1<<bit)) == 0)

/// Macro to wait until a bit is set
#define LOOP_UNTIL_BIT_IS_HI(var,bit) while(BIT_IS_LO(var,bit)) {;}

/// Macro to wait until a bit is cleared
#define LOOP_UNTIL_BIT_IS_LO(var,bit) while(BIT_IS_HI(var,bit)) {;}
//@}

/// @name Byte macros
//@{
/// Macro to extract the high 8 bits of a 16-bit value (Most Significant Byte)
#define U16_HI8(data) ((u8_t)((data>>8)&0xff))

/// Macro to extract the low 8 bits of a 16-bit value (Least Significant Byte)
#define U16_LO8(data) ((u8_t)(data&0xff))

/// Macro to extract the high 8 bits (bits 31..24) of a 32-bit value
#define U32_HI8(data) ((u8_t)((data>>24)&0xff))

/// Macro to extract the medium high 8 bits (bits 23..16) of a 32-bit value
#define U32_MH8(data) ((u8_t)((data>>16)&0xff))

/// Macro to extract the medium low 8 bits (bits 15..8) of a 32-bit value
#define U32_ML8(data) ((u8_t)((data>>8)&0xff))

/// Macro to extract the low 8 bits (bits 7..0) of a 32-bit value
#define U32_LO8(data) ((u8_t)(data&0xff))
//@}

/// @name General utility macros
//@{
/**
 * Macro to calculate division with rounding to nearest integer value
 *
 * Warning: This macro will only work correctly if (dividend >= 0)
 */
#define DIV_ROUND(dividend,divisor)     (((dividend+((divisor)>>1))/(divisor)))

/**
 * Macro to calculate division with rounding to nearest integer value
 * 
 * Warning: This macro will only work correctly if (dividend >= 0)
 */
#define DIV_ROUNDUP(dividend,divisor)   (((dividend+((divisor)-1))/(divisor)))

/// Macro to check if a value is within bounds (min <= value <= max ?)
#define BOUND(value,min,max)            (((value)>=(min))&&((value)<=(max)))

/// Macro to calculate the length of an array
#define ARRAY_LENGTH(array)             (sizeof(array)/sizeof((array)[0]))

/// Macro to see if a value is a power of two
#define VAL_IS_PWR_OF_TWO(value)        (((value)&((value)-1)) == 0)

/* _____PROJECT INCLUDES_____________________________________________________ */
/* 
 * Include board specific definitions, e.g. processor frequency.
 *
 * Note: This file is included at the end of "common.h", because "board.h"
 *       may need definitions like "u8_t", etc. which are defined at the top
 *       of this file.
 */
//#include "board.h"

//@}

/**
 *  @}
 */
#endif
