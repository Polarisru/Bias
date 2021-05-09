#ifndef ANALOG_H
#define ANALOG_H

#include "defines.h"

#define ANALOG_9V_PIN     GPIO_Pin_0

#define ANALOG_9V_PORT    GPIOA

void ANALOG_Configuration(void);
uint8_t ANALOG_GetValue(void);

#endif
