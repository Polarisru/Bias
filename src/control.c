#include "ad7293.h"
#include "comm.h"
#include "control.h"
#include "global.h"
#include "spi.h"
#include "uart.h"

static uint8_t CONTROL_ErrCode;

//#define ADC1_DR_Address             0x40012440

/** \brief Configure control module
 *
 * \return void
 *
 */
void CONTROL_Configuration(void)
{
//  ADC_InitTypeDef ADC_InitStruct;
//  DMA_InitTypeDef DMA_InitStructure;
//  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//
//  GPIO_InitStructure.GPIO_Pin = ST1_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_Init(ST1_GPIO, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = ST2_PIN;
//  GPIO_Init(ST2_GPIO, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = ST3_PIN;
//  GPIO_Init(ST3_GPIO, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = UIN_PIN;
//  GPIO_Init(UIN_GPIO, &GPIO_InitStructure);
//
//  ADC_DeInit(ADC1);
//  DMA_DeInit(DMA1_Channel1);
//  TIM_DeInit(TIM15);
//
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;    //ADC Adresse
//  //DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)CONTROL_Values;      //ADC Buffer
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;              //ADC -> DMA -> RAM
//  DMA_InitStructure.DMA_BufferSize = 4;                    //Buffer Size
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //ADC Datenword = 16bit
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;      //RAM Datenword = 16bit, beide MÜSSEN gleich sein
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                //DAM als Ring Buffer (DMA_Mode_Normal)
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;              //Hohe Prio
//  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                //Memory -> DMA -> Memory = Disabelt
//  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//
//  /* DMA1 Channel1 enable */
//  DMA_Cmd(DMA1_Channel1, ENABLE);
//
//  ADC_StructInit(&ADC_InitStruct);
//
//  ADC_InitStruct.ADC_Resolution = ADC_Resolution_8b;
//  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
//  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
//  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
//  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
//  ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward;
//  ADC_Init(ADC1, &ADC_InitStruct);
//
//  ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_239_5Cycles);
//  ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_239_5Cycles);
//  ADC_ChannelConfig(ADC1, ADC_Channel_8, ADC_SampleTime_239_5Cycles);
//  ADC_ChannelConfig(ADC1, ADC_Channel_9, ADC_SampleTime_239_5Cycles);
//
//  ADC_GetCalibrationFactor(ADC1);
//
//  /* ADC DMA request in circular mode */
//  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
//  /* Enable ADC_DMA */
//  ADC_DMACmd(ADC1, ENABLE);
//
//  ADC_Cmd(ADC1, ENABLE);
//
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//
//  /* Time base configuration */
//  TIM_TimeBaseStructure.TIM_Period = (SystemCoreClock / 1000 ) - 1; //1KHz
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//
//  /* TIM15 enable counter */
//  TIM_Cmd(TIM3, ENABLE);
//  /* TIM15 enable Trigger Output */
//  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
//
//  ADC_StartOfConversion(ADC1);
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
  uint16_t status;
  float temp;
  uint16_t counter = 0;
  uint8_t i;

  CONTROL_ErrCode = ERR_NONE;

  /**< Configure peripherals */
  CONTROL_Configuration();
  SPI_Configuration();
  AD7293_Configuration();

  CONTROL_ResetErrCode();

  vTaskDelay(100);

  while (1)
  {
    if (CONTROL_ErrCode != ERR_NONE)
      continue;
    alerts = AD7293_GetAlerts();
    if (alerts & AD7293_ALERTS_MASK)
    {
      /**< Something is going wrong, stop working */
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
        status = AD7293_GetCurrentAlerts();
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
        COMM_Send("Unknown");
        CONTROL_ErrCode = ERR_UNKNOWN;
      }
      COMM_Send("\n");
    }
    temp = AD7293_GetDrainCurrent(0);
    temp = AD7293_GetSupplyVoltage(0);
    temp = AD7293_GetTemperature(0);
    vTaskDelay(100);
    counter++;
    if (counter == 100)
      AD7293_SetGateVoltage(0, -2900);
  }
}
