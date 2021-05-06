#ifndef ACTIONS_H
#define ACTIONS_H

#include "defines.h"

enum {
  ACTION_INIT,
  ACTION_START,
  ACTION_SETT_POWER,
  ACTION_SETT_FAN,
  ACTION_SETT_ERR,
  ACTION_INPUT,
  ACTION_RUN,
  ACTION_PAUSE,
  ACTION_STOP,
  ACTION_CONTINUE,
  ACTION_WAIT,
  ACTION_ERROR,
  ACTION_LAST
};

typedef struct
{
  uint8_t pin;
  uint8_t err_code;
} TLedCheck;

#define ACTIONS_CLOCK_ENABLE  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE)

#define ACTIONS_TIMER         TIM16

#define ACTIONS_COUNT         1000UL

void ACTIONS_Task(void *pvParameters);

#endif
