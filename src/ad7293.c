#include <math.h>
#include "ad7293.h"
#include "delay.h"
#include "global.h"
#include "spi.h"

/** \brief Convert real temperature to decimal code
 *
 * \param [in] temperature, °C
 * \return Decimal code for AD7293
 *
 */
uint16_t AD7293_ConvertTemperature(float temperature)
{
  uint16_t value;

  if (temperature < 0)
  {
    value = 0x800 - (uint16_t)(fabs(temperature) * 8) - 1;
  } else
  {
    value = (uint16_t)(temperature * 8) | 0x800;
  }

  return (value << 4);
}

/** \brief Convert voltage of bipolar output to decimal code
 *
 * \param [in] voltage_mv Gate voltage for bipolar output, mV
 * \return Decimal code for AD7293
 *
 */
uint16_t AD7293_ConvertBiVoltage(int16_t voltage_mv)
{
  uint16_t value;

  if (voltage_mv < -5000)
    voltage_mv = -5000;

  value = (uint16_t)((uint32_t)(voltage_mv + 5000) * 4096 / 5000);

  return (value << 4);
}

/** \brief Convert voltage of unipolar output to decimal code
 *
 * \param [in] voltage_mv Gate voltage for unipolar output, mV
 * \return Decimal code for AD7293
 *
 */
uint16_t AD7293_ConvertUniVoltage(int16_t voltage_mv)
{
  uint16_t value;

  if (voltage_mv < 0)
    voltage_mv = 0;

  value = (uint16_t)((uint32_t)voltage_mv * 4096 / 5000);

  return (value << 4);
}

/** \brief Convert drain current to decimal code
 *
 * \param [in] current Drain current value, A
 * \return Decimal code for AD7293
 *
 */
uint16_t AD7293_ConvertCurrent(float current)
{
  uint16_t value;

  value = (int16_t)(current * AD7293_CURRENT_GAIN * 2048 * AD7293_SHUNT / AD7293_REF_VOLTAGE) + 2048;

  return (value << 4);
}

uint16_t AD7293_ConvertSupplyVoltage(uint16_t voltage)
{
  uint16_t value;

  value = (uint16_t)((uint32_t)voltage * 4096 / 50 / AD7293_REF_VOLTAGE / 10);

  return (value << 4);
}

/** \brief Write byte to a common register
 *
 * \param [in] reg Number of register
 * \param [in] data Data byte
 * \return void
 *
 */
void AD7293_WriteCommonByte(uint8_t reg, uint8_t data)
{
	uint8_t tx [4] = {0xFF, 0x00, 0x00, 0x00};

	tx [0] = AD7293_OP_WRITE | reg;
	tx [1] = data;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 2);
	SPI_Select(SPI_SELECT_NONE);
}

/** \brief Write word to a common register
 *
 * \param [in] reg Number of register
 * \param [in] data Data word
 * \return void
 *
 */
void AD7293_WriteCommonWord(uint8_t reg, uint16_t data)
{
	uint8_t tx [4] = {0xFF, 0x00, 0x00, 0x00};

	tx [0] = AD7293_OP_WRITE | reg;
	tx [1] = (uint8_t)(data >> 8);
	tx [2] = (uint8_t)data;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 3);
	SPI_Select(SPI_SELECT_NONE);
}

/** \brief Write byte to any register
 *
 * \param [in] reg Number of page
 * \param [in] reg Number of register
 * \param [in] data Data byte
 * \return void
 *
 */
void AD7293_WriteByte(uint8_t page, uint8_t reg, uint8_t data)
{
	uint8_t tx[4] = {0xFF, 0x00, 0x00, 0x00};

	tx[0] = AD7293_OP_WRITE | REGISTER_COMMON_PAGE_SELECT_POINTER;
	tx[1] = page;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 2);
	SPI_Select(SPI_SELECT_NONE);

	tx[0] = AD7293_OP_WRITE | reg;
	tx[1] = data;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 2);
	SPI_Select(SPI_SELECT_NONE);
}

/** \brief Write word to any register
 *
 * \param [in] reg Number of page
 * \param [in] reg Number of register
 * \param [in] data Data word
 * \return void
 *
 */
void AD7293_WriteWord(uint8_t page, uint8_t reg, uint16_t data)
{
	uint8_t tx[4] = {0xFF, 0x00, 0x00, 0x00};

	tx[0] = AD7293_OP_WRITE | REGISTER_COMMON_PAGE_SELECT_POINTER;
	tx[1] = page;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 2);
	SPI_Select(SPI_SELECT_NONE);

	tx[0] = AD7293_OP_WRITE | reg;
	tx[1] = (uint8_t)(data >> 8);
	tx[2] = (uint8_t)data;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 3);
	SPI_Select(SPI_SELECT_NONE);
}

