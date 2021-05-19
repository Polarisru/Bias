#include "ad7293.h"
#include "comm.h"
#include "control.h"
#include "eeprom.h"
#include "global.h"
#include "outputs.h"
#include "uart.h"
#include "utils.h"

const char commID[]   = "ID";
const char commGV[]   = "GV";
const char commDC[]   = "DC";
const char commT[]    = "T";
const char commE[]    = "E?";
const char commPON[]  = "PON";
const char commPOFF[] = "POFF";

const char commErrOk[]  = "OK";
const char commErrCmd[] = "E.C";
const char commErrPrm[] = "E.P";
const char commAnsID[] = "AD7293";

/** \brief Send answer
 *
 * \param [in] buffer Data to send
 * \return Nothing
 *
 */
void COMM_Send(char *buffer)
{
  uint8_t len = strlen(buffer);

  UART_Send((uint8_t*)buffer, len);
}

/** \brief RTOS task for processing UART connection
 */
void COMM_Task(void *pParameters)
{
  (void) pParameters;
  char buff[128];
  uint16_t pos;
  char *cmd;
  int32_t ival32;
  uint32_t uval32;
  char ch;

  /**< Configure UART connection */
  UART_Configuration();

  /**< Print actual configuration */
  COMM_Send("---Actual settings---\n");
  for (uval32 = 0; uval32 < GATES_NUM; uval32++)
  {
    sprintf(buff, "Gate%1lu voltage: %d.%03dV\n", uval32 + 1, EE_GateVoltage[uval32] / 1000, -(EE_GateVoltage[uval32] % 1000));
    COMM_Send(buff);
  }
  for (uval32 = 0; uval32 < DRAINS_NUM; uval32++)
  {
    sprintf(buff, "Drain%1lu current: %dmA\n", uval32 + 1, EE_DrainCurrent[uval32]);
    COMM_Send(buff);
  }
  sprintf(buff, "Supply voltage range: %d.%01d..%d.%01dV\n", EE_DrainVoltageMin / 10, EE_DrainVoltageMin % 10, EE_DrainVoltageMax / 10, EE_DrainVoltageMax % 10);
  COMM_Send(buff);
  sprintf(buff, "Temperature range: %d..%d°C\n", EE_TemperatureMin, EE_TemperatureMax);
  COMM_Send(buff);

  while (1)
  {
    /**< Wait for notification from the RX interrupt */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    cmd = UART_RxBuffer;
    pos = 0;
    while ((CHAR_CR != cmd[pos]) && (CHAR_NL != cmd[pos]) && cmd[pos])
      pos++;

    if (pos >= COMM_BUFFER_SIZE)
      pos = COMM_BUFFER_SIZE - 1;
    cmd[pos] = '\0';

    /**< Find command - skip blanks */
    while (*cmd && isspace(*cmd))
      cmd++;
    pos = 0;
    while (cmd[pos] && (!isspace(cmd[pos])))
      pos++;
    cmd[pos] = '\0';

    UTILS_StrUpr(cmd);

    if (0 == strncmp(cmd, commID, strlen(commID)))
    {
      strcpy(buff, commAnsID);
    } else
    if (0 == strncmp(cmd, commGV, strlen(commGV)))
    {
      /**< It is a GV command */
      if (sscanf(&cmd[strlen(commGV)], "%1lu%c%ld", &uval32, &ch, &ival32) < 2)
      {
        strcpy(buff, commErrPrm);
      } else
      {
        if ((uval32 == 0) || (uval32 > GATES_NUM))
        {
          strcpy(buff, commErrPrm);
        } else
        if ((ch != '?') && (ch != '='))
        {
          strcpy(buff, commErrPrm);
        } else
        if (ch == '=')
        {
          /**< It's a GVx= command */
          if (uval32 < 4)
          {
            if (ival32 < GATE14_MIN_VALUE)
              ival32 = GATE14_MIN_VALUE;
            if (ival32 > GATE14_MAX_VALUE)
              ival32 = GATE14_MAX_VALUE;
          } else
          {
            if (ival32 < GATE58_MIN_VALUE)
              ival32 = GATE58_MIN_VALUE;
            if (ival32 > GATE58_MAX_VALUE)
              ival32 = GATE58_MAX_VALUE;
          }
          EE_GateVoltage[uval32 - 1] = ival32;
          EEPROM_SaveVariable(&EE_GateVoltage[uval32 - 1]);
          strcpy(buff, commErrOk);
        } else
        {
          /**< It is a GVx? command */
          sprintf(buff, "%d", EE_GateVoltage[uval32 - 1]);
        }
      }
    } else
    if (0 == strncmp(cmd, commDC, strlen(commDC)))
    {
      /**< It is a DC command */
      if (sscanf(&cmd[strlen(commDC)], "%1lu%c%lu", &uval32, &ch, &ival32) < 2)
      {
        strcpy(buff, commErrPrm);
      } else
      {
        if ((uval32 == 0) || (uval32 > DRAINS_NUM))
        {
          strcpy(buff, commErrPrm);
        } else
        if ((ch != '?') && (ch != '='))
        {
          strcpy(buff, commErrPrm);
        } else
        if (ch == '=')
        {
          /**< It's a DCx= command */
          EE_DrainCurrent[uval32 - 1] = ival32;
          EEPROM_SaveVariable(&EE_DrainCurrent[uval32 - 1]);
          strcpy(buff, commErrOk);
        } else
        {
          /**< It is a DCx? command */
          sprintf(buff, "%u", EE_DrainCurrent[uval32 - 1]);
        }
      }
    } else
    if (0 == strncmp(cmd, commT, strlen(commT)))
    {
      /**< It is a T command */
      if (sscanf(&cmd[strlen(commT)], "%c%lu..%lu", &ch, &ival32, &uval32) < 1)
      {
        strcpy(buff, commErrPrm);
      } else
      {
        if ((ch != '?') && (ch != '='))
        {
          strcpy(buff, commErrPrm);
        } else
        {
          if (ch == '?')
          {
            /**< It is a T? command */
            sprintf(buff, "%d..%d", EE_TemperatureMin, EE_TemperatureMax);
          } else
          {
            /**< It's a T= command */
            EE_TemperatureMin = (int8_t)ival32;
            EE_TemperatureMax = (int8_t)uval32;
            EEPROM_SaveAllVariables();
            strcpy(buff, commErrOk);
          }
        }
      }
    } else
    if (0 == strncmp(cmd, commE, strlen(commE)))
    {
      /**< It's a E? command */
      sprintf(buff, "%u", CONTROL_GetErrCode());
    } else
    if (0 == strncmp(cmd, commPON, strlen(commPON)))
    {
      /**< It is a PON command */
      AD7293_SetPowerOn();
      strcpy(buff, commErrOk);
    } else
    if (0 == strncmp(cmd, commPOFF, strlen(commPOFF)))
    {
      /**< It is a POFF command */
      AD7293_SetPowerOff();
      strcpy(buff, commErrOk);
    } else
    {
      /**< It is not a right command, show error */
      strcpy(buff, commErrCmd);
    }
    strcat(buff, LINE_CRLF);
    COMM_Send(buff);
  }
}
