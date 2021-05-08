#include "global.h"
#include "uart.h"

char UART_RxBuffer[UART_BUFFER_SIZE];

uint8_t UART_RxHead;

/**< UART serial receive interrupt handle */
void UART_IRQHandler(void)
{
  uint8_t c;
  BaseType_t xHigherPriorityTaskWoken;

  if (USART_GetITStatus(UART_NUM, USART_IT_RXNE))
  {
    /**< We have not woken a task at the start of the ISR */
    xHigherPriorityTaskWoken = pdFALSE;

    //USART_ClearITPendingBit(UART_NUM, USART_IT_RXNE);

    c = USART_ReceiveData(UART_NUM) & 0x7F;

    if (c == CHAR_NL)
      return;

    UART_RxBuffer[UART_RxHead++] = c;

    if (c == CHAR_ESC)
    {
      UART_RxHead = 0;
      return;
    }

    if (c == CHAR_CR)
    {
      UART_RxBuffer[UART_RxHead] = 0;
      UART_RxHead = 0;
      vTaskNotifyGiveFromISR(xTaskComm, &xHigherPriorityTaskWoken);
    }

    /**< Now the buffer is empty we can switch context if necessary */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
  if (USART_GetITStatus(UART_NUM, USART_IT_TC))
  {
    /**< Transmission of one byte completed */
    USART_ClearITPendingBit(UART_NUM, USART_IT_TC);
  }
}

/** \brief Send serial data from buffer to PC
 *
 * \param [in] buf Output data buffer for sending via UART
 * \param [in] len Length of data to send
 * \return Nothing
 *
 */
void UART_Send(uint8_t *data, uint8_t len)
{
  while (len--)
  {
    //USART_SendData(UART_NUM, *data++);
    UART_NUM->TDR = *data++;
    /**< Wait till sending is complete */
    while ((UART_NUM->ISR & USART_ISR_TC) == 0);
  }
}

/** \brief Initialize UART module for communication with PC
 *
 * \return Nothing
 *
 */
void UART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;

  UART_CLOCK_ENABLE;

  /**< Configure USART Tx and Rx as alternate function */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = UART_TX_PIN | UART_RX_PIN;
  GPIO_Init(UART_GPIO, &GPIO_InitStructure);

  GPIO_PinAFConfig(UART_GPIO, UART_RX_SOURCE, UART_GPIO_AF);
  GPIO_PinAFConfig(UART_GPIO, UART_TX_SOURCE, UART_GPIO_AF);

  USART_InitStructure.USART_BaudRate = UART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART_NUM, &USART_InitStructure);
  /* Enable 8xUSARTs Receive interrupts */
  USART_ITConfig(UART_NUM, USART_IT_RXNE, ENABLE);
  /* Enable the 8xUSARTs */
  USART_Cmd(UART_NUM, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = UART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /**< Should use lower priority than FreeRTOS interrupts to allow interrupts usage for the RTOS */
  NVIC_SetPriority(UART_IRQn, 8);
}