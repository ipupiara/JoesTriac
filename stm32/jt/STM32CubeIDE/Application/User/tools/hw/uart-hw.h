/*
 * uart-comms.h
 *
 *  Created on: Oct 12, 2020
 *      Author: ixchel
 */

#ifndef ST_TEMPIX_TOOLS_UART_HW_H_
#define ST_TEMPIX_TOOLS_UART_HW_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include<stdio.h>
#include  <stm32f7xx_hal.h>
#include "cmsis_os.h"

#define charWidth sizeof(char)

#define dmaTxAvailable  0x01       //  rx dma stream occupied by touchgfx, only tx possible

#define useUsart2
//#define useUsart6


#ifdef useUsart2
#define  USART_Number USART2
#define txDMA_IRQHandler DMA1_Stream6_IRQHandler
#define txDMA_FLAG_TCIF DMA_FLAG_TCIF2_6
#define txDMA_FLAG_HTIF  DMA_FLAG_HTIF2_6
#define txDMA_FLAG_TEIF  DMA_FLAG_TEIF2_6
#define txDMA_FLAG_FEIF DMA_FLAG_FEIF2_6
#define txDMA_FLAG_DMEIF DMA_FLAG_DMEIF2_6

//#define rxDMA_IRQHandler DMA1_Stream6_IRQHandler
//#define rxDMA_FLAG_TCIF DMA_FLAG_TCIF2_6
//#define rxDMA_FLAG_HTIF  DMA_FLAG_HTIF2_6
//#define rxDMA_FLAG_TEIF  DMA_FLAG_TEIF2_6
//#define rxDMA_FLAG_FEIF DMA_FLAG_FEIF2_6
//#define rxDMA_FLAG_DMEIF DMA_FLAG_DMEIF2_6

#define  txDMA_Stream  DMA1_Stream6
#define	 txDMA_CHANNEL DMA_CHANNEL_4
//#define  rxDMA_Stream  DMA1_Stream5
//#define	 rxDMA_CHANNEL DMA_CHANNEL_4

#define USART_IRQHandler USART2_IRQHandler

#define txDMA_Stream_IRQn DMA1_Stream6_IRQn
#define USART_IRQn USART2_IRQn
//#define rxDMA_Stream_IRQn DMA_Stream_IRQn

/**USART2 GPIO Configuration
PA2     ------> USART2_TX
PA3     ------> USART2_RX
*/
#define   usart__HAL_RCC_DMA_CLK_ENABLE()  __HAL_RCC_DMA1_CLK_ENABLE()
#define usart__HAL_RCC_USART_CLK_ENABLE()  __HAL_RCC_USART2_CLK_ENABLE()
#define usart__HAL_RCC_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define txGPIO_PIN GPIO_PIN_2
#define rxGPIO_PIN GPIO_PIN_3
#define GPIO_AF_USART  GPIO_AF7_USART2
#define uartPort GPIOA

#define RCC_PERIPHCLK_USART RCC_PERIPHCLK_USART2
#define UsartClockSelection Usart2ClockSelection
#define RCC_USARTCLKSOURCE_PCLK RCC_USART2CLKSOURCE_PCLK1

#endif

#ifdef useUsart6
#define  USART_Number USART6
#define txDMA_IRQHandler DMA2_Stream6_IRQHandler
#define txDMA_FLAG_TCIF DMA_FLAG_TCIF2_6
#define txDMA_FLAG_HTIF  DMA_FLAG_HTIF2_6
#define txDMA_FLAG_TEIF  DMA_FLAG_TEIF2_6
#define txDMA_FLAG_FEIF DMA_FLAG_FEIF2_6
#define txDMA_FLAG_DMEIF DMA_FLAG_DMEIF2_6

//#define rxDMA_IRQHandler DMA1_Stream6_IRQHandler
//#define rxDMA_FLAG_TCIF DMA_FLAG_TCIF2_6
//#define rxDMA_FLAG_HTIF  DMA_FLAG_HTIF2_6
//#define rxDMA_FLAG_TEIF  DMA_FLAG_TEIF2_6
//#define rxDMA_FLAG_FEIF DMA_FLAG_FEIF2_6
//#define rxDMA_FLAG_DMEIF DMA_FLAG_DMEIF2_6

