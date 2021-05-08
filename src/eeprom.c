#include "crc16.h"
#include "global.h"
#include "eeprom.h"

uint8_t EEPROM_Page[EEPROM_PAGE_SIZE];

/**< This array is used to match global variables from RAM with their EEPROM representation */
const eeVal_t EEPROM_Values[] =
{
  {0x00, sizeof(uint8_t), (void*)&EE_ValuePWM,    {.byte = 100}},
  {0x01, sizeof(uint8_t), (void*)&EE_Time,        {.byte = 1}},
  {0x02, sizeof(uint8_t), (void*)&EE_FanTimeout,  {.byte = 30}},
  {0x03, sizeof(uint8_t), (void*)&EE_ErrOn,       {.byte = ERR_MODE_ON}}
};

/** \brief Recalculate EEPROM CRC
 *
 * \return New CRC16 value as uint16_t
 *
 */
uint16_t EEPROM_RecalculateCRC(void)
{
  uint16_t crc;

  crc = CRC16_CalcKearfott(EEPROM_Page, EEPROM_PAGE_SIZE - sizeof(uint16_t));
  /**< Write new CRC value to the start of EEPROM page */
  memcpy(&EEPROM_Page[EEPROM_PAGE_SIZE - sizeof(uint16_t)], (uint8_t*)&crc, sizeof(uint16_t));

  return crc;
}

/** \brief Calculate EEPROM CRC
 *
 * \return CRC16 value as uint16_t
 *
 */
uint16_t EEPROM_CalculateCRC(void)
{
  uint16_t crc;

  crc = CRC16_CalcKearfott(EEPROM_Page, EEPROM_PAGE_SIZE - sizeof(uint16_t));

  return crc;
}

/** \brief Save global variables to EEPROM copy
 *
 * \param [in] ee_copy Number of the copy to save to
 * \return void
 *
 */
void EEPROM_SaveCopy(uint8_t ee_copy)
{
  uint32_t addr;
  uint16_t crc;
  uint16_t i;

  switch (ee_copy)
  {
    case EE_COPY_1:
      addr = EEPROM_ADDR_COPY_1;
      break;
    case EE_COPY_2:
      addr = EEPROM_ADDR_COPY_2;
      break;
    default:
      return;
  }
  crc = EEPROM_RecalculateCRC();
  if (crc != *(uint16_t*)addr)
  {
    /**< Save only if CRC is different */
    /**< Erase EEPROM page */
    FLASH_ErasePage(addr);
    /**< Write page content to EEPROM */
    for (i = 0; i < EEPROM_PAGE_SIZE; i += sizeof(uint32_t))
      FLASH_ProgramWord(addr + i, *((uint32_t*)&EEPROM_Page[i]));
  }
}

/** \brief Save both EEPROM copies
 *
 * \return void
 *
 */
void EEPROM_SaveBoth(void)
{
  EEPROM_SaveCopy(EE_COPY_1);
  EEPROM_SaveCopy(EE_COPY_2);
}

/** \brief Load all variables from EEPROM
 *
 * \return void
 *
 */
void EEPROM_LoadVariables(void)
{
  uint8_t i;

  for (i = 0; i < sizeof(EEPROM_Values) / sizeof(eeVal_t); i++)
  {
    memcpy(EEPROM_Values[i].pVal, &EEPROM_Page[EEPROM_Values[i].address], EEPROM_Values[i].size);
  }
}

/** \brief Rewrite variable with address
 *
 * \param [in] Pointer to variable
 * \return void
 *
 */
void EEPROM_SaveVariable(void *var)
{
  uint8_t i;

  for (i = 0; i < sizeof(EEPROM_Values) / sizeof(eeVal_t); i++)
  {
    if (var == EEPROM_Values[i].pVal)
    {
      memcpy(&EEPROM_Page[EEPROM_Values[i].address], var, EEPROM_Values[i].size);
      //EEPROM_RecalculateCRC();
      EEPROM_SaveBoth();
      break;
    }
  }
}

