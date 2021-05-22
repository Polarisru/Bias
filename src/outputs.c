#include "outputs.h"

/**< Structure with hardware connections for outputs */
const TOutput OUTPUTS_Pins[OUTPUT_LAST] = {
  {OUTPUT_LED_GPIO, OUTPUT_LED_PIN},
  {OUTPUT_TEST_GPIO, OUTPUT_TEST_PIN}
};

/** \brief Switch output on/off
 *
 * \param [in] pin Number of output to switch
 * \param [in] on True = switch on, false = switch off
 * \return Nothing
 *
 */
void OUTPUTS_Switch(uint8_t pin, bool on)
{
  if (pin >= OUTPUT_LAST)
    return;

  if (on == true)
  {
    GPIO_SetBits(OUTPUTS_Pins[pin].GPIO, OUTPUTS_Pins[pin].GPIO_Pin);
  } else
  {
    GPIO_ResetBits(OUTPUTS_Pins[pin].GPIO, OUTPUTS_Pins[pin].GPIO_Pin);
  }
}

/** \brief Toggle output
 *
 * \param [in] pin Number of output to switch
 * \return Nothing
 *
 */
void OUTPUTS_Toggle(uint8_t pin)
{
  if (pin >= OUTPUT_LAST)
    return;

  if (GPIO_ReadOutputDataBit(OUTPUTS_Pins[pin].GPIO, OUTPUTS_Pins[pin].GPIO_Pin) == Bit_RESET)
    GPIO_SetBits(OUTPUTS_Pins[pin].GPIO, OUTPUTS_Pins[pin].GPIO_Pin);
  else
    GPIO_ResetBits(OUTPUTS_Pins[pin].GPIO, OUTPUTS_Pins[pin].GPIO_Pin);
}

/** \brief Configure outputs
 *
 * \return Nothing
 *
 */
void OUTPUTS_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  uint8_t i;

  GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  for (i = 0; i < OUTPUT_LAST; i++)
  {
    GPIO_InitStruct.GPIO_Pin = OUTPUTS_Pins[i].GPIO_Pin;
    GPIO_Init(OUTPUTS_Pins[i].GPIO, &GPIO_InitStruct);
  }
}
