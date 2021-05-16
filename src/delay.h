#ifndef DELAY_H
#define DELAY_H

#include "defines.h"

#define DELAY_CLOCK_ENABLE  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE)

#define DELAY_TIMER         TIM16

#define DELAY_COUNT         1000UL

#define DELAY_1USEC         uint8_t count = 48; while(count--)

void DELAY_Coniguration(void);
void DELAY_Usec(uint16_t usec);

#endif
