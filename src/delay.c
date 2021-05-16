#include "delay.h"

void DELAY_Coniguration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  DELAY_CLOCK_ENABLE;

  /**< Time base configuration, 1 second tick */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = DELAY_COUNT;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(DELAY_TIMER, &TIM_TimeBaseStructure);
}

void DELAY_Usec(uint16_t usec)
{
  DELAY_TIMER->CNT = 0;
  DELAY_TIMER->ARR = (uint16_t)(usec * SystemCoreClock / 1000000UL);
  DELAY_TIMER->SR = (uint16_t)~TIM_FLAG_Update;
  DELAY_TIMER->CR1 |= TIM_CR1_CEN;
  while (!(DELAY_TIMER->SR & TIM_FLAG_Update));
  DELAY_TIMER->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
}
