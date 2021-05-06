#include "actions.h"
#include "buttons.h"
#include "control.h"
#include "display.h"
#include "global.h"
#include "inputs.h"
#include "outputs.h"
#include "pwm.h"
#include "settings.h"
#include "sound.h"

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

  /* TIM Interrupts enable */
  //TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);
}

/**< Main application task */
void ACTIONS_Task(void *pvParameters)
{
  uint8_t action = ACTION_INIT;
  uint16_t counter;
  uint8_t  mins = 0;
  bool changed = false;
  uint8_t buttonVal = BUTTONS_VAL_NONE;
  char str[8];
  char str2[4];
  bool fanOn;
  uint8_t errCode;
  uint32_t fanCounter = 0;
  uint8_t stopSource;

  SETTINGS_Configuration();
  ACTIONS_Configuration();

  mins = EE_Time;

  while (1)
  {
    if ((xTaskGetTickCount() - fanCounter) > (uint32_t)(EE_FanTimeout + 1) * configTICK_RATE_HZ)
      OUTPUTS_Switch(OUTPUT_FAN1, false);
    switch (action)
    {
      case ACTION_INIT:
        SOUND_Play(ssOn);
        vTaskDelay(500);
        DISPLAY_Clear();
        TIM_Cmd(ACTIONS_TIMER, ENABLE);
        changed = true;
        action = ACTION_INPUT;
        break;
      case ACTION_START:
        counter = (uint16_t)mins * 60;
        CONTROL_ResetErrCode();
        EE_Time = mins;
        SETTINGS_WriteVar(&EE_Time);
        TIM_SetCounter(ACTIONS_TIMER, 0);
        TIM_ClearFlag(ACTIONS_TIMER, TIM_FLAG_Update);
        /**< Enable all signals */
        OUTPUTS_Switch(OUTPUT_LED2, true);
        OUTPUTS_Switch(OUTPUT_FAN1, true);
        OUTPUTS_Switch(OUTPUT_MOTOR, true);
        CONTROL_SetPwmLimit(EE_ValuePWM);
        PWM_SetValue(EE_ValuePWM);
        fanOn = true;
        /**< Reset fan timeout */
        fanCounter = xTaskGetTickCount();
        action = ACTION_RUN;
        break;
      case ACTION_SETT_POWER:
        buttonVal = BUTTONS_GetValue();
        if (buttonVal == BUTTONS_VAL_START)
        {
          SOUND_Play(ssClick);
          action = ACTION_INPUT;
          SETTINGS_WriteVar(&EE_ValuePWM);
          changed = true;
          break;
        } else
        if (buttonVal == BUTTONS_VAL_UP)
        {
          SOUND_Play(ssClick);
          if (EE_ValuePWM < PWM_COUNT)
          {
            EE_ValuePWM += 10;
            changed = true;
          }
        } else
        if (buttonVal == BUTTONS_VAL_DOWN)
        {
          SOUND_Play(ssClick);
          if (EE_ValuePWM > PWM_MIN_COUNT)
          {
            EE_ValuePWM -= 10;
            changed = true;
          }
        } else
        if (buttonVal == BUTTONS_VAL_SETT)
        {
          SOUND_Play(ssClick);
          action = ACTION_SETT_FAN;
          changed = true;
          break;
        }
        if (changed)
        {
          changed = false;
          sprintf(str, "P%3d", EE_ValuePWM);
          DISPLAY_Write(str);
        }
        break;
      case ACTION_SETT_FAN:
        buttonVal = BUTTONS_GetValue();
        if (buttonVal == BUTTONS_VAL_START)
        {
          SOUND_Play(ssClick);
          action = ACTION_INPUT;
          SETTINGS_WriteVar(&EE_FanTimeout);
          changed = true;
          break;
        } else
        if (buttonVal == BUTTONS_VAL_UP)
        {
          SOUND_Play(ssClick);
          if (EE_FanTimeout < FAN_TIMEOUT_MAX)
          {
            EE_FanTimeout += 5;
            changed = true;
          }
        } else
        if (buttonVal == BUTTONS_VAL_DOWN)
        {
          SOUND_Play(ssClick);
          if (EE_FanTimeout > FAN_TIMEOUT_MIN)
          {
            EE_FanTimeout -= 5;
            changed = true;
          }
        } else
        if (buttonVal == BUTTONS_VAL_SETT)
        {
          SOUND_Play(ssClick);
          action = ACTION_SETT_ERR;
          changed = true;
          break;
        }
        if (changed)
        {
          changed = false;
          sprintf(str, "F%3d", EE_FanTimeout);
          DISPLAY_Write(str);
        }
        break;
      case ACTION_SETT_ERR:
        /**< Enable/disable LED errors */
        buttonVal = BUTTONS_GetValue();
        if (buttonVal == BUTTONS_VAL_START)
        {
          SOUND_Play(ssClick);
          action = ACTION_INPUT;
          SETTINGS_WriteVar(&EE_ErrOn);
          changed = true;
          break;
        } else
        if ((buttonVal == BUTTONS_VAL_UP) || (buttonVal == BUTTONS_VAL_DOWN))
        {
          SOUND_Play(ssClick);
          if (EE_ErrOn == ERR_MODE_OFF)
            EE_ErrOn = ERR_MODE_ON;
          else
            EE_ErrOn = ERR_MODE_OFF;
          changed = true;
        } else
        if (buttonVal == BUTTONS_VAL_SETT)
        {
          SOUND_Play(ssClick);
          action = ACTION_SETT_POWER;
          changed = true;
          break;
        }
        if (changed)
        {
          changed = false;
          if (EE_ErrOn == ERR_MODE_OFF)
            strcpy(str, "E 0F");
          else
            strcpy(str, "E 0n");
          DISPLAY_Write(str);
        }
        break;
      case ACTION_INPUT:
        buttonVal = BUTTONS_GetValue();
        if (buttonVal == BUTTONS_VAL_START)
        {
          SOUND_Play(ssClick);
          action = ACTION_START;
          break;
        } else
        if (buttonVal == BUTTONS_VAL_UP)
        {
          SOUND_Play(ssClick);
          if (mins < MINS_VAL_MAX)
          {
            mins++;
            changed = true;
          }
        } else
        if (buttonVal == BUTTONS_VAL_DOWN)
        {
          SOUND_Play(ssClick);
          if (mins > MINS_VAL_MIN)
          {
            mins--;
            changed = true;
          }
        } else
        if (buttonVal == BUTTONS_VAL_SETT)
        {
          SOUND_Play(ssClick);
          action = ACTION_SETT_POWER;
          changed = true;
          break;
        }
        if (changed)
        {
          changed = false;
          sprintf(str, "%02d:00", mins);
          DISPLAY_Write(str);
        }
        break;
      case ACTION_CONTINUE:
        /**< Continue running */
        /**< Enable all signals */
        OUTPUTS_Switch(OUTPUT_LED2, true);
        OUTPUTS_Switch(OUTPUT_FAN1, fanOn);
        OUTPUTS_Switch(OUTPUT_MOTOR, true);
        PWM_SetValue(EE_ValuePWM);
        action = ACTION_RUN;
        break;
      case ACTION_RUN:
        /**< Reset fan timeout */
        fanCounter = xTaskGetTickCount();
        buttonVal = BUTTONS_GetValue();
        if (buttonVal == BUTTONS_VAL_START)
        {
          SOUND_Play(ssClick);
          DISPLAY_DoBlink(true);
          stopSource = STOP_SRC_BUTTON;
          action = ACTION_STOP;
          break;
        } else
        if (buttonVal == BUTTONS_VAL_FAN)
        {
          /**< Disable/enable fan */
          SOUND_Play(ssClick);
          fanOn = !fanOn;
          OUTPUTS_Switch(OUTPUT_LED2, true);
          if (fanOn == true)
            OUTPUTS_Switch(OUTPUT_FAN1, true);
          else
            OUTPUTS_Switch(OUTPUT_FAN1, false);
        }
        if (INPUTS_IsActive(INPUT_SENS) != false)
        {
          /**< Door is open, stop running */
          SOUND_Play(ssError);
          DISPLAY_DoBlink(true);
          stopSource = STOP_SRC_DOOR;
          action = ACTION_STOP;
          break;
        }
//          for (i = 0; i < 2; i++)
//          {
//            if (INPUTS_IsActive(ACTIONS_LedCheck[i].pin) == false)
//            {
//              errCounter[ACTIONS_LedCheck[i].err_code]++;
//              if (errCounter[ACTIONS_LedCheck[i].err_code] > ERR_COUNTER_MAX)
//                errCode = ACTIONS_LedCheck[i].err_code;
//            } else
//            {
//              errCounter[ACTIONS_LedCheck[i].err_code] = 0;
//            }
//          }
        if (EE_ErrOn == ERR_MODE_ON)
        {
          /**< Check LED errors */
          errCode = CONTROL_GetErrCode();
          if (errCode != ERR_CODE_NONE)
          {
            action = ACTION_ERROR;
            break;
          }
        }
        if (TIM_GetFlagStatus(ACTIONS_TIMER, TIM_FLAG_Update) == SET)
        {
          counter--;
          TIM_ClearFlag(ACTIONS_TIMER, TIM_FLAG_Update);
          if (fanOn == false)
            OUTPUTS_Toggle(OUTPUT_LED2);
          sprintf(str, "%02d:%02d", counter / 60, counter % 60);
          DISPLAY_Write(str);
        }
        if (counter == 0)
        {
          action = ACTION_STOP;
          stopSource = STOP_SRC_TIME;
          break;
        }
        break;
      case ACTION_STOP:
        /**< Disable all signals */
        OUTPUTS_Switch(OUTPUT_LED2, false);
        //OUTPUTS_Switch(OUTPUT_FAN1, false);
        OUTPUTS_Switch(OUTPUT_MOTOR, false);
        PWM_SwitchOff();
        //TIM_Cmd(ACTIONS_TIMER, DISABLE);
        DISPLAY_DoBlink(true);
        action = ACTION_WAIT;
        break;
      case ACTION_WAIT:
        if (TIM_GetFlagStatus(ACTIONS_TIMER, TIM_FLAG_Update) == SET)
        {
          TIM_ClearFlag(ACTIONS_TIMER, TIM_FLAG_Update);
          if (errCode == ERR_CODE_NONE)
            SOUND_Play(ssBeep);
          else
            SOUND_Play(ssError);
        }
        buttonVal = BUTTONS_GetValue();
        if (buttonVal == BUTTONS_VAL_START)
        {
          SOUND_Play(ssClick);
          DISPLAY_DoBlink(false);
          changed = true;
          action = ACTION_INPUT;
          break;
        }
        if (((buttonVal == BUTTONS_VAL_UP) || (buttonVal == BUTTONS_VAL_DOWN)) && (stopSource != STOP_SRC_ERR))
        {
          if (INPUTS_IsActive(INPUT_SENS) != false)
          {
            /**< Door is open, don't continue! */
            SOUND_Play(ssError);
          } else
          {
            SOUND_Play(ssClick);
            DISPLAY_DoBlink(false);
            action = ACTION_CONTINUE;
            break;
          }
        }
        if ((INPUTS_IsActive(INPUT_SENS) == false) && (stopSource == STOP_SRC_DOOR))
        {
          /**< Door is closed, continue running */
          SOUND_Play(ssClick);
          DISPLAY_DoBlink(false);
          action = ACTION_CONTINUE;
          break;
        }
        break;
      case ACTION_ERROR:
        /**< Disable all signals */
        OUTPUTS_Switch(OUTPUT_LED2, false);
        //OUTPUTS_Switch(OUTPUT_FAN1, false);
        OUTPUTS_Switch(OUTPUT_MOTOR, false);
        PWM_SwitchOff();
        strcpy(str, ERR_STR);
        if ((errCode >= ERR_CODE_LED1) && (errCode <= ERR_CODE_LED3))
        {
          strcat(str, "L");
          str2[0] = errCode - ERR_CODE_LED1 + '1';
          str2[1] = 0;
          strcat(str, str2);
        } else
        if (errCode == ERR_CODE_SENS)
        {
          strcat(str, ERR_STR_SENS);
        }
        DISPLAY_Write(str);
        DISPLAY_DoBlink(true);
        stopSource = STOP_SRC_ERR;
        action = ACTION_WAIT;
        break;
    }
    vTaskDelay(1);
  }
}
