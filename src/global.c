#include "global.h"

/**< EEPROM variables with default values */
uint8_t EE_ValuePWM;    // default power value
uint8_t EE_Time;        // default time value in minutes
uint8_t EE_FanTimeout;  // timeout for fan switch off
uint8_t EE_ErrOn;       // enable/disable LED errors

TaskHandle_t xTaskComm;
