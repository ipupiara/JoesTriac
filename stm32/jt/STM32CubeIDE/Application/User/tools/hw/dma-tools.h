#ifndef dma_tools
#define dma_tools

#ifdef  __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stm32f7xx_hal.h>
//#include <stm32f7xx_hal_adc.h>



#define  MAX( a, b ) ( ( a > b) ? a : b )

#define __HAL_I2C_TxDmaENABLE(__HANDLE__)  (SET_BIT((__HANDLE__)->Instance->CR1,  I2C_CR1_TXDMAEN))
#define __HAL_I2C_RxDmaENABLE(__HANDLE__)  (SET_BIT((__HANDLE__)->Instance->CR1,  I2C_CR1_RXDMAEN))
#define __HAL_I2C_AutoEndENABLE(__HANDLE__)  (SET_BIT((__HANDLE__)->Instance->CR2,  I2C_CR2_AUTOEND ))

enum {
	withoutHT = 0,
	withHT
};


void incDMAErrorCounter(DMA_HandleTypeDef *hdma);

void enableAllDmaInterrupts(DMA_HandleTypeDef* hdma, uint8_t exceptHT);

void clearDmaInterruptFlags(DMA_HandleTypeDef *hdma);

void i2cSendStart(I2C_HandleTypeDef *hi2c);

void i2cSendStop(I2C_HandleTypeDef *hi2c);

void i2cTransferConfig(I2C_HandleTypeDef *hi2c,  uint16_t DevAddress, uint8_t Size,  uint8_t Request);

// method copied from stm32f7xx_hal_dma.c
void DMA_SetTransferConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
uint8_t  dmaIsr(DMA_HandleTypeDef *hdma);

#ifdef  __cplusplus
}
#endif


#endif