/** \brief Read byte from any register
 *
 * \param [in] reg Number of page
 * \param [in] reg Number of register
 * \return Data byte
 *
 */
uint8_t AD7293_ReadByte(uint8_t page, uint8_t reg)
{
	uint8_t rx [3] = {0xFF, 0xFF, 0xFF};
	uint8_t tx [4] = {0xFF, 0x00, 0x00, 0x00};

	tx [0] =  AD7293_OP_WRITE | REGISTER_COMMON_PAGE_SELECT_POINTER;
	tx [1] = page;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 2);
	SPI_Select(SPI_SELECT_NONE);

	tx [0] = AD7293_OP_READ | reg;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 1);
	SPI_Receive(rx, 1);
	SPI_Select(SPI_SELECT_NONE);

	return rx[0];
}

/** \brief Read word from any register
 *
 * \param [in] reg Number of page
 * \param [in] reg Number of register
 * \return Data word
 *
 */
uint16_t AD7293_ReadWord(uint8_t page, uint8_t reg)
{
	uint8_t rx [3] = {0xFF, 0xFF, 0xFF};
	uint8_t tx [4] = {0xFF, 0x00, 0x00, 0x00};

	tx [0] = AD7293_OP_WRITE | REGISTER_COMMON_PAGE_SELECT_POINTER;
	tx [1] = page;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 2);
  SPI_Select(SPI_SELECT_NONE);

	tx [0] = AD7293_OP_READ | reg;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 1);
	SPI_Receive(rx, 2);
	SPI_Select(SPI_SELECT_NONE);

	return (((uint16_t)rx[0] << 8) | rx[1]);
}

/** \brief Read ID from AD7293 identification register
 *
 * \return ID value
 *
 */
uint16_t AD7293_ReadID(void)
{
	uint8_t rx[3] = {0xFF, 0xFF, 0xFF};
	uint8_t tx[4] = {0xFF, 0x00, 0x00, 0x00};

	tx[0] = AD7293_OP_READ | REGISTER_COMMON_DEVICE_ID;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 1);
	SPI_Receive(rx, 2);
  SPI_Select(SPI_SELECT_NONE);

	return (((uint16_t)rx[0] << 8) | rx[1]);
}

/** \brief Set gate voltage for selected gate
 *
 * \param [in] channel Number of gate
 * \param [in] voltage_mv Voltage value in mV
 * \return void
 *
 */
void AD7293_SetGateVoltage(uint8_t channel, int16_t voltage_mv)
{
  if (channel < 4)
  {
    AD7293_WriteWord(
      REGISTER_PAGE_RESULT_0,
      REGISTER_RESULT_0_BI_VOUT0 + channel,
      AD7293_ConvertBiVoltage(voltage_mv)
    );
  } else
  {
    AD7293_WriteWord(
      REGISTER_PAGE_RESULT_0,
      REGISTER_RESULT_0_UNI_VOUT0 + channel - 4,
      AD7293_ConvertUniVoltage(voltage_mv)
    );
  }
}

/** \brief Get power supply voltage for selected channel
 *
 * \param [in] channel Number of channel
 * \return Supply voltage as float, V
 *
 */
float AD7293_GetSupplyVoltage(uint8_t channel)
{
  if (channel >= DRAINS_NUM)
    return 0;

  uint16_t v = AD7293_ReadWord(REGISTER_PAGE_RESULT_1, REGISTER_RESULT_1_RS_0 + channel) >> 4;
  return (float)v * AD7293_REF_VOLTAGE * 50 / 4096;
}

/** \brief Get drain current for selected channel
 *
 * \param [in] channel Number of channel
 * \return Drain current as float, A
 *
 */
float AD7293_GetDrainCurrent(uint8_t channel)
{
  if (channel >= DRAINS_NUM)
    return 0;

  uint16_t v = AD7293_ReadWord(REGISTER_PAGE_RESULT_0, REGISTER_RESULT_0_ISENSE0 + channel) >> 4;
  return ((float)v - 2048) * AD7293_REF_VOLTAGE / AD7293_CURRENT_GAIN / 2048 / AD7293_SHUNT;
}

/** \brief Get temperature for selected channel
 *
 * \param [in] channel Number of channel
 * \return Temperature, °C
 *
 */
