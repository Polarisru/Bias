#include "defines.h"
#include "actions.h"
#include "comm.h"
#include "control.h"
#include "global.h"
#include "gpio.h"
#include "inputs.h"
#include "outputs.h"
#include "eeprom.h"

/**< LED blinking task */
void LED_Task(void *pvParameters)
{
  EEPROM_Configuration();

  while (1)
  {
    OUTPUTS_Switch(OUTPUT_LED1, true);
    vTaskDelay(500);
    OUTPUTS_Switch(OUTPUT_LED1, false);
    vTaskDelay(500);
  }
}

/**< Main function */
int main(void)
{
  GPIO_Configuration();
  //OUTPUTS_Configuration();
  //INPUTS_Configuration();

  /**< Create RTOS tasks */
  xTaskCreate(LED_Task, "LedTask", 50, NULL, LED_TASK_PRIORITY, NULL);
  xTaskCreate(COMM_Task, "CommTask", 100, NULL, COMM_TASK_PRIORITY, &xTaskComm);
  xTaskCreate(CONTROL_Task, "ControlTask", 50, NULL, CONTROL_TASK_PRIORITY, NULL);
  //xTaskCreate(ACTIONS_Task, "MainTask", 100, NULL, tskIDLE_PRIORITY + 2, NULL);
  /**< Start the scheduler */
	vTaskStartScheduler();
}
