#ifndef EEPROM_H
#define EEPROM_H

#include "defines.h"

#define EEPROM_ADDR_COPY_1      0x08007800
#define EEPROM_ADDR_COPY_2      0x08007C00

#define EEPROM_PAGE_SIZE        0x100

enum
{
  EE_COPY_1,
  EE_COPY_2
};

typedef struct
{
  uint8_t address;  /**< Address in EEPROM */
  uint8_t size;     /**< Size in bytes */
  void*   pVal;     /**< Pointer to variable in RAM */
  union {
    uint8_t  u8;
    int8_t   i8;
    uint16_t u16;
    int16_t  i16;
    uint32_t u32;
    float    fl;
  } defVal;
} eeVal_t;

void EEPROM_SaveVariable(void *var);
void EEPROM_SaveAllVariables(void);
void EEPROM_SetDefaults(void);
void EEPROM_Configuration(void);

#endif
