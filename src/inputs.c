#include "inputs.h"

/**< Structure with hardware connections for inputs */
const TInput INPUTS_Pins[INPUT_LAST] = {
  {INPUT_KEY1_GPIO, INPUT_KEY1_PIN},
  {INPUT_KEY2_GPIO, INPUT_KEY2_PIN},
  {INPUT_KEY3_GPIO, INPUT_KEY3_PIN},
  {INPUT_KEY4_GPIO, INPUT_KEY4_PIN},
  {INPUT_KEY5_GPIO, INPUT_KEY5_PIN},
  {INPUT_SENS_GPIO, INPUT_SENS_PIN}
  //{INPUT_ST1_GPIO, INPUT_ST1_PIN},
  //{INPUT_ST2_GPIO, INPUT_ST2_PIN},
  //{INPUT_ST3_GPIO, INPUT_ST3_PIN},
  //{INPUT_ST4_GPIO, INPUT_ST4_PIN},
  //{INPUT_ST5_GPIO, INPUT_ST5_PIN},
  //{INPUT_ST6_GPIO, INPUT_ST6_PIN}
};

/** \brief Check if input is active (high)
 *
 * \param [in] pin Number of the pin to check
 * \return True if high, false otherwise
 *
 */
bool INPUTS_IsActive(uint8_t pin)
{
  if (pin >= INPUT_LAST)
    return false;

  return (GPIO_ReadInputDataBit(INPUTS_Pins[pin].GPIO, INPUTS_Pins[pin].GPIO_Pin) == Bit_SET);
}

/** \brief Configure inputs
 *
 * \return Nothing
 *
 */
void INPUTS_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  uint8_t i;

  GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;

  for (i = 0; i < INPUT_LAST; i++)
  {
    //if ((i >= INPUT_ST1) && (i <= INPUT_ST3))
    //  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    //else
      GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Pin = INPUTS_Pins[i].GPIO_Pin;
    GPIO_Init(INPUTS_Pins[i].GPIO, &GPIO_InitStruct);
  }
}
