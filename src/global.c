#include "global.h"

/**< EEPROM variables with default values */
int16_t  EE_GateVoltage[GATES_NUM];
uint16_t EE_DrainCurrent[DRAINS_NUM];
uint16_t EE_DrainVoltageMin;
uint16_t EE_DrainVoltageMax;
int8_t   EE_TemperatureMin;
int8_t   EE_TemperatureMax;

volatile uint8_t GLOBAL_Reset;
uint16_t GLOBAL_ErrStatus;

TaskHandle_t xTaskComm;
