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

extern DMA_HandleTypeDef hdma_sdmmc2_rx;
extern DMA_HandleTypeDef hdma_sdmmc2_tx;

//SD_HandleTypeDef hsd2;
//DMA_HandleTypeDef hdma_sdmmc2_rx;
//DMA_HandleTypeDef hdma_sdmmc2_tx;

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



//  GPIO_InitTypeDef GPIO_InitStruct = {0};
///* USER CODE BEGIN MX_GPIO_Init_1 */
///* USER CODE END MX_GPIO_Init_1 */
//
//  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//  __HAL_RCC_GPIOD_CLK_ENABLE();
//  __HAL_RCC_GPIOG_CLK_ENABLE();
//  __HAL_RCC_GPIOH_CLK_ENABLE();
//  __HAL_RCC_GPIOI_CLK_ENABLE();
//
//
//  /**SDMMC2 GPIO Configuration
//  PB4     ------> SDMMC2_D3
//  PB3     ------> SDMMC2_D2
//  PD7     ------> SDMMC2_CMD
//  PD6     ------> SDMMC2_CK
//  PG10     ------> SDMMC2_D1
//  PG9     ------> SDMMC2_D0
//  */
//  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_3;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF10_SDMMC2;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF11_SDMMC2;
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//
//  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_9;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF11_SDMMC2;
//  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
//
//
//  /*Configure GPIO pin : PI15 */
//  GPIO_InitStruct.Pin = GPIO_PIN_15;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
//
///* USER CODE BEGIN MX_GPIO_Init_2 */
///* USER CODE END MX_GPIO_Init_2 */
}


static void mmcSdmmc2SdInit(void)
{

	  hsd2.Instance = SDMMC2;
	  hsd2.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
	  hsd2.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
	  hsd2.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
	  hsd2.Init.BusWide = SDMMC_BUS_WIDE_4B;
	  hsd2.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
	  hsd2.Init.ClockDiv = 0;


//  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
//
//	__HAL_RCC_SDMMC2_CLK_ENABLE();
//
//  hsd2.Instance = SDMMC2;
//  hsd2.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
//  hsd2.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
//  hsd2.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
//  hsd2.Init.BusWide = SDMMC_BUS_WIDE_4B;
//  hsd2.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
//  hsd2.Init.ClockDiv = 0;
//
//  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC2|RCC_PERIPHCLK_CLK48;
//  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
//  PeriphClkInitStruct.Sdmmc2ClockSelection = RCC_SDMMC2CLKSOURCE_CLK48;
//  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  HAL_NVIC_SetPriority(SDMMC2_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(SDMMC2_IRQn);
}

/**
  * Enable DMA controller clock
  */
static void mmcDMAInit(void)
{

	  __HAL_RCC_DMA2_CLK_ENABLE();

	  /* DMA interrupt init */
	  /* DMA2_Stream0_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	  /* DMA2_Stream5_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

//  /* DMA controller clock enable */
//  __HAL_RCC_DMA2_CLK_ENABLE();
//
//   hdma_sdmmc2_rx.Instance = DMA2_Stream0;
//   hdma_sdmmc2_rx.Init.Channel = DMA_CHANNEL_11;
//   hdma_sdmmc2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//   hdma_sdmmc2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//   hdma_sdmmc2_rx.Init.MemInc = DMA_MINC_ENABLE;
//   hdma_sdmmc2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//   hdma_sdmmc2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//   hdma_sdmmc2_rx.Init.Mode = DMA_PFCTRL;
//   hdma_sdmmc2_rx.Init.Priority = DMA_PRIORITY_LOW;
//   hdma_sdmmc2_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
//   hdma_sdmmc2_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
//   hdma_sdmmc2_rx.Init.MemBurst = DMA_MBURST_INC4;
//   hdma_sdmmc2_rx.Init.PeriphBurst = DMA_PBURST_INC4;
//   if (HAL_DMA_Init(&hdma_sdmmc2_rx) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   __HAL_LINKDMA(&hsd2,hdmarx,hdma_sdmmc2_rx);
//
//   /* SDMMC2_TX Init */
//   hdma_sdmmc2_tx.Instance = DMA2_Stream5;
//   hdma_sdmmc2_tx.Init.Channel = DMA_CHANNEL_11;
//   hdma_sdmmc2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//   hdma_sdmmc2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//   hdma_sdmmc2_tx.Init.MemInc = DMA_MINC_ENABLE;
//   hdma_sdmmc2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//   hdma_sdmmc2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//   hdma_sdmmc2_tx.Init.Mode = DMA_PFCTRL;
//   hdma_sdmmc2_tx.Init.Priority = DMA_PRIORITY_LOW;
//   hdma_sdmmc2_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
//   hdma_sdmmc2_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
//   hdma_sdmmc2_tx.Init.MemBurst = DMA_MBURST_INC4;
//   hdma_sdmmc2_tx.Init.PeriphBurst = DMA_PBURST_INC4;
//   if (HAL_DMA_Init(&hdma_sdmmc2_tx) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   __HAL_LINKDMA(&hsd2,hdmatx,hdma_sdmmc2_tx);
//
//  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
//  /* DMA2_Stream5_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
}

uint32_t  testMmc()
{
	res = FR_OK;
	if((res = (f_open(&SDFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)))
		{
//			res = FR_DISK_ERR;
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
		if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 1) != FR_OK)
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