/** \brief Save all variables to non-volatile EEPROM
 *
 * \return void
 *
 */
void EEPROM_SaveAllVariables(void)
{
  uint8_t i;

  for (i = 0; i < sizeof(EEPROM_Values) / sizeof(eeVal_t); i++)
  {
    memcpy(&EEPROM_Page[EEPROM_Values[i].address], EEPROM_Values[i].pVal, EEPROM_Values[i].size);
  }
  EEPROM_SaveBoth();
}

/** \brief Load EEPROM copy to global variables
 *
 * \param [in] ee_copy Number of the copy to load
 * \return void
 *
 */
void EEPROM_LoadCopy(uint8_t ee_copy)
{
  uint32_t addr;

  switch (ee_copy)
  {
    case EE_COPY_1:
      addr = EEPROM_ADDR_COPY_1;
      break;
    case EE_COPY_2:
      addr = EEPROM_ADDR_COPY_2;
      break;
    default:
      return;
  }

  memcpy(EEPROM_Page, (uint8_t*)addr, EEPROM_PAGE_SIZE);
  EEPROM_LoadVariables();
}

/** \brief Load default values to both copies
 *
 * \return void
 *
 */
void EEPROM_LoadDefault(void)
{
  uint8_t i;

  memset(EEPROM_Page, 0xff, EEPROM_PAGE_SIZE);
  for (i = 0; i < sizeof(EEPROM_Values) / sizeof(eeVal_t); i++)
  {
    memcpy(EEPROM_Values[i].pVal, &EEPROM_Values[i].defVal, EEPROM_Values[i].size);
    memcpy(&EEPROM_Page[EEPROM_Values[i].address], &EEPROM_Values[i].defVal, EEPROM_Values[i].size);
  }
  /**< Save both copies */
  EEPROM_SaveBoth();
}

/** \brief Configure EEPROM memory, perform self-test
 *
 * \return void
 *
 */
void EEPROM_Configuration(void)
{
  uint16_t crc;
  uint16_t crc_read;
  bool firstCopy, secondCopy;

  FLASH_Unlock();
  firstCopy = true;
  secondCopy = true;
  EEPROM_LoadCopy(EE_COPY_1);
  crc_read = *(uint16_t*)&EEPROM_Page[EEPROM_PAGE_SIZE - sizeof(uint16_t)];
  crc = CRC16_CalcKearfott(EEPROM_Page, EEPROM_PAGE_SIZE - sizeof(uint16_t));
  if (crc != crc_read)
  {
    /**< first EEPROM page is corrupted */
    firstCopy = false;
  }
  EEPROM_LoadCopy(EE_COPY_2);
  crc_read = *(uint16_t*)&EEPROM_Page[EEPROM_PAGE_SIZE - sizeof(uint16_t)];
  crc = CRC16_CalcKearfott(EEPROM_Page, EEPROM_PAGE_SIZE - sizeof(uint16_t));
  if (crc != crc_read)
  {
    /**< second EEPROM page is corrupted */
    secondCopy = false;
  }

  if ((secondCopy == false) && (secondCopy == false))
  {
    /**< Both copies are corrupted Reload default values */
    EEPROM_LoadDefault();
  } else
  if (secondCopy == false)
  {
    /**< Second copy is corrupted, reload with first copy */
    EEPROM_LoadCopy(EE_COPY_1);
    EEPROM_SaveCopy(EE_COPY_2);
  } else
  if (firstCopy == false)
  {
    /**< First copy is corrupted, reload with second copy */
    EEPROM_LoadCopy(EE_COPY_2);
    EEPROM_SaveCopy(EE_COPY_1);
  } else
  {
    /**< Both copies are Ok, just load first copy */
    EEPROM_LoadCopy(EE_COPY_1);
  }
}

