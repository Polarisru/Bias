#include "buttons.h"
#include "comm.h"
#include "outputs.h"
#include "pwm.h"
#include "uart.h"
#include "utils.h"

const char commKEY[] = "KEY";
const char commOUT[] = "OUT";
const char commPWM[] = "PWM";

const char commErrOk[]  = "OK";
const char commErrCmd[] = "E.C";
const char commErrPrm[] = "E.P";

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
  //uint8_t bVal;
  uint32_t intVal0, intVal1;

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

    if (0 == strncmp(cmd, commKEY, strlen(commKEY)))
    {
      /**< It is a KEY command */
      sprintf(buff, "K%1d", BUTTONS_GetValue());
    } else
    if (0 == strncmp(cmd, commOUT, strlen(commOUT)))
    {
      /**< It is a OUT command */
      //bVal = cmd[strlen(commOut)];
      //if (bVal < '0') || (bVal > '9')
      if (2 != sscanf(&cmd[strlen(commOUT)], "%1lu:%1lu", &intVal0, &intVal1))
      {
        strcpy(buff, commErrPrm);
      } else
      {
        //bVal -= '0';
        if (intVal1 == 0)
          OUTPUTS_Switch((uint8_t)intVal0, false);
        else
          OUTPUTS_Switch((uint8_t)intVal0, true);
        strcpy(buff, commErrOk);
      }
    } else
    if (0 == strncmp(cmd, commPWM, strlen(commPWM)))
    {
      /**< It is a PWM command */
      if (1 != sscanf(&cmd[strlen(commPWM)], "%lu", &intVal0))
      {
        strcpy(buff, commErrPrm);
      } else
      {
        PWM_SetValue((uint8_t)intVal0);
        strcpy(buff, commErrOk);
      }
    } else
    {
      /**< It is not a right command, show error */
      strcpy(buff, commErrCmd);
    }
    strcat(buff, LINE_CRLF);
    COMM_Send(buff);
  }
}
