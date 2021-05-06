#include "defines.h"
#include "actions.h"
#include "comm.h"
#include "control.h"
#include "display.h"
#include "eeprom.h"
#include "global.h"
#include "gpio.h"
#include "inputs.h"
#include "outputs.h"
#include "pwm.h"
#include "settings.h"
#include "sound.h"

/**< LED blinking task */
void LED_Task(void *pvParameters)
{
  //uint16_t i = 0;
  //uint8_t val;
  //char txt[8];

  while (1)
  {
    OUTPUTS_Switch(OUTPUT_LED1, true);
    vTaskDelay(500);
    OUTPUTS_Switch(OUTPUT_LED1, false);
    //EEPROM_WriteByte(i, i);
    vTaskDelay(500);
    //val = EEPROM_ReadByte(i);
//    i++;
//    txt[0] = '0' + (i / 60) / 10;
//    txt[1] = '0' + (i / 60) % 10;
//    txt[2] = ':';
//    txt[3] = '0' + (i % 60) / 10;
//    txt[4] = '0' + (i % 60) % 10;
//    txt[5] = 0;
//    DISPLAY_Write(txt);
  }
}

/**< Main function */
int main(void)
{
  GPIO_Configuration();
  DISPLAY_Configuration();
  OUTPUTS_Configuration();
  INPUTS_Configuration();
  //EEPROM_Configuration();
  PWM_Configuration();
  SOUND_Configuration();

  CONTROL_Configuration();

  /**< Create RTOS tasks */
  xTaskCreate(LED_Task, "LedTask", 100, NULL, tskIDLE_PRIORITY + 1, NULL);
  //xTaskCreate(COMM_Task, "CommTask", 400, NULL, tskIDLE_PRIORITY + 1, &xTaskComm);
  xTaskCreate(CONTROL_Task, "ControlTask", 100, NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(ACTIONS_Task, "MainTask", 200, NULL, tskIDLE_PRIORITY + 2, NULL);
  xTaskCreate(DISPLAY_Task, "DisplayTask", 100, NULL, tskIDLE_PRIORITY + 3, NULL);
  /**< Start the scheduler */
	vTaskStartScheduler();
}
