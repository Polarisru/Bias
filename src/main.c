#include "defines.h"
#include "delay.h"
#include "analog.h"
#include "comm.h"
#include "control.h"
#include "global.h"
#include "gpio.h"
#include "inputs.h"
#include "outputs.h"
#include "eeprom.h"

volatile uint8_t value;

/**< LED blinking task */
void LED_Task(void *pParameters)
{
  (void) pParameters;

  while (1)
  {
    vTaskDelay(1000);
    #ifdef DEF_REMOTE
    value = ANALOG_GetValue();
    if ((value > ANALOG_MAX) || (value < ANALOG_MIN))
      OUTPUTS_Switch(OUTPUT_LED, true);
    else
      OUTPUTS_Switch(OUTPUT_LED, false);
    #else
    OUTPUTS_Toggle(OUTPUT_LED);
    #endif
  }
}

/**< Main function */
int main(void)
{
  GPIO_Configuration();
  DELAY_Coniguration();
  OUTPUTS_Configuration();
  INPUTS_Configuration();
  ANALOG_Configuration();
  EEPROM_Configuration();

  /**< Create RTOS tasks */
  xTaskCreate(LED_Task, "LedTask", 50, NULL, LED_TASK_PRIORITY, NULL);
  xTaskCreate(COMM_Task, "CommTask", 300, NULL, COMM_TASK_PRIORITY, &xTaskComm);
  xTaskCreate(CONTROL_Task, "ControlTask", 50, NULL, CONTROL_TASK_PRIORITY, NULL);
  /**< Start the scheduler */
	vTaskStartScheduler();
}
