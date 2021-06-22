#include "global.h"
#include "inputs.h"

#ifdef DEF_NEW
#define INPUT_ALERT_EXTI_GPIO     EXTI_PortSourceGPIOA
#define INPUT_ALERT_EXTI_SOURCE   EXTI_PinSource1
#define INPUT_ALERT_EXTI_LINE     EXTI_Line1
#define INPUT_ALERT_IRQN          EXTI0_1_IRQn
#define INPUT_ALERT_IRQ_HANDLER   EXTI0_1_IRQHandler
#endif

#define INPUT_RESET_EXTI_GPIO     EXTI_PortSourceGPIOA
#define INPUT_RESET_EXTI_SOURCE   EXTI_PinSource2
#define INPUT_RESET_EXTI_LINE     EXTI_Line2
#define INPUT_RESET_IRQN          EXTI2_3_IRQn
#define INPUT_RESET_IRQ_HANDLER   EXTI2_3_IRQHandler

void INPUT_RESET_IRQ_HANDLER(void)
{
	if (EXTI_GetITStatus(INPUT_RESET_EXTI_LINE) != RESET)
  {
		//GLOBAL_Reset = true;
		GLOBAL_Reset |= (1 << INT_RESET);
		/**< Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(INPUT_RESET_EXTI_LINE);
	}
}

#ifdef DEF_NEW
void INPUT_ALERT_IRQ_HANDLER(void)
{
	if (EXTI_GetITStatus(INPUT_ALERT_EXTI_LINE) != RESET)
  {
		GLOBAL_ErrStatus = AD7293_GetAlerts();
		/**< Stop AD7293 immediately! */
		OUTPUTS_Switch(OUTPUT_RESET, false);
		GLOBAL_Reset |= (1 << INT_ALERT);
		/**< Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(INPUT_ALERT_EXTI_LINE);
	}
}
#endif

/**< Structure with hardware connections for inputs */
const TInput INPUTS_Pins[INPUT_LAST] = {
  #ifdef DEF_NEW
  {INPUT_ALERT_GPIO, INPUT_ALERT_PIN, GPIO_PuPd_NOPULL},
  #endif
  {INPUT_RESET_GPIO, INPUT_RESET_PIN, GPIO_PuPd_NOPULL}
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
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  uint8_t i;

  GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;

  for (i = 0; i < INPUT_LAST; i++)
  {
    GPIO_InitStruct.GPIO_PuPd = INPUTS_Pins[i].PuPd;
    GPIO_InitStruct.GPIO_Pin = INPUTS_Pins[i].GPIO_Pin;
    GPIO_Init(INPUTS_Pins[i].GPIO, &GPIO_InitStruct);
  }

	/**< Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/**< Connect EXTIx Line to DW Int pin */
	SYSCFG_EXTILineConfig(INPUT_RESET_EXTI_GPIO, INPUT_RESET_EXTI_SOURCE);

	/**< Configure EXTIx line for Ext.Reset interrupt */
	EXTI_InitStructure.EXTI_Line = INPUT_RESET_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/**< Enable external interrupt for RESET pin */
	NVIC_InitStructure.NVIC_IRQChannel = INPUT_RESET_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

  #ifdef DEF_NEW
	/**< Connect EXTIx Line to DW Int pin */
	SYSCFG_EXTILineConfig(INPUT_ALERT_EXTI_GPIO, INPUT_ALERT_EXTI_SOURCE);

  /**< Configure EXTIx line for ALERT interrupt */
	EXTI_InitStructure.EXTI_Line = INPUT_ALERT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = INPUT_ALERT_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	#endif
}
