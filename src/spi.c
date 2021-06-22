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
  #ifndef SPI_HW
  uint8_t i;
  #endif

  while (len--)
  {
    #ifndef SPI_HW
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
    #else
    SPI_SendData8(SPI1, *data);
    //while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
    SPI_ReceiveData8(SPI1);
    #endif
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
  #ifndef SPI_HW
  uint8_t i;
  #endif

  while (len--)
  {
    #ifndef SPI_HW
    for (i = 0; i < 8; i++)
    {
      *data <<= 1;
      if (SPI_MISO_GPIO->IDR & SPI_MISO_PIN)
        *data += 1;
      SPI_SCLK_GPIO->BSRR = SPI_SCLK_PIN;
      SPI_DELAY;
      SPI_SCLK_GPIO->BRR = SPI_SCLK_PIN;
    }
    #else
    SPI_SendData8(SPI1, 0x00);
    //while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
    *data = SPI_ReceiveData8(SPI1);
    #endif
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

/** \brief Reset SPI bus
 *
 * \return void
 *
 */
void SPI_Reset(void)
{
  SPI_UNSELECT;
  SPI_DELAY;
}

/** \brief Initialite SPI communication
 *
 * \return Nothing
 *
 */
void SPI_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStructure;

  GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStruct.GPIO_Pin = SPI_AD7293_CS_PIN;
  GPIO_Init(SPI_AD7293_CS_GPIO, &GPIO_InitStruct);

  #ifndef SPI_HW
  GPIO_InitStruct.GPIO_Pin = SPI_SCLK_PIN;
  GPIO_Init(SPI_SCLK_GPIO, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN;
  GPIO_Init(SPI_MOSI_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN;
  GPIO_Init(SPI_MISO_GPIO, &GPIO_InitStruct);
  #else
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Pin = SPI_SCLK_PIN | SPI_MISO_PIN | SPI_MOSI_PIN;
  GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
  GPIO_PinAFConfig(SPI_GPIO, GPIO_PinSource5, GPIO_AF_0);
  GPIO_PinAFConfig(SPI_GPIO, GPIO_PinSource6, GPIO_AF_0);
  GPIO_PinAFConfig(SPI_GPIO, GPIO_PinSource7, GPIO_AF_0);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  SPI_StructInit(&SPI_InitStructure);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
  SPI_Cmd(SPI1, ENABLE);
  #endif
}
