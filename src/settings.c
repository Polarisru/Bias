#include <string.h>
#include "eeprom.h"
#include "global.h"
#include "settings.h"

/**< This array is used to match global variables from RAM with their EEPROM representation */
const settVal_t SETTINGS_Values[] =
{
  {0x00, sizeof(uint8_t), (void*)&EE_ValuePWM, {.byte = 100}},
  {0x01, sizeof(uint8_t), (void*)&EE_Time, {.byte = 1}},
  {0x02, sizeof(uint8_t), (void*)&EE_FanTimeout, {.byte = 30}},
  {0x03, sizeof(uint8_t), (void*)&EE_ErrOn, {.byte = ERR_MODE_ON}}
};

/** \brief Compare three bytes, should be identical (2oo3)
 *
 * \param [in] a First byte to compare
 * \param [in] b Second byte to compare
 * \param [in] c Third byte to compare
 * \return Decision as uint8_t
 *
 */
uint8_t SETTINGS_Compare(uint8_t *a, uint8_t *b, uint8_t *c, uint8_t len)
{
  bool ab, ac, bc;

  ab = (memcmp(a, b, len) == 0);
  ac = (memcmp(a, c, len) == 0);
  bc = (memcmp(b, c, len) == 0);

  if (ab && ac)
    return SETTINGS_WRONG_NONE;
  if (bc && !ab)
    return SETTINGS_WRONG_A;
  if (ac && !bc)
    return SETTINGS_WRONG_B;
  if (ab && !bc)
    return SETTINGS_WRONG_C;
  return SETTINGS_WRONG_ALL;
}

/** \brief Write variable to settings memory (2oo3)
 *
 * \param [in] addr Address in FRAM
 * \param [in] data Data value to write
 * \param [in] len Length of data to write
 * \return Nothing
 *
 */
void SETTINGS_WriteValue(uint8_t addr, uint8_t *data, uint8_t len)
{
  uint8_t i, j;
  /**< Buffer for maximal data size */
  uint8_t buff[SETTINGS_MAX_SIZE];

  for (i = 0; i < 3; i++)
  {
    switch (i)
    {
      case 0:
        memcpy(buff, data, len);
        break;
      case 1:
        for (j = 0; j < len; j++)
          buff[j] = data[j] ^ 0xAA;
        break;
      case 2:
        for (j = 0; j < len; j++)
          buff[j] = data[j] ^ 0x55;
        break;
    }
    EEPROM_WriteBuffer((uint16_t)addr + i * SETTINGS_COPY_STEP + SETTINGS_START, buff, len);
  }
}

/** \brief Write variable to settings memory as pointer
 *
 * \param [in] data Pointer to data to write
 * \return Nothing
 *
 */
void SETTINGS_WriteVar(void *data)
{
  uint8_t i;

  for (i = 0; i < sizeof(SETTINGS_Values) / sizeof(settVal_t); i++)
  {
    if (data == SETTINGS_Values[i].pVal)
    {
      SETTINGS_WriteValue(SETTINGS_Values[i].address, (uint8_t*)data, SETTINGS_Values[i].size);
      break;
    }
  }
}

/** \brief Reset settings to default values
 *
 * \return Nothing
 *
 */
void SETTINGS_SetDefaults(void)
{
  uint8_t i;

  for (i = 0; i < sizeof(SETTINGS_Values) / sizeof(settVal_t); i++)
  {
    SETTINGS_WriteValue(SETTINGS_Values[i].address, (uint8_t*)&SETTINGS_Values[i].defVal, SETTINGS_Values[i].size);
  }
}

/** \brief Initialize EEPROM memory, perform self-test
 *
 * \return Nothing
 *
 */
void SETTINGS_Configuration(void)
{
  uint8_t i, j;
  uint16_t addr;
  uint8_t len;
  uint8_t buff_a[SETTINGS_MAX_SIZE];
  uint8_t buff_b[SETTINGS_MAX_SIZE];
  uint8_t buff_c[SETTINGS_MAX_SIZE];

  EEPROM_Configuration();

  for (i = 0; i < sizeof(SETTINGS_Values) / sizeof(settVal_t); i++)
  {
    addr = SETTINGS_Values[i].address + SETTINGS_START;
    len = SETTINGS_Values[i].size;
    EEPROM_ReadBuffer(addr, buff_a, len);
    EEPROM_ReadBuffer(addr + SETTINGS_COPY_STEP, buff_b, len);
    EEPROM_ReadBuffer(addr + 2 * SETTINGS_COPY_STEP, buff_c, len);
    for (j = 0; j < len; j++)
    {
      buff_b[j] ^= 0xAA;
      buff_c[j] ^= 0x55;
    }
    switch (SETTINGS_Compare(buff_a, buff_b, buff_c, len))
    {
      case SETTINGS_WRONG_NONE:
        /**< everything is Ok */
        break;
      case SETTINGS_WRONG_A:
        /**< First value (a) is wrong, correct it */
        for (j = 0; j < len; j++)
          buff_a[j] = buff_b[j];
        EEPROM_WriteBuffer(addr, buff_a, len);
        break;
      case SETTINGS_WRONG_B:
        /**< Second value (b) is wrong, correct it */
        for (j = 0; j < len; j++)
          buff_b[j] = buff_a[j] ^ 0xAA;
        EEPROM_WriteBuffer(addr + SETTINGS_COPY_STEP, buff_b, len);
        break;
      case SETTINGS_WRONG_C:
        /**< Third value (c) is wrong, correct it */
        for (j = 0; j < len; j++)
          buff_c[j] = buff_a[j] ^ 0x55;
        EEPROM_WriteBuffer(addr + 2 * SETTINGS_COPY_STEP, buff_c, len);
        break;
      case SETTINGS_WRONG_ALL:
        /**< Everything is wrong, save default value, don't update */
        memcpy(buff_a, &SETTINGS_Values[i].defVal, len);
        for (j = 0; j < len; j++)
        {
          buff_b[j] = buff_a[j] ^ 0xAA;
          buff_c[j] = buff_a[j] ^ 0x55;
        }
        EEPROM_WriteBuffer(addr, buff_a, len);
        EEPROM_WriteBuffer(addr + SETTINGS_COPY_STEP, buff_b, len);
        EEPROM_WriteBuffer(addr + 2 * SETTINGS_COPY_STEP, buff_c, len);
        break;
    }
    memcpy(SETTINGS_Values[i].pVal, buff_a, len);
  }
}

