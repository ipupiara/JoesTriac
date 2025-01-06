/*
 * sdDisk.c
 *
 *  Created on: Jan 6, 2025
 *      Author: Brigitte
 */

#include <stm32f7xx_hal.h>
#include <ffconf.h>
#include <ff.h>
#include "fatfs.h"
#include <string.h>
#include <sdDisk.h>

SD_HandleTypeDef hsd2;
DMA_HandleTypeDef hdma_sdmmc2_rx;
DMA_HandleTypeDef hdma_sdmmc2_tx;

FRESULT res; /* FatFs function common result code */
uint32_t byteswritten, bytesread; /* File write/read counts */
uint8_t wtext[] = "STM32 FATFS works great!"; /* File write buffer */
uint8_t rtext[_MAX_SS];/* File read buffer *//* USER CODE BEGIN Header */


static void mmcGPIOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /*Configure GPIO pin : PI15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}


static void mmcSdmmc2SdInit(void)
{

  /* USER CODE BEGIN SDMMC2_Init 0 */

  /* USER CODE END SDMMC2_Init 0 */

  /* USER CODE BEGIN SDMMC2_Init 1 */

  /* USER CODE END SDMMC2_Init 1 */
  hsd2.Instance = SDMMC2;
  hsd2.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd2.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd2.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd2.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd2.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd2.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDMMC2_Init 2 */

  /* USER CODE END SDMMC2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void mmcDMAInit(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

}

uint32_t  testMmc()
{
	res = FR_OK;
	if(f_open(&SDFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
		{
			res = FR_DISK_ERR;
		}
		else
		{

			//Write to the text file
			res = f_write(&SDFile, wtext, strlen((char *)wtext), (void *)&byteswritten);
			if((byteswritten == 0) || (res != FR_OK))
			{
				res = FR_DISK_ERR;
			}
			else
			{

				f_close(&SDFile);
			}
		}
	return res;
}

uint32_t initMmc()
{
	res = FR_OK;

	mmcGPIOInit();
	mmcDMAInit();
	mmcSdmmc2SdInit();
	MX_FATFS_Init();
		if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) != FR_OK)
		{
			res = FR_DISK_ERR;
		}
//		else
//		{
//			if(f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, rtext, sizeof(rtext)) != FR_OK)
//		    {
//				Error_Handler();
//		    }
		res = testMmc();
		return res;
}

uint32_t mmcStoreDefines(uint32_t * persistentDefines)
{
	res = FR_OK;

	return res;
}

uint32_t writeToLogfile(char* transmitBuffer)
{
	res = FR_OK;

	return res;
}
