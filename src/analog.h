#ifndef ANALOG_H
#define ANALOG_H

#include "defines.h"

#define ANALOG_REF          3.3f
#define ANALOG_DIV          6
#define ANALOG_SUPPLY       9.0f
#define ANALOG_DEVIATION    0.25f
#define ANALOG_RESOLUTION   256

#define ANALOG_MIN          (uint8_t)((ANALOG_SUPPLY - ANALOG_DEVIATION) / ANALOG_DIV / ANALOG_REF * ANALOG_RESOLUTION)
#define ANALOG_MAX          (uint8_t)((ANALOG_SUPPLY + ANALOG_DEVIATION) / ANALOG_DIV / ANALOG_REF * ANALOG_RESOLUTION)

void ANALOG_Configuration(void);
uint8_t ANALOG_GetValue(void);

#endif
