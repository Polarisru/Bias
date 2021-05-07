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
  OUTPUTS_Configuration();
  INPUTS_Configuration();

  CONTROL_Configuration();

  /**< Create RTOS tasks */
  xTaskCreate(LED_Task, "LedTask", 50, NULL, tskIDLE_PRIORITY + 1, NULL);
  //xTaskCreate(COMM_Task, "CommTask", 400, NULL, tskIDLE_PRIORITY + 1, &xTaskComm);
  xTaskCreate(CONTROL_Task, "ControlTask", 50, NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(ACTIONS_Task, "MainTask", 100, NULL, tskIDLE_PRIORITY + 2, NULL);
  /**< Start the scheduler */
	vTaskStartScheduler();
}
