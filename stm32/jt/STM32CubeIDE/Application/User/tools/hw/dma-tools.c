#include <dma-tools.h>


// structure copied from stm32f7xx_hal_dma.c
typedef struct
{
   uint32_t ISR;
   uint32_t Reserved0;
   uint32_t IFCR;  /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;

uint16_t  feCounter;
uint16_t  teCounter;
uint16_t  dmeCounter;


void clearDmaInterruptFlags(DMA_HandleTypeDef *hdma)
{
	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;
	regs->IFCR = 0x3FU << ((DMA_HandleTypeDef *)hdma)->StreamIndex;

}

uint8_t  dmaIsr(DMA_HandleTypeDef *hdma)
{
	uint8_t res = 0x00;
	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;
	res = (uint8_t)((regs->ISR >> ((DMA_HandleTypeDef *)hdma)->StreamIndex) & 0x3FU);
	return res;
}

void incDMAErrorCounter(DMA_HandleTypeDef *hdma)
{
	if (__HAL_DMA_GET_FLAG(hdma,__HAL_DMA_GET_TE_FLAG_INDEX(hdma))) {
		++ teCounter;
	}
	if (__HAL_DMA_GET_FLAG(hdma,__HAL_DMA_GET_FE_FLAG_INDEX(hdma))) {
		++ feCounter;
	}
	if (__HAL_DMA_GET_FLAG(hdma,__HAL_DMA_GET_DME_FLAG_INDEX(hdma))) {
		++ dmeCounter;
	}
}


void i2cTransferConfig(I2C_HandleTypeDef *hi2c,  uint16_t DevAddress, uint8_t Size,  uint8_t Request)
{
  MODIFY_REG(hi2c->Instance->CR2, (I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RD_WRN ),
       (uint32_t)(((uint32_t)DevAddress << (I2C_CR2_SADD_Pos + 1)) | ((uint32_t)Size << I2C_CR2_NBYTES_Pos)  |
    		   ((uint32_t)Request)<< I2C_CR2_RD_WRN_Pos));
}


// structure copied from stm32f7xx_hal_dma.c
void DMA_SetTransferConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
  /* Clear DBM bit */
  hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);

  /* Configure DMA Stream data length */
  hdma->Instance->NDTR = DataLength;

  /* Memory to Peripheral */
  if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
  {
    /* Configure DMA Stream destination address */
    hdma->Instance->PAR = DstAddress;

    /* Configure DMA Stream source address */
    hdma->Instance->M0AR = SrcAddress;
  }
  /* Peripheral to Memory */
  else
  {
    /* Configure DMA Stream source address */
    hdma->Instance->PAR = SrcAddress;

    /* Configure DMA Stream destination address */
    hdma->Instance->M0AR = DstAddress;
  }
}

void i2cSendStart(I2C_HandleTypeDef *hi2c)
{
	WRITE_REG(hi2c->Instance->CR2, ((uint32_t)1U << I2C_CR2_START_Pos));
}

void i2cSendStop(I2C_HandleTypeDef *hi2c)
{
	WRITE_REG(hi2c->Instance->CR2, ((uint32_t)1U << I2C_CR2_STOP_Pos));
}

void enableAllDmaInterrupts(DMA_HandleTypeDef* hdma, uint8_t exceptHT)
{
	__HAL_DMA_ENABLE_IT(hdma,DMA_IT_TC);
	if (exceptHT != withoutHT) {
		__HAL_DMA_ENABLE_IT(hdma,DMA_IT_HT);
	}
	__HAL_DMA_ENABLE_IT(hdma,DMA_IT_TE);
	__HAL_DMA_ENABLE_IT(hdma,DMA_IT_FE);
	__HAL_DMA_ENABLE_IT(hdma,DMA_IT_DME);
}
