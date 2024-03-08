
#include <adcControl.h>
#include <stm32f7xx_hal.h>
#include "cmsis_os.h"
#include <mainJt.h>
#include <TriacIntr.h>
#include <TriacPID.h>


ADC_HandleTypeDef currentSensorADC;

osTimerId_t   mainJtAdcTimer;

float currentAmpsValue;
uint16_t currentAmpsADCValue;

float gradAmps; //   (delta amperes) / (delta adc)   ....
float gradAdc;
//uint32_t calibHighADC;
//uint32_t calibLowADC;

void updateGradAmps()
{
	float dADC;
	float dAmps;
	dAmps = calibHighAmps - calibLowAmps;
	dADC = getDefinesCalibHighAdc() - getDefinesCalibLowAdc();
	if ( fabs(dADC) > 1) {
		gradAmps = dAmps / dADC;
	} else gradAmps = 0;
	if (fabs (dAmps) > 1)  {
		gradAdc = dADC / dAmps;
	} else gradAdc = 0;
}


void adcTimerCallback(void *argument)
{
	currentSensorADC.Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
}

uint32_t getCurrentAmpsADCValue()
{
	uint32_t res;
//	taskENTER_CRITICAL();
	res = currentAmpsADCValue;
//	taskEXIT_CRITICAL();
	return res;
}

void setCurrentAmpsADCValue(uint32_t adcV )
{
//	taskENTER_CRITICAL();
	currentAmpsADCValue = adcV;
//	taskEXIT_CRITICAL();
}

void adcValueReceived(uint16_t adcVal)
{
//	taskENTER_CRITICAL();
	setCurrentAmpsADCValue(adcVal);
//	taskEXIT_CRITICAL();
}

float adcVoltage()
{
	int16_t ampsAdcHex;
	float   ampsAdcF;
	float   adcMaxF = 0x0FFF;

	float    Vf;

	ampsAdcHex = getCurrentAmpsADCValue();
	ampsAdcF  = ampsAdcHex;
	Vf = (ampsAdcF * 3.3) / adcMaxF;

	return Vf;
}

float currentAmps()
{
	uint32_t adcVal;
	float res = 0.0;

	adcVal = getCurrentAmpsADCValue();

	int32_t diffAdc = ((uint32_t) (adcVal)) - ((uint32_t) ( getDefinesCalibLowAdc()))  ;

	float diffI = (gradAmps * diffAdc);

	res = calibLowAmps +  diffI;
	return res;
}

float getCurrentAmpsValue()
{
	float  res;
	res = currentAmps();
	return res;
}


void startAdcTimer()
{
	osStatus_t  status;

	status = osTimerStart (mainJtAdcTimer, pidStepDelays);
	if (status !=  osOK)  {
		errorHandler((uint32_t)status ,goOn," osTimerStart "," osStarted ");
	}
}

void stopAdcTimer()
{
	osStatus_t  status;

	status = osTimerStop (mainJtAdcTimer);
	if (status !=  osOK)  {
		errorHandler((uint32_t)status ,goOn," osTimerStart "," osStarted ");
	}
}


void eocIrqHandler(ADC_HandleTypeDef* hadc)
{
	if (hadc == &currentSensorADC) {
		uint16_t adcV = 0;
		adcV = currentSensorADC.Instance->DR;
		setCurrentAmpsADCValue(adcV);

		CMainJtEventT  ev;
		memset(&ev, 0x0, sizeof(ev));
		ev.evType = adcTick;
		ev.mainUnion.advV = adcV;
		osStatus_t status =  sendEventToMainJtMessageQ( &ev, isFromIsr);
		if (status != osOK) {
			errorHandler(status,goOn," status ","eocIrqHandler");
		}
	}
//	if (hadc == &throttlePotiADC) {
//
//		// update sensor value
//	}
}

