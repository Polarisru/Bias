#include "inputs.h"

/**< Structure with hardware connections for inputs */
const TInput INPUTS_Pins[INPUT_LAST] = {
  {INPUT_RESET_GPIO, INPUT_RESET_PIN, GPIO_PuPd_DOWN}
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
    GPIO_InitStruct.GPIO_PuPd = INPUTS_Pins[i].PuPd;
    GPIO_InitStruct.GPIO_Pin = INPUTS_Pins[i].GPIO_Pin;
    GPIO_Init(INPUTS_Pins[i].GPIO, &GPIO_InitStruct);
  }
}
