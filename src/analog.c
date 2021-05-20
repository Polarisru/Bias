#include "analog.h"

#define ANALOG_9V_PIN       GPIO_Pin_0

#define ANALOG_9V_PORT      GPIOA

#define ANALOG_9V_CHANNEL   ADC_Channel_0

/** \brief Analog module configuration
 *
 * \return void
 *
 */
void ANALOG_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStruct;
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = ANALOG_9V_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(ANALOG_9V_PORT, &GPIO_InitStructure);

  ADC_DeInit(ADC1);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  ADC_StructInit(&ADC_InitStruct);

  ADC_InitStruct.ADC_Resolution = ADC_Resolution_8b;
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStruct);

  ADC_ChannelConfig(ADC1, ANALOG_9V_CHANNEL, ADC_SampleTime_239_5Cycles);

  ADC_GetCalibrationFactor(ADC1);

  ADC_Cmd(ADC1, ENABLE);

  ADC_StartOfConversion(ADC1);
}

/** \brief Get converted value from ADC
 *
 * \return Last converted value
 *
 */
uint8_t ANALOG_GetValue(void)
{
  return (uint8_t)ADC_GetConversionValue(ADC1);
}
