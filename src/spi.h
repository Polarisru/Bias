#ifndef SPI_H
#define SPI_H

#include "defines.h"

#ifndef DEF_REMOTE
  #define SPI_AD7293_CS_PIN     GPIO_Pin_7
#else
  #define SPI_AD7293_CS_PIN     GPIO_Pin_4
#endif
#define SPI_MOSI_PIN          GPIO_Pin_7
#define SPI_MISO_PIN          GPIO_Pin_6
#define SPI_SCLK_PIN          GPIO_Pin_5

#ifndef DEF_REMOTE
  #define SPI_AD7293_CS_GPIO    GPIOF
#else
  #define SPI_AD7293_CS_GPIO    GPIOA
#endif
#define SPI_MOSI_GPIO         GPIOA
#define SPI_MISO_GPIO         GPIOA
#define SPI_SCLK_GPIO         GPIOA

#define SPI_DELAY       {__NOP(); __NOP(); __NOP(); __NOP();}

enum
{
  SPI_SELECT_NONE,
  SPI_SELECT_AD7293
};

void SPI_Configuration(void);
void SPI_Transmit(uint8_t *data, uint8_t len);
void SPI_Receive(uint8_t *data, uint8_t len);
void SPI_Select(uint8_t device);

#endif
