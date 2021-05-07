#include "actions.h"
#include "control.h"
#include "eeprom.h"
#include "global.h"
#include "inputs.h"
#include "outputs.h"

/** \brief Configure actions module, just starts timer
 *
 * \return Nothing
 *
 */
void ACTIONS_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  ACTIONS_CLOCK_ENABLE;

  /**< Time base configuration, 1 second tick */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = ACTIONS_COUNT;
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock / 1 / ACTIONS_COUNT) - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(ACTIONS_TIMER, &TIM_TimeBaseStructure);
}

/**< Main application task */
void ACTIONS_Task(void *pvParameters)
{
  uint8_t action = ACTION_INIT;
  uint8_t errCode;

  EEPROM_Configuration();
  ACTIONS_Configuration();

  while (1)
  {
    switch (action)
    {
      case ACTION_INIT:
        break;
    }
    vTaskDelay(1);
  }
}
