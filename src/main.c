#include "defines.h"
#include "analog.h"
#include "comm.h"
#include "control.h"
#include "global.h"
#include "gpio.h"
#include "inputs.h"
#include "outputs.h"
#include "eeprom.h"

/**< LED blinking task */
void LED_Task(void *pParameters)
{
  (void) pParameters;

  EEPROM_Configuration();

  while (1)
  {
    #ifdef DEF_REMOTE
    if (ANALOG_GetValue() > 0x40)
      OUTPUTS_Switch(OUTPUT_LED, false);
    else
      OUTPUTS_Switch(OUTPUT_LED, true);
    #else
    OUTPUTS_Toggle(OUTPUT_LED);
    #endif
    vTaskDelay(1000);
  }
}

/**< Main function */
int main(void)
{
  GPIO_Configuration();
  OUTPUTS_Configuration();
  //INPUTS_Configuration();
  //ANALOG_Configuration();

  /**< Create RTOS tasks */
  xTaskCreate(LED_Task, "LedTask", 50, NULL, LED_TASK_PRIORITY, NULL);
  xTaskCreate(COMM_Task, "CommTask", 300, NULL, COMM_TASK_PRIORITY, &xTaskComm);
  //xTaskCreate(CONTROL_Task, "ControlTask", 50, NULL, CONTROL_TASK_PRIORITY, NULL);
  /**< Start the scheduler */
	vTaskStartScheduler();
}
