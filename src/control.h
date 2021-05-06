#ifndef CONTROL_H
#define CONTROL_H

#include "defines.h"

#define CTRL_ERR_CONTER     100
#define CTRL_ERR_LEVEL      0.8f
/**< 0.1V sense voltage * 33k/1k / 3.3V reference * 8 bit ADC */
#define CTRL_ST_FACTOR      (uint8_t)(0.1f * 33 / 1 / 3.3f * 255 * CTRL_ERR_LEVEL)

void CONTROL_Configuration(void);
void CONTROL_SetPwmLimit(uint8_t value);
void CONTROL_ResetErrCode(void);
uint8_t CONTROL_GetErrCode(void);
void CONTROL_Task(void *pvParameters);

#endif
