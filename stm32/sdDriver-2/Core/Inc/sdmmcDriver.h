/*
 * sdmmcDriver.h
 *
 *  Created on: Jan 31, 2025
 *      Author: diego
 */

#ifndef INC_SDMMCDRIVER_H_
#define INC_SDMMCDRIVER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx_hal.h"
#include <fatfs.h>
#include <string.h>


 uint32_t initSdmmc();

 void DMA2_Stream0_IRQHandler(void);
 void DMA2_Stream5_IRQHandler(void);
 void SDMMC2_IRQHandler(void);



#ifdef __cplusplus
}
#endif

#endif /* INC_SDMMCDRIVER_H_ */
