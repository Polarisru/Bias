#include "ad7293.h"
#include "comm.h"
#include "eeprom.h"
#include "global.h"
#include "outputs.h"
#include "uart.h"
#include "utils.h"

const char commID[]   = "ID";
const char commGV[]   = "GV";
const char commDC[]   = "DC";
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
  char buff[128];
  uint16_t pos;
  char *cmd;
  int32_t ival32;
  uint32_t uval32;
  char ch;

  /**< Configure UART connection */
  UART_Configuration();

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
