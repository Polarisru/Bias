#ifndef GLOBAL_H
#define GLOBAL_H

#include "defines.h"

/**< EEPROM variables with default values */
int16_t  EE_GateVoltage[GATES_NUM];
uint16_t EE_DrainCurrent[DRAINS_NUM];
uint16_t EE_DrainVoltageMin;
uint16_t EE_DrainVoltageMax;
int8_t   EE_TemperatureMin;
int8_t   EE_TemperatureMax;

bool GLOBAL_Reset;

TaskHandle_t xTaskComm;

#endif