float AD7293_GetTemperature(uint8_t channel)
{
  if (channel >= AD7293_TEMPERATURE_LAST)
    return 0.0f;

  float temperature = 1.0f;

  uint16_t v = AD7293_ReadWord(REGISTER_PAGE_RESULT_0, REGISTER_RESULT_0_TSENSEINT + channel) >> 4;
  if (v < 0x800)
  {
    temperature = -1.0f;
    v = 0x0800 - v;
  }
  v &= 0x7FF;
  temperature = temperature * v *0.125f;

  return temperature;
}

/** \brief Get common alerts word
 *
 * \return Alerts value
 *
 */
uint16_t AD7293_GetAlerts(void)
{
  return AD7293_ReadWord(REGISTER_ALERT, REGISTER_ALERT_SUM);
}

/** \brief Get TSENSEx alerts word
 *
 * \return Alerts value
 *
 */
uint16_t AD7293_GetTemperatureAlerts(void)
{
  return AD7293_ReadWord(REGISTER_ALERT, REGISTER_ALERT_TSENSX);
}

/** \brief Get ISENSEx alerts word
 *
 * \return Alerts value
 *
 */
uint16_t AD7293_GetCurrentAlerts(void)
{
  return AD7293_ReadWord(REGISTER_ALERT, REGISTER_ALERT_ISENSX);
}

/** \brief Reset alerts
 *
 * \return void
 *
 */
void AD7293_ResetAlerts(void)
{
  AD7293_WriteWord(REGISTER_ALERT, REGISTER_ALERT_SUM, 0xFFFF);
}

/** \brief Set GPIO value (low/high)
 *
 * \param [in] value New value
 * \return void
 *
 */
void AD7293_SetGpio(uint8_t value)
{
  AD7293_WriteCommonByte(REGISTER_COMMON_GPIO, value);
}

/** \brief Switch power on (PA_ON = 1)
 *
 * \return void
 *
 */
void AD7293_SetPowerOn(void)
{
  AD7293_WriteWord(
		REGISTER_PAGE_CONFIGURATION,
		REGISTER_CONFIGURATION_PA_ON_CONTROL,
		REGISTER_CONFIGURATION_PA_ON_ENABLE
  );
}

/** \brief Switch power off (PA_ON = 0)
 *
 * \return void
 *
 */
void AD7293_SetPowerOff(void)
{
  AD7293_WriteWord(
		REGISTER_PAGE_CONFIGURATION,
		REGISTER_CONFIGURATION_PA_ON_CONTROL,
		0
  );
}

/** \brief Perform software reset
 *
 * \return void
 *
 */
void AD7293_Reset(void)
{
	AD7293_WriteCommonWord(REGISTER_COMMON_SOFTWARE_RESET, 0x7293);
	vTaskDelay(10);
	AD7293_WriteCommonWord(REGISTER_COMMON_SOFTWARE_RESET, 0x0000);
}

/** \brief Configure AD7293
 *
 * \param void
 * \return True if AD7293 was detected
 *
 */
