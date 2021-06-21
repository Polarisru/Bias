#include "ad7293.h"
#include "comm.h"
#include "control.h"
#include "delay.h"
#include "global.h"
#include "inputs.h"
#include "outputs.h"
#include "spi.h"

static uint8_t CONTROL_ErrCode;
volatile float temp;

/** \brief Configure control module
 *
 * \return void
 *
 */
void CONTROL_Configuration(void)
{
  OUTPUTS_Switch(OUTPUT_RESET, true);
}

/** \brief Reset error codes
 *
 * \return void
 *
 */
void CONTROL_ResetErrCode(void)
{
  CONTROL_ErrCode = ERR_NONE;
}

/** \brief Get error codes value
 *
 * \return uint8_t
 *
 */
uint8_t CONTROL_GetErrCode(void)
{
  return CONTROL_ErrCode;
}

/**< Control task for monitoring */
void CONTROL_Task(void *pParameters)
{
  (void) pParameters;
  uint16_t alerts;
  uint16_t status, status_c;
  uint8_t i;

  GLOBAL_Reset = 0;

  CONTROL_ErrCode = ERR_NONE;

  CONTROL_ResetErrCode();

  /**< Configure peripherals */
  CONTROL_Configuration();
  SPI_Configuration();

  while (1)
  {
    COMM_Send("123\n");
    vTaskDelay(100);
  }

  COMM_Send("AD7293: ");
  if (AD7293_Configuration() == false)
  {
    COMM_Send("NOT DETECTED!\n");
    while (1);
  }
  COMM_Send("DETECTED\n");

  /**< Wait for 1ms before enabling output */
  vTaskDelay(1);
  /**< Enable PA_ON */
  AD7293_SetPowerOn();

  /**< Wait for 5ms */
  vTaskDelay(5);
  /**< Set gate1..4 voltages from EEPROM (bipolar outputs) */
  for (i = 0; i < 4; i++)
    AD7293_SetGateVoltage(i, EE_GateVoltage[i]);

  /**< Wait for 2us */
  DELAY_Usec(2);
  /**< Set gate5..8 voltages from EEPROM (unipolar outputs) */
  for (i = 4; i < GATES_NUM; i++)
    AD7293_SetGateVoltage(i, EE_GateVoltage[i]);

  /**< Wait for 5ms */
  vTaskDelay(5);
  /**< Set RF_SWITCH pin */
  OUTPUTS_Switch(OUTPUT_SWITCH, true);

  vTaskDelay(20);

  GLOBAL_Reset = 0;
  //AD7293_ResetAlerts();
  AD7293_EnablePowerMonitoring();

  while (1)
  {
    if (CONTROL_ErrCode != ERR_NONE)
    {
      vTaskDelay(100);
      continue;
    }
//    DELAY_Usec(30);
//    temp = AD7293_GetDrainCurrent(0);
//    temp = AD7293_GetSupplyVoltage(0);
//    temp = AD7293_GetTemperature(0);
    #ifdef DEF_NEW
    if (GLOBAL_Reset > 0)
    #else
    DELAY_Usec(30);
    alerts = AD7293_GetAlerts();
    if ((alerts & AD7293_ALERTS_MASK) || ((GLOBAL_Reset > 0) && (INPUTS_IsActive(INPUT_RESET) == true)))
    #endif
    {
      #ifdef DEF_NEW
      alerts = AD7293_GetAlerts();
      #endif
      /**< Something is going wrong, stop working */
      if (alerts & REGISTER_ALERT_SUM_ISENSX_HIGH)
      {
        /**< Short-circuit */
        status_c = AD7293_GetCurrentAlerts();
        AD7293_SetPowerOff();
      }
      /**< Reset RF_SWITCH pin */
      OUTPUTS_Switch(OUTPUT_SWITCH, false);
      /**< Wait for 3ms */
      vTaskDelay(3);
      /**< Reset bi-polar outputs */
      for (i = 0; i < 4; i++)
        AD7293_SetGateVoltage(i, GATE14_MIN_VALUE);

      /**< Wait for 100ms */
      vTaskDelay(100);
      /**< Disable PA_ON */
      AD7293_SetPowerOff();
      /**< Show terminal message with error code */
      COMM_Send("ERROR: ");
      if (alerts & REGISTER_ALERT_SUM_RSX_LOW)
      {
        COMM_Send("Power supply low");
        CONTROL_ErrCode = ERR_SUPPLY_LOW;
      } else
      if (alerts & REGISTER_ALERT_SUM_RSX_HIGH)
      {
        COMM_Send("Power supply high");
        CONTROL_ErrCode = ERR_SUPPLY_HIGH;
      } else
      if (alerts & (REGISTER_ALERT_SUM_TSENSX_HIGH | REGISTER_ALERT_SUM_TSENSX_LOW))
      {
        status = AD7293_GetTemperatureAlerts();
        if (status & REGISTER_ALERT_TSENSX_HIGH_D1)
        {
          COMM_Send("D1 Temperature high");
          CONTROL_ErrCode = ERR_TEMP_D1_HIGH;
        } else
        if (status & REGISTER_ALERT_TSENSX_HIGH_D0)
        {
          COMM_Send("D0 Temperature high");
          CONTROL_ErrCode = ERR_TEMP_D0_HIGH;
        } else
        if (status & REGISTER_ALERT_TSENSX_HIGH_INT)
        {
          COMM_Send("INT Temperature high");
          CONTROL_ErrCode = ERR_TEMP_INT_HIGH;
        } else
        if (status & REGISTER_ALERT_TSENSX_LOW_D1)
        {
          COMM_Send("D1 Temperature low");
          CONTROL_ErrCode = ERR_TEMP_D1_LOW;
        } else
        if (status & REGISTER_ALERT_TSENSX_LOW_D0)
        {
          COMM_Send("D0 Temperature low");
          CONTROL_ErrCode = ERR_TEMP_D0_LOW;
        } else
        if (status & REGISTER_ALERT_TSENSX_LOW_INT)
        {
          COMM_Send("INT Temperature low");
          CONTROL_ErrCode = ERR_TEMP_INT_LOW;
        }
      } else
      if (alerts & REGISTER_ALERT_SUM_ISENSX_HIGH)
      {
        status = status_c;
        if (status & REGISTER_ALERT_ISENSX_HIGH_0)
        {
          COMM_Send("Drain1 current high");
          CONTROL_ErrCode = ERR_DC1;
        } else
        if (status & REGISTER_ALERT_ISENSX_HIGH_1)
        {
          COMM_Send("Drain2 current high");
          CONTROL_ErrCode = ERR_DC2;
        } else
        if (status & REGISTER_ALERT_ISENSX_HIGH_2)
        {
          COMM_Send("Drain3 current high");
          CONTROL_ErrCode = ERR_DC3;
        } else
        if (status & REGISTER_ALERT_ISENSX_HIGH_3)
        {
          COMM_Send("Drain4 current high");
          CONTROL_ErrCode = ERR_DC4;
        }
      }
      if (CONTROL_ErrCode == ERR_NONE)
      {
        COMM_Send("External reset");
        CONTROL_ErrCode = ERR_UNKNOWN;
      }
      COMM_Send("\n");
    }
  }
}
