#include "spi.h"

/** \brief Transmit data via SPI bus to selected device
 *
 * \param [in] data Data buffer
 * \param [in] len Data length
 * \return void
 *
 */
void SPI_Transmit(uint8_t *data, uint8_t len)
{
  uint8_t i;

  while (len--)
  {
    for (i = 0; i < 8; i++)
    {
      if (*data & 0x80)
        SPI_MOSI_GPIO->BSRR = SPI_MOSI_PIN;
      else
        SPI_MOSI_GPIO->BRR = SPI_MOSI_PIN;
      SPI_SCLK_GPIO->BSRR = SPI_SCLK_PIN;
      SPI_DELAY;
      SPI_SCLK_GPIO->BRR = SPI_SCLK_PIN;
      *data <<= 1;
    }
    SPI_MOSI_GPIO->BRR = SPI_MOSI_PIN;
    data++;
  }
}

/** \brief Receive data via SPI bus from selected device
 *
 * \param [in] data Data buffer
 * \param [in] len Data length
 * \return void
 *
 */
void SPI_Receive(uint8_t *data, uint8_t len)
{
  uint8_t i;

  while (len--)
  {
    for (i = 0; i < 8; i++)
    {
      *data <<= 1;
      if (SPI_MISO_GPIO->IDR & SPI_MISO_PIN)
        *data += 1;
      SPI_SCLK_GPIO->BSRR = SPI_SCLK_PIN;
      SPI_DELAY;
      SPI_SCLK_GPIO->BRR = SPI_SCLK_PIN;
    }
    data++;
  }
}

void SPI_Select(uint8_t device)
{
  switch (device)
  {
    case SPI_SELECT_AD7293:
      SPI_AD7293_CS_GPIO->BRR = SPI_AD7293_CS_PIN;
      break;
    case SPI_SELECT_NONE:
      SPI_AD7293_CS_GPIO->BSRR = SPI_AD7293_CS_PIN;
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

  GPIO_InitStruct.GPIO_Pin = SPI_AD7293_CS_PIN;
  GPIO_Init(SPI_AD7293_CS_GPIO, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = SPI_SCLK_PIN;
  GPIO_Init(SPI_SCLK_GPIO, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN;
  GPIO_Init(SPI_MOSI_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN;
  GPIO_Init(SPI_MISO_GPIO, &GPIO_InitStruct);
}
