#ifndef CONTROL_H
#define CONTROL_H

#include "defines.h"

void CONTROL_Configuration(void);
void CONTROL_ResetErrCode(void);
uint8_t CONTROL_GetErrCode(void);
void CONTROL_Task(void *pvParameters);

#endif
