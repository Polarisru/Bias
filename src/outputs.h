#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "defines.h"

enum {
  OUTPUT_LED,
  OUTPUT_LAST
};

typedef struct
{
  GPIO_TypeDef* GPIO;
  uint16_t      GPIO_Pin;
} TOutput;

#ifdef DEF_REMOTE
  #define OUTPUT_LED_PIN     GPIO_Pin_3
  #define OUTPUT_LED_GPIO    GPIOA
#else
  #define OUTPUT_LED_PIN     GPIO_Pin_11
  #define OUTPUT_LED_GPIO    GPIOB
#endif

void OUTPUTS_Switch(uint8_t pin, bool on);
void OUTPUTS_Toggle(uint8_t pin);
void OUTPUTS_Configuration(void);

#endif