void awdIrqHandler(ADC_HandleTypeDef* hadc)
{
	if (hadc == &currentSensorADC) {
		// update value
		// stop servo engine  --   engine at movement boarder
	}
//	if (hadc == &throttlePotiADC) {
//		// update value
//		// stop servo engine
//	}
}



void ADC_IRQ_(ADC_HandleTypeDef* hadc)
{
	uint32_t tmp1 = 0, tmp2 = 0;

	// end of conversion
	tmp1 = __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC);
	tmp2 = __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_EOC);
	if(tmp1 && tmp2)
	{
		eocIrqHandler(hadc);
		__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_STRT | ADC_FLAG_EOC);

	}

	// analog watchdog
//	tmp1 = __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_AWD);
//	tmp2 = __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_AWD);
//	if(tmp1 && tmp2)
//	{
//		awdIrqHandler(hadc);
//		__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD);
//	}
//	if (hadc->Instance->SR & ADC_FLAG_OVR)  {   // EOCS currently not set, so no OVR checked
//
//		__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
//	}
}

void ADC_IRQHandler(void)
{
	ADC_IRQ_(&currentSensorADC);
//	ADC_IRQ_(&throttlePotiADC);
}


static void ADC1_currentSensor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  __HAL_RCC_ADC1_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /**ADC1 GPIO Configuration
  PA0/WKUP     ------> ADC1_IN0
  */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  currentSensorADC.Instance = ADC1;
  currentSensorADC.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  currentSensorADC.Init.Resolution = ADC_RESOLUTION_12B;
  currentSensorADC.Init.ScanConvMode = ADC_SCAN_DISABLE;
  currentSensorADC.Init.ContinuousConvMode = DISABLE;
  currentSensorADC.Init.DiscontinuousConvMode = DISABLE;
  currentSensorADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  currentSensorADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  currentSensorADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  currentSensorADC.Init.NbrOfConversion = 1;
  currentSensorADC.Init.DMAContinuousRequests = DISABLE;
  currentSensorADC.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&currentSensorADC) != HAL_OK)
  {

  }

//  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
//  AnalogWDGConfig.HighThreshold = 2000;
//  AnalogWDGConfig.LowThreshold = 100;
//  AnalogWDGConfig.Channel = ADC_CHANNEL_0;
//  AnalogWDGConfig.ITMode = ENABLE;
//  if (HAL_ADC_AnalogWDGConfig(&currentSensorADC, &AnalogWDGConfig) != HAL_OK)
//  {
//
//  }

  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&currentSensorADC, &sConfig) != HAL_OK)
  {

  }

  __HAL_ADC_ENABLE_IT(&currentSensorADC,ADC_IT_EOC);
  HAL_NVIC_SetPriority(ADC_IRQn, triacApplicationIsrPrio, 0);
  HAL_NVIC_EnableIRQ(ADC_IRQn);
}


void setAdcLowerThreshold(ADC_HandleTypeDef* hadc,uint32_t  limit)
{
	hadc->Instance->LTR = limit;
}

void setAdcUpperThreshold(ADC_HandleTypeDef* hadc,uint32_t  limit)
{
	hadc->Instance->HTR = limit;
}

void startADC()
{
	updateGradAmps();
	currentAmpsValue = currentSensorADC.Instance->DR;
	__HAL_ADC_ENABLE(&currentSensorADC);
	startAdcTimer();
}

void stopADC()
{
	stopAdcTimer();
	__HAL_ADC_DISABLE(&currentSensorADC);
}

void initAdc()
{
	currentAmpsValue = 0.0;
	updateGradAmps();

	mainJtAdcTimer= osTimerNew (adcTimerCallback, osTimerPeriodic, (void *) 0x02, NULL);
	if (mainJtAdcTimer  == NULL)   {
		errorHandler((uint32_t)mainJtAdcTimer ,stop,"mainJtAdcTimer ","initAdc");
	}

	ADC1_currentSensor_Init();


//  todo eventually set adon to 0 when not executing triac regulation (save energy ) tobe tested !
}


