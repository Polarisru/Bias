#include "ad7293.h"
#include "spi.h"

void AD7293_WriteByte(uint8_t page, uint8_t reg, uint8_t data)
{
	uint8_t tx[4] = {0xFF, 0x00, 0x00, 0x00};

	tx[0] = AD7293_OP_WRITE | REGISTER_COMMON_PAGE_SELECT_POINTER;
	tx[1] = page;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 3);
	SPI_Select(SPI_SELECT_NONE);

	tx[0] = AD7293_OP_WRITE | reg;
	tx[1] = data;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 2);
	SPI_Select(SPI_SELECT_NONE);
}

void AD7293_WriteWord(uint8_t page, uint8_t reg, uint16_t data)
{
	uint8_t tx[4] = {0xFF, 0x00, 0x00, 0x00};

	tx[0] = AD7293_OP_WRITE | REGISTER_COMMON_PAGE_SELECT_POINTER;
	tx[1] = page;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 3);
	SPI_Select(SPI_SELECT_NONE);

	tx[0] = AD7293_OP_WRITE | reg;
	tx[1] = (uint8_t)(data >> 8);
	tx[2] = (uint8_t)data;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 3);
	SPI_Select(SPI_SELECT_NONE);
}

uint16_t AD7293_ReadID(void)
{
	uint8_t rx[3] = {0xFF, 0xFF, 0xFF};
	uint8_t tx[4] = {0xFF, 0x00, 0x00, 0x00};

	tx[0] = AD7293_OP_READ | REGISTER_COMMON_DEVICE_ID;
	SPI_Select(SPI_SELECT_AD7293);
	SPI_Transmit(tx, 1);
	SPI_Transmit(rx, 2);
  SPI_Select(SPI_SELECT_NONE);

	return (((uint16_t)rx[0] << 8) | rx[1]);
}

bool AD7293_Configuration(void)
{
  return (AD7293_ReadID() == AD7293_ID);
}
