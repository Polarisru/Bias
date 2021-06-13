#ifndef INPUTS_H
#define INPUTS_H

#include "defines.h"

enum {
  #ifdef DEF_NEW
  INPUT_ALERT,
  #endif
  INPUT_RESET,
  INPUT_LAST
};

#define INPUT_ALERT_PIN     GPIO_Pin_1
#define INPUT_RESET_PIN     GPIO_Pin_2

#define INPUT_ALERT_GPIO    GPIOA
#define INPUT_RESET_GPIO    GPIOA

typedef struct
{
  GPIO_TypeDef*     GPIO;
  uint16_t          GPIO_Pin;
  GPIOPuPd_TypeDef  PuPd;
} TInput;

void INPUTS_Configuration(void);
bool INPUTS_IsActive(uint8_t pin);

#endif
