#ifndef GLOBAL_H
#define GLOBAL_H

#include "defines.h"

/**< EEPROM variables with default values */
uint8_t EE_ValuePWM;
uint8_t EE_Time;
uint8_t EE_FanTimeout;
uint8_t EE_ErrOn;

TaskHandle_t xTaskComm;

#endif
