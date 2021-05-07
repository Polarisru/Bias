#ifndef EEPROM_H
#define EEPROM_H

#include "defines.h"

#define EEPROM_ADDR_COPY_1      0x00400000
#define EEPROM_ADDR_COPY_2      0x00400100

#define EEPROM_PAGE_SIZE        0x100

enum
{
  EE_COPY_1,
  EE_COPY_2
};

typedef struct
{
  uint8_t address;  /**< Address in FRAM */
  uint8_t size;     /**< Size in bytes */
  void*   pVal;     /**< Pointer to variable in RAM */
  union {
    uint8_t  byte;
    uint16_t word;
    uint32_t dword;
    float    fl;
  } defVal;
} eeVal_t;

void EEPROM_WriteVar(void *data);
void EEPROM_SetDefaults(void);
void EEPROM_Configuration(void);

#endif
