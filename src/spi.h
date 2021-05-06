#ifndef SPI_H
#define SPI_H

#include "defines.h"

#define SPI_EE_CS_PIN     GPIO_Pin_4
#define SPI_EE_DO_PIN     GPIO_Pin_2
#define SPI_EE_DI_PIN     GPIO_Pin_10
#define SPI_EE_CLK_PIN    GPIO_Pin_5

#define SPI_EE_CS_GPIO    GPIOA
#define SPI_EE_DO_GPIO    GPIOB
#define SPI_EE_DI_GPIO    GPIOB
#define SPI_EE_CLK_GPIO   GPIOA

#define SPI_DELAY       {__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();}

enum
{
  SPI_SELECT_NONE,
  SPI_SELECT_EEPROM
};

void SPI_Configuration(void);
void SPI_Transmit(uint16_t data, uint8_t bits);
uint16_t SPI_Receive(uint8_t bits);
void SPI_Select(uint8_t device);

#endif
