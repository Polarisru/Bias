#include "spi.h"

/** \brief Transmit data via SPI bus to selected device
 *
 * \param [in] data Word with data value
 * \param [in] bits Number of bits to transmit
 * \return Nothing
 *
 */
void SPI_Transmit(uint16_t data, uint8_t bits)
{
  uint8_t i;

  while (bits--)
  {
    i = bits;
    if ((1 << i) & data)
      SPI_EE_DI_GPIO->BSRR = SPI_EE_DI_PIN;
    else
      SPI_EE_DI_GPIO->BRR = SPI_EE_DI_PIN;
    SPI_EE_CLK_GPIO->BSRR = SPI_EE_CLK_PIN;
    SPI_DELAY;
    SPI_EE_CLK_GPIO->BRR = SPI_EE_CLK_PIN;
  }
  SPI_EE_DI_GPIO->BRR = SPI_EE_DI_PIN;
}

/** \brief Receive data via SPI bus from selected device
 *
 * \param [in] bits Number of bits to receive
 * \return Received data value as uint16_t
 *
 */
uint16_t SPI_Receive(uint8_t bits)
{
  uint16_t dout = 0;
  uint8_t i;

  while (bits--)
  {
    i = bits;
    if (SPI_EE_DO_GPIO->IDR & SPI_EE_DO_PIN)
      dout |= (1 << i);
    SPI_EE_CLK_GPIO->BSRR = SPI_EE_CLK_PIN;
    SPI_DELAY;
    SPI_EE_CLK_GPIO->BRR = SPI_EE_CLK_PIN;
  }

  return dout;
}

void SPI_Select(uint8_t device)
{
  switch (device)
  {
    case SPI_SELECT_EEPROM:
      SPI_EE_CS_GPIO->BSRR = SPI_EE_CS_PIN;
      break;
    case SPI_SELECT_NONE:
      SPI_EE_CS_GPIO->BRR = SPI_EE_CS_PIN;
      break;
  }
}

/** \brief Initialite SPI communication
 *
 * \return Nothing
 *
 */
void SPI_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStruct.GPIO_Pin = SPI_EE_CS_PIN;
  GPIO_Init(SPI_EE_CS_GPIO, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = SPI_EE_CLK_PIN;
  GPIO_Init(SPI_EE_CLK_GPIO, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = SPI_EE_DI_PIN;
  GPIO_Init(SPI_EE_DI_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Pin = SPI_EE_DO_PIN;
  GPIO_Init(SPI_EE_DO_GPIO, &GPIO_InitStruct);
}
