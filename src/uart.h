#ifndef UART_H
#define UART_H

#include "defines.h"

#define UART_BUFFER_SIZE				(256)

#define UART_BAUDRATE	        	(115200L)

#define UART_NUM                USART1

#define UART_CLOCK_ENABLE       RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)

#define UART_GPIO             	GPIOA

#ifdef DEF_REMOTE
  #define UART_RX_SOURCE          GPIO_PinSource10
  #define UART_TX_SOURCE          GPIO_PinSource9
#else
  #define UART_RX_SOURCE          GPIO_PinSource3
  #define UART_TX_SOURCE          GPIO_PinSource2
#endif
#define UART_IRQn              	USART1_IRQn
#define UART_IRQHandler        	USART1_IRQHandler

#define UART_RX_PIN            	(1 << UART_RX_SOURCE)
#define UART_TX_PIN            	(1 << UART_TX_SOURCE)

#define UART_GPIO_AF            GPIO_AF_1

#define UART_DMA_CHANNEL        DMA1_Channel2
#define UART_DR_BASE            ((uint32_t)&(USART1->TDR))
#define UART_DMA_RESET_FLAGS    (DMA1_FLAG_TC2 | DMA1_FLAG_HT2 | DMA1_FLAG_TE2)

char UART_RxBuffer[UART_BUFFER_SIZE];

void UART_Send(uint8_t *data, uint8_t len);
void UART_Configuration(void);

#endif
