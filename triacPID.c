
#include <math.h>
#include <avr/eeprom.h>
#include "triacPID.h"
#include "TriacDefines.h"
#include "TriacIntr.h"

float gradAmps; // amperes ....

void updateGradAmps()
{
	if ( fabs(calibHighAdc - calibLowAdc) > 1) {  
		gradAmps = (calibHighAmps - calibLowAmps) / (calibHighAdc - calibLowAdc);
	} else gradAmps = 0;	
}


void InitializePID(real kp, real ki, real kd, real error_thresh, real step_time)
{
    // Initialize controller parameters
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
    m_error_thresh = error_thresh;

    // Controller step time and its inverse
    m_h = step_time;
    m_inv_h = 1 / step_time;

    // Initialize integral and derivative calculations
    m_integral = 0;
    m_started = 0;
	
	calibHighCms = eeprom_read_word((uint16_t*)calibHighCmsEEPROMpos);
	calibLowCms = eeprom_read_word((uint16_t*)calibLowCmsEEPROMpos);
	calibHighAdc = eeprom_read_word((uint16_t*)calibHighAdcEEPROMpos);
	calibLowAdc = eeprom_read_word((uint16_t*)calibLowAdcEEPROMpos);
	 updateGradAmps();
}

real Update(real error)
{
    // Set q to 1 if the error magnitude is below
    // the threshold and 0 otherwise
    real q;
    if (fabs(error) < m_error_thresh)
        q = 1;
    else
        q = 0;

    // Update the error integral
    m_integral += m_h*q*error;

    // Compute the error derivative
    real deriv;
    if (!m_started)
    {
        m_started = 1;
        deriv = 0;
    }
    else
        deriv = (error - m_prev_error) * m_inv_h;

    m_prev_error = error;

    // Return the PID controller actuator command
    return m_kp*(error + m_ki*m_integral + m_kd*deriv);
}


void storeCalibLowCms(uint16_t cl)
{
	eeprom_write_word((uint16_t*)calibLowCmsEEPROMpos,cl);
	calibLowCms = cl;
}

void storeCalibHighCms(uint16_t ch)
{
	eeprom_write_word((uint16_t*)calibHighCmsEEPROMpos,ch);
	calibHighCms = ch;
}

float currentAmps()
{
	int16_t adcAmps, res;
	adcAmps = ampsADCValue();
	res = calibLowAmps + (gradAmps * (adcAmps - calibLowAdc  ));
	return res;
}

void calcNextTriacDelay()
{  
	float err;
	float corr, newDelay;
	err = currentAmps()  - desiredAmps ;
	corr = Update(err);
	newDelay = triacTriggerDelayCms - corr;
	setTriacTriggerDelay(newDelay);
	
}


void storeCalibLowAdc(uint16_t cl)
{
	eeprom_write_word((uint16_t*)calibLowAdcEEPROMpos,cl);
	calibLowAdc = cl;	 
	updateGradAmps();
}

void storeCalibHighAdc(uint16_t ch)
{
	eeprom_write_word((uint16_t*)calibHighAdcEEPROMpos,ch);
	calibHighAdc = ch;
	updateGradAmps();
}