#define USART_IRQHandler USART6_IRQHandler
#define txDMA_Stream_IRQHandler DMA2_Stream6_IRQHandler
#define txDMA_Stream_IRQn DMA2_Stream6_IRQn
#define USART_IRQn USART6_IRQn
//#define rxDMA_Stream_IRQn DMA_Stream_IRQn

/**USART2 GPIO Configuration
PC6     ------> USART6_TX
PC7     ------> USART6_RX
*/
#define   usart__HAL_RCC_DMA_CLK_ENABLE()  __HAL_RCC_DMA2_CLK_ENABLE()
#define usart__HAL_RCC_USART_CLK_ENABLE()  __HAL_RCC_USART6_CLK_ENABLE()
#define usart__HAL_RCC_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE();

#define txGPIO_PIN GPIO_PIN_6
#define rxGPIO_PIN GPIO_PIN_7
#define GPIO_AF_USART  GPIO_AF8_USART6
#define uartPort GPIOC

#define RCC_PERIPHCLK_USART RCC_PERIPHCLK_USART6
#define UsartClockSelection Usart6ClockSelection
#define RCC_USARTCLKSOURCE_PCLK RCC_USART6CLKSOURCE_PCLK2

#define  txDMA_Stream  DMA2_Stream6
#define	 txDMA_CHANNEL DMA_CHANNEL_6
//#define  rxDMA_Stream  DMA1_Stream6
//#define	 rxDMA_CHANNEL DMA_CHANNEL_4

#endif

#ifdef useUsart7

#define  USART_Number UART7
#define txDMA_IRQHandler DMA1_Stream1_IRQHandler
#define txDMA_FLAG_TCIF DMA_FLAG_TCIF1_5
#define txDMA_FLAG_HTIF  DMA_FLAG_HTIF1_5
#define txDMA_FLAG_TEIF  DMA_FLAG_TEIF1_5
#define txDMA_FLAG_FEIF DMA_FLAG_FEIF1_5
#define txDMA_FLAG_DMEIF DMA_FLAG_DMEIF1_5

//#define rxDMA_IRQHandler DMA1_Stream6_IRQHandler
//#define rxDMA_FLAG_TCIF DMA_FLAG_TCIF2_6
//#define rxDMA_FLAG_HTIF  DMA_FLAG_HTIF2_6
//#define rxDMA_FLAG_TEIF  DMA_FLAG_TEIF2_6
//#define rxDMA_FLAG_FEIF DMA_FLAG_FEIF2_6
//#define rxDMA_FLAG_DMEIF DMA_FLAG_DMEIF2_6

#define USART_IRQHandler UART7_IRQHandler

#define txDMA_Stream_IRQn DMA1_Stream1_IRQn
#define USART_IRQn UART7_IRQn
//#define rxDMA_Stream_IRQn DMA_Stream_IRQn

/**USART7 GPIO Configuration
PF7     ------> USART2_TX
PF6     ------> USART2_RX
*/
#define   usart__HAL_RCC_DMA_CLK_ENABLE()   __HAL_RCC_DMA1_CLK_ENABLE()
#define usart__HAL_RCC_USART_CLK_ENABLE()  __HAL_RCC_UART7_CLK_ENABLE()
#define usart__HAL_RCC_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()

#define rxGPIO_PIN GPIO_PIN_6
#define txGPIO_PIN GPIO_PIN_7
#define GPIO_AF_USART  GPIO_AF8_UART7
#define uartPort GPIOF

#define RCC_PERIPHCLK_USART RCC_PERIPHCLK_UART7
#define UsartClockSelection Uart7ClockSelection
#define RCC_USARTCLKSOURCE_PCLK RCC_UART7CLKSOURCE_PCLK1

#define  txDMA_Stream  DMA1_Stream1
#define	 txDMA_CHANNEL DMA_CHANNEL_5
//#define  rxDMA_Stream  DMA1_Stream6
//#define	 rxDMA_CHANNEL DMA_CHANNEL_4


#endif



extern osMessageQueueId_t uartSendSemaphoreQ;

uint8_t initUartHw();

uint8_t startUartHw();

osStatus_t sendUartString(char* sndStr);

#ifdef  __cplusplus
}
#endif


#endif /* ST_TEMPIX_TOOLS_UART_HW_H_ */
