
#include <adcControl.h>
#include <stm32f7xx_hal.h>

ADC_HandleTypeDef currentSensorADC;
//ADC_HandleTypeDef throttlePotiADC;

//  todo create timer and start adc by command to reduce cpu usage
//       or reduce prescaler alternatively.
//  use twa for current sensor (avoid servo start current peak mismatch)

void eocIrqHandler(ADC_HandleTypeDef* hadc)
{
	if (hadc == &currentSensorADC) {
		//  update sensor value
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

	tmp1 = __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC);
	tmp2 = __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_EOC);
	if(tmp1 && tmp2)
	{
		eocIrqHandler(hadc);
		__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_STRT | ADC_FLAG_EOC);

	}
	tmp1 = __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_AWD);
	tmp2 = __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_AWD);
	if(tmp1 && tmp2)
	{
		awdIrqHandler(hadc);
		__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD);
	}
}

void ADC_IRQHandler(void)
{
	ADC_IRQ_(&currentSensorADC);
//	ADC_IRQ_(&throttlePotiADC);
}



/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_currentSensor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  __HAL_RCC_ADC1_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /**ADC1 GPIO Configuration
  PA0/WKUP     ------> ADC1_IN0
  */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  currentSensorADC.Instance = ADC1;
  currentSensorADC.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  currentSensorADC.Init.Resolution = ADC_RESOLUTION_12B;
  currentSensorADC.Init.ScanConvMode = ADC_SCAN_DISABLE;
  currentSensorADC.Init.ContinuousConvMode = ENABLE;
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
  /** Configure the analog watchdog
  */
//  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
//  AnalogWDGConfig.HighThreshold = 2000;
//  AnalogWDGConfig.LowThreshold = 100;
//  AnalogWDGConfig.Channel = ADC_CHANNEL_0;
//  AnalogWDGConfig.ITMode = ENABLE;
//  if (HAL_ADC_AnalogWDGConfig(&currentSensorADC, &AnalogWDGConfig) != HAL_OK)
//  {
//
//  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&currentSensorADC, &sConfig) != HAL_OK)
  {

  }

  HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(ADC_IRQn);
}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
//static void MX_ADC2_throttlePoti_Init(void)
//{
//  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
//  ADC_ChannelConfTypeDef sConfig = {0};
//
//  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
//  */
//  throttlePotiADC.Instance = ADC2;
//  throttlePotiADC.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
//  throttlePotiADC.Init.Resolution = ADC_RESOLUTION_12B;
//  throttlePotiADC.Init.ScanConvMode = ADC_SCAN_DISABLE;
//  throttlePotiADC.Init.ContinuousConvMode = ENABLE;
//  throttlePotiADC.Init.DiscontinuousConvMode = DISABLE;
//  throttlePotiADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//  throttlePotiADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//  throttlePotiADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//  throttlePotiADC.Init.NbrOfConversion = 1;
//  throttlePotiADC.Init.DMAContinuousRequests = DISABLE;
//  throttlePotiADC.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//  if (HAL_ADC_Init(&throttlePotiADC) != HAL_OK)
//  {
//
//  }
//  /** Configure the analog watchdog
//  */
//  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
//  AnalogWDGConfig.HighThreshold = 3000;
//  AnalogWDGConfig.LowThreshold = 200;
//  AnalogWDGConfig.Channel = ADC_CHANNEL_1;
//  AnalogWDGConfig.ITMode = ENABLE;
//  if (HAL_ADC_AnalogWDGConfig(&throttlePotiADC, &AnalogWDGConfig) != HAL_OK)
//  {
//
//  }
//  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
//  */
//  sConfig.Channel = ADC_CHANNEL_1;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
//  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
//  if (HAL_ADC_ConfigChannel(&throttlePotiADC, &sConfig) != HAL_OK)
//  {
//
//  }
//
////  BSP_IntVectSet(ADC_IRQn,tempixIsrPrioLevel,CPU_INT_KA,ADC_IRQHandler);
////  BSP_IntEnable(ADC_IRQn);
//  __HAL_ADC_ENABLE_IT(&currentSensorADC,ADC_IT_AWD);
//  __HAL_ADC_ENABLE_IT(&throttlePotiADC,ADC_IT_EOC);
//  // error handling of adc ?
//}

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
	__HAL_ADC_ENABLE(&currentSensorADC);
//	__HAL_ADC_ENABLE(&throttlePotiADC);
	currentSensorADC.Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
//	throttlePotiADC.Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
}

void stopADC()
{
	__HAL_ADC_DISABLE(&currentSensorADC);
//	__HAL_ADC_DISABLE(&throttlePotiADC);
}

void initAdc()
{
	MX_ADC1_currentSensor_Init();
//	MX_ADC2_throttlePoti_Init();

}