bool AD7293_Configuration(void)
{
  uint8_t i;

  AD7293_ReadID();
  if (AD7293_ReadID() != AD7293_ID)
    return false;

  /**< AD7293 setup */
  AD7293_Reset();
  /**< Disable all DACs */
  AD7293_WriteCommonByte(REGISTER_COMMON_DAC_ENABLE, 0x00);
  /**< Set pins as GPIOs */
	AD7293_WriteWord(
		REGISTER_PAGE_CONFIGURATION,
    REGISTER_CONFIGURATION_DIGITAL_IO_FUNCTION,
    REGISTER_CONFIGURATION_DIGITAL_OUTPUT_GPIO7 | DIGITAL_OUTPUT_DEFAULT
  );
  /**< Enable GPIOs */
	AD7293_WriteWord(
		REGISTER_PAGE_CONFIGURATION,
		REGISTER_CONFIGURATION_DIGITAL_OUTPUT_ENABLE,
		REGISTER_CONFIGURATION_DIGITAL_OUTPUT_GPIO7
  );
  /**< Setup Bipolar DAC Offset, 0b10 << 4: -5 V to 0 V */
	AD7293_WriteByte(
		REGISTER_PAGE_OFFSET0,
		REGISTER_OFFSET_0_BI_VOUT0_OFFSET,
		0x20
	);
	AD7293_WriteByte(
		REGISTER_PAGE_OFFSET0,
		REGISTER_OFFSET_0_BI_VOUT1_OFFSET,
		0x20
	);
	AD7293_WriteByte(
		REGISTER_PAGE_OFFSET0,
		REGISTER_OFFSET_0_BI_VOUT2_OFFSET,
		0x20
	);
	AD7293_WriteByte(
		REGISTER_PAGE_OFFSET0,
		REGISTER_OFFSET_0_BI_VOUT3_OFFSET,
		0x20
	);
	/**< Setup internal reference */
	AD7293_WriteWord(
		REGISTER_PAGE_CONFIGURATION,
		REGISTER_CONFIGURATION_GENERAL,
		REGISTER_CONFIGURATION_GENERAL_ADC_REF_INT
		);
  /**< Setup ISENSEx Gain Register */
  AD7293_WriteWord(
    REGISTER_PAGE_CONFIGURATION,
    REGISTER_CONFIGURATION_ISENS_X_GAIN,
    REGISTER_CONFIGURATION_ISENS_0_SET(REGISTER_CONFIGURATION_ISENS_X_GAIN_6_25) |
    REGISTER_CONFIGURATION_ISENS_1_SET(REGISTER_CONFIGURATION_ISENS_X_GAIN_6_25) |
    REGISTER_CONFIGURATION_ISENS_2_SET(REGISTER_CONFIGURATION_ISENS_X_GAIN_6_25) |
    REGISTER_CONFIGURATION_ISENS_3_SET(REGISTER_CONFIGURATION_ISENS_X_GAIN_6_25)
  );
  /**< Enable temperature sensors */
	AD7293_WriteWord(
		REGISTER_PAGE_CONFIGURATION,
		REGISTER_CONFIGURATION_TEMPERATURE_SENSOR_ENABLE,
		REGISTER_CONFIGURATION_TEMPERATURE_SENSOR_ENABLE_D1 |
		REGISTER_CONFIGURATION_TEMPERATURE_SENSOR_ENABLE_D2 |
		REGISTER_CONFIGURATION_TEMPERATURE_SENSOR_ENABLE_INT
  );
  /**< Enable current sensors */
	AD7293_WriteWord(
		REGISTER_PAGE_CONFIGURATION,
		REGISTER_CONFIGURATION_ISENSX_ENABLE,
		REGISTER_CONFIGURATION_ISENSX_ENABLE_0 |
		REGISTER_CONFIGURATION_ISENSX_ENABLE_1 |
		REGISTER_CONFIGURATION_ISENSX_ENABLE_2 |
		REGISTER_CONFIGURATION_ISENSX_ENABLE_3
  );
	/**< Open-loop for all channels */
	AD7293_WriteWord(
		REGISTER_PAGE_CONFIGURATION,
		REGISTER_CONFIGURATION_CLOSE_LOOP_CONTROL,
		0x0000
	);
	/**< Enable voltage monitoring in the background */
	AD7293_WriteWord(
		REGISTER_PAGE_CONFIGURATION,
		REGISTER_CONFIGURATION_MON_ENABLE,
		0xFFFF
  );
  /**< Set limits for all important parameters */
  /**< Set high limit for temperature */
  for (i = 0; i < AD7293_TEMPERATURE_LAST; i++)
    AD7293_WriteWord(
      REGISTER_HIGH_LIMIT_0,
      REGISTER_HIGH_LIMIT_0_TSENSEINT + i,
      AD7293_ConvertTemperature((float)EE_TemperatureMax)
    );
  /**< Set high limit for current */
  for (i = 0; i < DRAINS_NUM; i++)
  AD7293_WriteWord(
    REGISTER_HIGH_LIMIT_0,
    REGISTER_HIGH_LIMIT_0_ISENSE0 + i,
    AD7293_ConvertCurrent((float)EE_DrainCurrent[i] / 1000)
  );
  /**< Set high limit for supply voltage */
  AD7293_WriteWord(
    REGISTER_HIGH_LIMIT_1,
    REGISTER_HIGH_LIMIT_1_RS_0,
    AD7293_ConvertSupplyVoltage(EE_DrainVoltageMax)
  );
  /**< Set low limit for temperature */
  for (i = 0; i < AD7293_TEMPERATURE_LAST; i++)
    AD7293_WriteWord(
      REGISTER_LOW_LIMIT_0,
      REGISTER_LOW_LIMIT_0_TSENSEINT,
      AD7293_ConvertTemperature((float)EE_TemperatureMin)
    );
  /**< Set low limit for supply voltage */
  AD7293_WriteWord(
    REGISTER_LOW_LIMIT_1,
    REGISTER_LOW_LIMIT_1_RS_0,
    AD7293_ConvertSupplyVoltage(EE_DrainVoltageMin)
  );
  /**< Enable all DACs */
	AD7293_WriteCommonByte(
		REGISTER_COMMON_DAC_ENABLE,
		0xFF
	);

  /**< Setup zero voltages for bipolar outputs */
  for (i = 0; i < 4; i++)
    AD7293_SetGateVoltage(i, GATE14_MIN_VALUE);

  return true;
}
