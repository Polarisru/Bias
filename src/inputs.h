#ifndef INPUTS_H
#define INPUTS_H

#include "defines.h"

enum {
  INPUT_KEY1,
  INPUT_LAST
};

#define INPUT_KEY1_PIN      GPIO_Pin_13

#define INPUT_KEY1_GPIO     GPIOC

typedef struct
{
  GPIO_TypeDef* GPIO;
  uint16_t      GPIO_Pin;
} TInput;

void INPUTS_Configuration(void);
bool INPUTS_IsActive(uint8_t pin);

#endif
